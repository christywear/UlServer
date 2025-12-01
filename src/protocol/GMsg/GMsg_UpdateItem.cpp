// GMsg_UpdateItem.cpp  -*- C++ -*-
// $Id: GMsg_UpdateItem.cpp,v 1.6 1997-07-29 19:32:10-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_UpdateItem.h"
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

#include "../../../include/Protocol/GMsg/GMsg_UpdateItem.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"
#include "../../../include/Game/LmItem.h"

////
// constructor
////

GMsg_UpdateItem::GMsg_UpdateItem()
  : LmMesg(GMsg::UPDATEITEM, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(LmItem::DEFAULT_INSTANCE);
}

////
// destructor
////

GMsg_UpdateItem::~GMsg_UpdateItem()
{
  // empty
}

////
// Init
////

void GMsg_UpdateItem::Init(const LmItem& item)
{
  SetItem(item);
}

////
// hton
////

void GMsg_UpdateItem::hton()
{
  data_.item.ConvertToNetwork();
}

////
// ntoh
////

void GMsg_UpdateItem::ntoh()
{
  data_.item.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_UpdateItem::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_UpdateItem[%p,%d]: "), this, sizeof(GMsg_UpdateItem));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T(">\n"));
    Item().Dump(f, indent + 1);
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void GMsg_UpdateItem::Dump(FILE*, int) const
{
    // empty
}
#endif /* !USE_DEBUG */

const LmItem& GMsg_UpdateItem::Item() const
{
    return data_.item;
}

void GMsg_UpdateItem::SetItem(const LmItem& item)
{
    data_.item = item;
}