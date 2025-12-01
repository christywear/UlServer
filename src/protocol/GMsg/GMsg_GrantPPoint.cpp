// GMsg_GrantPPoint.cpp  -*- C++ -*-
// $Id: GMsg_GrantPPoint.cpp,v 1.4 1997-12-05 14:44:55-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_GrantPPoint.h"
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

#include "../../../include/Protocol/GMsg/GMsg_GrantPPoint.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_GrantPPoint::GMsg_GrantPPoint()
  : LmMesg(GMsg::GRANTPPOINT, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, _T("no reason"));
}

////
// destructor
////

GMsg_GrantPPoint::~GMsg_GrantPPoint()
{
  // empty
}

////
// Init
////

void GMsg_GrantPPoint::Init(lyra_id_t player_id, const TCHAR* why)
{
  SetPlayerID(player_id);
  SetWhy(why);
}


////
// hton
////

void GMsg_GrantPPoint::hton()
{
	HTONL(data_.player_id);
  // not converted: why
}

////
// ntoh
////

void GMsg_GrantPPoint::ntoh()
{
	NTOHL(data_.player_id);
  // not converted: why
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_GrantPPoint::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_GrantPPoint[%p]: "), this);
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("player id=%d why=%s>\n"), PlayerID(), Why());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

////
// SetWhy
////

void GMsg_GrantPPoint::SetWhy(const TCHAR* why)
{
  _tcsnccpy(data_.why, why, MAX_WHY);
  TRUNC(data_.why, MAX_WHY);
}

#ifndef USE_DEBUG
void GMsg_GrantPPoint::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_GrantPPoint::PlayerID() const
{
	return data_.player_id;
}

void GMsg_GrantPPoint::SetPlayerID(lyra_id_t value)
{
	data_.player_id = value;
}

const TCHAR* GMsg_GrantPPoint::Why(void) const
{
	return data_.why;
}

