// src/UnifiedMain.cpp
// 🚀 THE UNIFIED HEART OF UNDERLIGHT 2025
// Merges Master, Game, and Level servers into one Windows 11 process.

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <tchar.h> 

#include "../include/platform/Platform.h"
#include "../include/protocol/net/AutoTransport.h" 
#include "../include/protocol/net/GameNet.h"
#include "../include/protocol/net/PacketManager.h" 
#include "../include/Game/CommandRouter.h"
#include "../include/Game/ServerHandlers.h"        
#include "../include/Game/LmLog.h"                 

int _tmain(int argc, TCHAR* argv[]) {
    // 1. SETUP LOGGER
    LmLog logger;
    logger.SetLogOpened(true);
    logger.Log("SERVER STARTUP: Unified Process");

    const TCHAR* rootDir = (argc > 1) ? argv[1] : _T(".");
    logger.Log(_T("Root Directory: %s"), rootDir);

    // 2. INITIALIZE NETWORK HARDWARE
    // "Create()" spins up the listeners (ENet Host + Steam Callbacks).
    // It is now ready to accept Reliable (TCP-mode) and Unreliable (UDP-mode) 
    // packets from any source.
    logger.Log("Initializing Network Listeners...");
    auto transport = AutoTransport::Create();

    if (!transport) {
        logger.Error("FATAL: Failed to initialize network transport.");
        return -1;
    }

    // 3. WIRE THE PUMP (GameNet)
    // GameNet pumps the transport we just created.
    auto net = std::make_unique<GameNet>(*transport);

    // 4. WIRE THE BRAIN (Router)
    auto router = std::make_unique<CommandRouter>();
    ULNet::RegisterHandlers(*router);
    logger.Log("Logic Handlers Registered.");

    // Connect Router to PacketManager
    PacketManager::Get()->SetRouter(router.get());

    // 5. THE SYSTEM HEARTBEAT
    // Main's only job is to provide the 'Tick'.
    logger.Log("SYSTEM READY. Entering Main Loop.");

    bool running = true;
    while (running) {
        // TICK NETWORK
        // Poll: Reads incoming packets (Reliable & Unreliable) from all ports.
        net->Poll();

        // Update: Flushes outgoing queues (Reliable & Unreliable) to clients.
        net->Update();

        // CPU Saver (1000 ticks/sec)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    logger.Log("Server shutting down...");
    return 0;
}