// RMsg_LoginAck.cpp  -*- C++ -*-
// $Id: RMsg_LoginAck.cpp,v 1.13 1997-11-17 14:05:06-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "RMsg_LoginAck.h"
#endif
#include "../../../include/platform/Platform.h" //temp fix for old style legacy defines
#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#include "LmSockAddrInet.h" //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/RMsg/RMsg_LoginAck.h"
#include "../../../include/Protocol/RMsg/RMsg.h"
#include "../../../include/Core/LyraDefs.h"

////
// constructor
////

RMsg_LoginAck::RMsg_LoginAck()
  : LmMesg(RMsg::LOGINACK, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(LOGIN_UNKNOWN, Lyra::ID_UNKNOWN, Lyra::ID_UNKNOWN, INADDR_ANY, 0);
}

////
// destructor
////

RMsg_LoginAck::~RMsg_LoginAck()
{
  // empty
}

////
// Init
////

void RMsg_LoginAck::Init(int status, lyra_id_t roomid, lyra_id_t levelid)
{
  SetStatus(status);
  SetRoomID(roomid);
  SetLevelID(levelid);
}

void RMsg_LoginAck::Init(int status, lyra_id_t roomid, lyra_id_t levelid, unsigned long server_ip, int server_port)
{
  SetStatus(status);
  SetRoomID(roomid);
  SetLevelID(levelid);
  SetServerIP(server_ip);
  SetServerPort(server_port);
}

////
// hton
////

void RMsg_LoginAck::hton()
{
  HTONL(data_.status);
  HTONL(data_.roomid);
  HTONL(data_.levelid);
  HTONL(data_.server_port);
  // not converted: server_ip
}

////
// ntoh
////

void RMsg_LoginAck::ntoh()
{
  NTOHL(data_.status);
  NTOHL(data_.roomid);
  NTOHL(data_.levelid);
  NTOHL(data_.server_port);
  // not converted: server_ip
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void RMsg_LoginAck::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<RMsg_LoginAck[%p,%d]: "), this, sizeof(RMsg_LoginAck));
#ifndef WIN32 // relies on server classes
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("level=%u room=%u status=%c server=%s:%d>\n"),
	    LevelID(), RoomID(), Status(), LmSockAddrInet::AddressString(ServerIP()), ServerPort());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
#endif
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void RMsg_LoginAck::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

int RMsg_LoginAck::Status() const
{
	return data_.status;
}

lyra_id_t RMsg_LoginAck::RoomID() const
{
	return data_.roomid;
}

lyra_id_t RMsg_LoginAck::LevelID() const
{
	return data_.levelid;
}

unsigned long RMsg_LoginAck::ServerIP() const
{
	return data_.server_ip;
}

int RMsg_LoginAck::ServerPort() const
{
	return data_.server_port;
}

void RMsg_LoginAck::SetStatus(int status)
{
	data_.status = status;
}

void RMsg_LoginAck::SetRoomID(lyra_id_t roomid)
{
	data_.roomid = roomid;
}

void RMsg_LoginAck::SetLevelID(lyra_id_t levelid)
{
	data_.levelid = levelid;
}

void RMsg_LoginAck::SetServerIP(unsigned long server_ip)
{
	data_.server_ip = server_ip;
}

void RMsg_LoginAck::SetServerPort(int server_port)
{
	data_.server_port = server_port;
}