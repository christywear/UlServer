// GMsg_AgentLogin.cpp  -*- C++ -*-
// $Id: GMsg_AgentLogin.cpp,v 1.1 1997-10-15 16:22:40-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_AgentLogin.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT

#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <cstdio>


#include "../../../include/Protocol/GMsg/GMsg_AgentLogin.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"
#include "../../../include/core/LnMD5.h"

////
// constructor
////

GMsg_AgentLogin::GMsg_AgentLogin()
  : LmMesg(GMsg::AGENTLOGIN, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(DEFAULT_VERSION, _T("name"), Lyra::PORT_UNKNOWN, 0, 0);
}

////
// destructor
////

GMsg_AgentLogin::~GMsg_AgentLogin()
{
  // empty
}

////
// Init
////

void GMsg_AgentLogin::Init(int version, const TCHAR* playername, int serv_port, lyra_id_t billing_id, short tcp_only)
{
  SetVersion(version);
  SetPlayerName(playername);
  SetServerPort(serv_port);
  SetBillingID(billing_id);
  SetTCPOnly(tcp_only);
}

////
// hton
////

void GMsg_AgentLogin::hton()
{
  HTONL(data_.version);
  HTONL(data_.serv_port);
  HTONL(data_.billing_id);
  HTONS( data_.tcp_only );  
// no conversion: playername, hash
}

////
// ntoh
////

void GMsg_AgentLogin::ntoh()
{
  NTOHL(data_.version);
  NTOHL(data_.serv_port);
  NTOHL(data_.billing_id);
  NTOHS(data_.tcp_only);
  // no conversion: playername, hash
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_AgentLogin::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_AgentLogin[%p,%d]: "), this, sizeof(GMsg_AgentLogin));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("billing_id=%u version=%d name='%s' hash=<NOT PRINTED> servport=%d tcp_only=%d>\n"),
	    BillingID(), Version(), PlayerName(), ServerPort(), TCPOnly());
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

void GMsg_AgentLogin::SetPlayerName(const TCHAR* playername)
{
 _tcsnccpy(data_.playername, playername, sizeof(data_.playername));
  TRUNC(data_.playername, sizeof(data_.playername));
}

/*
void GMsg_AgentLogin::SetPassword(const TCHAR* password)
{
 _tcsnccpy(data_.password, password, sizeof(data_.password));
  TRUNC(data_.password, sizeof(data_.password));
}
*/
////
// SetHash
////

void GMsg_AgentLogin::SetHash(const MD5Hash_t hash)
{
	memcpy((void*)data_.hash, (void*)hash, sizeof(MD5Hash_t));
}

#ifndef USE_DEBUG
void GMsg_AgentLogin::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

int GMsg_AgentLogin::Version() const
{
	return data_.version;
}

const TCHAR* GMsg_AgentLogin::PlayerName() const
{
	return data_.playername;
}
/*
const TCHAR* GMsg_AgentLogin::Password() const
{
  return data_.password;
}
*/
const MD5Hash_t* GMsg_AgentLogin::HashPtr() const
{
	return &(data_.hash);
}

int GMsg_AgentLogin::ServerPort() const
{
	return data_.serv_port;
}

short GMsg_AgentLogin::TCPOnly() const
{
	return data_.tcp_only;
}

void GMsg_AgentLogin::SetTCPOnly(short tcp_only)
{
	data_.tcp_only = tcp_only;
}

lyra_id_t GMsg_AgentLogin::BillingID() const
{
	return data_.billing_id;
}

void GMsg_AgentLogin::SetVersion(int version)
{
	data_.version = version;
}

void GMsg_AgentLogin::SetServerPort(int serv_port)
{
	data_.serv_port = serv_port;
}

void GMsg_AgentLogin::SetBillingID(lyra_id_t billing_id)
{
	data_.billing_id = billing_id;
}
