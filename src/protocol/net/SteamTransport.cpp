#include <protocol/net/SteamTransport.h>
#include <iostream>
#include <cstdint>
#include <protocol/net/ITransport.h>
#include <protocol/net/NetTypes.h>

// STEAM HEADERS
#include <steam/steam_gameserver.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingtypes.h>
#include <steam/steamtypes.h>
#include <steam/steam_api_common.h>

// CORE LOGGING
#include <Game/LmLog.h> 

SteamTransport::SteamTransport()
{
    // -----------------------------------------------------------------------
    // 1. IGNITION: Initialize Steam Game Server API FIRST
    // -----------------------------------------------------------------------
    // 5 Args: IP(Auto), GamePort, QueryPort, Mode, Version
    if (!SteamGameServer_Init(0, 27020, 27021, eServerModeNoAuthentication, "1.0.0.0"))
    {
        LmLog::Instance()->Error("FATAL: SteamGameServer_Init failed!");
        return;
    }

    // -----------------------------------------------------------------------
    // 2. Safety Check: Verify Interfaces exist
    // -----------------------------------------------------------------------
    if (!SteamNetworkingUtils())
    {
        LmLog::Instance()->Error("FATAL: SteamNetworkingUtils is NULL.");
        return;
    }

    // 3. Register Callbacks
    m_CallbackConnectionStatusChanged.Register(this, &SteamTransport::OnConnectionStatusChanged);

    // 4. Configure Relay
    SteamNetworkingUtils()->InitRelayNetworkAccess();

    // 5. Create Listen Socket (UDP 27020)
    SteamNetworkingIPAddr addr{};
    addr.Clear();
    addr.m_port = 27020;

    // FIX: Use SteamGameServerNetworkingSockets() for SERVER
    if (!SteamGameServerNetworkingSockets())
    {
        LmLog::Instance()->Error("FATAL: SteamGameServerNetworkingSockets is NULL.");
        return;
    }

    listenSocket_ = SteamGameServerNetworkingSockets()->CreateListenSocketIP(addr, 0, nullptr);

    if (listenSocket_ == k_HSteamListenSocket_Invalid)
    {
        LmLog::Instance()->Error("SteamTransport: Failed to create listen socket on 27020");
        return;
    }

    pollGroup_ = SteamGameServerNetworkingSockets()->CreatePollGroup();
    if (pollGroup_ == k_HSteamNetPollGroup_Invalid)
    {
        LmLog::Instance()->Error("SteamTransport: Failed to create poll group");
        return;
    }

    LmLog::Instance()->Log("SteamTransport: Listening on port 27020");
}

SteamTransport::~SteamTransport()
{
    m_CallbackConnectionStatusChanged.Unregister();

    // Gracefully close connections
    for (auto& [id, conn] : connections_)
    {
        if (SteamGameServerNetworkingSockets())
        {
            SteamGameServerNetworkingSockets()->CloseConnection(conn, 0, "shutdown", false);
        }
    }
    connections_.clear();

    if (pollGroup_ != k_HSteamNetPollGroup_Invalid && SteamGameServerNetworkingSockets())
    {
        SteamGameServerNetworkingSockets()->DestroyPollGroup(pollGroup_);
        pollGroup_ = k_HSteamNetPollGroup_Invalid;
    }

    if (listenSocket_ != k_HSteamListenSocket_Invalid && SteamGameServerNetworkingSockets())
    {
        SteamGameServerNetworkingSockets()->CloseListenSocket(listenSocket_);
        listenSocket_ = k_HSteamListenSocket_Invalid;
    }

    // SHUTDOWN THE ENGINE
    SteamGameServer_Shutdown();
}

bool SteamTransport::send(PlayerId to, const Buffer& data, SendMode mode)
{
    if (data.empty()) return true;
    if (!SteamGameServerNetworkingSockets()) return false;

    const int flags = (mode == SendMode::Reliable)
        ? k_nSteamNetworkingSend_Reliable
        : k_nSteamNetworkingSend_UnreliableNoDelay;

    auto it = connections_.find(to);
    if (it == connections_.end()) return false;

    SteamGameServerNetworkingSockets()->SendMessageToConnection(
        it->second,
        data.data(),
        static_cast<uint32>(data.size()),
        flags,
        nullptr
    );

    return true;
}

void SteamTransport::poll()
{
    // FIX: Use Server Callbacks
    SteamGameServer_RunCallbacks();

    if (pollGroup_ == k_HSteamNetPollGroup_Invalid) return;
    if (!SteamGameServerNetworkingSockets()) return;

    ISteamNetworkingMessage* msgs[32];
    int count = SteamGameServerNetworkingSockets()->ReceiveMessagesOnPollGroup(pollGroup_, msgs, 32);

    for (int i = 0; i < count; ++i)
    {
        ISteamNetworkingMessage* m = msgs[i];
        PlayerId from = 0;

        // Lookup connection ID
        for (auto& [id, conn] : connections_)
        {
            if (conn == m->m_conn)
            {
                from = id;
                break;
            }
        }

        if (onData_ && from != 0)
        {
            onData_(from, static_cast<const uint8_t*>(m->m_pData), static_cast<size_t>(m->m_cbSize));
        }
        m->Release();
    }
}

void SteamTransport::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
{
    if (!SteamGameServerNetworkingSockets()) return;

    switch (info->m_info.m_eState)
    {
    case k_ESteamNetworkingConnectionState_Connecting:
    {
        SteamGameServerNetworkingSockets()->AcceptConnection(info->m_hConn);
        SteamGameServerNetworkingSockets()->SetConnectionPollGroup(info->m_hConn, pollGroup_);

        PlayerId id = nextId_++;
        connections_[id] = info->m_hConn;

        std::cout << "Steam: client connected id=" << id << "\n";
        break;
    }

    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        PlayerId dead = 0;
        for (auto& [id, conn] : connections_)
        {
            if (conn == info->m_hConn)
            {
                dead = id;
                break;
            }
        }
        if (dead)
        {
            connections_.erase(dead);
            std::cout << "Steam: client disconnected id=" << dead << "\n";
        }
        SteamGameServerNetworkingSockets()->CloseConnection(info->m_hConn, 0, nullptr, false);
        break;
    }
    default: break;
    }
}