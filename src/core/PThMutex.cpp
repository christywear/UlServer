// PThMutex.cpp  -*- C++ -*-
// $Id: PThMutex.cpp,v 1.3 1997-06-17 23:14:41-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "PThMutex.h"
#endif

#include "../../include/Core/PThMutex.h"

PThMutex::PThMutex()
{
}

PThMutex::~PThMutex()
{
#ifdef WIN32
	pthread_mutex_destroy(&mutex_);
#endif
}

int PThMutex::Init()
{
#ifdef WIN32
	return pthread_mutex_init(&mutex_, 0);
#else
	return pth_mutex_init(&mutex_);
#endif
}

int PThMutex::Lock()
{
#ifdef WIN32
	return pthread_mutex_lock(&mutex_);
#else
	return pth_mutex_acquire(&mutex_, FALSE, NULL);
#endif
}

int PThMutex::TryLock()
{ // non-blocking version
#ifdef WIN32
	return pthread_mutex_trylock(&mutex_);
#else
	return pth_mutex_acquire(&mutex_, TRUE, NULL);
#endif
}

int PThMutex::UnLock()
{
#ifdef WIN32
	return pthread_mutex_unlock(&mutex_);
#else
	return pth_mutex_release(&mutex_);
#endif
}
