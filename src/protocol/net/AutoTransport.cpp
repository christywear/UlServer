#include <protocol/net/AutoTransport.h>
#include <protocol/net/EnetTransport.h>
#include <protocol/net/SteamTransport.h>

AutoTransport::AutoTransport(Mode mode) : mode_(mode) {
    // Shared lambda to relay data from child -> AutoTransport -> GameNet
    auto relay = [this](PlayerId p, const uint8_t* d, size_t s) {
        if (onData_) onData_(p, d, s);
    };

    if (mode_ == Mode::STEAM) {
        steam_ = std::make_unique<SteamTransport>();
        steam_->setHandler(relay); // 🔌 WIRED!
    }
    else {
        enet_ = std::make_unique<EnetTransport>();
        enet_->setHandler(relay);  // 🔌 WIRED!
    }
}

bool AutoTransport::send(PlayerId to, const Buffer& data)
{
    return (mode_ == Mode::STEAM)
        ? steam_->send(to, data)
        : enet_->send(to, data);
}

void AutoTransport::poll()
{
    if (mode_ == Mode::STEAM)
        steam_->poll();
    else
        enet_->poll();
}
