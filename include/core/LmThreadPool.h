// LmThreadPool.h  -*- C++ -*-
// $Id: LmThreadPool.h,v 1.7 1998-04-16 19:03:16-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// thread pool class

#ifndef INCLUDED_LmThreadPool
#define INCLUDED_LmThreadPool

#ifdef __GNUC__
#pragma interface
#endif
#include "../../include/platform/Platform.h" //temp fix for old style legacy defines
#include <stdio.h>
#include <list>

#include "LyraDefs.h"

// forward declarations

class LmThread;

// class declarations

class LmThreadPool {

public:

  LmThreadPool();
  ~LmThreadPool();

  //public accessor
  static LmThreadPool* Instance() { return s_instance; }

  void AddThread(int id, LmThread* thread);
  void RemoveThread(int id, bool del = true);
  void RemoveThread(LmThread* thread, bool del = true);
  LmThread* GetThread(int id) const;
  bool HasThread(int id) const;

  void GetThreadIDs(std::list<int>& tids);

  void Dump(FILE* f, int indent = 0) const;

private:
	//private accessor
	static LmThreadPool* s_instance;
  // not implemented
  LmThreadPool(const LmThreadPool&);
  //operator=(const LmThreadPool&);

  // implementation private
  struct TP;
  TP* tp_;

  LmThread* get_thread(int id) const;

};

#endif /* INCLUDED_LmThreadPool */
