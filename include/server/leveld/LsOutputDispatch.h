// LsOutputDispatch.h  -*- C++ -*-
// $Id: LsOutputDispatch.h,v 1.9 1997-08-17 16:49:23-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// output message dispatcher

#ifndef INCLUDED_LsOutputDispatch
#define INCLUDED_LsOutputDispatch

#ifdef __GNUC__
#pragma interface
#endif

#include <list>

#include "..\..\Core\LyraDefs.h"
#include "..\..\Protocol\LmDispatch.h"

// class forward declarations


// the class

class LsOutputDispatch : public LmDispatch {

public:

  LsOutputDispatch();
  ~LsOutputDispatch();

  //public accessor
  static LsOutputDispatch* Instance() { return s_instance; }

  void SendMessage(LmMesg* msg, LmConnection* conn);
  void SendMessage(LmSrvMesgBuf* mbuf, LmConnection* conn);

  void Dump(FILE* f, int indent = 0) const;

protected:

  LmThread* ComputeTarget(LmSrvMesgBuf* mbuf, LmConnection* conn);

private:
	//private accessor
	static LsOutputDispatch* s_instance;

};

#endif /* INCLUDED_LsOutputDispatch */
