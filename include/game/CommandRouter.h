#pragma once

#include <unordered_map>
#include <functional>
#include <cstdint>
#include <protocol/net/NetTypes.h>
#include <protocol/net/BinaryReader.h>
#include <Game/Verbs.h>
#include <Game/CommandContext.h>

class CommandRouter {
public:
    // This is the "Blueprint" for a handler function
    using Handler = std::function<void(PlayerId, BinaryReader&, CommandContext&)>;

    // 1. Register: Map a Verb (like MOVE) to a Lambda/Function
    void registerHandler(VerbId verb, Handler handler) {
        handlers_[static_cast<uint16_t>(verb)] = std::move(handler);
    }

    // 2. Dispatch: Find the right code and EXECUTE IT! ⚡
    void dispatch(PlayerId from, VerbId verb, Buffer& payload, CommandContext& ctx) {
        auto it = handlers_.find(static_cast<uint16_t>(verb));
        if (it != handlers_.end()) {
            // We create the reader RIGHT HERE so the handler can just start reading
            BinaryReader reader(payload.data(), payload.size());
            it->second(from, reader, ctx);
        }
    }

private:
    // A simple map: VerbID -> Code to run
    std::unordered_map<uint16_t, Handler> handlers_;
};