// GMsg_PPointAck.cpp  -*- C++ -*-
// $Id: GMsg_PPointAck.cpp,v 1.1 1997-09-19 16:42:16-07 jason Exp $
// Copyright 2002 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_PPointAck.h"
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

#include "../../../include/Protocol/GMsg/GMsg_PPointAck.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_PPointAck::GMsg_PPointAck()
  : LmMesg(GMsg::PPOINTACK, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, 0);
}

////
// destructor
////

GMsg_PPointAck::~GMsg_PPointAck()
{
  // empty
}

////
// Init
////

void GMsg_PPointAck::Init(int type, int result)
{
  SetType(type);
  SetResult(result);
  //SetVar1(var1);
  //SetVar2(var2);
}

////
// hton
////

void GMsg_PPointAck::hton()
{
  //HTONL(data_.type);
  //HTONS(data_.result);
  //HTONL(data_.var1);
  //HTONL(data_.var2);
}

////
// ntoh
////

void GMsg_PPointAck::ntoh()
{
  //NTOHL(data_.type);
  //NTOHS(data_.result);
  //NTOHL(data_.var1);
  //NTOHL(data_.var2);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_PPointAck::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_PPointAck[%p]: "), this);
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("type: %c, result: %c \n"), Type(), Result());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void GMsg_PPointAck::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

unsigned char GMsg_PPointAck::Type() const
{
	return data_.type;
}

unsigned char GMsg_PPointAck::Result() const
{
	return data_.result;
}
/*
int GMsg_PPointAck::Var1() const
{
  return data_.var1;
}

int GMsg_PPointAck::Var2() const
{
  return data_.var2;
}

void GMsg_PPointAck::SetVar1(int value)
{
  data_.var1 = value;
}

void GMsg_PPointAck::SetVar2(int value)
{
  data_.var2 = value;
}
*/
void GMsg_PPointAck::SetType(unsigned char value)
{
	data_.type = value;
}

void GMsg_PPointAck::SetResult(unsigned char value)
{
	data_.result = value;
}
