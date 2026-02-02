#include <protocol/net/AutoTransport.h>
#include <core/LyraDefs.h> // <--- REQUIRED for Lyra::ServerPort()
#include <Game/LmLog.h>
#include <memory>
#include <cstdint>
#include <protocol/net/EnetTransport.h>
#include <protocol/net/ITransport.h>
#include <protocol/net/NetTypes.h>
#include <protocol/net/SteamTransport.h>

std::unique_ptr<ITransport> AutoTransport::Create() {
    return std::make_unique<AutoTransport>();
}

AutoTransport::AutoTransport() {
    // 1. Initialize ENet (Port from LyraDefs)
    // We assume Lyra::ServerPort() returns the starting UDP port (e.g. 7777)
    uint16_t enetPort = GsConfig::ServerPort();
    // --- FIX: Force default if config is missing ---
    if (enetPort == 0) {
        enetPort = 7777;
        LmLog::Instance()->Warning("Config returned Port 0. Defaulting ENet to 7777.");
    }
    // -----------------------------------------------
    enet_ = std::make_unique<EnetTransport>(enetPort, 64); // 64 Max Peers

    // 2. Initialize Steam (Port 27020 Hardcoded per request)
    steam_ = std::make_unique<SteamTransport>();

    // 3. Setup Routing Callbacks
    // When ENet gets a packet, tag it and forward up.
    enet_->setHandler([this](PlayerId from, const uint8_t* data, size_t size) {
        routeTable_[from] = TransportType::ENet; // Remember this ID uses ENet
        if (this->onData_) this->onData_(from, data, size);
    });

    // When Steam gets a packet, tag it and forward up.
    steam_->setHandler([this](PlayerId from, const uint8_t* data, size_t size) {
        routeTable_[from] = TransportType::Steam; // Remember this ID uses Steam
        if (this->onData_) this->onData_(from, data, size);
    });

    LmLog::Instance()->Log("AutoTransport: Bridging ENet (Port %d) and Steam (Port 27020)", enetPort);
}

AutoTransport::~AutoTransport() = default;

void AutoTransport::poll() {
    // Poll BOTH networks every tick
    if (enet_) enet_->poll();
    if (steam_) steam_->poll();
}

bool AutoTransport::send(PlayerId to, const Buffer& data, SendMode mode) {
    // 1. Look up the route
    auto it = routeTable_.find(to);
    if (it == routeTable_.end()) {
        // Fallback: If unknown, try both? 
        // Safer to just return false. The client must speak first.
        return false;
    }

    // 2. Dispatch
    if (it->second == TransportType::ENet) {
        return enet_->send(to, data, mode);
    }
    else if (it->second == TransportType::Steam) {
        return steam_->send(to, data, mode);
    }

    return false;
}