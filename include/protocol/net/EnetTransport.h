#pragma once

#include <protocol/net/ITransport.h>
#include <memory>
#include <map>

// Forward declarations to keep our "Sanity Shield" strong!
struct _ENetHost;
struct _ENetPeer;
typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;

class EnetTransport : public ITransport {
public:
    EnetTransport();
    ~EnetTransport() override;

    // The high-level send we've been dreaming of!
    bool send(PlayerId to, const Buffer& data) override;

    // The heartbeat of the network - call this every frame!
    void poll() override;

private:
    ENetHost* server_ = nullptr;

    // Maps our PlayerId to the real ENet peer handle
    std::map<PlayerId, ENetPeer*> peers_;
};