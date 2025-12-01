// RMsg_RoomLoginAck.cpp  -*- C++ -*-
// $Id: RMsg_RoomLoginAck.cpp,v 1.13 1997-11-17 14:05:06-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "RMsg_RoomLoginAck.h"
#endif
#include "../../../include/platform/Platform.h" //temp fix for old style legacy defines
#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //?
#include "../../../include/Protocol/LmSockAddrInet.h" 
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/RMsg/RMsg_RoomLoginAck.h"
#include "../../../include/Protocol/RMsg/RMsg.h"
#include "../../../include/Core/LyraDefs.h"

////
// constructor
////

RMsg_RoomLoginAck::RMsg_RoomLoginAck()
  : LmMesg(RMsg::ROOMLOGINACK, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(LOGIN_UNKNOWN, 0);
}

////
// destructor
////

RMsg_RoomLoginAck::~RMsg_RoomLoginAck()
{
  // empty
}

////
// Init
////

void RMsg_RoomLoginAck::Init(short status, short num_neighbors)
{
  SetStatus(status);
  SetNumNeighbors(num_neighbors);
}

////
// hton
////

void RMsg_RoomLoginAck::hton()
{
  HTONS(data_.status);
  HTONS(data_.num_neighbors);
}

////
// ntoh
////

void RMsg_RoomLoginAck::ntoh()
{
  NTOHS(data_.status);
  NTOHS(data_.num_neighbors);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void RMsg_RoomLoginAck::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<RMsg_RoomLoginAck[%p,%d]: "), this, sizeof(RMsg_RoomLoginAck));
#ifndef WIN32 // relies on server classes
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("status=%d num neighbors=%d>\n"),
	    Status(), NumNeighbors());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
#endif
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void RMsg_RoomLoginAck::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

short RMsg_RoomLoginAck::Status() const
{
	return data_.status;
}

short RMsg_RoomLoginAck::NumNeighbors() const
{
	return data_.num_neighbors;
}

void RMsg_RoomLoginAck::SetStatus(short status)
{
	data_.status = status;
}

void RMsg_RoomLoginAck::SetNumNeighbors(short num_neighbors)
{
	data_.num_neighbors = num_neighbors;
}
