// RMsg_LeaveRoom.cpp  -*- C++ -*-
// $Id: RMsg_LeaveRoom.cpp,v 1.9 1997-07-29 19:32:22-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "RMsg_LeaveRoom.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/RMsg/RMsg_LeaveRoom.h"
#include "../../../include/Protocol/RMsg/RMsg.h"
#include "../../../include/Core/LyraDefs.h"
#include <protocol/LmMesg.h>

////
// constructor
////

RMsg_LeaveRoom::RMsg_LeaveRoom()
  : LmMesg(RMsg::LEAVEROOM, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, RMsg_LeaveRoom::UNKNOWN, 0, 0);
}

////
// destructor
////

RMsg_LeaveRoom::~RMsg_LeaveRoom()
{
  // empty
}

////
// Init
////

void RMsg_LeaveRoom::Init(lyra_id_t playerid, int status, int lastx, int lasty)
{
  SetPlayerID(playerid);
  SetStatus(status);
  SetLastPosition(lastx, lasty);
}

////
// hton
////

void RMsg_LeaveRoom::hton()
{
  HTONL(data_.playerid);
  HTONL(data_.status);
  HTONS(data_.lastx);
  HTONS(data_.lasty);
}

////
// ntoh
////

void RMsg_LeaveRoom::ntoh()
{
  NTOHL(data_.playerid);
  NTOHL(data_.status);
  NTOHS(data_.lastx);
  NTOHS(data_.lasty);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void RMsg_LeaveRoom::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<RMsg_LeaveRoom[%p,%d]: "), this, sizeof(RMsg_LeaveRoom));
 if (this->ByteOrder() == (int)::ByteOrder::HOST) {
   _ftprintf(f, _T("playerid=%u status=%c lastpos=(%d,%d)>\n"), PlayerID(), Status(), LastX(), LastY());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void RMsg_LeaveRoom::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t RMsg_LeaveRoom::PlayerID() const
{
	return data_.playerid;
}

int RMsg_LeaveRoom::Status() const
{
	return data_.status;
}

int RMsg_LeaveRoom::LastX() const
{
	return data_.lastx;
}

int RMsg_LeaveRoom::LastY() const
{
	return data_.lasty;
}

void RMsg_LeaveRoom::SetStatus(int status)
{
	data_.status = status;
}

void RMsg_LeaveRoom::SetPlayerID(lyra_id_t playerid)
{
	data_.playerid = playerid;
}

void RMsg_LeaveRoom::SetLastPosition(int lastx, int lasty)
{
	data_.lastx = lastx;
	data_.lasty = lasty;
}