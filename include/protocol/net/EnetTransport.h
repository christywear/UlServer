#pragma once

#include <protocol/net/ITransport.h> // Inherit types from here!
#include <enet/enet.h>
#include <unordered_map>

class EnetTransport : public ITransport {
public:
    // Standard setup
    explicit EnetTransport(uint16_t port = 12345, size_t maxPeers = 32);
    ~EnetTransport() override;

    // --- ITransport Implementation ---

    // Matches base class signature exactly
    bool send(PlayerId to, const Buffer& data, SendMode mode) override;

    // Matches base class signature exactly
    void poll() override;

private:
    ENetHost* host_ = nullptr;

    // Map internal ENet pointers to your PlayerIds
    std::unordered_map<PlayerId, ENetPeer*> peers_;

    // Reverse map for quick lookups during poll
    // (Or you can store PlayerId in peer->data)

    PlayerId nextId_ = 1;
};