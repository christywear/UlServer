#include <protocol/net/EnetTransport.h>
#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <protocol/net/ITransport.h>
#include <protocol/net/NetTypes.h>
#include <enet/enet.h>
#include <enet/types.h>

// Helper: ID <-> Peer Pointer 
static PlayerId PeerToId(ENetPeer* peer) {
    return static_cast<PlayerId>(reinterpret_cast<std::uintptr_t>(peer));
}

static ENetPeer* IdToPeer(PlayerId id) {
    return reinterpret_cast<ENetPeer*>(static_cast<std::uintptr_t>(id));
}

EnetTransport::EnetTransport(uint16_t port, size_t maxPeers) {
    if (enet_initialize() != 0) {
        std::cerr << "ENet: Failed to initialize\n";
        return;
    }

    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = port;

    // Create Host (Server)
    host_ = enet_host_create(&address, maxPeers, 2, 0, 0);

    if (!host_) {
        std::cerr << "ENet: Failed to create host on port " << port << "\n";
        return;
    }
    std::cout << "ENet: Listening on port " << port << "\n";
}

EnetTransport::~EnetTransport() {
    if (host_) {
        enet_host_destroy(host_);
        host_ = nullptr;
    }
    enet_deinitialize();
}

bool EnetTransport::send(PlayerId to, const Buffer& data, SendMode mode) {
    if (!host_) return false;

    ENetPeer* peer = IdToPeer(to);
    if (!peer) return false;

    enet_uint32 flags = 0;
    if (mode == SendMode::Reliable) {
        flags = ENET_PACKET_FLAG_RELIABLE;
    }

    ENetPacket* packet = enet_packet_create(data.data(), data.size(), flags);
    enet_peer_send(peer, 0, packet);
    return true;
}

void EnetTransport::poll() {
    if (!host_) return;

    ENetEvent event;
    while (enet_host_service(host_, &event, 0) > 0) {
        switch (event.type) {
<<<<<<< Updated upstream
        case ENET_EVENT_TYPE_CONNECT:
            // Give them a temporary ID or wait for login
            // For now, let's just store them!
            uint32_t temporaryId = (uint32_t)event.peer->address.host;
            peers_[temporaryId] = event.peer;
            std::cout << "🌐 ENet: Peer " << temporaryId << " connected!" << std::endl;
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            if (onData_) {
                // Map event.peer to your PlayerId system
                PlayerId pid = reinterpret_cast<uintptr_t>(event.peer);
                onData_(pid, event.packet->data, event.packet->dataLength);
=======
        case ENET_EVENT_TYPE_CONNECT: {
            PlayerId id = PeerToId(event.peer);
            std::cout << "ENet: Connect " << id << "\n";
            // No peers_ map needed since we use pointer casting
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT: {
            PlayerId id = PeerToId(event.peer);
            std::cout << "ENet: Disconnect " << id << "\n";
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
            PlayerId from = PeerToId(event.peer);
            if (onData_) {
                onData_(from, event.packet->data, event.packet->dataLength);
>>>>>>> Stashed changes
            }
            enet_packet_destroy(event.packet);
<<<<<<< Updated upstream
            break; // 🛑 STOP the fall-through!

        case ENET_EVENT_TYPE_DISCONNECT:
            // Clean up so we don't try to send to a ghost! 👻
            // peers_.erase(someId);
            break;

        default:
            break;
=======
            break;
        }
        default: break;
>>>>>>> Stashed changes
        }
    }
}