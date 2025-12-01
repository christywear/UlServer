// SMsg_Ping.cpp  -*- C++ -*-
// $Id: SMsg_Ping.cpp,v 1.1 1997-08-15 16:30:14-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_Ping.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //?
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/SMsg/SMsg_Ping.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"

////
// constructor
////

SMsg_Ping::SMsg_Ping()
  : LmMesg(SMsg::PING, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, PONG);
}

////
// destructor
////

SMsg_Ping::~SMsg_Ping()
{
  // empty
}

////
// Init
////

void SMsg_Ping::Init(int nonce, int ptype)
{
  SetNonce(nonce);
  SetPingType(ptype);
}

////
// hton
////

void SMsg_Ping::hton()
{
  HTONL(data_.nonce);
  HTONL(data_.type);
}

////
// ntoh
////

void SMsg_Ping::ntoh()
{
  NTOHL(data_.nonce);
  NTOHL(data_.type);
}

////
// Dump: print to FILE stream
////

void SMsg_Ping::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_Ping[%p,%d]: "), this, sizeof(SMsg_Ping));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("nonce=%d type=%d>\n"), Nonce(), PingType());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}

void SMsg_Ping::InitPing(int nonce)
{
	Init(nonce, PING);
}

void SMsg_Ping::InitPong(int nonce)
{
	Init(nonce, PONG);
}

int SMsg_Ping::Nonce() const
{
	return data_.nonce;
}

int SMsg_Ping::PingType() const
{
	return data_.type;
}

void SMsg_Ping::SetNonce(int nonce)
{
	data_.nonce = nonce;
}

void SMsg_Ping::SetPingType(int ptype)
{
	data_.type = ptype;
}