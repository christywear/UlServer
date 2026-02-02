// LmLogNull.h
#ifndef INCLUDED_LmLogNull
#define INCLUDED_LmLogNull

#include "LmLog.h"

// INHERITANCE FIX: Now inherits from LmLog
class LmLogNull : public LmLog {

public:
    LmLogNull() {
        // THE TRICK: We are a normal logger, but we represent "Closed".
        // The Base class templates check LogOpened(). If false, they do nothing.
        // This effectively makes us a Null Logger without rewriting code.
        SetLogOpened(false);
    }

    virtual ~LmLogNull() {}

    // We can override AssertionFailed if we want to silence crashes
    // (Optional - usually you still want asserts even in null logs)
    // void AssertionFailed(const char* expr, const char* filename, int linenum) override {}

private:
    LmLogNull(const LmLogNull&) = delete;
    LmLogNull& operator=(const LmLogNull&) = delete;
};

#endif /* INCLUDED_LmLogNull */