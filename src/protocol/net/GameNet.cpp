#include <protocol/net/GameNet.h>
#include <protocol/net/PacketManager.h>
#include <cstdint>
#include <mutex>
#include <utility>
#include <protocol/net/ITransport.h>
#include <protocol/net/NetTypes.h>

GameNet* GameNet::instance_ = nullptr;

GameNet::GameNet(ITransport& transport) : transport_(transport) {
    instance_ = this;
    transport_.setHandler([](PlayerId from, const uint8_t* data, size_t size) {
        PacketManager::Get()->ProcessIncoming(from, data, size);
    });
}

GameNet::~GameNet() { instance_ = nullptr; }

void GameNet::Poll() {
    transport_.poll();
}

void GameNet::EnqueuePacket(PlayerId target, Buffer&& rawBytes) {
    std::lock_guard<std::mutex> lock(queueMutex_);
    // Emplace constructs the struct in place, moving the vector
    sendQueue_.push_back({ target, std::move(rawBytes) });
}

void GameNet::Update() {
    std::lock_guard<std::mutex> lock(queueMutex_);

    while (!sendQueue_.empty()) {
        auto& item = sendQueue_.front();

        // Pass directly to transport
        transport_.send(item.target, item.data, SendMode::Reliable);

        sendQueue_.pop_front();
    }
}