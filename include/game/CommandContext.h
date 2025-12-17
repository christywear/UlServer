#pragma once

// Don't include GameNet.h here! Just forward declare. 🛡️
class GameNet;
class World; // You'll probably want this later too!

struct CommandContext {
    GameNet& net;
    // World& world; // Example of other tools

    // We MUST provide a constructor because of the reference!
    CommandContext(GameNet& netPtr) : net(netPtr) {}
};