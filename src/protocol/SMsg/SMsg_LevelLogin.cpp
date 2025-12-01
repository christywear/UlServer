// SMsg_LevelLogin.cpp  -*- C++ -*-
// $Id: SMsg_LevelLogin.cpp,v 1.12 1998-04-17 16:32:00-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_LevelLogin.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/SMsg/SMsg_LevelLogin.h"

////
// constructor
////

SMsg_LevelLogin::SMsg_LevelLogin()
  : LmMesg(SMsg::LEVELLOGIN, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, _T("(unknown)"), 'L', INADDR_ANY, Lyra::PORT_UNKNOWN,
       LmAvatar::DEFAULT_INSTANCE, Lyra::ID_UNKNOWN, LmPeerUpdate::DEFAULT_INSTANCE, 0, _T("none"), 0, 0, 0);
}

////
// destructor
////

SMsg_LevelLogin::~SMsg_LevelLogin()
{
  // empty
}

void SMsg_LevelLogin::Init(lyra_id_t playerid, const TCHAR* playername, int acct_type, unsigned long ipaddr,
			   int serverport, const LmAvatar& avatar, lyra_id_t roomid,
			   const LmPeerUpdate& update, unsigned char hidden, const TCHAR* description, 
			   unsigned char newly_alert, unsigned char newly_awakened, unsigned char unused)
{
  SetPlayerID(playerid);
  SetPlayerName(playername);
  SetAccountType(acct_type);
  SetIPAddress(ipaddr);
  SetServerPort(serverport);
  SetAvatar(avatar);
  SetRoomID(roomid);
  SetPeerUpdate(update);
  SetHidden(hidden);
//  SetTCPOnly(tcp_only);
  SetDescription(description);
  SetNewlyAlert(newly_alert);
  SetNewlyAwakened(newly_awakened);
}

////
// hton
////

void SMsg_LevelLogin::hton()
{
  HTONL(data_.playerid);
  HTONL(data_.acct_type);
  HTONL(data_.udp_portnum);
  data_.avatar.ConvertToNetwork();
  HTONL(data_.roomid);
  data_.update.ConvertToNetwork();
  // no conversion: ip address, playername, description, 
  // hidden, unused, newly_alert, newly_awakened
}

////
// ntoh
////

void SMsg_LevelLogin::ntoh()
{
  NTOHL(data_.playerid);
  NTOHL(data_.acct_type);
  NTOHL(data_.udp_portnum);
  data_.avatar.ConvertToHost();
  NTOHL(data_.roomid);
  data_.update.ConvertToHost();
  // no conversion: ip address, playername, description, 
  // newly_alert, newly_awakened, hidden, unused
  calc_size();
}

////
// SetPlayerName
////

void SMsg_LevelLogin::SetPlayerName(const TCHAR* playername)
{
 _tcsnccpy(data_.playername, playername, sizeof(data_.playername));
}

////
// Dump: print to FILE stream
////

void SMsg_LevelLogin::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_LevelLogin[%p,%d]: "), this, sizeof(SMsg_LevelLogin));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("player(%u)='%s' acct=%c addr=%lu port=%d room=%d hidden=%d newly_alert = %d>\n"),
	    PlayerID(), PlayerName(), AccountType(), IPAddress(), ServerPort(), RoomID(), 
		Hidden(), NewlyAlert());
    Avatar().Dump(f, indent + 1);
    PeerUpdate().Dump(f, indent + 1);
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}

////
// SetDescription
////

void SMsg_LevelLogin::SetDescription(const TCHAR* description)
{
 _tcsnccpy(data_.description, description, sizeof(data_.description));
  TRUNC(data_.description, sizeof(data_.description));
  calc_size();
}

////
// calc_size
////

void SMsg_LevelLogin::calc_size()
{
  // initial size: whole structure minus variable-length text
  int size = sizeof(data_t) - sizeof(data_.description);
  // add string length, plus 1 for null
  size += (_tcslen(data_.description) + 1);
  SetMessageSize(size);
}

lyra_id_t SMsg_LevelLogin::PlayerID() const
{
	return data_.playerid;
}

const TCHAR* SMsg_LevelLogin::PlayerName() const
{
	return data_.playername;
}

int SMsg_LevelLogin::AccountType() const
{
	return data_.acct_type;
}

unsigned long SMsg_LevelLogin::IPAddress() const
{
	return data_.ipaddr;
}

int SMsg_LevelLogin::ServerPort() const
{
	return data_.udp_portnum;
}

const LmAvatar& SMsg_LevelLogin::Avatar() const
{
	return data_.avatar;
}

lyra_id_t SMsg_LevelLogin::RoomID() const
{
	return data_.roomid;
}

const LmPeerUpdate& SMsg_LevelLogin::PeerUpdate() const
{
	return data_.update;
}

unsigned char SMsg_LevelLogin::Hidden() const
{
	return data_.hidden;
}

unsigned char SMsg_LevelLogin::NewlyAlert() const
{
	return data_.newly_alert;
}

unsigned char SMsg_LevelLogin::NewlyAwakened() const
{
	return data_.newly_awakened;
}

//unsigned char SMsg_LevelLogin::TCPOnly() const
//{  return data_.tcp_only;}

const TCHAR* SMsg_LevelLogin::Description() const
{
	return data_.description;
}

void SMsg_LevelLogin::SetPlayerID(lyra_id_t playerid)
{
	data_.playerid = playerid;
}

void SMsg_LevelLogin::SetAccountType(int acct_type)
{
	data_.acct_type = acct_type;
}

void SMsg_LevelLogin::SetIPAddress(unsigned long ipaddr)
{
	data_.ipaddr = ipaddr;
}

void SMsg_LevelLogin::SetServerPort(int sport)
{
	data_.udp_portnum = sport;
}

void SMsg_LevelLogin::SetAvatar(const LmAvatar& avatar)
{
	data_.avatar = avatar;
}

void SMsg_LevelLogin::SetRoomID(lyra_id_t roomid)
{
	data_.roomid = roomid;
}

void SMsg_LevelLogin::SetPeerUpdate(const LmPeerUpdate& update)
{
	data_.update = update;
}

void SMsg_LevelLogin::SetHidden(unsigned char hidden)
{
	data_.hidden = hidden;
}

void SMsg_LevelLogin::SetNewlyAlert(unsigned char alert)
{
	data_.newly_alert = alert;
}

void SMsg_LevelLogin::SetNewlyAwakened(unsigned char newly_awakened)
{
	data_.newly_awakened = newly_awakened;
}

//void SMsg_LevelLogin::SetTCPOnly(unsigned char tcp_only)
//{  data_.tcp_only = tcp_only; }
