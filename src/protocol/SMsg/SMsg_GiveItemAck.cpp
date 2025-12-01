// SMsg_GiveItemAck.cpp  -*- C++ -*-
// $Id: SMsg_GiveItemAck.cpp,v 1.1 1997-09-19 16:42:02-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_GiveItemAck.h"
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

#include "../../../include/Protocol/SMsg/SMsg_GiveItemAck.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"

////
// constructor
////

SMsg_GiveItemAck::SMsg_GiveItemAck()
  : LmMesg(SMsg::GIVEITEMACK, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, 0, 0, LmItemHdr::DEFAULT_INSTANCE);
}

////
// destructor
////

SMsg_GiveItemAck::~SMsg_GiveItemAck()
{
  // empty
}

////
// Init
////

void SMsg_GiveItemAck::Init(int status, lyra_id_t sourceid, lyra_id_t targetid, const LmItemHdr& itemheader)
{
  SetStatus(status);
  SetSourceID(sourceid);
  SetTargetID(targetid);
  SetItemHeader(itemheader);
}

////
// hton
////

void SMsg_GiveItemAck::hton()
{
  HTONL(data_.status);
  HTONL(data_.sourceid);
  HTONL(data_.targetid);
  data_.itemheader.ConvertToNetwork();
}

////
// ntoh
////

void SMsg_GiveItemAck::ntoh()
{
  NTOHL(data_.status);
  NTOHL(data_.sourceid);
  NTOHL(data_.targetid);
  data_.itemheader.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void SMsg_GiveItemAck::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_GiveItemAck[%p]: "), this);
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
void SMsg_GiveItemAck::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

int SMsg_GiveItemAck::Status() const
{
	return data_.status;
}

lyra_id_t SMsg_GiveItemAck::SourceID() const
{
	return data_.sourceid;
}

lyra_id_t SMsg_GiveItemAck::TargetID() const
{
	return data_.targetid;
}

const LmItemHdr& SMsg_GiveItemAck::ItemHeader() const
{
	return data_.itemheader;
}

void SMsg_GiveItemAck::SetStatus(int status)
{
	data_.status = status;
}

void SMsg_GiveItemAck::SetSourceID(lyra_id_t sourceid)
{
	data_.sourceid = sourceid;
}

void SMsg_GiveItemAck::SetTargetID(lyra_id_t targetid)
{
	data_.targetid = targetid;
}

void SMsg_GiveItemAck::SetItemHeader(const LmItemHdr& itemheader)
{
	data_.itemheader = itemheader;
}
