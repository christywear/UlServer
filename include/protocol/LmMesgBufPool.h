// LmMesgBufPool.h  -*- C++ -*-
// $Id: LmMesgBufPool.h,v 1.10 1997-07-18 16:05:05-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// message buffer pool

#ifndef INCLUDED_LmMesgBufPool
#define INCLUDED_LmMesgBufPool

#ifdef __GNUC__
#pragma interface
#endif

#include <stdio.h>

#include "..\Core\LyraDefs.h"
#include "..\Core\PThMutex.h"

// forward declarations

class LmSrvMesgBuf;
class LmMesgBufPoolImp;

// the message buffer class

class LmMesgBufPool {

public:

  enum {
    // constants
    MAX_SPAREBUFS = 8,
    DEFAULT_MSGSIZE = 32,
    DEFAULT_REAPAGE = (60 * 5)
  };

public:

  LmMesgBufPool(int maxsparebufs = MAX_SPAREBUFS);
  ~LmMesgBufPool();
  //public assessor
  static LmMesgBufPool* Instance() { return s_instance; }
  LmSrvMesgBuf* AllocateBuffer(int msize = DEFAULT_MSGSIZE, int use_count = 1);
  void ReturnBuffer(LmSrvMesgBuf* pbuf);
  int FreeOldBuffers(int age = DEFAULT_REAPAGE);

  void Dump(FILE* f, int indent = 0) const;

private:
    //private assessor
    static LmMesgBufPool* s_instance;
  // not implemented
  LmMesgBufPool(const LmMesgBufPool&);
  //operator=(const LmMesgBufPool&);

  LmMesgBufPoolImp* imp_;
  mutable PThMutex lock_;
  
  int num_allocated_; // total # of calls to AllocateBuffer()
  int num_returned_;  // total # of calls to ReturnBuffer()

};

#endif /* INCLUDED_LmMesgBufPool */
