// GsPlayerThreadSet.h  -*- C++ -*-
// $Id: GsPlayerThreadSet.h,v 1.4 1998-01-27 10:18:29-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// manage set of player threads

#ifndef INCLUDED_GsPlayerThreadSet
#define INCLUDED_GsPlayerThreadSet

#include <stdio.h>
#include <list>

#include "..\..\Core\LyraDefs.h"
#include "..\..\Core\PThMutex.h"

// class forward declarations


class GsPlayer;
class GsPlayerThread;

// the class

class GsPlayerThreadSet {

public:

  GsPlayerThreadSet();
  ~GsPlayerThreadSet();
  //public accessor
  static GsPlayerThreadSet* Instance() { return s_instance; }
  void StartIdlePlayerThreads(int num_threads = 16);

  GsPlayerThread* GetPlayerThread();
  void ReturnPlayerThread(GsPlayerThread* pthr);

  void Dump(FILE* f, int indent = 0) const;

private:
	//private accessor
	static GsPlayerThreadSet* s_instance;
  // not implemented
  GsPlayerThreadSet(const GsPlayerThreadSet&);
  //operator=(const GsPlayerThreadSet&);

  GsPlayerThread* start_player_thread();


  mutable PThMutex lock_;

  // list of player threads
  typedef std::list<GsPlayerThread*> ps_t;
  ps_t pthrs_;

};

#endif /* INCLUDED_GsPlayerThreadSet */
