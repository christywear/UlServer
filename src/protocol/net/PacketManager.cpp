#include <protocol/net/PacketManager.h>
#include <protocol/net/PacketEnvelope.h>
#include <protocol/net/GameNet.h> 
#include <Game/LmLog.h> 
#include <Game/CommandRouter.h> 
#include <protocol/net/BinaryWriter.h>
#include <protocol/net/NetTypes.h>

#include <algorithm> // Required for std::min
#include <limits>    // Required for std::numeric_limits
#include <cstring>
#include <memory>
#include <cstdint>
#include <mutex>
#include <utility>
#include <game/Verbs.h>
#include <protocol/net/PacketAssembler.h>

// ----------------------------------------------------------------------------
// DISPATCH: Sends data OUT
// ----------------------------------------------------------------------------
void PacketManager::Dispatch(PlayerId to, int verb, std::shared_ptr<BinaryWriter> data) {
    const size_t MTU = 1200;
    const size_t HEADER_SIZE = sizeof(PacketEnvelope);
    const size_t MAX_PAYLOAD = MTU - HEADER_SIZE;

    size_t totalBytes = data->size();

    // Assumes BinaryWriter has: const uint8_t* data() const { return buffer_.data(); }
    const uint8_t* rawData = data->data();

    // 1. Calculate required fragments
    size_t requiredFrags = (totalBytes == 0) ? 1 : (totalBytes + MAX_PAYLOAD - 1) / MAX_PAYLOAD;

    // 2. FUTURE PROOF CHECK
    using FragType = decltype(PacketEnvelope::fragmentCount);
    size_t maxCapacity = (std::numeric_limits<FragType>::max)(); // Parens block Windows macros

    if (requiredFrags > maxCapacity) {
        LmLog::Instance()->Error(
            "CRITICAL: Packet overflow! Size: %zu bytes requires %zu fragments, but protocol limit is %zu.",
            totalBytes, requiredFrags, maxCapacity
        );
        return;
    }

    // 3. Safe Cast
    auto fragCount = static_cast<FragType>(requiredFrags);
    uint32_t msgId = nextMsgId_.fetch_add(1, std::memory_order_relaxed);

    size_t offset = 0;

    for (size_t i = 0; i < fragCount; i++) {
        // A. Prepare the Header Local Struct
        PacketEnvelope header{};
        header.magic = kPacketMagic;
        header.version = kPacketVersion;
        header.verb = (uint16_t)verb;
        header.msgId = msgId;
        header.fragmentIndex = static_cast<FragType>(i);
        header.fragmentCount = fragCount;

        size_t remaining = (totalBytes > offset) ? (totalBytes - offset) : 0;
        size_t chunkSize = (std::min)(MAX_PAYLOAD, remaining); // Parens block Windows macros

        header.fragmentBytes = (uint16_t)chunkSize;

        // B. SERIALIZATION: Create the final byte buffer HERE.
        // We allocate exactly what's needed for Header + Chunk
        Buffer frame;
        frame.resize(HEADER_SIZE + chunkSize);

        // Copy Header into start of frame
        std::memcpy(frame.data(), &header, HEADER_SIZE);

        // Copy Payload chunk into frame (after header)
        if (chunkSize > 0) {
            std::memcpy(frame.data() + HEADER_SIZE, rawData + offset, chunkSize);
        }

        // C. Hand off ownership to GameNet
        // GameNet takes the raw bytes via move. It doesn't know or care about headers.
        GameNet::Instance()->EnqueuePacket(to, std::move(frame));

        offset += chunkSize;
    }
}

// ----------------------------------------------------------------------------
// PROCESS INCOMING: Receives data IN
// ----------------------------------------------------------------------------
void PacketManager::ProcessIncoming(PlayerId from, const uint8_t* data, size_t size) {
    // 1. Thread-Safe Assembler Lookup
    PacketAssembler* assembler = nullptr;
    {
        std::lock_guard<std::mutex> lock(assemblerMutex_);
        assembler = &assemblers_[from]; // Creates one if it doesn't exist
    }

    // 2. Feed the assembler (It handles stitching fragments)
    assembler->feed(data, size, [this, from](VerbId verb, const uint8_t* payload, size_t pSize) {

        // 3. When a full packet is ready, this lambda fires:
        if (router_) {
            // Wrap raw bytes in a Buffer vector for safety
            Buffer buf(payload, payload + pSize);

            try {
                // Pass to CommandRouter (Note: Router creates the Context internally now)
                router_->dispatch(from, verb, buf);
            }
            catch (...) {
                LmLog::Instance()->Error("Exception processing packet from %llu", from);
            }
        }
    });
}

// ----------------------------------------------------------------------------
// REMOVE SESSION: Cleanup
// ----------------------------------------------------------------------------
void PacketManager::RemoveSession(PlayerId player) {
    std::lock_guard<std::mutex> lock(assemblerMutex_);
    assemblers_.erase(player);
}