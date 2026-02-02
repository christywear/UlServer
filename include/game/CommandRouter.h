#pragma once

/*
USAGE:
CommandRouter router;
router.registerHandler(VerbId::CAST_ART, [](PlayerId from, BinaryReader& r, CommandContext& ctx) {
    // ... logic ...
});
*/

#include <unordered_map>
#include <functional>
#include <protocol/net/NetTypes.h>
#include <Game/Verbs.h>
#include <protocol/net/BinaryReader.h>
#include <Game/CommandContext.h>

class CommandRouter {
public:
    using Handler = std::function<void(PlayerId from, BinaryReader& reader, CommandContext& ctx)>;

    void registerHandler(VerbId verb, Handler handler) {
        handlers_[verb] = std::move(handler);
    }

    // Called by PacketManager after reassembly
    void dispatch(PlayerId from, VerbId verb, const Buffer& payload)
    {
        auto it = handlers_.find(verb);
        if (it == handlers_.end()) {
            return; // Unknown verb
        }

        // 1. Setup the Reader
        BinaryReader reader(payload);

        // 2. Setup the Context (No GameNet needed!)
        CommandContext ctx(from);

        // 3. Run Logic
        it->second(from, reader, ctx);
    }

private:
    std::unordered_map<VerbId, Handler> handlers_;
};