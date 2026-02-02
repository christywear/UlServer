#pragma once
#include <cstdint>

#pragma pack(push, 1)

// 16 bytes total. Small, robust, reassembly-safe.
struct PacketEnvelope {
    uint16_t magic;            // 0x554C = 'U''L'
    uint8_t  version;          // bump if framing changes
    uint8_t  flags;            // reserved (reliable/unreliable/etc later)
    uint16_t verb;             // VerbId
    uint32_t msgId;            // groups fragments for a single message
    uint16_t fragmentIndex;    // 0..fragmentCount-1
    uint16_t fragmentCount;    // 1 if not fragmented
    uint16_t fragmentBytes;    // payload bytes in THIS fragment
};

#pragma pack(pop)

static constexpr uint16_t kPacketMagic = 0x554C; // 'U''L'
static constexpr uint8_t  kPacketVersion = 1;

static_assert(sizeof(PacketEnvelope) == 16, "PacketEnvelope must be 16 bytes");

inline bool PacketEnvelopeLooksSane(const PacketEnvelope& h) {
    if (h.magic != kPacketMagic) return false;
    if (h.version != kPacketVersion) return false;
    if (h.fragmentCount == 0) return false;
    if (h.fragmentIndex >= h.fragmentCount) return false;
    // fragmentBytes can be 0 for weird cases, but usually you want >0.
    return true;
}
