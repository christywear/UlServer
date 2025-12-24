// GsNetworkInput.cpp  -*- C++ -*-
// $Id: GsNetworkInput.cpp,v 1.40 1998/06/18 00:53:13 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

//HELLO!

#ifdef __GNUC__
#pragma implementation "GsNetworkInput.h"
#endif
#include "../../../include/platform/Platform.h" //temp fix for old style legacy defines
#include <errno.h>
#include <string.h>
#ifndef WIN32
#include <stdlib.h>
#include <unistd.h> //linux
#include <time.h>

#endif
#include "../../../include/Server/Gamed/GsNetworkInput.h"
#include "../../../include/DB/LmLevelDBC.h"
#include "../../../include/DB/LmGlobalDB.h"
#include "../../../include/Server/Gamed/GsMessageReader.h"
#include "../../../include/Server/Gamed/GsMain.h"
#include "../../../include/Server/Gamed/GsInputDispatch.h"
#include "../../../include/Game/LmLog.h"
#include "../../../include/Server/Gamed/GsUtil.h"
#include "../../../include/Protocol/LmMesgBuf.h"
#include "../../../include/Protocol/SMsg/SMsg_All.h"
#include "../../../include/Core/LmLocker.h"
#include "../../../include/Protocol/LmConnection.h"
#include "../../../include/Protocol/LmSrvMesgBuf.h"
#include "../../../include/Protocol/LmConnectionSet.h"
#include "../../../include/Protocol/LmConnectionList.h"
#include "../../../include/Protocol/LmMesgBufPool.h"
#include "../../../include/Protocol/GMsg/GMsg_All.h"
#include "../../../include/Server/Gamed/GsPlayer.h"
#include "../../../include/Server/Gamed/GsPlayerSet.h"
#include "../../../include/Game/LmLogFile.h"
#include "../../../include/Server/Gamed/GsMacros.h"
#include "../../../include/Server/Gamed/GsPlayerList.h"

#include "../../../include/core/LmNew.h" //takes care of declare_thefilename macro
DECLARE_TheFileName;

////
// Constructor
////

GsNetworkInput::GsNetworkInput()
  : LmNetworkInput(GsInputDispatch::Instance(), LmConnectionSet::Instance(), LmMesgBufPool::Instance(),
		   LmLog::Instance() /* &logf_ */ )
{
  DECLARE_TheLineNum;
  open_log();
  register_message_handlers();
  AddListener(LmSocket::Instance());
  StartIdleReaders(6);
}

////
// Destructor
////

GsNetworkInput::~GsNetworkInput()
{
  DECLARE_TheLineNum;
  close_log();
}

////
// StartIdleReaders - start up given number of idle message reader threads
////

void GsNetworkInput::StartIdleReaders(int num_readers)
{
  DECLARE_TheLineNum;
  for (int i = 0; i < num_readers; ++i) {
    AddReader(StartReader());
  }
}

////
// Run
////

void GsNetworkInput::Run()
{
  //TLOG_Debug(_T("Network input thread pid = %d"), getpid());
  LmUtil::BlockAllSignals();
  DECLARE_TheLineNum;
  LmNetworkInput::Run();
}

////
// StartReader - create and start a message reader thread
////

LmMessageReader* GsNetworkInput::StartReader()
{
  DEFMETHOD(GsNetworkInput, StartReader);
  DECLARE_TheLineNum;
  GsMessageReader* thr = LmNEW(GsMessageReader(Log()));
  PThAttr attr;
  attr.Init();
  attr.SetStackSize(131072);
  // create thread
  if (thr->Create(&attr) < 0) {
    TLOG_Error(_T("%s: could not create reader thread: %s"), method, strerror(errno));
    LmDELETE(thr);
    thr = 0;
  }
  //  TLOG_Debug(_T("%s: network input reader thread pid %u created"), method, getpid());
  return thr;
}

////
// Dump
////

