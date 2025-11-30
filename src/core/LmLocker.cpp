// LmLocker.cpp  -*- C++ -*-
// $Id: LmLocker.cpp,v 1.3 1997-06-17 23:14:41-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmLocker.h"
#endif

#include "../../include/core/LmLocker.h"

LmLocker::LmLocker(PThMutex& m)
	: m_(m)
{
	m_.Lock();
	locked_ = true;
}

LmLocker::LmLocker(const PThMutex& m)
	: m_(const_cast<PThMutex&>(m)) // useful for when the mutex is used in const methods
{
	m_.Lock();
	locked_ = true;
}

void LmLocker::UnLock()
{
	if (locked_) {
		m_.UnLock();
		locked_ = false;
	}
}

LmLocker::~LmLocker()
{
	UnLock();
}