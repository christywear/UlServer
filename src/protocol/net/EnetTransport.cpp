#include <protocol/net/EnetTransport.h>
#include <enet/enet.h> // The real guts!
#include <iostream>
#include <protocol/net/NetTypes.h>

EnetTransport::EnetTransport() {
    if (enet_initialize() != 0) {
        std::cerr << "❌ ENet: Failed to initialize!" << std::endl;
        return;
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 12345; // Underlight Classic Port!

    // Create a server host: 32 max players, 2 channels, no bandwidth limits
    server_ = enet_host_create(&address, 32, 2, 0, 0);

    if (server_ == nullptr) {
        std::cerr << "❌ ENet: Failed to create host!" << std::endl;
    }
    else {
        std::cout << "🚀 ENet Transport active on port " << address.port << "!" << std::endl;
    }
}

EnetTransport::~EnetTransport() {
    if (server_) {
        enet_host_destroy(server_);
    }
    enet_deinitialize();
}

bool EnetTransport::send(PlayerId to, const Buffer& data) {
    auto it = peers_.find(to);
    if (it == peers_.end()) return false;

    // Create a RELIABLE packet. ENet handles fragmentation/retries for us!
    ENetPacket* packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);

    // Send on channel 0
    return enet_peer_send(it->second, 0, packet) == 0;
}

void EnetTransport::poll() {
    if (!server_) return;

    ENetEvent event;
    while (enet_host_service(server_, &event, 0) > 0) {
        switch (event.type) {
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
            }

            // 🧹 CRITICAL CLEANUP!
            enet_packet_destroy(event.packet);
            break; // 🛑 STOP the fall-through!

        case ENET_EVENT_TYPE_DISCONNECT:
            // Clean up so we don't try to send to a ghost! 👻
            // peers_.erase(someId);
            break;

        default:
            break;
        }
    }
}