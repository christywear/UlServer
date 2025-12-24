// GsUtil.h  -*- C++ -*-
// $Id: GsUtil.h,v 1.13 1997-10-22 19:22:04-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// utility function class

#ifndef INCLUDED_GsUtil
#define INCLUDED_GsUtil

#ifdef __GNUC__
#pragma interface
#endif

#include "..\..\Core\LyraDefs.h"

// class forward declarations

class LmMesg;
class LmConnection;
class LmThread;
class LmLevelDBC;
class GsMain;
class GsPlayer;

// class declarations

struct GsUtil {
  static void SendInternalMessage(LmMesg& msg, LmThread* thread);
  static int SendInternalMessage(LmMesg& msg, int threadid);
  static void BroadcastInternalMessage(LmMesg& msg, int threadid, bool s2s = false);
  static void BroadcastInternalMessagePlayers(LmMesg& msg);
  static void Send_Error(LmConnection* conn, int msgtype, const TCHAR* fmt, ...);
  static void Send_GMsg_Error(LmConnection* conn, int msgtype, const TCHAR* fmt, ...);
  static void Send_SMsg_Error(LmConnection* conn, int msgtype, const TCHAR* fmt, ...);
  static void Send_RMsg_LevelLoginAck(LmConnection* conn, int status, lyra_id_t roomid, lyra_id_t levelid);
  static void Send_RMsg_RoomLoginAck(LmConnection* conn, int status);
  static void FakeLogout(LmConnection* conn);
  static void FakeLogout(GsPlayer* player);
  static LmConnection* ConnectToLevelServer(const LmLevelDBC* ldb);
  static int NightmareXP(int mare_index);
  static void HandleItemError(const TCHAR* method, int rc, int sc);
  static void HandlePlayerError(const TCHAR* method, int rc, int sc, bool exit);
};

#endif /* INCLUDED_GsUtil */
