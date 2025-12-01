// Platform.h  (very lightweight, no heavy includes)
#pragma once

// Normalize Windows detection
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(win32)
#ifndef UL_WINDOWS
#define UL_WINDOWS 1
#endif
#endif

// Normalize POSIX-ish (Linux, BSD, macOS, etc.)
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#ifndef UL_POSIX
#define UL_POSIX 1
#endif
#endif

// --------------------------------------------------------------------
// Legacy compatibility shims
// These exist so old code using WIN32 / win32 continues to behave
// correctly while you migrate toward UL_WINDOWS / UL_POSIX.
// --------------------------------------------------------------------

#if defined(UL_WINDOWS)
  // If old code checks WIN32 or win32, make sure they see "Windows".
#ifndef WIN32
#define WIN32 1
#endif
#ifndef win32
#define win32 1
#endif
#endif
#ifndef UL_SERVER_WIN32 //added to fix pid_t missing
#define UL_SERVER_WIN32 1
#endif
// You can add more later if needed, e.g. UL_MACOS, UL_BSD, etc.
// #if defined(__APPLE__)
//   #define UL_MACOS 1
// #endif
