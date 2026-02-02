#include <protocol/net/PacketAssembler.h>
#include <protocol/net/PacketEnvelope.h>
#include <cstring>

void PacketAssembler::reset() {
    staging_.clear();
    inflight_.clear();
}

void PacketAssembler::dropAll() {
    staging_.clear();
    inflight_.clear();
}

void PacketAssembler::feed(const uint8_t* data, size_t size, const CommandHandler& onComplete) {
    if (!data || size == 0) return;

    // Append new bytes into staging (stream-safe).
    staging_.insert(staging_.end(), data, data + size);

    // Hard cap: if the sender is spewing nonsense, cut it off.
    if (staging_.size() > maxBufferedBytes_) {
        dropAll();
        return;
    }

    // Process as many full frames as possible.
    while (staging_.size() >= sizeof(PacketEnvelope)) {
        PacketEnvelope h{};
        std::memcpy(&h, staging_.data(), sizeof(PacketEnvelope));

        if (!PacketEnvelopeLooksSane(h)) {
            // Desync / garbage. Best move is to drop everything to resync fast.
            dropAll();
            return;
        }

        const size_t frameBytes = sizeof(PacketEnvelope) + h.fragmentBytes;
        if (staging_.size() < frameBytes) {
            // not enough data yet
            break;
        }

        const uint8_t* fragPayload = staging_.data() + sizeof(PacketEnvelope);

        // If unfragmented, dispatch immediately.
        if (h.fragmentCount == 1) {
            if (onComplete) {
                onComplete(static_cast<VerbId>(h.verb), fragPayload, h.fragmentBytes);
            }
        }
        else {
            // Fragmented: store and reassemble when complete.
            auto& msg = inflight_[h.msgId];

            // If this is a new in-flight message, initialize it.
            if (msg.fragCount == 0) {
                msg.verb = static_cast<VerbId>(h.verb);
                msg.fragCount = h.fragmentCount;
                msg.frags.resize(h.fragmentCount);
                msg.received = 0;
                msg.totalBytes = 0;
            }
            else {
                // If the msgId collides with different verb or fragCount, drop it.
                if (msg.verb != static_cast<VerbId>(h.verb) || msg.fragCount != h.fragmentCount) {
                    inflight_.erase(h.msgId);
                }
            }

            // Store fragment if not already received.
            if (h.fragmentIndex < msg.frags.size() && msg.frags[h.fragmentIndex].empty()) {
                msg.frags[h.fragmentIndex].assign(fragPayload, fragPayload + h.fragmentBytes);
                msg.received++;
                msg.totalBytes += h.fragmentBytes;
            }

            // If complete: concatenate in-order and dispatch.
            if (msg.received == msg.fragCount) {
                Buffer whole;
                whole.reserve(msg.totalBytes);

                for (uint16_t i = 0; i < msg.fragCount; ++i) {
                    const auto& part = msg.frags[i];
                    if (part.empty()) {
                        // Shouldn't happen, but if it does: drop message.
                        whole.clear();
                        break;
                    }
                    whole.insert(whole.end(), part.begin(), part.end());
                }

                if (!whole.empty() && onComplete) {
                    onComplete(msg.verb, whole.data(), whole.size());
                }

                inflight_.erase(h.msgId);
            }
        }

        // Consume this frame from staging.
        staging_.erase(staging_.begin(), staging_.begin() + frameBytes);

        if (staging_.size() > maxBufferedBytes_) {
            dropAll();
            return;
        }
    }
}
