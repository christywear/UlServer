#pragma once
#include <protocol/net/NetTypes.h>
#include <functional>

// Signature: (Who sent it, pointer to bytes, how many bytes)
using OnDataReceived = std::function<void(PlayerId, const uint8_t*, size_t)>;

/// virtual transport class for networking
class ITransport {
public:
    virtual ~ITransport() = default;

    // default send format for msgs
    virtual bool send(PlayerId to, const Buffer& data) = 0;

    // Pumping the network heart! 💓
    virtual void poll() = 0;

    // This is the "Wiring" method! 
    // It lets GameNet plug itself into the Transport.
    void setHandler(OnDataReceived handler) { onData_ = std::move(handler); }

protected:
    // We use 'protected' so EnetTransport and SteamTransport 
    // can actually see and use this variable! 🛡️
    OnDataReceived onData_;
};