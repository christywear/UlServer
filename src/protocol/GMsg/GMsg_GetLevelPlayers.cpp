// GMsg_GetLevelPlayers.cpp  -*- C++ -*-
// $Id: GMsg_GetLevelPlayers.cpp,v 1.3 1997-07-29 19:32:10-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_GetLevelPlayers.h"
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

#include "../../../include/Protocol/GMsg/GMsg_GetLevelPlayers.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_GetLevelPlayers::GMsg_GetLevelPlayers()
  : LmMesg(GMsg::GETLEVELPLAYERS, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN);
}

////
// destructor
////

GMsg_GetLevelPlayers::~GMsg_GetLevelPlayers()
{
  // empty
}

////
// Init
////

void GMsg_GetLevelPlayers::Init(lyra_id_t levelid)
{
  SetLevelID(levelid);
}

////
// hton
////

void GMsg_GetLevelPlayers::hton()
{
  HTONL(data_.levelid);
}

////
// ntoh
////

void GMsg_GetLevelPlayers::ntoh()
{
  NTOHL(data_.levelid);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_GetLevelPlayers::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_GetLevelPlayers[%p,%d]: "), this, sizeof(GMsg_GetLevelPlayers));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("level=%u>\n"), LevelID());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void GMsg_GetLevelPlayers::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_GetLevelPlayers::LevelID() const
{
	return data_.levelid;
}

void GMsg_GetLevelPlayers::SetLevelID(lyra_id_t levelid)
{
	data_.levelid = levelid;
}