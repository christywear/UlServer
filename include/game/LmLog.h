#ifndef LMLOG_H
#define LMLOG_H

#include <vector>
#include <utility>
#include <string>
#include <tchar.h>
#include <core/lyradefs.h>

// STOP! Do not include RMsg headers here. It causes circular dependency hell.
// We don't need to know what RMsg is if we are just ignoring it.

// --------------------------------------------------------------------
//  ARGUMENT SANITIZERS
// --------------------------------------------------------------------
template <typename T> auto LogArg(T&& t) { return std::forward<T>(t); }
inline const char* LogArg(const std::string& s) { return s.c_str(); }
inline const wchar_t* LogArg(const std::wstring& s) { return s.c_str(); }

class LmLog {
public:
    LmLog();
    virtual ~LmLog();
    static LmLog* Instance() { return s_instance; }

    // --- MODERN INTERFACE (Keep these) ---
    template <typename... Args>
    void Log(const std::string& fmt, Args&&... args) {
        if (LogOpened()) WriteLogA("LOG ", fmt.c_str(), LogArg(std::forward<Args>(args))...);
    }
    template <typename... Args>
    void Warning(const std::string& fmt, Args&&... args) {
        if (LogOpened()) WriteLogA("WARN ", fmt.c_str(), LogArg(std::forward<Args>(args))...);
    }
    template <typename... Args>
    void Debug(const std::string& fmt, Args&&... args) {
        if (LogOpened()) WriteLogA("DEBUG ", fmt.c_str(), LogArg(std::forward<Args>(args))...);
    }
    template <typename... Args>
    void Error(const std::string& fmt, Args&&... args) {
        if (LogOpened()) WriteLogA("ERR ", fmt.c_str(), LogArg(std::forward<Args>(args))...);
    }

<<<<<<< Updated upstream
  void SetLogLevel(int level);
  int LogLevel() const;
  bool LogOpened() const;
=======
    // --- LEGACY INTERFACE (Wide Chars) ---
    template <typename... Args>
    void Log(const wchar_t* fmt, Args&&... args) {
        if (LogOpened()) WriteLogW(L"LOG ", fmt, LogArg(std::forward<Args>(args))...);
    }
    template <typename... Args>
    void Warning(const wchar_t* fmt, Args&&... args) {
        if (LogOpened()) WriteLogW(L"WARN ", fmt, LogArg(std::forward<Args>(args))...);
    }
    template <typename... Args>
    void Debug(const wchar_t* fmt, Args&&... args) {
        if (LogOpened()) WriteLogW(L"DEBUG ", fmt, LogArg(std::forward<Args>(args))...);
    }
    template <typename... Args>
    void Error(const wchar_t* fmt, Args&&... args) {
        if (LogOpened()) WriteLogW(L"ERR ", fmt, LogArg(std::forward<Args>(args))...);
    }
>>>>>>> Stashed changes

    // --- LEGACY REDIRECTS (Int Level) ---
    template <typename... Args>
    void Debug(int level, const wchar_t* fmt, Args&&... args) {
        Debug(fmt, std::forward<Args>(args)...);
    }

    // ----------------------------------------------------------------
    // THE BLACK HOLE: SECURITY
    // ----------------------------------------------------------------
    // This swallows ANY call to Security(...) regardless of arguments.
    // It fixes the RMsg errors because we don't even check the types.
    template <typename... Args>
    void Security(Args&&... args) {
        // Do nothing. Compiler optimizes this away to a NOP. stubby stub to continue debugging
    }

    // ----------------------------------------------------------------

    void WriteLog(const char* prefix, const char* fmt, ...);
    virtual void AssertionFailed(const TCHAR* expr, const TCHAR* filename, int linenum);

    void SetLogLevel(int level);
    int  LogLevel() const;
    void SetLogOpened(bool opened);
    bool LogOpened() const;
    void FlushLog();

private:
<<<<<<< Updated upstream

  // methods/operations not implemented
  LmLog(const LmLog&);
  //operator=(const LmLog&);

  int log_level_;
  bool log_opened_;
=======
    void WriteLogA(const char* prefix, const char* fmt, ...);
    void WriteLogW(const wchar_t* prefix, const wchar_t* fmt, ...);
>>>>>>> Stashed changes

    int  log_level_;
    bool log_opened_;
    static LmLog* s_instance;
};

#endif