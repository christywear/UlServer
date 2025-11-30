// LmFuncTimer.cpp  -*- C++ -*-
// $Id: LmFuncTimer.cpp,v 1.1 1997-10-20 17:54:25-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmFuncTimer.h"
#endif

#include "../../include/core/LmFuncTimer.h"

LmFuncTimer::LmFuncTimer(int& num_calls, unsigned long& total_time, unsigned long& last_time)
    : total_time_(total_time),
    last_time_(last_time)
{
    // increment number of calls
    num_calls++;
    // start func timer
    timer_.Start();
}

LmFuncTimer::~LmFuncTimer()
{
    // stop func timer
    timer_.Stop();
    // store last func time
    last_time_ = timer_.MilliSeconds();
    // increment total time
    total_time_ += last_time_;
}