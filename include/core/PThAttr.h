// include/core/PThAttr.h
#ifndef INCLUDED_PThAttr
#define INCLUDED_PThAttr

#include "../platform/Platform.h"

#if defined(UL_WINDOWS) || defined(_WIN32)
    // 🛡️ WINDOWS: Dummy Implementation
    // std::thread doesn't need these attributes in the same way
class PThAttr {
public:
    PThAttr() {}
    ~PThAttr() {}

    void Init() {}
    // All these do nothing on Windows now, preventing errors
    void SetStackSize(int size) { /* Ignored */ }
    void SetJoinable(bool joinable) { /* Ignored */ }
    void SetDetachState(int state) { /* Ignored */ }
    void SetScope(int scope) { /* Ignored */ }
};

#else
    // 🐧 LINUX: Legacy Definition
#include <pthread.h>
class PThAttr {
public:
    PThAttr();
    ~PThAttr();
    void Init();
    void SetStackSize(int size);
    void SetJoinable(bool joinable);
    void SetDetachState(int state);
    void SetScope(int scope);
    pthread_attr_t* Attr() { return &attr_; }
private:
    pthread_attr_t attr_;
};
#endif

#endif // INCLUDED_PThAttr