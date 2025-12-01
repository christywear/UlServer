// GMsg_DestroyRoomItem.cpp  -*- C++ -*-
// $Id: GMsg_DestroyRoomItem.cpp,v 1.3 1997-07-29 19:32:10-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_DestroyRoomItem.h"
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

#include "../../../include/Protocol/GMsg/GMsg_DestroyRoomItem.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"
#include "../../../include/Game/LmItemHdr.h"

////
// constructor
////

GMsg_DestroyRoomItem::GMsg_DestroyRoomItem()
  : LmMesg(GMsg::DESTROYROOMITEM, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, LmItemHdr::DEFAULT_INSTANCE);
}

////
// destructor
////

GMsg_DestroyRoomItem::~GMsg_DestroyRoomItem()
{
  // empty
}

////
// Init
////

void GMsg_DestroyRoomItem::Init(lyra_id_t roomid, const LmItemHdr& itemhdr)
{
  SetRoomID(roomid);
  SetItemHeader(itemhdr);
}

////
// hton
////

void GMsg_DestroyRoomItem::hton()
{
  HTONL(data_.roomid);
  data_.itemhdr.ConvertToNetwork();
}

////
// ntoh
////

void GMsg_DestroyRoomItem::ntoh()
{
  NTOHL(data_.roomid);
  data_.itemhdr.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_DestroyRoomItem::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_DestroyRoomItem[%p,%d]: "), this, sizeof(GMsg_DestroyRoomItem));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("room=%u>\n"), RoomID());
    ItemHeader().Dump(f, indent + 1);
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void GMsg_DestroyRoomItem::Dump(FILE*, int) const
{
    // empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_DestroyRoomItem::RoomID() const
{
    return data_.roomid;
}

const LmItemHdr& GMsg_DestroyRoomItem::ItemHeader() const
{
    return data_.itemhdr;
}

void GMsg_DestroyRoomItem::SetRoomID(lyra_id_t roomid)
{
    data_.roomid = roomid;
}

void GMsg_DestroyRoomItem::SetItemHeader(const LmItemHdr& itemhdr)
{
    data_.itemhdr = itemhdr;
}