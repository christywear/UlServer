// include/core/PThCond.h
// 🛡️ WINDOWS 11 EDITION: Modern C++ Replacement
// Wraps std::condition_variable_any to mimic POSIX pthread_cond

#ifndef INCLUDED_PThCond
#define INCLUDED_PThCond

#include "LyraDefs.h"
#include "PThMutex.h"

#if defined(UL_WINDOWS) || defined(_WIN32)
#include <condition_variable>
#include <mutex>
#include <chrono>

class PThCond {
public:
    PThCond() {}
    ~PThCond() {}

    int Init() { return 0; } // Auto-init in C++

    // Wait for the condition.
    // Requires mutex to be locked by the caller!
    int Wait(PThMutex* mutex) {
        // 1. Get reference to underlying std::recursive_mutex
        std::recursive_mutex& std_mtx = (std::recursive_mutex&)(*mutex);

        // 2. Adopt the lock (tell C++ "we already own this")
        std::unique_lock<std::recursive_mutex> lk(std_mtx, std::adopt_lock);

        // 3. Wait (atomically unlocks, waits, relocks)
        m_cond.wait(lk);

        // 4. Release ownership so lk destructor doesn't unlock it
        // (The caller logic expects to still hold the lock)
        lk.release();

        return 0;
    }

    // Timed Wait
    int TimedWait(PThMutex* mutex, const struct timespec* abstime) {
        std::recursive_mutex& std_mtx = (std::recursive_mutex&)(*mutex);
        std::unique_lock<std::recursive_mutex> lk(std_mtx, std::adopt_lock);

        // Convert timespec to chrono duration roughly
        // Note: This is a loose conversion for porting; precise absolute time 
        // mapping on Windows varies, but this is usually sufficient for timeouts.
        auto now_sec = std::chrono::system_clock::now();
        auto target_sec = std::chrono::seconds(abstime->tv_sec) + std::chrono::nanoseconds(abstime->tv_nsec);
        // We treat the abstime as a duration from Epoch, roughly matching system_clock
        std::chrono::system_clock::time_point tp(std::chrono::duration_cast<std::chrono::system_clock::duration>(target_sec));

        m_cond.wait_until(lk, tp);

        lk.release();
        return 0;
    }

    int Signal() {
        m_cond.notify_one();
        return 0;
    }

    int Broadcast() {
        m_cond.notify_all();
        return 0;
    }

private:
    // "any" allows us to use recursive_mutex
    std::condition_variable_any m_cond;
};

#else
    // 🐧 LINUX: Legacy Definition (Keep original if compiling for Linux later)
#include <pthread.h>
class PThCond {
public:
    PThCond();
    ~PThCond();
    int Init();
    int Wait(PThMutex* mutex);
    int TimedWait(PThMutex* mutex, const struct timespec* abstime);
    int Signal();
    int Broadcast();
private:
    pthread_cond_t cond_;
};
#endif

#endif /* INCLUDED_PThCond */