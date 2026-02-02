#pragma once
#include <protocol/net/NetTypes.h> // <--- Ensure this is here
#include <functional>

enum class SendMode : uint8_t { Reliable, Unreliable };

// Callback: (SenderID, DataPointer, Size)
using OnDataReceived = std::function<void(PlayerId, const uint8_t*, size_t)>;

class ITransport {
public:
    virtual ~ITransport() = default;

    virtual bool send(PlayerId to, const Buffer& data, SendMode mode) = 0;
    virtual void poll() = 0;

    void setHandler(OnDataReceived handler) { onData_ = std::move(handler); }

protected:
    OnDataReceived onData_;
};