// LsUtil.cpp  -*- C++ -*-
// $Id: LsUtil.cpp,v 1.24 1998-02-12 15:33:40-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LsUtil.h"
#endif

#include "../../../include/platform/Platform.h" //temp fix for old style legacy defines
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>

#include <list>
#include <algorithm>

#include "../../../include/Server/Leveld/LsUtil.h"
#include "../../../include/Server/Leveld/LsMain.h"
#include "../../../include/Core/LmThread.h"
#include "../../../include/Core/LmThreadPool.h"
#include "../../../include/Protocol/LmMesg.h"
#include "../../../include/Protocol/LmSrvMesgBuf.h"
#include "../../../include/Protocol/LmMesgBufPool.h"
#include "../../../include/Protocol/RMsg/RMsg_All.h"
#include "../../../include/Protocol/SMsg/SMsg_All.h"
#include "../../../include/Server/Leveld/LsOutputDispatch.h"
#include "../../../include/Game/LmLogFile.h"
#include "../../../include/DB/LmServerDBC.h"
#include "../../../include/DB/LmLevelDBC.h"
#include "../../../include/Server/Leveld/LsPlayer.h"
#include "../../../include/Server/Leveld/LsPlayerList.h"
#include "../../../include/DB/LmItemDBC.h"
#include <core/LmThreadPool.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include "../../../include/core/LmNew.h" //takes care of declare_thefilename macro
DECLARE_TheFileName;

////
// SendInternalMessage
///

void LsUtil::SendInternalMessage(LmMesg& msg, LmThread* thr)
{
  DEFMETHOD(LsUtil, SendInternalMessage_1);
  DECLARE_TheLineNum;
  // allocate message buffer
  LmSrvMesgBuf* mbuf = LmMesgBufPool::Instance()->AllocateBuffer(msg.MessageSize());
  // read message into it
  mbuf->ReadMessage(msg);
  // send to thread, null connection
  thr->PassMessage(mbuf, 0);
}

int LsUtil::SendInternalMessage(LmMesg& msg, int threadid)
{
  DEFMETHOD(LsUtil, SendInternalMessage_2);
  DECLARE_TheLineNum;
  // look up target thread
  LmThread* thr = LmThreadPool::Instance()->GetThread(threadid);
  if (!thr) {
    LmLog::Instance()->Error(_T("%s: could not get thread for id %d"), method, threadid);
    return -1;
  }
  SendInternalMessage(msg, thr);
  // return successful
  return 0;
}

////
// BroadcastInternalMessage
////

void LsUtil::BroadcastInternalMessage(LmMesg& msg, int threadid, bool sendself)
{
  DEFMETHOD(LsUtil, BroadcastInternalMessage);
  DECLARE_TheLineNum;
  // get ids of all threads in server
  std::list<int> tids;
  LmThreadPool::Instance()->GetThreadIDs(tids);
  // if not sending to source thread, remove it from list
  if (!sendself) {
    remove(tids.begin(), tids.end(), threadid);
  }
  tids.unique();
  // allocate message buffer
  LmSrvMesgBuf* mbuf = LmMesgBufPool::Instance()->AllocateBuffer(msg.MessageSize(), tids.size());
  // read message into it
  mbuf->ReadMessage(msg);
  // send to each thread, from null connection (internal)
  for (std::list<int>::iterator i = tids.begin(); !(bool)(i == tids.end()); ++i) {
    LmThread* thr = LmThreadPool::Instance()->GetThread(*i);
    if (thr) {
      //main->Log()->Debug("%s: sending message[%p] to thread %d", method, mbuf, *i);
      thr->PassMessage(mbuf, 0);
    }
    else {
      LmLog::Instance()->Error(_T("%s: threadid %d not found in pool?"), method, *i);
    }
  }
}

////
// Send_RMsg_Error - send through game server proxy
////

void LsUtil::Send_RMsg_Error(LsPlayer* player, int msgtype, const TCHAR* fmt, ...)
{
  DECLARE_TheLineNum;
  RMsg_Error msg;
  TCHAR errstring[RMsg_Error::ERRMSG_LEN * 2];  // slack space, don't have snprintf
  va_list args;

  va_start(args, fmt);
 _vstprintf(errstring, fmt, args);
  va_end(args);

  msg.Init(msgtype, errstring);
  LsUtil::Send_SMsg_Proxy(player, msg);
}

