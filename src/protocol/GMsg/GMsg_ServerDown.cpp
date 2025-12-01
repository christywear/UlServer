// GMsg_ServerDown.cpp  -*- C++ -*-
// $Id: GMsg_ServerDown.cpp,v 1.6 1997-07-29 19:32:10-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_ServerDown.h"
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

#include "../../../include/Protocol/GMsg/GMsg_ServerDown.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_ServerDown::GMsg_ServerDown()
  : LmMesg(GMsg::SERVERDOWN, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(STATUS_UNKNOWN);
}

////
// destructor
////

GMsg_ServerDown::~GMsg_ServerDown()
{
  // empty
}

////
// Init
////

void GMsg_ServerDown::Init(int status)
{
  SetStatus(status);
}

////
// hton
////

void GMsg_ServerDown::hton()
{
  HTONL(data_.status);
}

////
// ntoh
////

void GMsg_ServerDown::ntoh()
{
  NTOHL(data_.status);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_ServerDown::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_ServerDown[%p,%d]: "), this, sizeof(GMsg_ServerDown));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("status=%c>\n"), Status());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void GMsg_ServerDown::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

int GMsg_ServerDown::Status() const
{
	return data_.status;
}

void GMsg_ServerDown::SetStatus(int status)
{
	data_.status = status;
}