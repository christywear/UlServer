#include <protocol/net/SteamTransport.h>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingsockets.h>
#include <iostream>
#include <protocol/net/NetTypes.h>

// We need a way for the static callback to find our instance 
// (In a real game, you'd have a singleton or a global pointer)
static SteamTransport* g_pSteamTransportInstance = nullptr;

SteamTransport::SteamTransport() {
    g_pSteamTransportInstance = this;
    interface_ = SteamNetworkingSockets();

    if (interface_) {
        SteamNetworkingIPAddr addr;
        addr.Clear();
        addr.m_port = 12345;

        listen_socket_ = interface_->CreateListenSocketIP(addr, 0, nullptr);

        // --- 🎯 THE FIX: CREATE A POLL GROUP ---
        poll_group_ = interface_->CreatePollGroup();

        std::cout << "🚀 Steam Transport active on port " << addr.m_port << "!" << std::endl;
    }
}

SteamTransport::~SteamTransport() {
    if (interface_) {
        if (poll_group_) interface_->DestroyPollGroup(poll_group_);
        if (listen_socket_) interface_->CloseListenSocket(listen_socket_);
    }
    g_pSteamTransportInstance = nullptr;
}

bool SteamTransport::send(PlayerId to, const Buffer& data) {
    auto it = connections_.find(to);
    if (it == connections_.end()) return false;

    EResult res = interface_->SendMessageToConnection(
        it->second,
        data.data(),
        static_cast<uint32_t>(data.size()),
        k_nSteamNetworkingSend_Reliable,
        nullptr
    );

    return res == k_EResultOK;
}

void SteamTransport::poll() {
    if (!interface_ || !poll_group_) return;

    // 1. Run Steam Callbacks (This triggers OnNetConnectionStatusChanged)
    SteamAPI_RunCallbacks();

    // 2. 🎯 THE FIX: READ FROM THE POLL GROUP!
    ISteamNetworkingMessage* incoming_msgs[32];
    int num_msgs = interface_->ReceiveMessagesOnPollGroup(poll_group_, incoming_msgs, 32);

    for (int i = 0; i < num_msgs; ++i) {
        auto* msg = incoming_msgs[i];

        if (onData_) {
            // 🎯 THE WIRING! 
            // For Steam, we can use the connection handle as the ID for now
            PlayerId pid = static_cast<PlayerId>(msg->m_conn);
            onData_(pid, (const uint8_t*)msg->m_pData, msg->m_cbSize);
        }

        msg->Release();
    }
}

// --- 🚪 THE BOUNCER: HANDLING CONNECTIONS ---
void SteamTransport::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) {
    auto* transport = g_pSteamTransportInstance;
    if (!transport || !transport->interface_) return;

    switch (pInfo->m_info.m_eState) {
    case k_ESteamNetworkingConnectionState_Connecting:
        // Someone is knocking! Let them in! 🤝
        transport->interface_->AcceptConnection(pInfo->m_hConn);
        // Put them in the poll group so we can hear their messages!
        transport->interface_->SetConnectionPollGroup(pInfo->m_hConn, transport->poll_group_);
        std::cout << "🤝 Accepted new Steam connection!" << std::endl;
        break;

    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        std::cout << "🔌 Steam player disconnected." << std::endl;
        transport->interface_->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
        break;

    default:
        break;
    }
}