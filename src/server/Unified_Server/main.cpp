// src/UnifiedMain.cpp
// 🚀 THE UNIFIED HEART OF UNDERLIGHT 2025
// Merges Master, Game, and Level servers into one Windows 11 process.

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <tchar.h> // REQUIRED for _tmain and TCHAR

// 1. Include Platform & Modern Net
#include "../include/platform/Platform.h"
#include "../include/protocol/net/AutoTransport.h"
#include "../include/protocol/net/GameNet.h"
#include "../include/Game/CommandRouter.h"
#include "../include/Game/ServerHandlers.h"


// --- MAIN ENTRY (Using _tmain for TCHAR compatibility) --------------------

int _tmain(int argc, TCHAR* argv[]) {
    // 1. Force Console to UTF-16 Mode (Optional, keeps logs clean)
    // _setmode(_fileno(stdout), _O_U16TEXT); 

    std::wcout << L"🌌 STARTING UNDERLIGHT UNIFIED SERVER (Windows 11 Edition) 🚀" << std::endl;

    // 1. Root Directory Setup
    // Now argv matches TCHAR automatically!
    const TCHAR* rootDir = (argc > 1) ? argv[1] : _T(".");

    std::wcout << L"📂 Root Directory: " << rootDir << std::endl;

    // 2. Initialize Modern Networking
    std::cout << "🔌 Initializing Modern Network Stack..." << std::endl;
    auto transport = std::make_unique<AutoTransport>(AutoTransport::Mode::ENET);
    auto router = std::make_unique<CommandRouter>();
    ULNet::RegisterHandlers(*router);
    auto net = std::make_unique<GameNet>(*transport, *router);


    // 5. THE MASTER LOOP
    // 5. THE MASTER LOOP
    std::cout << "💓 Unified Server Loop Running (Ctrl+C to stop)..." << std::endl;

    // We need a loop to keep the main thread alive!
    bool running = true;
    while (running) {
        // 1. Poll the network for new packets
        net->poll();

        // 2. Don't melt the CPU (sleep for 1ms)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // 3. Optional: Check for GsConfig::SigTerm() to exit gracefully
        /*
        if (GsConfig::SigTerm()) {
            running = false;
        }
        */
    }

    std::cout << "🛑 Server shutting down..." << std::endl;
    return 0;
    return 0;
}