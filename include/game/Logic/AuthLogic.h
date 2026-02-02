#pragma once
#include <protocol/net/NetTypes.h>
#include <protocol/net/BinaryReader.h>
#include <Game/CommandContext.h>

namespace AuthLogic {
    // Phase 1: Check Version & Status
    void HandlePreLogin(PlayerId from, BinaryReader& reader, CommandContext& ctx);

    // Phase 2: Check Credentials
    void HandleLogin(PlayerId from, BinaryReader& reader, CommandContext& ctx);
}