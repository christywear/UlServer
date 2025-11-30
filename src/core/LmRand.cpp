// LmRand.cpp  -*- C++ -*-
// $Id: LmRand.cpp,v 1.1 1997-08-27 23:59:51-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#include "../../include/core/LmRand.h"

void LmRand::InitSeed(long seed)
{
#ifdef WIN32
	srand(seed);
#else
	srand48(seed);
#endif
}

int LmRand::Generate(int min, int max)
{
#ifdef WIN32
	return min + (rand() % (max - min + 1));
#else
	return min + (lrand48() % (max - min + 1));
#endif
}

int LmRand::ChooseOne(int* choices, int num_choices)
{
	return choices[Generate(0, num_choices - 1)];
}