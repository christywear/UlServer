// LmMesgHdr.cpp  -*- C++ -*-
// $Id: LmMesgHdr.cpp,v 1.13 1997-07-29 18:15:00-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// Implementation of LmMesgHdr class

#ifdef __GNUC__
#pragma implementation "LmMesgHdr.h"
#endif

#include "../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../include/core/unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../include/Protocol/LmMesgHdr.h"
#include "../../include/Core/LyraDefs.h"

////
// constructor
////

LmMesgHdr::LmMesgHdr()
{
  Init(DEFAULT_MSGTYPE, DEFAULT_MSGSIZE);
}

////
// Init: given type and length in host order
////

void LmMesgHdr::Init(int mtype, int msize)
{
  SetMessageType(mtype);
  SetMessageSize(msize);
  SetByteOrder(ByteOrder::HOST);
}

////
// ConvertToNetwork
////

void LmMesgHdr::ConvertToNetwork()
{
  if (byteorder_ != ByteOrder::NETWORK) {
    HTONS(hdr_.mesg_type);
    HTONS(hdr_.mesg_size);
    byteorder_ = ByteOrder::NETWORK;
  }
}

////
// ConvertToHost
////

void LmMesgHdr::ConvertToHost()
{
  if (byteorder_ != ByteOrder::HOST) {
    NTOHS(hdr_.mesg_type);
    NTOHS(hdr_.mesg_size);
    byteorder_ = ByteOrder::HOST;
  }
}

////
// MessageType: return in host order always
////

int LmMesgHdr::MessageType() const
{
  return (byteorder_ == ByteOrder::HOST) ? hdr_.mesg_type : ntohs(hdr_.mesg_type);
}

////
// MessageSize: return in host order always
////

int LmMesgHdr::MessageSize() const
{
  return (byteorder_ == ByteOrder::HOST) ? hdr_.mesg_size : ntohs(hdr_.mesg_size);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void LmMesgHdr::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LmMesgHdr[%p,%d]: order=%c mtype=%d msize=%d hsize=%d haddr=%p>\n"), this, sizeof(LmMesgHdr),
	  ByteOrder(), MessageType(), MessageSize(), HeaderSize(), HeaderAddress());
}
#endif

////
// Dump1: print to FILE stream in "shorthand" format
////

#ifdef USE_DEBUG
void LmMesgHdr::Dump1(FILE* f) const
{
 _ftprintf(f, _T("[%d:%d]"), MessageType(), MessageSize());
}
#endif

#ifndef USE_DEBUG
void LmMesgHdr::Dump(FILE*, int) const
{
    // empty
}

void LmMesgHdr::Dump1(FILE*) const
{
    // empty
}
#endif /* !USE_DEBUG */

int LmMesgHdr::ByteOrder() const
{
    return byteorder_;
}

int LmMesgHdr::HeaderSize()
{
    return sizeof(lyra_mesg_hdr_t);
}

void* LmMesgHdr::HeaderAddress()
{
    return &hdr_;
}

const void* LmMesgHdr::HeaderAddress() const
{
    return &hdr_;
}

void LmMesgHdr::SetByteOrder(int byteorder)
{
    byteorder_ = byteorder;
}

void LmMesgHdr::SetMessageType(int mtype)
{
    hdr_.mesg_type = mtype;
}

void LmMesgHdr::SetMessageSize(int msize)
{
    hdr_.mesg_size = msize;
}
