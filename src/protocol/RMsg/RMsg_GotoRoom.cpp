// RMsg_GotoRoom.cpp  -*- C++ -*-
// $Id: RMsg_GotoRoom.cpp,v 1.10 1997-07-29 19:32:22-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "RMsg_GotoRoom.h"
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

#include "../../../include/Protocol/RMsg/RMsg_GotoRoom.h"
#include "../../../include/Protocol/RMsg/RMsg.h"
#include "../../../include/Core/LyraDefs.h"

////
// constructor
////

RMsg_GotoRoom::RMsg_GotoRoom()
  : LmMesg(RMsg::GOTOROOM, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, LmPeerUpdate::DEFAULT_INSTANCE, 0, 0);
}

////
// destructor
////

RMsg_GotoRoom::~RMsg_GotoRoom()
{
  // empty
}

////
// Init
////

void RMsg_GotoRoom::Init(lyra_id_t roomid, const LmPeerUpdate& update, int lastx, int lasty)
{
  SetRoomID(roomid);
  SetPeerUpdate(update);
  SetLastPosition(lastx, lasty);
}

////
// hton
////

void RMsg_GotoRoom::hton()
{
  HTONL(data_.roomid);
  HTONS(data_.lastx);
  HTONS(data_.lasty);
  data_.update.ConvertToNetwork();
}

////
// ntoh
////

void RMsg_GotoRoom::ntoh()
{
  NTOHL(data_.roomid);
  NTOHS(data_.lastx);
  NTOHS(data_.lasty);
  data_.update.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void RMsg_GotoRoom::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<RMsg_GotoRoom[%p,%d]: "), this, sizeof(RMsg_GotoRoom));
 if (this->ByteOrder() == (int)::ByteOrder::HOST) {
   _ftprintf(f, _T("roomid=%u lastpos=(%d,%d)>\n"), RoomID(), LastX(), LastY());
    PeerUpdate().Dump(f, indent + 1);
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void RMsg_GotoRoom::Dump(FILE*, int) const
{
    // empty
}
#endif /* !USE_DEBUG */

lyra_id_t RMsg_GotoRoom::RoomID() const
{
    return data_.roomid;
}

const LmPeerUpdate& RMsg_GotoRoom::PeerUpdate() const
{
    return data_.update;
}

int RMsg_GotoRoom::LastX() const
{
    return data_.lastx;
}

int RMsg_GotoRoom::LastY() const
{
    return data_.lasty;
}

void RMsg_GotoRoom::SetRoomID(lyra_id_t roomid)
{
    data_.roomid = roomid;
}

void RMsg_GotoRoom::SetPeerUpdate(const LmPeerUpdate& update)
{
    data_.update = update;
}

void RMsg_GotoRoom::SetLastPosition(int lastx, int lasty)
{
    data_.lastx = lastx;
    data_.lasty = lasty;
}
