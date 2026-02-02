#include <Game/ServerHandlers.h>
#include <Game/CommandRouter.h>
#include <Game/CommandContext.h>
#include <Game/Verbs.h>
#include <protocol/net/BinaryReader.h>

// --- LOGIC SYSTEMS ---
#include <Game/Logic/AuthLogic.h> 
#include <protocol/net/NetTypes.h>
#include <cstdint>
#include <string>
// #include <Game/Logic/CombatSystem.h>   <-- Add these as you build them
// #include <Game/Logic/MovementSystem.h> 

namespace ULNet {

    void RegisterHandlers(CommandRouter& router) {

        // -------------------------------------------------
        // 🔑 AUTHENTICATION (Forward to AuthLogic)
        // -------------------------------------------------
        router.registerHandler((VerbId)VerbId::PRELOGIN,
            [](PlayerId from, BinaryReader& reader, CommandContext& ctx)
        {
            // Just forward the tools to AuthLogic. Let it do the work.
            AuthLogic::HandlePreLogin(from, reader, ctx);
        });

        // -------------------------------------------------
        // 🏃 MOVEMENT (Forward to Movement System)
        // -------------------------------------------------
        router.registerHandler((VerbId)VerbId::MOVE_UPDATE,
            [](PlayerId from, BinaryReader& reader, CommandContext& ctx)
        {
            float x = reader.read<float>();
            float y = reader.read<float>();
            float z = reader.read<float>();
            float rot = reader.read<float>();

            // "Hey MovementSystem, update this guy."
            // MovementSystem::UpdatePosition(from, x, y, z, rot);
        });

        // -------------------------------------------------
        // ⚔️ COMBAT (Forward to Combat System)
        // -------------------------------------------------
        router.registerHandler((VerbId)VerbId::CAST_ART,
            [](PlayerId from, BinaryReader& reader, CommandContext& ctx)
        {
            uint32_t artId = reader.read<uint32_t>();
            uint32_t targetId = reader.read<uint32_t>();

            // "Hey CombatSystem, queue this spell."
            // CombatSystem::QueueCast(from, targetId, artId);

            // Optional: Send immediate technical ACK if needed
            // ctx.Reply(VerbId::ACK, (uint16_t)VerbId::CAST_ART); 
        });

        // -------------------------------------------------
        // 💬 CHAT (Forward to Chat Manager)
        // -------------------------------------------------
        router.registerHandler((VerbId)VerbId::CHAT_MESSAGE,
            [](PlayerId from, BinaryReader& reader, CommandContext& ctx)
        {
            std::string msg = reader.readString();
            std::string channel = reader.readString();

            // "Hey ChatManager, broadcast this."
            // ChatManager::Broadcast(from, channel, msg);
        });
    }
}