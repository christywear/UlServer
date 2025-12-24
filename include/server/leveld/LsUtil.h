// LsUtil.h  -*- C++ -*-
// $Id: LsUtil.h,v 1.11 1997-10-22 19:22:15-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// utility function class

#ifndef INCLUDED_LsUtil
#define INCLUDED_LsUtil

#ifdef __GNUC__
#pragma interface
#endif

#include <list>

#include "..\..\Core\LyraDefs.h"

// class forward declarations

class LmMesg;
class LmConnection;
class LsMain;
class LsPlayer;
class LsPlayerList;
class LmThread;

// class declarations

struct LsUtil {
  static void SendInternalMessage(LmMesg& msg, LmThread* thr);
  static int SendInternalMessage(LmMesg& msg, int threadid);
  static void BroadcastInternalMessage(LmMesg& msg,int threadid, bool s2s = false);
  static void Send_SMsg_Error(LmConnection* conn, int msgtype, const TCHAR* fmt, ...);
  static void Send_SMsg_Proxy(LmConnection* conn, lyra_id_t playerid, LmMesg& msg);
  static void Send_SMsg_Proxy(LsPlayer* player, LmMesg& msg);
  static void Send_SMsg_Proxy(LsPlayerList& player_list, LmMesg& msg);
  static void Send_RMsg_Error(LsPlayer* player, int msgtype, const TCHAR* fmt, ...);
  static void Send_RMsg_LevelLoginAck(LsPlayer* player, int status, lyra_id_t roomid);
  static void Send_RMsg_RoomLoginAck(LsPlayer* player, short status, short num_neighbors);
  static void HandleItemError(const TCHAR* method, int rc, int sc);
};

#endif /* INCLUDED_LsUtil */
