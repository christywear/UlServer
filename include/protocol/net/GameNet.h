#pragma once

#include <protocol/net/PacketAssembler.h>
#include <protocol/net/ITransport.h>
#include <protocol/net/BinaryWriter.h>
#include <protocol/net/PacketEnvelope.h> // 🆕 REQUIRED FOR THE WRAPPER!
#include <Game/CommandRouter.h>      
#include <Game/CommandContext.h>     

class GameNet {
public:
    explicit GameNet(ITransport& transport, CommandRouter& router);

    // --- THE FULLY FUNCTIONAL SEND ---
    template<typename... Args>
    void sendCommand(PlayerId to, VerbId verb, Args&&... args) {
        Buffer payload;
        BinaryWriter writer(payload);

        // 1. Write a BLANK Envelope (10 bytes) 📦
        // We use zeroes for size because we haven't written the args yet!
        PacketEnvelope header{ static_cast<uint16_t>(verb), 0, 0, 1 };
        writer.write<PacketEnvelope>(header);

        // 2. Pack the Args using Fold Expressions 🧊
        (writer.write(std::forward<Args>(args)), ...);

        // 3. BACK-PATCH the size! 🎯
        // Calculated as: Total Size - 10 bytes of header
        uint32_t actualSize = static_cast<uint32_t>(payload.size() - sizeof(PacketEnvelope));

        // Offset 2 is the exact location of 'payloadSize' in our 10-byte struct
        std::memcpy(&payload[2], &actualSize, 4);

        // 4. BLAST IT OUT! 🚀
        transport_.send(to, payload);
    }

    void poll();

private:
    ITransport& transport_;
    CommandRouter& router_;
    PacketAssembler assembler_;
};