////
// Send_RMsg_LoginAck
////

void LsUtil::Send_RMsg_LevelLoginAck(LsPlayer* player, int status, lyra_id_t roomid)
{
  DECLARE_TheLineNum;
  RMsg_LoginAck msg;
  msg.Init(status, roomid, LmLevelDBC::Instance()->LevelID());
  LsUtil::Send_SMsg_Proxy(player, msg);
}

////
// Send_RMsg_RoomLoginAck
////

void LsUtil::Send_RMsg_RoomLoginAck(LsPlayer* player, short status, short num_neighbors)
{
  DECLARE_TheLineNum;
  RMsg_RoomLoginAck msg;
  msg.Init(status, num_neighbors);
  LsUtil::Send_SMsg_Proxy(player, msg);
}
////
// Send_SMsg_Proxy - send message to player through game server
////

void LsUtil::Send_SMsg_Proxy(LmConnection* conn, lyra_id_t playerid, LmMesg& msg)
{
  DECLARE_TheLineNum;
  SMsg_Proxy msg_proxy;
  msg_proxy.Init(playerid, SMsg_Proxy::PROXY_FORWARD, msg);
  LsOutputDispatch::Instance()->SendMessage(&msg_proxy, conn);
}

void LsUtil::Send_SMsg_Proxy(LsPlayer* player, LmMesg& msg)
{
  DECLARE_TheLineNum;
  LsUtil::Send_SMsg_Proxy(player->Connection(), player->PlayerID(), msg);
}

void LsUtil::Send_SMsg_Proxy(LsPlayerList& player_list, LmMesg& msg)
{
  DECLARE_TheLineNum;
  // unique the list, in case there are duplicates
  player_list.sort();
  player_list.unique();
  // unfortunately there's currently no way to multicast these through the game
  // server, they must be sent one-by-one.
  LsPlayerList::iterator i;
  for (i = player_list.begin(); !(bool)(i == player_list.end()); ++i) {
    LsPlayer* player = *i;
    LsUtil::Send_SMsg_Proxy(player->Connection(), player->PlayerID(), msg);
  }
}

////
// Send_SMsg_Error
////

void LsUtil::Send_SMsg_Error(LmConnection* conn, int msgtype, const TCHAR* fmt, ...)
{
  DECLARE_TheLineNum;
  SMsg_Error msg;
  TCHAR errstring[SMsg_Error::ERRMSG_LEN * 2];  // slack space, don't have snprintf
  va_list args;

  va_start(args, fmt);
 _vstprintf(errstring, fmt, args);
  va_end(args);

  msg.Init(msgtype, errstring);
  LsOutputDispatch::Instance()->SendMessage(&msg, conn);
}

////
// HandleItemError - handle error returned from LmItemDBC
////

void LsUtil::HandleItemError(const TCHAR* calling_method, int rc, int sc)
{
  static bool send_mail = true;
  switch (rc) {
  case 0:
    return;
  case LmItemDBC::MYSQL_ERROR: {
    LmLog::Instance()->Error(_T("%s: itemdb fatal error encountered; sqlcode=%d"), calling_method, sc);
    if (send_mail) {
      // send mail to database admin
      TCHAR hname[256];
// *** STRING LITERAL ***  
     _stprintf(hname, _T("(unknown)"));
      gethostname((char*)(hname), sizeof(hname));
      /*SendMail(_T("leveld@underlight"), _T("Underlight: database error"),
		       _T("HostName: %s\n") _T("Server Info: level %d, pid %lu\n")  _T("Error: item database fatal error in method %s\n") _T("Reason: sqlcode %d\n"),
		       hname,LmLevelDBC::Instance()->LevelID(), (unsigned long)_getpid(),
		       calling_method, sc);*/
      send_mail = false; // only do this once
    }
    // shut down server
#ifdef WIN32 
	//main->SetSIGTERM(true); global shutdown temp nuked !Christy look at reimplimentation
#else
    kill(getpid(), SIGTERM);
#endif
  }
  break;
  default:
    LmLog::Instance()->Error(_T("%s: itemdb unknown return code %d; sqlcode=%d"), calling_method, rc, sc);
    break;
  }
}
