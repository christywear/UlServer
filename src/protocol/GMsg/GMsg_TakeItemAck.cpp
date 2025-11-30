// GMsg_TakeItemAck.cpp  -*- C++ -*-
// $Id: GMsg_TakeItemAck.cpp,v 1.1 1997-09-18 16:51:12-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_TakeItemAck.h"
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

#include "../../../include/Protocol/GMsg/GMsg_TakeItemAck.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_TakeItemAck::GMsg_TakeItemAck()
  : LmMesg(GMsg::TAKEITEMACK, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, LmItemHdr::DEFAULT_INSTANCE);
}

////
// destructor
////

GMsg_TakeItemAck::~GMsg_TakeItemAck()
{
  // empty
}

////
// Init
////

void GMsg_TakeItemAck::Init(int status, const LmItemHdr& itemheader)
{
  SetStatus(status);
  SetItemHeader(itemheader);
}

////
// hton
////

void GMsg_TakeItemAck::hton()
{
  HTONL(data_.status);
  data_.itemheader.ConvertToNetwork();
}

////
// ntoh
////

void GMsg_TakeItemAck::ntoh()
{
  NTOHL(data_.status);
  data_.itemheader.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_TakeItemAck::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_TakeItemAck[%p]: "), this);
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
void GMsg_TakeItemAck::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

int GMsg_TakeItemAck::Status() const
{
	return data_.status;
}

const LmItemHdr& GMsg_TakeItemAck::ItemHeader() const
{
	return data_.itemheader;
}

void GMsg_TakeItemAck::SetStatus(int status)
{
	data_.status = status;
}

void GMsg_TakeItemAck::SetItemHeader(const LmItemHdr& itemheader)
{
	data_.itemheader = itemheader;
}