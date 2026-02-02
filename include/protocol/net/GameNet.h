#pragma once
#include <protocol/net/ITransport.h>
#include <protocol/net/NetTypes.h>
#include <mutex>
#include <deque>
#include <vector>

class GameNet {
public:
    explicit GameNet(ITransport& transport);
    ~GameNet();

    static GameNet* Instance() { return instance_; }

    void Poll();
    void Update();

    // ENCAPSULATION FIX:
    // We only accept raw Bytes. We don't care what the header looks like.
    // We use std::move to enforce "Delete its version once sent" (ownership transfer).
    void EnqueuePacket(PlayerId target, Buffer&& rawBytes);

private:
    static GameNet* instance_;
    ITransport& transport_;

    // Internal Queue: Just ID + Bytes
    struct QueuedItem {
        PlayerId target;
        Buffer   data;
    };

    std::mutex queueMutex_;
    std::deque<QueuedItem> sendQueue_;
};