#include <protocol/net/PacketAssembler.h> // Ensure this path is correct for your project!
#include <cstring>
#include <Game/Verbs.h> // Need this to understand what a VerbId is!
#include <protocol/net/PacketEnvelope.h>

void PacketAssembler::feed(const uint8_t* data, size_t size, const CommandHandler& onComplete) {
    stagingArea_.insert(stagingArea_.end(), data, data + size);

    // Header is now 10 bytes (PacketEnvelope)
    while (stagingArea_.size() >= sizeof(PacketEnvelope)) {
        PacketEnvelope header;
        std::memcpy(&header, stagingArea_.data(), sizeof(PacketEnvelope));

        if (stagingArea_.size() >= (sizeof(PacketEnvelope) + header.payloadSize)) {
            const uint8_t* payloadStart = stagingArea_.data() + sizeof(PacketEnvelope);

            if (onComplete) {
                onComplete(static_cast<VerbId>(header.verb), payloadStart, header.payloadSize);
            }

            stagingArea_.erase(stagingArea_.begin(), stagingArea_.begin() + sizeof(PacketEnvelope) + header.payloadSize);
        }
        else {
            break;
        }
    }
}