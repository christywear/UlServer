#pragma once

#include "ITransport.h"
#include <memory>
#include <cstdint>
#include <map>

// Forward declarations to keep it clean!
struct ISteamNetworkingSockets;
struct SteamNetConnectionStatusChangedCallback_t;
typedef uint32_t HSteamListenSocket;
typedef uint32_t HSteamNetConnection;
typedef uint32_t HSteamNetPollGroup;

class SteamTransport : public ITransport {
public:
    SteamTransport();
    ~SteamTransport() override;

    bool send(PlayerId to, const Buffer& data) override;
    void poll() override;

private:
    // This is the static callback Steam needs
    static void OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

    ISteamNetworkingSockets* interface_ = nullptr;
    HSteamListenSocket listen_socket_ = 0;
    HSteamNetPollGroup poll_group_ = 0; // The "Big Ear" for all messages! 👂

    std::map<PlayerId, HSteamNetConnection> connections_;
};