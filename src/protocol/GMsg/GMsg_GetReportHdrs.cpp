// GMsg_GetReportHdrs.cpp  -*- C++ -*-
// $Id: GMsg_GetReportHdrs.cpp,v 1.6 1998-02-04 19:03:35-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_GetReportHdrs.h"
#endif

#ifdef WIN32
#define STRICT
#include "unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/GMsg/GMsg_GetReportHdrs.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"


////
// constructor
////

GMsg_GetReportHdrs::GMsg_GetReportHdrs()
  : LmMesg(GMsg::GETREPORTHDRS, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, 0, 0, 0, Lyra::ID_UNKNOWN);
}

////
// destructor
////

GMsg_GetReportHdrs::~GMsg_GetReportHdrs()
{
  // empty
}

////
// Init
////

void GMsg_GetReportHdrs::Init(lyra_id_t goalid, short guild, short level_num, int sessionid, lyra_id_t lastreport)
{
  SetGoalID(goalid);
  SetGuild(guild);
  SetLevelNum(level_num);
  SetSessionID(sessionid);
  SetLastReport(lastreport);
}

////
// hton
////

void GMsg_GetReportHdrs::hton()
{
  HTONL(data_.goalid);
  HTONS(data_.guild);
  HTONS(data_.level_num);
  HTONL(data_.sessionid);
  HTONL(data_.lastreport);
}

////
// ntoh
////

void GMsg_GetReportHdrs::ntoh()
{
  NTOHL(data_.goalid);
  NTOHS(data_.guild);
  NTOHS(data_.level_num);
  NTOHL(data_.sessionid);
  NTOHL(data_.lastreport);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_GetReportHdrs::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_GetReportHdrs[%p,%d]: "), this, sizeof(GMsg_GetReportHdrs));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("id=%u guild=%d level_num=%d sessionid=%u lastreport=%u>\n"), GoalID(), Guild(), LevelNum(), SessionID(), LastReport());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void GMsg_GetReportHdrs::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_GetReportHdrs::GoalID() const
{
	return data_.goalid;
}

short GMsg_GetReportHdrs::Guild() const
{
	return data_.guild;
}

short GMsg_GetReportHdrs::LevelNum() const
{
	return data_.level_num;
}

int GMsg_GetReportHdrs::SessionID() const
{
	return data_.sessionid;
}

lyra_id_t GMsg_GetReportHdrs::LastReport() const
{
	return data_.lastreport;
}

void GMsg_GetReportHdrs::SetGoalID(lyra_id_t goalid)
{
	data_.goalid = goalid;
}

void GMsg_GetReportHdrs::SetGuild(short guild)
{
	data_.guild = guild;
}

void GMsg_GetReportHdrs::SetLevelNum(short level_num)
{
	data_.level_num = level_num;
}

void GMsg_GetReportHdrs::SetSessionID(int sessionid)
{
	data_.sessionid = sessionid;
}

void GMsg_GetReportHdrs::SetLastReport(lyra_id_t lastreport)
{
	data_.lastreport = lastreport;
}
