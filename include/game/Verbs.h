// src/Game/Verbs.h
#pragma once
#include <cstdint>

enum class VerbId : uint16_t {
    PRELOGIN = 0x0001,
    PRELOGIN_ACK = 0x0002,
    LOGIN_ACK = 0x0003,
    MOVE_UPDATE = 0x0004,
    CAST_ART = 0x0005,
    CHAT_MESSAGE = 0x0006,
};
