// GMsg_GotoLevel.cpp  -*- C++ -*-
// $Id: GMsg_GotoLevel.cpp,v 1.4 1997-07-29 19:32:10-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_GotoLevel.h"
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

#include "../../../include/Protocol/GMsg/GMsg_GotoLevel.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_GotoLevel::GMsg_GotoLevel()
  : LmMesg(GMsg::GOTOLEVEL, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, Lyra::ID_UNKNOWN, LmPeerUpdate::DEFAULT_INSTANCE);
}

////
// destructor
////

GMsg_GotoLevel::~GMsg_GotoLevel()
{
  // empty
}

////
// Init
////

void GMsg_GotoLevel::Init(lyra_id_t levelid, lyra_id_t roomid, const LmPeerUpdate& update)
{
  SetLevelID(levelid);
  SetRoomID(roomid);
  SetPeerUpdate(update);
}

////
// hton
////

void GMsg_GotoLevel::hton()
{
  HTONL(data_.levelid);
  HTONL(data_.roomid);
  data_.update.ConvertToNetwork();
}

////
// ntoh
////

void GMsg_GotoLevel::ntoh()
{
  NTOHL(data_.levelid);
  NTOHL(data_.roomid);
  data_.update.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_GotoLevel::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_GotoLevel[%p,%d]: "), this, sizeof(GMsg_GotoLevel));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("level=%u room=%u>\n"), LevelID(), RoomID());
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
void GMsg_GotoLevel::Dump(FILE*, int) const
{
    // empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_GotoLevel::LevelID() const
{
    return data_.levelid;
}

lyra_id_t GMsg_GotoLevel::RoomID() const
{
    return data_.roomid;
}

const LmPeerUpdate& GMsg_GotoLevel::PeerUpdate() const
{
    return data_.update;
}

void GMsg_GotoLevel::SetLevelID(lyra_id_t levelid)
{
    data_.levelid = levelid;
}

void GMsg_GotoLevel::SetRoomID(lyra_id_t roomid)
{
    data_.roomid = roomid;
}

void GMsg_GotoLevel::SetPeerUpdate(const LmPeerUpdate& update)
{
    data_.update = update;
}
