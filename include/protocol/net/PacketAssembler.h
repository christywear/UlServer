#pragma once
#include <protocol/net/NetTypes.h>
#include <Game/Verbs.h>
#include <functional>

// The callback signature for when a full packet is ready!
using CommandHandler = std::function<void(VerbId, const uint8_t*, size_t)>;

class PacketAssembler {
public:
    PacketAssembler() = default;

    // Push raw bytes into the assembly line
    void feed(const uint8_t* data, size_t size, const CommandHandler& onComplete);

private:
    Buffer stagingArea_; // Our "waiting room" for incomplete data
};