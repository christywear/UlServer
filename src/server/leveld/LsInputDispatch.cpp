// LsInputDispatch.cpp  -*- C++ -*-
// $Id: LsInputDispatch.cpp,v 1.28 1998-04-17 17:40:32-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LsInputDispatch.h"
#endif
#include "../../../include/platform/Platform.h" //temp fix for old style legacy defines
#include <stdio.h>
#ifndef WIN32
#include <stdlib.h>
#include <unistd.h>
#endif

#include "../../../include/Server/Leveld/LsInputDispatch.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/LmSrvMesgBuf.h"
#include "../../../include/Protocol/LmConnection.h"
#include "../../../include/Server/Leveld/LsPlayer.h"
#include "../../../include/Server/Leveld/LsPlayerSet.h"
#include "../../../include/Core/LmThreadPool.h"
#include "../../../include/Protocol/RMsg/RMsg.h"
#include "../../../include/Protocol/SMsg/SMsg.h"
#include "../../../include/Protocol/LyraMessage.h"
#include "../../../include/Protocol/LmMesgBufPool.h"
#include <core/LmThreadPool.h>
#include <server/leveld/LsInputDispatch.h>

//init tracker
LsInputDispatch* LsInputDispatch::s_instance = nullptr;

////
// Constructor
////

LsInputDispatch::LsInputDispatch()
  : LmDispatch(LmMesgBufPool::Instance())
{
    s_instance = this;
  initialize_table();
}

////
// Destructor
////

LsInputDispatch::~LsInputDispatch()
{
    if (s_instance == this)
        s_instance == nullptr;
}

////
// ComputeTarget
////

LmThread* LsInputDispatch::ComputeTarget(LmSrvMesgBuf* mbuf, LmConnection* /* conn */)
{
  // check if message type is in dispatch table
  int target = GetTarget(mbuf->Header().MessageType());
  if (target == 0) {
    return 0;  // not found, no thread to handle message
  }
  // case on dispatch target
  switch (target) {
  case DT_LEVEL: {  // level thread
    return LmThreadPool::Instance()->GetThread(THREAD_LEVELSERVER);  //LmThreadPool
  }
  break;
  case DT_ROOM: {  // room thread
    return LmThreadPool::Instance()->GetThread(THREAD_ROOMSERVER);
  }
  break;
  default:
    break;
  }
  // default, no dispatch type match found
  return 0;
}

////
// Dump
////

void LsInputDispatch::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LsInputDispatch[%p,%d]: main=[%p]>\n"), this, sizeof(LsInputDispatch));
  // base class
  LmDispatch::Dump(f, indent + 1);
}

////
// message dispatch tables
////

// messages that the level thread accepts
static const int level_messages[] = {
  SMsg::LEVELLOGIN,
  SMsg::LOGIN,
  SMsg::LOGOUT,
  SMsg::SERVERERROR,
  SMsg::PING,
  SMsg::GETSERVERSTATUS,
  SMsg::ROTATE_LOGS, 
  SMsg::DUMP_STATE,
  SMsg::RESETPORT,
  SMsg::UNIVERSEBROADCAST
};
static const int num_level_messages = sizeof(level_messages) / sizeof(int);

// messages that the room thread accepts
static const int room_messages[] = {
  SMsg::GETITEM,
  SMsg::PUTITEM,
  SMsg::PROXY,
  SMsg::DESTROYROOMITEM,
  SMsg::GETLEVELPLAYERS,
  SMsg::GIVEITEM,
  SMsg::TAKEITEMACK,
  SMsg::LOCATEAVATAR,
  SMsg::SHOWITEM,
  SMsg::SETAVATARDESCRIPTION,
};
static const int num_room_messages = sizeof(room_messages) / sizeof(int);
  
////
// initialize_table
////

void LsInputDispatch::initialize_table()
{
  AddMappings(level_messages, num_level_messages, DT_LEVEL);
  AddMappings(room_messages, num_room_messages, DT_ROOM);
}
