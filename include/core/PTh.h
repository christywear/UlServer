// PTh.h  -*- C++ -*-
// 🛡️ WINDOWS 11 EDITION: Modern C++ Replacement
// Replaces pthread logic with std::thread inline.

#ifndef INCLUDED_PTh
#define INCLUDED_PTh

#include <stdio.h>
#include <iostream>

#include "LyraDefs.h"
#include "PThAttr.h"

#if defined(UL_WINDOWS) || defined(_WIN32)
#include <thread>
#include <atomic>
#else
#include <pthread.h>
#endif

// PTh class - Abstract Base Class for Threads

class PTh {

public:

    PTh() : running_(false)
#if defined(UL_WINDOWS) || defined(_WIN32)
        , m_stdThread(nullptr)
#endif
    {
    }

    virtual ~PTh() {
        // Ensure we don't leak a running thread
#if defined(UL_WINDOWS) || defined(_WIN32)
        if (m_stdThread) {
            if (m_stdThread->joinable()) {
                m_stdThread->detach(); // or join, depending on desired cleanup
            }
            delete m_stdThread;
            m_stdThread = nullptr;
        }
#endif
    }

    // 🚀 Start the Thread
    int Create(const PThAttr* p_attr = 0) {
        if (running_) return -1; // Already running

#if defined(UL_WINDOWS) || defined(_WIN32)
        running_ = true;
        // Launch std::thread and point it to our static helper
        m_stdThread = new std::thread(&PTh::entry, this);
        return 0;
#else
        // Keep linux logic if needed, or stub
        return -1;
#endif
    }

    int Join(void** status = 0) {
#if defined(UL_WINDOWS) || defined(_WIN32)
        if (m_stdThread && m_stdThread->joinable()) {
            m_stdThread->join();
            return 0;
        }
        return -1;
#else
        return 0;
#endif
    }

    // Legacy method stubs
    int Cancel() { return 0; }

    bool IsRunning() const { return running_; }
    bool IsActive() const { return running_; }

    // Dump State
    void Dump(FILE* f, int indent = 0) const {
        // fprintf(f, "Thread Running: %d\n", running_);
    }

protected:

    // Called by the thread loop when it finishes
    void DoneRunning() {
        running_ = false;
    }

    // Sleep Helper
    void YieldSlice() {
#if defined(UL_WINDOWS) || defined(_WIN32)
        std::this_thread::yield();
#endif
    }

    // THE REAL WORKER FUNCTION (Must be implemented by subclasses like GsGameThread)
    virtual void Run() = 0;

private:

    // Static entry point to bridge C++ classes with OS threads
    static void entry(void* arg) {
        PTh* pThis = static_cast<PTh*>(arg);
        if (pThis) {
            pThis->Run();         // Execute the logic
            pThis->DoneRunning(); // Cleanup flag
        }
    }

    // Prevent copying
    PTh(const PTh&);

    // State
    std::atomic<bool> running_;

#if defined(UL_WINDOWS) || defined(_WIN32)
    std::thread* m_stdThread;
#else
    pthread_t thread_;
#endif

};

#endif /* INCLUDED_PTh */