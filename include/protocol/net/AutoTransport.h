#pragma once
#include <protocol/net/ITransport.h>
#include <protocol/net/EnetTransport.h>
#include <protocol/net/SteamTransport.h>
#include <memory>
#include <unordered_map>

class AutoTransport : public ITransport {
public:
    // Factory that creates the Composite Transport
    static std::unique_ptr<ITransport> Create();

    AutoTransport();
    ~AutoTransport();

    // ITransport Interface
    bool send(PlayerId to, const Buffer& data, SendMode mode) override;
    void poll() override;

private:
    // Sub-Transports
    std::unique_ptr<EnetTransport> enet_;
    std::unique_ptr<SteamTransport> steam_;

    // Routing Table: Which transport does Player X use?
    enum class TransportType { ENet, Steam };
    std::unordered_map<PlayerId, TransportType> routeTable_;
};