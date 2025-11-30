// LmMesg.cpp  -*- C++ -*-
// $Id: LmMesg.cpp,v 1.14 1997-07-29 18:15:00-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// LmMesg implementation

#ifdef __GNUC__
#pragma implementation "LmMesg.h"
#endif

#include <stdio.h>
#include <string.h>

#include "../../include/Protocol/LmMesg.h"
#include "../../include/Protocol/LmMesgBuf.h"

////
// constructor
////

LmMesg::LmMesg(int msgtype, int max_msgsize, int msgsize, void* dptr)
  : byteorder_(ByteOrder::HOST),
    msgtype_(msgtype),
    max_msgsize_(max_msgsize),
    msgsize_(msgsize),
    dptr_(dptr)
{
  // empty
}

////
// destructor
////

LmMesg::~LmMesg()
{
  // empty -- don't delete data pointer, it belongs to the derived object!
}

////
// Read: initialize from LmMesgBuf; return 0 if successful, -1 otherwise
//   (if buffer not the same message type, or if the buffer contains a message
//   that is too large, or if the derived InitRaw() method returns -1)
////

int LmMesg::Read(LmMesgBuf& ibuf)
{
  // check message type, size
  if ((ibuf.Header().MessageType() == msgtype_) && (ibuf.MessageSize() <= max_msgsize_)) {
    return InitRaw(ibuf.MessageAddress(), ibuf.MessageSize());
  }
  // nope
  return -1;
}

////
// Read: initialize from LmMesgBuf; return 0 if successful, -1 otherwise
//    this version reads and exact # of bytes into the message; useful for peeking
////

int LmMesg::Read(LmMesgBuf& ibuf, unsigned int num_bytes)
{
  // check message type, size
  if ((ibuf.Header().MessageType() == msgtype_) && (num_bytes <= max_msgsize_)) {
    return InitRaw(ibuf.MessageAddress(), num_bytes);
  }
  // nope
  return -1;
}


////
// InitRaw: initialize from network-ordered raw bits; this default implementation
//   works for messages that never change size; message classes that have 
//   a dynamic length must override this method or bad things happen.
//   returns 0 if ok, -1 if error (never an error from this definition, however)
////

int LmMesg::InitRaw(const void* data, int datalen)
{
  memset(dptr_, 0, max_msgsize_); // zero out message data
  memcpy(dptr_, data, datalen);   // copy incoming message data
  SetMessageSize(datalen);
  ntoh();                         // convert to host byte order
  SetByteOrder(ByteOrder::HOST);
  return 0;                       // return OK
}

////
// ConvertToNetwork
////

void LmMesg::ConvertToNetwork()
{
  if (byteorder_ != ByteOrder::NETWORK) {
    hton();
    byteorder_ = ByteOrder::NETWORK;
  }
}

////
// ConvertToHost
////

void LmMesg::ConvertToHost()
{
  if (byteorder_ != ByteOrder::HOST) {
    ntoh();
    byteorder_ = ByteOrder::HOST;
  }
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void LmMesg::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LmMesg[%p,%d]: order=%c msize=%d/%d mtype=%d maddr=%p>\n"), this, sizeof(LmMesg),
	  ByteOrder(), MessageSize(), MaxMessageSize(), MessageType(), MessageAddress());
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void LmMesg::Dump(FILE*, int) const
{
    // empty
}
#endif /* !USE_DEBUG */

const void* LmMesg::MessageAddress() const
{
    return dptr_;
}

int LmMesg::ByteOrder() const
{
    return byteorder_;
}

int LmMesg::MessageSize() const
{
    return msgsize_;
}

int LmMesg::MaxMessageSize() const
{
    return max_msgsize_;
}

int LmMesg::MessageType() const
{
    return msgtype_;
}

void LmMesg::SetMessageAddress(void* dptr)
{
    dptr_ = dptr;
}

void LmMesg::SetByteOrder(int byteorder)
{
    byteorder_ = byteorder;
}

void LmMesg::SetMessageSize(int msgsize)
{
    msgsize_ = msgsize;
}

void LmMesg::SetMaxMessageSize(int max_msgsize)
{
    max_msgsize_ = max_msgsize;
}

void LmMesg::SetMessageType(int msgtype)
{
    msgtype_ = msgtype;
}
