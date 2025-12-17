#include <Game/ServerHandlers.h>
#include <Game/CommandRouter.h>  // <--- NOW THIS HAS THE CLASS DEFINITION! ✅
#include <Game/CommandContext.h>
#include <Game/Verbs.h>
#include <protocol/net/BinaryReader.h>
#include <iostream>

namespace ULNet {

    void RegisterHandlers(CommandRouter& router) {

        // --- MOVEMENT HANDLER ---
        router.registerHandler(VerbId::MOVE_UPDATE, [](PlayerId from, BinaryReader& reader, CommandContext& ctx) {
            float x = reader.read<float>();
            float y = reader.read<float>();
            float z = reader.read<float>();
            std::cout << "🏃 Player " << from << " moved to (" << x << ", " << y << ", " << z << ")" << std::endl;
        });

        // --- COMBAT HANDLER ---
        router.registerHandler(VerbId::CAST_ART, [](PlayerId from, BinaryReader& reader, CommandContext& ctx) {
            uint32_t artId = reader.read<uint32_t>();
            uint32_t targetId = reader.read<uint32_t>();
            std::cout << "⚔️ Player " << from << " cast Art " << artId << " on " << targetId << "!" << std::endl;
        });

        // --- CHAT HANDLER ---
        router.registerHandler(VerbId::CHAT_MESSAGE, [](PlayerId from, BinaryReader& reader, CommandContext& ctx) {
            std::string msg = reader.readString();
            std::cout << "💬 [Chat] Player " << from << ": " << msg << std::endl;
        });
    }
}