void GsNetworkInput::Dump(FILE* f, int indent) const
{
  DECLARE_TheLineNum;
  INDENT(indent, f);
 _ftprintf(f, _T("<GsNetworkInput[%p,%d]: main=[%p]>\n"), this, sizeof(GsNetworkInput));
  LmNetworkInput::Dump(f, indent + 1);
}

////
// open_log
////

void GsNetworkInput::open_log()
{
  // logf_.Init("gs", "in", main_->ServerPort());
  // logf_.Open(LmGlobalDB::Instance()->LogDir());
}

////
// close_log
////

void GsNetworkInput::close_log()
{
  // logf_.Close();
}

////
// register_message_handlers
////


void GsNetworkInput::register_message_handlers()
{
  DECLARE_TheLineNum;
  SetDefaultHandler((MsgHandler) &GsNetworkInput::handle_Default);
  RegisterHandler(SMsg::GS_ACTION, (MsgHandler)&GsNetworkInput::handle_SMsg_GS_Action);
}


////
// handle_Default
////

void GsNetworkInput::handle_Default(LmSrvMesgBuf* msgbuf, LmConnection* conn)
{
  DEFMETHOD(GsNetworkInput, (GsNetworkInput::handle_Default));
  DECLARE_TheLineNum;
  HANDLER_ENTRY(true);
  // print error message to log
  TLOG_Error(_T("%s: unknown message type %d, size %d received"), method, msg_type, msg_size);
}

////
// handle_SMsg_GS_Action
////

void GsNetworkInput::handle_SMsg_GS_Action(LmSrvMesgBuf* msgbuf, LmConnection* conn)
{
  DEFMETHOD(GsNetworkInput, handle_SMsg_GS_Action);
  DECLARE_TheLineNum;
  HANDLER_ENTRY(false);
  // pre-conditions
  CHECK_CONN_NULL();
  // accept message
  ACCEPT_MSG(SMsg_GS_Action, false); // don't send error
  // TLOG_Debug(_T("%s: action=%c"), method, msg.Action());
  // switch on action to perform
  switch (msg.Action()) {
  case SMsg_GS_Action::ACTION_EXIT:
    TLOG_Log(_T("%s: shutting down"), method);
    SetDone();
    break;
  case SMsg_GS_Action::ACTION_CHECK_IDLE_PLAYERS:
    handle_SMsg_GS_Action_CheckIdlePlayers();
    break;
  case SMsg_GS_Action::ACTION_CHECK_IDLE_CLIENTS:
    handle_SMsg_GS_Action_CheckIdleClients();
    break;
  case SMsg_GS_Action::ACTION_CHECK_MSG_THREADS:
    handle_SMsg_GS_Action_CheckMessageThreads();
    break;
  case SMsg_GS_Action::ACTION_HEARTBEAT:
    TLOG_Log(_T("%s: network input thread running"), method);
    break;
  default:
    TLOG_Error(_T("%s: illegal action code %c"), method, msg.Action());
    break;
  }
}

////
// handle_SMsg_GS_Action_CheckIdlePlayers
////

