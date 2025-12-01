// SMsg_LocateAvatar.cpp  -*- C++ -*-
// $Id: SMsg_LocateAvatar.cpp,v 1.2 1997-10-02 14:19:09-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_LocateAvatar.h"
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

#include "../../../include/Protocol/SMsg/SMsg_LocateAvatar.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"

////
// constructor
////

SMsg_LocateAvatar::SMsg_LocateAvatar()
  : LmMesg(SMsg::LOCATEAVATAR, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, 0, 0);
}

////
// destructor
////

SMsg_LocateAvatar::~SMsg_LocateAvatar()
{
  // empty
}

////
// Init
////

void SMsg_LocateAvatar::Init(lyra_id_t locatorid, lyra_id_t playerid, lyra_id_t roomid)
{
  SetLocatorID(locatorid);
  SetPlayerID(playerid);
  SetRoomID(roomid);
}

////
// hton
////

void SMsg_LocateAvatar::hton()
{
  HTONL(data_.locatorid);
  HTONL(data_.playerid);
  HTONL(data_.roomid);
}

////
// ntoh
////

void SMsg_LocateAvatar::ntoh()
{
  NTOHL(data_.locatorid);
  NTOHL(data_.playerid);
  NTOHL(data_.roomid);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void SMsg_LocateAvatar::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_LocateAvatar[%p]: "), this);
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("locator=%u player=%u room=%u>\n"), LocatorID(), PlayerID(), RoomID());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void SMsg_LocateAvatar::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t SMsg_LocateAvatar::LocatorID() const
{
	return data_.locatorid;
}

lyra_id_t SMsg_LocateAvatar::PlayerID() const
{
	return data_.playerid;
}

lyra_id_t SMsg_LocateAvatar::RoomID() const
{
	return data_.roomid;
}

void SMsg_LocateAvatar::SetLocatorID(lyra_id_t locatorid)
{
	data_.locatorid = locatorid;
}

void SMsg_LocateAvatar::SetPlayerID(lyra_id_t playerid)
{
	data_.playerid = playerid;
}

void SMsg_LocateAvatar::SetRoomID(lyra_id_t roomid)
{
	data_.roomid = roomid;
}