// SMsg_GiveItem.cpp  -*- C++ -*-
// $Id: SMsg_GiveItem.cpp,v 1.1 1997-09-18 16:49:05-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_GiveItem.h"
#endif

#ifdef WIN32
#define STRICT
#include "unix.h"
#include <winsock.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/SMsg/SMsg_GiveItem.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"

////
// constructor
////

SMsg_GiveItem::SMsg_GiveItem()
  : LmMesg(SMsg::GIVEITEM, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, 0, LmItem::DEFAULT_INSTANCE);
}

////
// destructor
////

SMsg_GiveItem::~SMsg_GiveItem()
{
  // empty
}

////
// Init
////

void SMsg_GiveItem::Init(lyra_id_t sourceid, lyra_id_t targetid, const LmItem& item)
{
  SetSourceID(sourceid);
  SetTargetID(targetid);
  SetItem(item);
}

////
// hton
////

void SMsg_GiveItem::hton()
{
  HTONL(data_.sourceid);
  HTONL(data_.targetid);
  data_.item.ConvertToNetwork();
}

////
// ntoh
////

void SMsg_GiveItem::ntoh()
{
  NTOHL(data_.sourceid);
  NTOHL(data_.targetid);
  data_.item.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void SMsg_GiveItem::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_GiveItem[%p]: "), this);
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("data>\n"));
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void SMsg_GiveItem::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t SMsg_GiveItem::SourceID() const
{
	return data_.sourceid;
}

lyra_id_t SMsg_GiveItem::TargetID() const
{
	return data_.targetid;
}

const LmItem& SMsg_GiveItem::Item() const
{
	return data_.item;
}

void SMsg_GiveItem::SetSourceID(lyra_id_t sourceid)
{
	data_.sourceid = sourceid;
}

void SMsg_GiveItem::SetTargetID(lyra_id_t targetid)
{
	data_.targetid = targetid;
}

void SMsg_GiveItem::SetItem(const LmItem& item)
{
	data_.item = item;
}