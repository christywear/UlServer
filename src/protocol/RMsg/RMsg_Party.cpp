// RMsg_Party.cpp  -*- C++ -*-
// $Id: RMsg_Party.cpp,v 1.8 1997-07-29 19:32:22-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "RMsg_Party.h"
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

#include "../../../include/Protocol/RMsg/RMsg_Party.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/RMsg/RMsg.h"

////
// constructor
////

RMsg_Party::RMsg_Party()
  : LmMesg(RMsg::PARTY, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(RMsg_Party::UNKNOWN, Lyra::ID_UNKNOWN, RMsg_Party::RC_UNKNOWN);
}

////
// destructor
////

RMsg_Party::~RMsg_Party()
{
  // empty
}

////
// Init
////

void RMsg_Party::Init(int req_type, lyra_id_t playerid, int rc)
{
  SetRequestType(req_type);
  SetPlayerID(playerid);
  SetResponseCode(rc);
}

////
// hton
////

void RMsg_Party::hton()
{
  HTONL(data_.playerid);
  HTONS(data_.req_type);
  HTONS(data_.rc);
}

////
// ntoh
////

void RMsg_Party::ntoh()
{
  NTOHL(data_.playerid);
  NTOHS(data_.req_type);
  NTOHS(data_.rc);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void RMsg_Party::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<RMsg_Party[%p,%d]: "), this, sizeof(RMsg_Party));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("request=%c code=%d player=%u>\n"), RequestType(), ResponseCode(), PlayerID());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void RMsg_Party::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

void RMsg_Party::InitJoin(lyra_id_t playerid, int reason)
{
	Init(JOIN, playerid, reason);
}

void RMsg_Party::InitAccept(lyra_id_t playerid)
{
	Init(ACCEPT, playerid, RC_UNUSED);
}

void RMsg_Party::InitReject(lyra_id_t playerid, int reason)
{
	Init(REJECT, playerid, reason);
}

void RMsg_Party::InitLeave(lyra_id_t playerid, int reason)
{
	Init(LEAVE, playerid, reason);
}

int RMsg_Party::RequestType() const
{
	return data_.req_type;
}

lyra_id_t RMsg_Party::PlayerID() const
{
	return data_.playerid;
}

int RMsg_Party::ResponseCode() const
{
	return data_.rc;
}

void RMsg_Party::SetPlayerID(lyra_id_t playerid)
{
	data_.playerid = playerid;
}

void RMsg_Party::SetRequestType(int rt)
{
	data_.req_type = rt;
}

void RMsg_Party::SetResponseCode(int rc)
{
	data_.rc = rc;
}