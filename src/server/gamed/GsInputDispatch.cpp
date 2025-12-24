// GsInputDispatch.cpp  -*- C++ -*-
// $Id: GsInputDispatch.cpp,v 1.39 1998/05/02 00:09:10 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "GsInputDispatch.h"
#endif
#include "../../../include/platform/Platform.h" //temp fix for old style legacy defines
#ifndef WIN32
#include <stdlib.h>
#include <unistd.h> //linux
#endif
#include "../../../include/Server/Gamed/GsInputDispatch.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Server/Gamed/GsMain.h"
#include "../../../include/Protocol/LmSrvMesgBuf.h"
#include "../../../include/Protocol/LmConnection.h"
#include "../../../include/Server/Gamed/GsPlayer.h"
#include "../../../include/Server/Gamed/GsPlayerSet.h"
#include "../../../include/Core/LmThreadPool.h"
#include "../../../include/Protocol/GMsg/GMsg.h"
#include "../../../include/Protocol/RMsg/RMsg.h"
#include "../../../include/Protocol/SMsg/SMsg.h"
#include "../../../include/Protocol/SMsg/SMsg_DBQuery.h"
#include "../../../include/Protocol/SMsg/SMsg_DBQueryAck.h"
#include "../../../include/Protocol/GMsg/GMsg_Ping.h"
#include "../../../include/Game/LmLogFile.h"
#include "../../../include/Protocol/LyraMessage.h"
#include <protocol/LmMesgBufPool.h>

//init accessor
GsInputDispatch* GsInputDispatch::s_instance = nullptr;

////
// Constructor
////

GsInputDispatch::GsInputDispatch()
  : LmDispatch(LmMesgBufPool::Instance())
{
    s_instance = this;
  initialize_table();
}

////
// Destructor
////

GsInputDispatch::~GsInputDispatch()
{
    if (s_instance == this)
        s_instance == nullptr;
}

////
// ComputeTarget
////

LmThread* GsInputDispatch::ComputeTarget(LmSrvMesgBuf* mbuf, LmConnection* conn)
{
  DEFMETHOD(GsInputDispatch, ComputeTarget);
  int m_type = mbuf->Header().MessageType();
  //LmLog::Instance()->Debug(_T("%s: mbuf=%p conn=%p; mtype=%d msize=%d"), method, mbuf, conn, m_type, mbuf->Header().MessageSize());
  // check if message type is in dispatch table
  int target = GetTarget(m_type);
  if (target == 0) {
    LmLog::Instance()->Warning(_T("%s: message type %d not found in table"), method, m_type);
    return 0;  // not found, no thread to handle message
  }
  // if message is a ping message, target depends on ping type
  if (m_type == GMsg::PING) {
    GMsg_Ping msg;
    msg.Read(*mbuf);
    if (msg.PingType() == GMsg_Ping::PING_GAME_THREAD) {
      target = DT_GAME;
    }
    else {
      target = DT_PLAYER;
    }
  }
  LmThread* thread = 0;
  // case on dispatch target
  switch (target) {
  case DT_GAME: {  // game thread
    thread = LmThreadPool::Instance()->GetThread(THREAD_GAMESERVER);
    //LmLog::Instance()->Debug(_T("%s: dispatching to game thread (%p)"), method, thread);
  }
  break;
  case DT_PLAYER: {  // player thread, from client
	  // generally the connection ID has the player ID used to locate the proper thread
	  // but, if it's a database ack, we need to crack open the message
	lyra_id_t player_id = conn->ID();
	if (m_type == SMsg::DS_DB_QUERY_ACK_PT) {
		SMsg_DBQueryAck msg;
		// because db return values can be huge, we don't want to decode the whole
		// thing here; we just peek at the first four bytes (player_id)
		msg.Read(*mbuf, sizeof(lyra_id_t));
		player_id = msg.ID();
    }
    thread = LmThreadPool::Instance()->GetThread(player_id);
    //LmLog::Instance()->Debug(_T("%s: dispatching message; connection/player id=%u, thread=%p, mtype=%d"), method, conn->ID(), thread, m_type);
  }
  break;
  case DT_FORWARD: { // forward to player thread, from server
    thread = LmThreadPool::Instance()->GetThread(THREAD_FORWARD);
    //LmLog::Instance()->Debug(_T("%s: dispatching to forward thread (%p)"), method, thread);
  }
  break;
  case DT_POSITION: { // position thread, from client
    thread = LmThreadPool::Instance()->GetThread(THREAD_POSITION);
    //LmLog::Instance()->Debug(_T("%s: dispatching to position thread (%p)"), method, thread);
	}
  break;
					
  default:
    break;
  }
  // return computed thread, or null
  return thread;
}

