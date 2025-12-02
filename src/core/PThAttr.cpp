#ifdef UL_POSIX
// PThAttr.cpp  -*- C++ -*-
// $Id: PThAttr.cpp,v 1.3 1997-06-17 23:14:41-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "PThAttr.h"
#endif
#include "../../include/platform/Platform.h" //temp fix for old style legacy defines
#include "../../include/Core/PThAttr.h"

// class methods -- all inline
PThAttr::PThAttr()
{
#ifndef WIN32
	attr_ = pth_attr_new();
#endif
}

PThAttr::~PThAttr()
{
#ifdef WIN32
	pthread_attr_destroy(&attr_);
#else
	pth_attr_destroy(attr_);
#endif
}

int PThAttr::Init()
{
#ifdef WIN32
	return pthread_attr_init(&attr_);
#else
	return pth_attr_init(attr_);
#endif
}

int PThAttr::SetJoinable(int joinable)
{
#ifdef WIN32
	return pthread_attr_setdetachstate(&attr_, joinable);
#else
	return pth_attr_set(attr_, PTH_ATTR_JOINABLE, joinable);
#endif
}

int PThAttr::GetJoinable(int* joinable) const
{
#ifdef WIN32
	return pthread_attr_getdetachstate(&attr_, joinable);
#else
	return pth_attr_get(attr_, PTH_ATTR_JOINABLE, joinable);
#endif
}

int PThAttr::SetStackSize(unsigned int stacksize)
{
#ifdef WIN32
	return pthread_attr_setstacksize(&attr_, stacksize);
#else
	return pth_attr_set(attr_, PTH_ATTR_STACK_SIZE, stacksize);
#endif
}

int PThAttr::GetStackSize(unsigned int* stacksize) const
{
#ifdef WIN32
	return pthread_attr_getstacksize(&attr_, stacksize);
#else
	return pth_attr_get(attr_, PTH_ATTR_STACK_SIZE, stacksize);
#endif
}

#endif