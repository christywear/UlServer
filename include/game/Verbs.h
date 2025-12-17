#pragma once
#include <cstdint>

enum class VerbId : uint16_t
{
    INVALID = 0,

    // --- player actions ---
    CAST_ART = 1,
    MOVE_UPDATE = 2,
    EMOTE = 3,
    CHAT_MESSAGE = 4,

    // --- server responses ---
    ART_RESULT = 100,
    STAT_UPDATE = 101,
    POSITION_SYNC = 102,

    // --- GM / admin ---
    GM_SUMMON = 200,
    GM_TELEPORT = 201,
};
