// PTh.cpp -*- C++ -*-
// $Id: PTh.cpp,v 1.11 1997-08-20 01:34:36-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// Implementation of PTh class, a C++ wrapper around a POSIX thread

#ifdef __GNUC__
#pragma implementation "PTh.h"
#endif

#include "../../include/Core/PTh.h"
#include "../../include/Core/PThAttr.h"
#include "../../include/Core/LyraDefs.h"

////
// constructor
////

PTh::PTh()
  : running_(false)
{
	stackaddr_ = 0;
}

////
// destructor
////

PTh::~PTh()
{
#ifndef WIN32
	if (0 != stackaddr_)
		LmDELETEARRAY(stackaddr_);
#endif
  // check for running?  (and then what?)
}

////
// Create: creates thread
//   returns -1 if thread has already been started, or result from pth_create()
////

int PTh::Create(const PThAttr* p_Attr)
{
  // only allow one thread per object to be running
  if (running_) return -1;
  running_ = true;

  // start thread
#ifdef WIN32
  return pthread_create(&thread_, (p_Attr ? &p_Attr->attr_ : 0), entry, this);
#else

  // create the stack to be used for the thread	
  // we can't trust the pTh memory management!
  unsigned int stacksize;
  p_Attr->GetStackSize(&stacksize);
  stackaddr_ = LmNEW(char[stacksize]);
  pth_attr_set(p_Attr->attr_, PTH_ATTR_STACK_ADDR, stackaddr_);

  thread_ = pth_spawn(p_Attr->attr_, entry, this);
  return 1;
#endif
}

////
// entry: static entry point for threads; simply calls "Run" for given thread object
////

void* PTh::entry(void* arg)
{
  PTh* this_ = (PTh*) arg;
  this_->Run();
  this_->DoneRunning();
#ifndef WIN32
  pth_exit(0);
#endif
  return 0;
}

////
// Dump
////

void PTh::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<PTh[%p,%d]: thread=%lu running=%d>\n"), this, sizeof(PTh), thread_, running_);
}

bool PTh::IsRunning() const
{
	return running_;
}

bool PTh::IsActive() const
{
#ifdef WIN32
	// *** NOTE: pthread_kill not implemented yet
	// This causes Cancel to be called on ALL threads
	// when the thread pool is deleted.
	return true;
	//return (pthread_kill(thread_, 0) == 0);
#else
	return pth_raise(thread_, 0);
#endif
}

void PTh::DoneRunning()
{
	running_ = false;
}

#ifdef WIN32
INLINE pthread_t PTh::Thread() const
#else
INLINE pth_t PTh::Thread() const
#endif
{
	return thread_;
}

int PTh::Cancel()
{
#ifdef WIN32
	return pthread_cancel(thread_);
#else
	return pth_cancel(thread_);
#endif
}

void PTh::Exit(void* status)
{
#ifdef WIN32
	pthread_exit(status);
#else
	pth_exit(status);
#endif
}

int PTh::Join(void** status)
{
#ifdef WIN32
	return pthread_join(thread_, status);
#else
	return pth_join(thread_, status);
#endif
}

#ifdef WIN32
pthread_t PTh::Self()
{
	return pthread_self();
}
#else
pth_t PTh::Self()
{
	return pth_self();
}
#endif

#ifdef WIN32
void PTh::YieldSlice()
{
	Sleep(0);
}
#else
void PTh::Yield()
{
	pth_yield(NULL);
}
#endif

