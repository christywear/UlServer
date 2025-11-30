// LsSignalThread.cpp  -*- C++ -*-
// $Id: LsSignalThread.cpp,v 1.53 1998-03-11 15:31:01-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LsSignalThread.h"
#endif
#include "../../../include/platform/Platform.h" //temp fix for old style legacy defines

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#ifndef WIN32
#include <stdlib.h>
#include <unistd.h>
#endif

#include "../../../include/Server/Leveld/LsLevelThread.h"
#include "../../../include/Server/Leveld/LsSignalThread.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Game/LmLog.h"
#include "../../../include/DB/LmLevelDBC.h"
#include "../../../include/DB/LmGlobalDB.h"
#include "../../../include/Protocol/LmSrvMesgBuf.h"
#include "../../../include/Protocol/LmMesg.h"
#include "../../../include/Core/LmThreadPool.h"
#include "../../../include/Protocol/LmMesgBufPool.h"
#include "../../../include/Server/Leveld/LsMain.h"
#include "../../../include/Protocol/SMsg/SMsg_LS_Action.h"
#include "../../../include/Server/Leveld/LsUtil.h"
#include "../../../include/Core/LmTimer.h"
#include "../../../include/Server/Leveld/LsPlayer.h"
#include "../../../include/Server/Leveld/LsPlayerSet.h"
#include "../../../include/Protocol/LmSocket.h"
#include "../../../include/Protocol/RMsg/RMsg_PlayerUpdate.h"
#include "../../../include/Protocol/LmConnection.h"
#include "../../../include/Protocol/LmConnectionSet.h"
#include "../../../include/Server/Leveld/LsMacros.h"
#include "../../../include/Server/Leveld/LsPlayerList.h"
#include "../../../include/Core/LmRand.h"

#include "../../../include/core/LmNew.h" //takes care of declare_thefilename macro
DECLARE_TheFileName;

////
// Constructor
////

LsSignalThread::LsSignalThread(LsMain* lsmain)
  : LmSignalThread(lsmain->BufferPool(), lsmain->Log() /* &logf_ */),
    main_(lsmain),
    msgbuf_(0)

{
  DECLARE_TheLineNum;

  register_message_handlers();
  register_signal_handlers();
}

////
// Destructor
////

LsSignalThread::~LsSignalThread()
{
  DECLARE_TheLineNum;
  LmDELETE(msgbuf_);
}

////
// Dump
////

void LsSignalThread::Dump(FILE* f, int indent) const
{
  DECLARE_TheLineNum;
  INDENT(indent, f);
 _ftprintf(f, _T("<LsSignalThread[%p,%d]: main=[%p] >\n"), this, sizeof(LsSignalThread),	  main_);
  msgbuf_->Dump(f, indent + 1);
  LmSignalThread::Dump(f, indent + 1);
}

////
// Run
////

void LsSignalThread::Run()
{
  DECLARE_TheLineNum;
  //  TLOG_Debug(_T("Signal thread starting"));
  //  TLOG_Debug(_T("%d ready, %d running, %d new, %d dead threads"), pth_ctrl(PTH_CTRL_GETTHREADS_READY), pth_ctrl(PTH_CTRL_GETTHREADS_RUNNING), pth_ctrl(PTH_CTRL_GETTHREADS_NEW), pth_ctrl(PTH_CTRL_GETTHREADS_DEAD));
  LmSignalThread::Run();
}


////
// register_message_handlers
////

void LsSignalThread::register_message_handlers()
{
  DECLARE_TheLineNum;
  SetDefaultHandler((MsgHandler) &LsSignalThread::handle_Default);
  RegisterHandler(SMsg::LS_ACTION,  (MsgHandler)&LsSignalThread::handle_SMsg_LS_Action);
}


////
// handle_Default: default message handler
////

void LsSignalThread::handle_Default(LmSrvMesgBuf* msgbuf, LmConnection* conn)
{
  DEFMETHOD(LsSignalThread, handle_Default);
  DECLARE_TheLineNum;
  HANDLER_ENTRY(true);
  // print error message to log
  TLOG_Error(_T("%s: unknown message type %d, size %d received"), method, msg_type, msg_size);
}

////
// handle_SMsg_LS_Action
////

void LsSignalThread::handle_SMsg_LS_Action(LmSrvMesgBuf* msgbuf, LmConnection* conn)
{
  DEFMETHOD(LsSignalThread, handle_SMsg_LS_Action);
  DECLARE_TheLineNum;
  HANDLER_ENTRY(true);
  // pre-conditions
  CHECK_CONN_NULL();
  // accept message
  ACCEPT_MSG(SMsg_LS_Action, false); // don't send error
  // process
  //  TLOG_Debug(_T("%s: action=%c"), method, msg.Action());
  switch (msg.Action()) {
  case SMsg_LS_Action::ACTION_EXIT:
    // TLOG_Log(_T("%s: exiting"), method);
    SetDone();
    break;
  default:
    //    TLOG_Error(_T("%s: illegal action code %c"), method, msg.Action());
    break;
  }
}

////
// register_signal_handlers
////

void LsSignalThread::register_signal_handlers()
{
  DECLARE_TheLineNum;
  SetDefaultSignalHandler((SignalHandler)&LsSignalThread::handle_SIG_Default);
  RegisterSignalHandler(SIGTERM, (SignalHandler)&LsSignalThread::handle_SIGTERM);
}


////
// handle_SIG_Default
////

void LsSignalThread::handle_SIG_Default(int sig)
{
  return;
  DEFMETHOD(LsSignalThread, handle_SIG_Default);
  DECLARE_TheLineNum;
  //  char buffer[256]; 
  // _stprintf(buffer, "%s: unknown signal '%s' (%d) received", method, strsignal(sig), sig);
  //  main_->SetSIGERR(buffer);
}



////
// handle_SIGTERM: shut down, and exit
////

void LsSignalThread::handle_SIGTERM(int sig)
{
  DEFMETHOD(LsSignalThread, handle_SIGTERM);
  DECLARE_TheLineNum;

  if (sig != SIGTERM) {
    return;
  }
  main_->SetSIGTERM(true);
}




