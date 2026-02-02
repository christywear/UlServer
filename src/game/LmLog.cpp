// LmLog.cpp
// Modern Implementation

#ifdef __GNUC__
#pragma implementation "LmLog.h"
#endif



#include "../../include/Core/LyraDefs.h"
#include "../../include/Game/LmLog.h"
#include <cstdarg>
#include <cstdio>
#include <vector>
#include <iostream>

<<<<<<< Updated upstream

////
// Constructor
////

LmLog::LmLog()
  : log_level_(0),
    log_opened_(false)
{
  // empty
}

////
// Destructor
////

LmLog::~LmLog()
{
  // empty
=======
LmLog* LmLog::s_instance = nullptr;

LmLog::LmLog() : log_level_(0), log_opened_(false) {
    s_instance = this;
}

LmLog::~LmLog() {
    if (s_instance == this) s_instance = nullptr;
>>>>>>> Stashed changes
}

void LmLog::SetLogLevel(int level) { log_level_ = level; }
int  LmLog::LogLevel() const { return log_level_; }
void LmLog::SetLogOpened(bool opened) { log_opened_ = opened; }
bool LmLog::LogOpened() const { return log_opened_; }

// ----------------------------------------------------------------
//  THE WORKER
// ----------------------------------------------------------------
// This receives already-sanitized arguments from the templates.
// 1. Strings are already converted to char*
// 2. Ints are Ints
// 3. We just format and print.

void LmLog::WriteLog(const char* prefix, const char* fmt, ...)
{
    // Start Variadic Processing
    va_list args;
    va_start(args, fmt);

    // Dynamic Buffer Sizing Strategy
    // We start with 1KB. If the message is huge, we grow.
    std::vector<char> buffer(1024);

    while (true) {
        va_list args_copy;
        va_copy(args_copy, args);

        // vsnprintf returns how many chars we NEED if the buffer is too small
        int needed = vsnprintf(buffer.data(), buffer.size(), fmt, args_copy);

        va_end(args_copy);

        if (needed < 0) {
            // Encoding error
            break;
        }

        if (needed < (int)buffer.size()) {
            // Success! It fit.
            // OUTPUT TO CONSOLE (Replace this line with your File writing logic)
            fprintf(stdout, "%s%s\n", prefix, buffer.data());
            break;
        }

        // Buffer too small? Resize to exactly what we need (+1 for null terminator)
        buffer.resize(needed + 1);
    }

    va_end(args);
}

void LmLog::AssertionFailed(const TCHAR* expr, const TCHAR* filename, int linenum) {
    // Force open so we see the crash
    bool wasOpen = LogOpened();
    SetLogOpened(true);

    WriteLog("ASSERT", "Assertion failed: %s at %s:%d", expr, filename, linenum);

    // Restore state (though we probably crashed anyway)
    SetLogOpened(wasOpen);
}
// ----------------------------------------------------------------
//  ANSI WORKER (For New Code)
// ----------------------------------------------------------------
void LmLog::WriteLogA(const char* prefix, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::vector<char> buffer(1024);

    while (true) {
        va_list args_copy;
        va_copy(args_copy, args);
        int needed = vsnprintf(buffer.data(), buffer.size(), fmt, args_copy);
        va_end(args_copy);

        if (needed < 0) break;
        if (needed < (int)buffer.size()) {
            // Print Prefix + Message
            fprintf(stdout, "%s%s\n", prefix, buffer.data());
            break;
        }
        buffer.resize(needed + 1);
    }
    va_end(args);
}

// ----------------------------------------------------------------
//  UNICODE WORKER (For Legacy Code)
// ----------------------------------------------------------------
void LmLog::WriteLogW(const wchar_t* prefix, const wchar_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::vector<wchar_t> buffer(1024);

    while (true) {
        va_list args_copy;
        va_copy(args_copy, args);
        // Use the Wide version of vsnprintf
        int needed = _vsnwprintf(buffer.data(), buffer.size(), fmt, args_copy);
        va_end(args_copy);

        // _vsnwprintf behavior varies slightly on return values, 
        // but generally returns count or -1 if truncation/error.
        if (needed < 0 && needed != -1) break; // Real error

        // If it fits (needed >= 0 and < size) OR (needed == -1 but we want to be safe)
        // Note: MSVC _vsnwprintf returns -1 if it doesn't fit. Standard is 'needed'.
        // Simple resizing logic for Windows:
        if (needed >= 0 && needed < (int)buffer.size()) {
            // Success! Print (Use wprintf for wide chars)
            fwprintf(stdout, L"%s%s\n", prefix, buffer.data());
            break;
        }

        // Resize strategy: Double it if -1, or set to needed+1
        size_t newSize = (needed > 0) ? (needed + 1) : (buffer.size() * 2);
        buffer.resize(newSize);
    }
    va_end(args);
}

void LmLog::FlushLog()
{
    //stub
}