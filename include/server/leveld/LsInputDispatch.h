// LsInputDispatch.h  -*- C++ -*-
// $Id: LsInputDispatch.h,v 1.7 1997-07-14 19:25:22-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// level server input message dispatcher

#ifndef INCLUDED_LsInputDispatch
#define INCLUDED_LsInputDispatch

#ifdef __GNUC__
#pragma interface
#endif

#include "..\..\Core\LyraDefs.h"
#include "..\..\Protocol\LmDispatch.h"

// class forward declarations


class LmSrvMesgBuf;
class LmConnection;
class LmThread;

// the class

class LsInputDispatch : public LmDispatch {

public:

  LsInputDispatch();
  ~LsInputDispatch();
  //public accessor
  static LsInputDispatch* Instance() { return s_instance; }
  void Dump(FILE* f, int indent = 0) const;
  LmThread* ComputeTarget(LmSrvMesgBuf* mbuf, LmConnection* conn);
protected:

  

private:
    //private accessor
    static LsInputDispatch* s_instance;

  // target threads
  enum {
    DT_LEVEL = 'L',
    DT_ROOM = 'R'
  };

  void initialize_table();

};

#endif /* INCLUDED_LsInputDispatch */
