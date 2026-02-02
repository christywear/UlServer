#pragma once
#include <protocol/net/NetTypes.h>
#include <Game/Verbs.h>
#include <functional>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Called ONLY when we have a full, reassembled payload.
using CommandHandler = std::function<void(VerbId verb, const uint8_t* payload, size_t payloadSize)>;

class PacketAssembler {
public:
    PacketAssembler() = default;

    // Feed raw bytes from transport. Handles:
    // - partial frames (TCP-style stream behavior)
    // - multiple frames in one feed
    // - fragment reassembly by msgId
    void feed(const uint8_t* data, size_t size, const CommandHandler& onComplete);

    // Optional: clear all partial state (e.g. on disconnect)
    void reset();

    // Tune how much junk we allow before dropping.
    void setMaxBufferedBytes(size_t bytes) { maxBufferedBytes_ = bytes; }

private:
    struct InFlight {
        VerbId verb{};
        uint16_t fragCount = 0;
        uint16_t received = 0;
        std::vector<Buffer> frags; // indexed by fragmentIndex
        size_t totalBytes = 0;
    };

private:
    Buffer staging_; // stream buffer (for partial headers/frames)
    std::unordered_map<uint32_t, InFlight> inflight_; // msgId -> assembly

    size_t maxBufferedBytes_ = 4 * 1024 * 1024; // 4MB safety cap

private:
    void dropAll(); // if input goes insane
};
