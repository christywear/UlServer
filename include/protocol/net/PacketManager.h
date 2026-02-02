#pragma once
#include <protocol/net/PacketAssembler.h>
#include <protocol/net/BinaryWriter.h>
#include <memory>
#include <atomic>
#include <unordered_map>
#include <mutex>
#include <cstdint>
#include "protocol/net/NetTypes.h"

class PacketManager {
public:
    static PacketManager* Get() { static PacketManager inst; return &inst; }

    void Dispatch(PlayerId to, int verb, std::shared_ptr<BinaryWriter> data);
    void ProcessIncoming(PlayerId from, const uint8_t* data, size_t size);

    // Call this when a player disconnects to free memory!
    void RemoveSession(PlayerId player);

    void SetRouter(class CommandRouter* router) { router_ = router; }

private:
    PacketManager() = default;

    // FIX: One assembler per player!
    std::mutex assemblerMutex_;
    std::unordered_map<PlayerId, PacketAssembler> assemblers_;

    class CommandRouter* router_ = nullptr;
    std::atomic<uint32_t> nextMsgId_{ 1 };
};