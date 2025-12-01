// SMsg_GS_Logout.cpp  -*- C++ -*-
// $Id: SMsg_GS_Logout.cpp,v 1.8 1997-10-03 18:30:57-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_GS_Logout.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/SMsg/SMsg_GS_Logout.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"

////
// constructor
////

SMsg_GS_Logout::SMsg_GS_Logout()
  : LmMesg(SMsg::GS_LOGOUT, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, 0, 0);
}

////
// destructor
////

SMsg_GS_Logout::~SMsg_GS_Logout()
{
  // empty
}

////
// Init
////

void SMsg_GS_Logout::Init(lyra_id_t playerid, int online, int status)
{
  SetPlayerID(playerid);
  SetOnline(online);
  SetStatus(status);
}

////
// hton
////

void SMsg_GS_Logout::hton()
{
  // internal message - not used
}

////
// ntoh
////

void SMsg_GS_Logout::ntoh()
{
  // internal message - not used
}

////
// Dump: print to FILE stream
////

void SMsg_GS_Logout::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
  _ftprintf(f, _T("<SMsg_GS_Logout[%p,%d]: "), this, sizeof(SMsg_GS_Logout));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("playerid=%u online=%d status=%c>\n"), PlayerID(), Online(), Status());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}

lyra_id_t SMsg_GS_Logout::PlayerID() const
{
	return data_.playerid;
}

int SMsg_GS_Logout::Online() const
{
	return data_.online;
}

int SMsg_GS_Logout::Status() const
{
	return data_.status;
}

void SMsg_GS_Logout::SetPlayerID(lyra_id_t playerid)
{
	data_.playerid = playerid;
}

void SMsg_GS_Logout::SetOnline(int online)
{
	data_.online = online;
}

void SMsg_GS_Logout::SetStatus(int status)
{
	data_.status = status;
}