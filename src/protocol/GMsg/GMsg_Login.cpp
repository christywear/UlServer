// GMsg_Login.cpp  -*- C++ -*-
// $Id: GMsg_Login.cpp,v 1.12 1998-04-17 16:37:44-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_Login.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h>//??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/GMsg/GMsg_Login.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"
#include "../../../include/core/LnMD5.h"

////
// constructor
////

GMsg_Login::GMsg_Login()
  : LmMesg(GMsg::LOGIN, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(DEFAULT_VERSION, _T("name"), Lyra::PORT_UNKNOWN, 0, 0, 0);
}

////
// destructor
////

GMsg_Login::~GMsg_Login()
{
  // empty
}

////
// Init
////

void GMsg_Login::Init(int version, const TCHAR* playername,  int serv_port, short pmare_type, int subversion, short tcp_only)
{
  SetVersion(version);
  SetSubVersion(subversion);
  SetPlayerName(playername);
  // SetPassword(password);
  SetServerPort(serv_port);
  SetPMareType(pmare_type);
//  SetFirewall(firewall);
  SetTCPOnly(tcp_only);
}

////
// hton
////

void GMsg_Login::hton()
{
  HTONL(data_.version);
  HTONL(data_.subversion);
  HTONL(data_.serv_port);
  //HTONL(data_.firewall);
  HTONS(data_.pmare_type);
  HTONS(data_.tcp_only);
  // no conversion: playername, password
}

////
// ntoh
////

void GMsg_Login::ntoh()
{
  NTOHL(data_.version);
  NTOHL(data_.subversion);
  NTOHL(data_.serv_port);
  //NTOHL(data_.firewall);
  NTOHS(data_.pmare_type);
  NTOHS(data_.tcp_only);
  // no conversion: playername, password, description
  calc_size();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_Login::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_Login[%p,%d]: "), this, sizeof(GMsg_Login));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("pmare_type=%u version=%d/%d name='%s' hash=<NOT PRINTED> servport=%d tcp_only=%d>\n"),
	    PMareType(), Version(), SubVersion(), PlayerName(), ServerPort(), TCPOnly());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

////
// SetPlayerName
////

void GMsg_Login::SetPlayerName(const TCHAR* playername)
{
 _tcsnccpy(data_.playername, playername, sizeof(data_.playername));
  TRUNC(data_.playername, sizeof(data_.playername));
}

void GMsg_Login::SetHash(const MD5Hash_t hash)
{
	memcpy((void*)data_.hash, (void*)hash, sizeof(MD5Hash_t));
}


////
// calc_size
////

void GMsg_Login::calc_size()
{
  // initial size: whole structure minus variable-length text
  int size = sizeof(data_t);
  SetMessageSize(size);
}

#ifndef USE_DEBUG
void GMsg_Login::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

int GMsg_Login::Version() const
{
	return data_.version;
}

int GMsg_Login::SubVersion() const
{
	return data_.subversion;
}

const TCHAR* GMsg_Login::PlayerName() const
{
	return data_.playername;
}

/*const TCHAR* GMsg_Login::Password() const
{
  return data_.password;
}
*/
const MD5Hash_t* GMsg_Login::HashPtr() const
{
	return &(data_.hash);
}

int GMsg_Login::ServerPort() const
{
	return data_.serv_port;
}
/*
int GMsg_Login::Firewall() const
{
  return data_.firewall;
}
*/

short GMsg_Login::PMareType() const
{
	return data_.pmare_type;
}

short GMsg_Login::TCPOnly() const
{
	return data_.tcp_only;
}

void GMsg_Login::SetVersion(int version)
{
	data_.version = version;
}

void GMsg_Login::SetSubVersion(int subversion)
{
	data_.subversion = subversion;
}

void GMsg_Login::SetServerPort(int serv_port)
{
	data_.serv_port = serv_port;
}
/*
void GMsg_Login::SetFirewall(int firewall)
{
  data_.firewall = firewall;
}
*/

void GMsg_Login::SetPMareType(short pmare_type)
{
	data_.pmare_type = pmare_type;
}

void GMsg_Login::SetTCPOnly(short tcp_only)
{
	data_.tcp_only = tcp_only;
}
