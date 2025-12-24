// include/core/PThMutex.h
#ifndef INCLUDED_PThMutex
#define INCLUDED_PThMutex

#include "../platform/Platform.h"
// ❌ REMOVED: #include <core/LmLocker.h> (Prevents circular dependency)

#if defined(UL_WINDOWS) || defined(_WIN32)
#include <mutex>

// 🛡️ WINDOWS: Modern C++ Implementation
class PThMutex {
public:
    PThMutex() {}
    ~PThMutex() {}

    void Init() { /* Auto-inits */ }

    void Lock() {
        m_mutex.lock();
    }

    void Unlock() {
        // 🤫 Tell VS Analyzer to ignore "Caller failing to hold lock" warning
#pragma warning( push )
#pragma warning( disable : 26110 )
        m_mutex.unlock();
#pragma warning( pop )
    }

    // 🛠️ FIX: Alias for legacy code using "UnLock"
    void UnLock() { Unlock(); }

private:
    std::recursive_mutex m_mutex;
};

#else
    // 🐧 LINUX: Keep Original Logic
#include <pthread.h>
class PThMutex {
public:
    PThMutex();
    ~PThMutex();
    void Init();
    void Lock();
    void Unlock();
    void UnLock(); // Ensure linux side matches if needed
private:
    pthread_mutex_t mutex_;
};
#endif

#endif // INCLUDED_PThMutex