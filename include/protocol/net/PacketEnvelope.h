#pragma once
#include <cstdint>

#pragma pack(push, 1) // 🛡️ NO PADDING! Keep it exactly 10 bytes.
struct PacketEnvelope {
    uint16_t verb;          // What is this?
    uint32_t payloadSize;   // How big is the data? (Supports massive packets!)
    uint16_t fragmentIndex; // Which slice is this? (0 if single)
    uint16_t fragmentCount; // Total slices? (1 if single)
};
#pragma pack(pop)