void GsNetworkInput::handle_SMsg_GS_Action_CheckIdlePlayers()
{
  DEFMETHOD(GsNetworkInput, handle_SMsg_GS_Action_CheckIdlePlayers);
  DECLARE_TheLineNum;
//  TLOG_Debug(_T("%s: checking idle players"), method);
  // get list of players
  GsPlayerList plist;
  GsPlayerSet::Instance()->GetPlayerList(plist);
  for (GsPlayerList::iterator i = plist.begin(); !(bool)(i == plist.end()); ++i) {
    GsPlayer* player = *i;
    LmConnection* conn = player->Connection();

    int idle_time = player->IdleTime();
//    TLOG_Debug(_T("%s: checking idle timeout for player %u (%p), conn=%u, idle time=%u, online=%u"), method, player->PlayerID(), player, conn, idle_time, player->Online());

    if (!conn) {
      TLOG_Warning(_T("%s: null connection for player %u [%p]"), method, player->PlayerID(), player);
      // log them out
      GsUtil::FakeLogout(player);
      continue;
    }
    // only make these checks if player has been online for a bit,  else a 
    // player in the login phase could be logged out with weird consequences
    if (player->Online() > PLAYER_TIMEOUT) {
      // check that connection is a client
      if (conn->Type() != LmConnection::CT_CLIENT) {
	TLOG_Warning(_T("%s: conn type %c != client for player %u"), method, conn->Type(), player->PlayerID());
	// log them out
	GsUtil::FakeLogout(player);
	continue;
      }
      // check that connection id matches player id
      if (conn->ID() != player->PlayerID()) {
	TLOG_Warning(_T("%s: conn id %u != player id %u"), method, conn->ID(), player->PlayerID());
	// log them out
	GsUtil::FakeLogout(player);
	continue;
      }
    }
	// check idle time
   if ((player->DB().AccountType() == LmPlayerDB::ACCT_MONSTER) &&
       (idle_time > AGENT_TIMEOUT))    { 
      TLOG_Debug(_T("%s: idle timeout for agent %u (%p) (ghosted?)"), method, player->PlayerID(), player);
      // log them out
      GsUtil::FakeLogout(player);
      continue;
    }
   if ((player->DB().AccountType() != LmPlayerDB::ACCT_MONSTER) &&
       (idle_time > PLAYER_TIMEOUT))   { 
      TLOG_Debug(_T("%s: idle timeout for player %u (%p) (ghosted?)"), method, player->PlayerID(), player);
      // log them out
      GsUtil::FakeLogout(player);
      continue;
    }

  }
}

////
// handle_SMsg_GS_Action_CheckIdleClients
////

void GsNetworkInput::handle_SMsg_GS_Action_CheckIdleClients()
{
  DEFMETHOD(GsNetworkInput, handle_SMsg_GS_Action_CheckIdleClients);
  DECLARE_TheLineNum;
  //TLOG_Debug(_T("%s: checking idle clients"), method);
  // get list of clients
  LmConnectionList conn_list;
  LmConnectionSet::Instance()->GetConnectionList(conn_list);
  //TLOG_Debug(_T("%s: checking idle clients - number of connections: %d"), method, conn_list.size());
  // check each non-player client
  for (LmConnectionList::iterator i = conn_list.begin(); !(bool)(i == conn_list.end()); ++i) {
    LmConnection* conn = *i;

	if (!conn) 
		LmConnectionSet::Instance()->RemoveConnection(conn);

    // check if connection is a client, and that player is live
    if ((conn->Type() == LmConnection::CT_CLIENT) && (conn->ID() > 0) && (GsPlayerSet::Instance()->IsInGame(conn->ID()))) 
      continue;
   
    // check if connection is a level server (which never are timed out)
    if (conn->Type() == LmConnection::CT_LSRV) 
      continue;

//	if (conn->IsClosed()) // socket already closed
//		continue;

    // check if last incoming message was past the idle timeout
	if (conn->IdleIn() > CLIENT_TIMEOUT) {
			LmConnectionSet::Instance()->RemoveConnection(conn); 
	}
  }
}

////
// handle_SMsg_GS_Action_CheckMessageThreads
////

void GsNetworkInput::handle_SMsg_GS_Action_CheckMessageThreads()
{
  DEFMETHOD(GsNetworkInput, handle_SMsg_GS_Action_CheckMessageThreads);
  DECLARE_TheLineNum;
#if 0
  static const TCHAR* method = _T("handle_SMsg_GS_Action_CheckMessageThreads");
  // check each message thread
  for (reader_list_t::iterator i = readers_.begin(); i != readers_.end(); ++i) {
    GsMessageReader* thr = (*i).thread;
    if (thr->StartTime() == 0) { // not in use
      continue;
    }
    if (thr->IsRunning() && (LmUtil::TimeSince(thr->StartTime()) > MSG_THREAD_TIMEOUT)) {
      TLOG_Debug(_T("%s: reader thread %p timed out"), method, thr);
      thr->Cancel();
      GsUtil::FakeLogout((*i).conn);
    }
  }
#endif
}

