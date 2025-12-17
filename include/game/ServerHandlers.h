#pragma once

#include <protocol/net/NetTypes.h>

// Forward declaration so we don't need the full header here! 🛡️
class CommandRouter;

namespace ULNet {

    // This plugs the logic into the router!
    void RegisterHandlers(CommandRouter& router);

}