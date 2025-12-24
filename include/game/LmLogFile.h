// LmLogFile.h  -*- C++ -*-
// $Id: LmLogFile.h,v 1.3 1997-11-17 14:05:36-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// Lyra log class based on stdio files, thread safe

#ifndef INCLUDED_LmLogFile
#define INCLUDED_LmLogFile

#ifdef __GNUC__
#pragma interface
#endif

#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>

#include "..\Core\LyraDefs.h"
#include "..\Game\LmLog.h"
#include "..\Core\PThMutex.h"

// log class

class LmLogFile : public LmLog {

public:

  LmLogFile();
  virtual ~LmLogFile();
  //public accessor
  static LmLogFile* Instance() { return s_instance; }
  void Init(const TCHAR* prefix, const TCHAR* suffix = _T(""), int instance = 0, pid_t pid = 0);
  int Open(const TCHAR* directory);
  void UseStream(FILE* stream);
  void Close(bool verbose = true);
  void FlushLog();

  FILE* Stream();

protected:

  virtual void WriteLogEntry(const TCHAR* prefix, const TCHAR* fmt, va_list args);

private:
    //private accessor
    static LmLogFile* s_instance;
  // methods/operations not implemented
  LmLogFile(const LmLogFile&);
  //  operator=(const LmLogFile&);

  void flush_log();
  void write_header(const TCHAR* str, bool flush = false);

  // constants
  enum {
    MAX_HOSTNAME = 256, // maximum hostname length
    MAX_PROGNAME = 32,  // maximum program name length
    MAX_LOGNAME = 64    // maximum logfile name (without directory)
  };

  // data members
  PThMutex lock_;
  FILE* fp_;
  int instance_;
  pid_t pid_;
  //TCHAR hostname_[MAX_HOSTNAME];
  TCHAR progname_[MAX_PROGNAME];
  TCHAR logname_[MAX_LOGNAME];

};

#endif /* INCLUDED_LmLogFile */
