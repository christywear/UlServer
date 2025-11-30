// LmGoalInfo.cpp  -*- C++ -*-
// $Id: LmGoalInfo.cpp,v 1.2 1997-11-17 14:04:43-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#include <stdio.h>
#include <string.h>

#include "../../include/Game/LmGoalInfo.h"

////
// Constructor
////

LmGoalInfo::LmGoalInfo()
{
  Init();
}

////
// Init - reset
////

void LmGoalInfo::Init()
{
  // zero out
  memset(&gi_, 0, sizeof(gi_));
}

////
// Dump
////

void LmGoalInfo::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LmGoalInfo[%p,%d]: "), this, sizeof(LmGoalInfo));
  Dump1(f);
 _ftprintf(f, _T(">\n"));
}

////
// Dump1
////

void LmGoalInfo::Dump1(FILE* f) const
{
 _ftprintf(f, _T("[id=%u,creator=%u,guild=%d,rank=%d,accepts=%d/%d,expire=%d,votes=%d:%d,vexpire=%d,status=%d,flags=%d]"),
	  GoalID(), CreatorID(), Guild(), Rank(), NumAccepted(), MaxAccepts(),
	  ExpirationTime(), NumberYes(), NumberNo(), VoteExpiration(), 
	  Status(), Flags());
}

lyra_id_t LmGoalInfo::GoalID() const
{
	return gi_.goalid;
}

int LmGoalInfo::CreatorID() const
{
	return gi_.creator;
}

int LmGoalInfo::Guild() const
{
	return gi_.guild;
}

int LmGoalInfo::Rank() const
{
	return gi_.rank;
}

int LmGoalInfo::MaxAccepts() const
{
	return gi_.maxaccepts;
}

int LmGoalInfo::ExpirationTime() const
{
	return gi_.expirationtime;
}

int LmGoalInfo::NumberYes() const
{
	return gi_.numberyes;
}

int LmGoalInfo::NumberNo() const
{
	return gi_.numberno;
}

int LmGoalInfo::VoteExpiration() const
{
	return gi_.voteexpiration;
}

int LmGoalInfo::Status() const
{
	return gi_.status;
}

int LmGoalInfo::NumAccepted() const
{
	return gi_.numaccepted;
}

int LmGoalInfo::Flags() const
{
	return gi_.flags;
}

bool LmGoalInfo::IsGuardianManaged() const
{
	return (gi_.flags & 1) ? true : false;
}

void LmGoalInfo::SetGoalID(lyra_id_t goalid)
{
	gi_.goalid = goalid;
}

void LmGoalInfo::SetCreatorID(int creator)
{
	gi_.creator = creator;
}

void LmGoalInfo::SetGuild(int guild)
{
	gi_.guild = guild;
}

void LmGoalInfo::SetRank(int rank)
{
	gi_.rank = rank;
}

void LmGoalInfo::SetMaxAccepts(int maxaccepts)
{
	gi_.maxaccepts = maxaccepts;
}

void LmGoalInfo::SetExpirationTime(int expirationtime)
{
	gi_.expirationtime = expirationtime;
}

void LmGoalInfo::SetNumberYes(int numberyes)
{
	gi_.numberyes = numberyes;
}

void LmGoalInfo::SetNumberNo(int numberno)
{
	gi_.numberno = numberno;
}

void LmGoalInfo::SetVoteExpiration(int voteexpiration)
{
	gi_.voteexpiration = voteexpiration;
}

void LmGoalInfo::SetStatus(int status)
{
	gi_.status = status;
}

void LmGoalInfo::SetNumAccepted(int numaccepted)
{
	gi_.numaccepted = numaccepted;
}

void LmGoalInfo::SetFlags(int flags)
{
	gi_.flags = flags;
}
