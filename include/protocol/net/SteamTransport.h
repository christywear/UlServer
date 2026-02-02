#pragma once

#include <protocol/net/ITransport.h>

#include <steam/steam_api.h>                 // CCallbackManual
#include <steam/isteamnetworkingsockets.h>   // ISteamNetworkingSockets

#include <unordered_map>
#include "NetTypes.h"
#include <steam/steamnetworkingtypes.h>
#include <steam/steam_api_common.h>

class SteamTransport : public ITransport {
public:
    SteamTransport();
    ~SteamTransport() override;

    bool send(PlayerId to, const Buffer& data, SendMode mode) override;
    void poll() override;

private:
    HSteamListenSocket listenSocket_ = k_HSteamListenSocket_Invalid;
    HSteamNetPollGroup pollGroup_ = k_HSteamNetPollGroup_Invalid;

    // Server-side: map PlayerId → connection handle
    std::unordered_map<PlayerId, HSteamNetConnection> connections_;

    // Used on server to generate stable PlayerIds
    PlayerId nextId_ = 1;

    // ---- callback hook ----
    void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);

    // NOTE: many SDKs do NOT have a ctor that takes (this, &Fn). Use Register().
    CCallbackManual<SteamTransport, SteamNetConnectionStatusChangedCallback_t>
        m_CallbackConnectionStatusChanged;
};
