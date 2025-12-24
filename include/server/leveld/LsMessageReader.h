// LsMessageReader.h  -*- C++ -*-
// $Id: LsMessageReader.h,v 1.9 1997-09-16 03:03:04-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// level server message reader thread

#ifndef INCLUDED_LsMessageReader
#define INCLUDED_LsMessageReader

#ifdef __GNUC__
#pragma interface
#endif

#include <stdio.h>

#include "..\..\Core\LyraDefs.h"
#include "..\..\Protocol\LmMessageReader.h"
#include <Core/LmThread.h>

// class forward declarations

class LmLog;

// class declarations

class LsMessageReader : public LmMessageReader {

public:

  LsMessageReader(LmLog* log);
  ~LsMessageReader();

  void Dump(FILE* f, int indent = 0) const;

protected:

  void Run();
  bool HandleError(LmConnection* conn, int errcode, int mtype, int msize);

private:

 

};

#endif /* INCLUDED_LsMessageReader */
