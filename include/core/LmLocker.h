// include/core/LmLocker.h
#ifndef INCLUDED_LmLocker
#define INCLUDED_LmLocker

#include "PThMutex.h"

#if defined(UL_WINDOWS) || defined(_WIN32)
    // 🛡️ WINDOWS: Simple Scope Guard
class LmLocker {
public:
    // Lock on creation
    LmLocker(PThMutex& mutex) : m_mutex(mutex) {
        m_mutex.Lock();
    }

    // Unlock on destruction (scope exit)
    ~LmLocker() {
        m_mutex.Unlock();
    }

private:
    PThMutex& m_mutex;
};

#else
    // 🐧 LINUX: Legacy Definition
class LmLocker {
public:
    LmLocker(PThMutex& mutex);
    ~LmLocker();
private:
    PThMutex* mutex_;
};
#endif

#endif // INCLUDED_LmLocker