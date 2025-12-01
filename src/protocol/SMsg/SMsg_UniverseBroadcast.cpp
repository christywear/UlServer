// SMsg_UniverseBroadcast.cpp  -*- C++ -*-
// $Id: SMsg_UniverseBroadcast.cpp,v 1.9 1997-08-04 18:39:22-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_UniverseBroadcast.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../../include/core/unix.h"
#include <winsock.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/SMsg/SMsg_UniverseBroadcast.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"
#include "../../../include/Protocol/LmMesgBuf.h"

////
// constructor
////

SMsg_UniverseBroadcast::SMsg_UniverseBroadcast()
  : LmMesg(SMsg::UNIVERSEBROADCAST, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  data_.mtype = 0;
  data_.msize = 0;
  calc_size();
}

////
// destructor
////

SMsg_UniverseBroadcast::~SMsg_UniverseBroadcast()
{
  // empty
}

////
// Init
////

void SMsg_UniverseBroadcast::Init(LmMesg& msg)
{
  SetEnclosedMessage(msg);
}

void SMsg_UniverseBroadcast::Init(LmMesgBuf* msgbuf)
{
  SetEnclosedMessage(msgbuf);
}

////
// SetEnclosedMessage
////

void SMsg_UniverseBroadcast::SetEnclosedMessage(LmMesg& msg)
{
  // init header info
  data_.mtype = msg.MessageType();
  data_.msize = msg.MessageSize();
  // copy message data, in NETWORK order
  msg.ConvertToNetwork();
  memcpy(data_.msgbytes, msg.MessageAddress(), data_.msize);
  msg.ConvertToHost(); // leave in host order upon return
  // variable-length message
  calc_size();
}

void SMsg_UniverseBroadcast::SetEnclosedMessage(LmMesgBuf* msgbuf)
{
  // init header info
  data_.mtype = msgbuf->Header().MessageType();
  data_.msize = msgbuf->Header().MessageSize();
  // copy message data, in NETWORK order
  memcpy(data_.msgbytes, msgbuf->MessageAddress(), data_.msize);
  // variable-length message
  calc_size();
}

////
// CopyToBuffer - copy enclosed message into given message buffer
////

void SMsg_UniverseBroadcast::CopyToBuffer(LmMesgBuf& msgbuf)
{
  // create header
  LmMesgHdr mhdr;
  mhdr.Init(EnclosedMessageType(), EnclosedMessageSize());
  // init buffer
  msgbuf.ReadHeader(mhdr);
  // copy message bytes into buffer
  memcpy(msgbuf.MessageAddress(), MessageBytes(), EnclosedMessageSize());
}

////
// hton
////

void SMsg_UniverseBroadcast::hton()
{
  HTONL(data_.mtype);
  HTONL(data_.msize);
  // no conversion: msgbytes
}

////
// ntoh
////

void SMsg_UniverseBroadcast::ntoh()
{
  NTOHL(data_.mtype);
  NTOHL(data_.msize);
  // no conversion: msgbytes
  calc_size(); // variable-size message
}

////
// Dump: print to FILE stream
////

void SMsg_UniverseBroadcast::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_UniverseBroadcast[%p,%d]: "), this, sizeof(SMsg_UniverseBroadcast));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("mtype=%d msize=%d>\n"),
	    EnclosedMessageType(), EnclosedMessageSize());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}

////
// calc_size
////

void SMsg_UniverseBroadcast::calc_size()
{
  // initial size: whole structure, minus variable-sized field (message bytes)
  int size = sizeof(data_t) - sizeof(data_.msgbytes);
  // add message bytes length
  size += data_.msize;
  SetMessageSize(size);
}

int SMsg_UniverseBroadcast::EnclosedMessageType() const
{
	return data_.mtype;
}

int SMsg_UniverseBroadcast::EnclosedMessageSize() const
{
	return data_.msize;
}

const TCHAR* SMsg_UniverseBroadcast::MessageBytes() const
{
	return data_.msgbytes;
}