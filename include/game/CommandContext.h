#pragma once
#include <protocol/net/NetTypes.h>
#include <protocol/net/NetworkInterface.h>
#include <Game/Verbs.h> // <--- REQUIRED so we know what VerbId is

class CommandContext {
public:
    explicit CommandContext(PlayerId from) : sender_(from) {}
    PlayerId Sender() const { return sender_; }

    // --- NETWORK SEND HELPERS ---

    template<typename... Args>
    void Reply(VerbId verb, Args... args) {
        // FIX: static_cast<int>(verb) is REQUIRED for enum classes
        NetworkInterface::Send(sender_, static_cast<int>(verb), args...);
    }

    template<typename... Args>
    void SendTo(PlayerId to, VerbId verb, Args... args) {
        // FIX: static_cast<int>(verb)
        NetworkInterface::Send(to, static_cast<int>(verb), args...);
    }

private:
    PlayerId sender_;
};