////
// Dump
////

void GsInputDispatch::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GsInputDispatch[%p,%d]: main=[%p]>\n"), this, sizeof(GsInputDispatch));
  // base class
  LmDispatch::Dump(f, indent + 1);
}

////
// message dispatch tables
////

// messages that the game thread accepts
static const int game_messages[] = {
  // GMsg
  GMsg::PRELOGIN,
  GMsg::LOGIN,
  GMsg::AGENTLOGIN,
  GMsg::CREATEPLAYER,
  GMsg::SERVERERROR,
  // SMsg
  SMsg::LOGIN,
  SMsg::LOGOUT,
  SMsg::SERVERERROR,
  SMsg::GETSERVERSTATUS,
  SMsg::ROTATE_LOGS, 
  SMsg::DUMP_STATE,
  SMsg::UNIVERSEBROADCAST,
  // SMsg
  SMsg::DS_DB_QUERY_ACK_GT,
  // RMsg
  RMsg::SERVERERROR,
};
static const int num_game_messages = sizeof(game_messages) / sizeof(int);

// messages that the player thread accepts
static const int player_messages[] = {
  // GMsg
  GMsg::LOGOUT,
  GMsg::GETITEM,
  GMsg::PUTITEM,
  GMsg::CREATEITEM,
  GMsg::DESTROYITEM,
  GMsg::UPDATEITEM,
  GMsg::UPDATESTATS,
  GMsg::GOTOLEVEL,
  GMsg::CHANGESTAT,
  GMsg::DESTROYROOMITEM,
  GMsg::GETLEVELPLAYERS,
  GMsg::CHANGEAVATAR,
  GMsg::GOAL,
  GMsg::GETGOALHDRS,
  GMsg::GETREPORTHDRS,
  GMsg::POSTGOAL,
  GMsg::POSTREPORT,
  GMsg::GIVEITEM,
  GMsg::TAKEITEMACK,
  GMsg::LOCATEAVATAR,
  GMsg::PING,
  GMsg::GETITEMDESCRIPTION,
  GMsg::SHOWITEM,
  GMsg::SETAVATARDESCRIPTION,
  GMsg::GETPLAYERNAME,
  GMsg::USEPPOINT,
  GMsg::GRANTPPOINT,
  GMsg::PPOINTACK,
  // SMsg
  SMsg::DS_DB_QUERY_ACK_PT,
  // RMsg
  RMsg::LOGOUT,
  RMsg::GOTOROOM,
  RMsg::PARTY,
  RMsg::SPEECH,
  RMsg::PLAYERMSG,
  RMsg::PING,
  RMsg::GETAVATARDESCRIPTION,
  RMsg::GETROOMDESCRIPTION,
};
static const int num_player_messages = sizeof(player_messages) / sizeof(int);

// messages that the forward thread accepts
static const int forward_messages[] = {
  SMsg::ITEMDROP,
  SMsg::ITEMPICKUP,
  SMsg::PROXY,
  SMsg::GIVEITEM,
  SMsg::GIVEITEMACK,
  SMsg::TAKEITEMACK,
  SMsg::LOCATEAVATAR,
  SMsg::PING,
  SMsg::PARTYLEADER,
  SMsg::SHOWITEM,
};
static const int num_forward_messages = sizeof(forward_messages) / sizeof(int);

// messages that the position thread accepts
static const int position_messages[] = {
	RMsg::UPDATE,
};

static const int num_position_messages = sizeof(position_messages) / sizeof(int);


////
// initialize_table
////

void GsInputDispatch::initialize_table()
{
  AddMappings(game_messages, num_game_messages, DT_GAME);
  AddMappings(player_messages, num_player_messages, DT_PLAYER);
  AddMappings(forward_messages, num_forward_messages, DT_FORWARD);
  AddMappings(position_messages, num_position_messages, DT_POSITION);
}
