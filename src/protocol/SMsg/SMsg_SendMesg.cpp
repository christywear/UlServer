// SMsg_SendMesg.cpp  -*- C++ -*-
// $Id: SMsg_SendMesg.cpp,v 1.1 1997-08-17 16:32:02-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_SendMesg.h"
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

#include "../../../include/Protocol/SMsg/SMsg_SendMesg.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"

////
// constructor
////

SMsg_SendMesg::SMsg_SendMesg()
  : LmMesg(SMsg::SENDMESG, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, 0);
}

////
// destructor
////

SMsg_SendMesg::~SMsg_SendMesg()
{
  // empty
}

////
// Init
////

void SMsg_SendMesg::Init(LmConnection* conn, LmSrvMesgBuf* mbuf)
{
  SetConnection(conn);
  SetMesgBuf(mbuf);
}

////
// hton
////

void SMsg_SendMesg::hton()
{
  // nothing -- internal message
}

////
// ntoh
////

void SMsg_SendMesg::ntoh()
{
  // nothing -- internal message
}

////
// Dump: print to FILE stream
////

void SMsg_SendMesg::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_SendMesg[%p,%d]: "), this, sizeof(SMsg_SendMesg));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("conn=[%p] mbuf=[%p]>\n"), Connection(), MesgBuf());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}

LmConnection* SMsg_SendMesg::Connection() const
{
	return data_.conn;
}

LmSrvMesgBuf* SMsg_SendMesg::MesgBuf() const
{
	return data_.mbuf;
}

void SMsg_SendMesg::SetConnection(LmConnection* conn)
{
	data_.conn = conn;
}

void SMsg_SendMesg::SetMesgBuf(LmSrvMesgBuf* mbuf)
{
	data_.mbuf = mbuf;
}
