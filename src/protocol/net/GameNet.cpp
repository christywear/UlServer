#include <protocol/net/GameNet.h>
#include <protocol/net/BinaryWriter.h>
#include <Game/Verbs.h>
#include <Game/CommandRouter.h> 
#include <Game/CommandContext.h> 
#include <iostream>
#include <protocol/net/ITransport.h>

GameNet::GameNet(ITransport& transport, CommandRouter& router)
    : transport_(transport), router_(router)
{
    // --- THE RECEIVE WIRING 📥 ---
    transport_.setHandler([this](PlayerId from, const uint8_t* data, size_t size) {

        // The Assembler handles the "Receive" logic by stitching fragments
        assembler_.feed(data, size, [this, from](VerbId verb, const uint8_t* payload, size_t pSize) {

            // When a full packet is ready, convert to Buffer
            Buffer buf(payload, payload + pSize);

            // Context gives the handler access back to this GameNet instance
            CommandContext ctx(*this);

            // DISPATCH: The Brain finds the code and runs it! 🧠
            router_.dispatch(from, verb, buf, ctx);
        });
    });

    std::cout << "🎮 GameNet: Send/Receive Pipeline Wired to Transport!" << std::endl;
}

void GameNet::poll() {
    transport_.poll(); // Keeps the heart beating! 💓
}