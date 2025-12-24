// LsOutputDispatch.cpp  -*- C++ -*-
// $Id: LsOutputDispatch.cpp,v 1.12 1997-08-17 16:49:23-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LsOutputDispatch.h"
#endif

#include <stdarg.h>
#include <stdio.h>

#include "../../../include/Server/Leveld/LsOutputDispatch.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Server/Leveld/LsMain.h"
#include "../../../include/Core/LmThreadPool.h"
#include "../../../include/Protocol/RMsg/RMsg_All.h"
#include "../../../include/DB/LmLevelDBC.h"
#include "../../../include/DB/LmRoomDB.h"
#include "../../../include/Protocol/SMsg/SMsg_SendMesg.h"
#include "../../../include/Protocol/LmMesgBufPool.h"
#include "../../../include/Protocol/LmSrvMesgBuf.h"

//init tracker
LsOutputDispatch* LsOutputDispatch::s_instance = nullptr;
////
// Constructor
////

LsOutputDispatch::LsOutputDispatch()
  : LmDispatch(LmMesgBufPool::Instance())
{
  //register accessor
    s_instance = this;
}

////
// Destructor
////

LsOutputDispatch::~LsOutputDispatch()
{
    if (s_instance == this)
        s_instance == nullptr;
}

////
// SendMessage
////

void LsOutputDispatch::SendMessage(LmMesg* msg, LmConnection* conn)
{
  // put message in a buffer
  LmSrvMesgBuf* mbuf = LmMesgBufPool::Instance()->AllocateBuffer(msg->MessageSize());
  mbuf->ReadMessage(*msg);
  // send it
  SendMessage(mbuf, conn);
}

void LsOutputDispatch::SendMessage(LmSrvMesgBuf* mbuf, LmConnection* conn)
{
  // create "send message" message
  SMsg_SendMesg out_msg;
  out_msg.Init(conn, mbuf);
  // dispatch it
  if (DispatchMessage(&out_msg, 0) < 0) {
    LmMesgBufPool::Instance()->ReturnBuffer(mbuf);
  }
}

////
// ComputeTarget
////

LmThread* LsOutputDispatch::ComputeTarget(LmSrvMesgBuf* /* mbuf */, LmConnection* /* conn*/)
{
  // output goes to network output thread, always
  return LmThreadPool::Instance()->GetThread(THREAD_NETOUTPUT);
}

////
// Dump
////

void LsOutputDispatch::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LsOutputDispatch[%p,%d]: main=%p>\n"), this, sizeof(LsOutputDispatch));
  LmDispatch::Dump(f, indent + 1);
}
