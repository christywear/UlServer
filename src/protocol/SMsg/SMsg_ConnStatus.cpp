// SMsg_ConnStatus.cpp  -*- C++ -*-
// $Id: SMsg_ConnStatus.cpp,v 1.2 1998-02-03 23:34:20-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "SMsg_ConnStatus.h"
#endif

#ifdef WIN32
#define STRICT
#include "unix.h"
#include <winsock.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/SMsg/SMsg_ConnStatus.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/SMsg/SMsg.h"
#include "../../../include/Protocol/LmConnection.h"

////
// constructor
////

SMsg_ConnStatus::SMsg_ConnStatus()
  : LmMesg(SMsg::CONNSTATUS, sizeof(data_t), sizeof(data_t), &data_)
{
  Init(0);
}

////
// destructor
////

SMsg_ConnStatus::~SMsg_ConnStatus()
{
  // empty
}

////
// Init
////

void SMsg_ConnStatus::Init(int num_conns)
{
  SetNumConnections(num_conns);
}

////
// InitConnection
////

void SMsg_ConnStatus::InitConnection(int conn_num, LmConnection* conn)
{
  if (conn_num_ok(conn_num) && conn) {
    SetConnectionType(conn_num, conn->Type());
    SetConnectionID(conn_num, conn->ID());
    SetRemoteIP(conn_num, conn->Socket().PeerName().IPAddress());
    SetRemotePort(conn_num, conn->Socket().PeerName().Port());
    SetLocalIP(conn_num, conn->Socket().SockName().IPAddress());
    SetLocalPort(conn_num, conn->Socket().SockName().Port());
    SetOnline(conn_num, conn->Online());
    SetIdleIn(conn_num, conn->IdleIn());
    SetIdleOut(conn_num, conn->IdleOut());
    SetMessagesIn(conn_num, conn->MessagesIn());
    SetBytesIn(conn_num, conn->BytesIn());
    SetMessagesOut(conn_num, conn->MessagesOut());
    SetBytesOut(conn_num, conn->BytesOut());
    SetMinMessageType(conn_num, conn->MinMessageType());
    SetMaxMessageType(conn_num, conn->MaxMessageType());
  }
}

////
// hton
////

void SMsg_ConnStatus::hton()
{
  for (int i = 0; i < NumConnections(); ++i) {
    HTONL(data_.conns[i].conn_type);
    HTONL(data_.conns[i].conn_id);
    HTONL(data_.conns[i].local_port);
    HTONL(data_.conns[i].remote_port);
    HTONL(data_.conns[i].online);
    HTONL(data_.conns[i].idle_in);
    HTONL(data_.conns[i].idle_out);
    HTONL(data_.conns[i].msgs_in);
    HTONL(data_.conns[i].bytes_in);
    HTONL(data_.conns[i].msgs_out);
    HTONL(data_.conns[i].bytes_out);
    HTONL(data_.conns[i].min_msg);
    HTONL(data_.conns[i].max_msg);
    // not converted: local_ip, remote_ip
  }
  HTONL(data_.num_conns); // after everything
}

////
// ntoh
////

void SMsg_ConnStatus::ntoh()
{
  NTOHL(data_.num_conns); // before everything
  for (int i = 0; i < NumConnections(); ++i) {
    NTOHL(data_.conns[i].conn_type);
    NTOHL(data_.conns[i].conn_id);
    NTOHL(data_.conns[i].remote_port);
    NTOHL(data_.conns[i].local_port);
    NTOHL(data_.conns[i].online);
    NTOHL(data_.conns[i].idle_in);
    NTOHL(data_.conns[i].idle_out);
    NTOHL(data_.conns[i].msgs_in);
    NTOHL(data_.conns[i].bytes_in);
    NTOHL(data_.conns[i].msgs_out);
    NTOHL(data_.conns[i].bytes_out);
    NTOHL(data_.conns[i].min_msg);
    NTOHL(data_.conns[i].max_msg);
    // not converted: local_ip, remote_ip
  }
  calc_size();
}

////
// Dump: print to FILE stream
////

void SMsg_ConnStatus::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<SMsg_ConnStatus[%p,%d]: "), this, sizeof(SMsg_ConnStatus));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("num=%d>\n"), NumConnections());
    for (int i = 0; i < NumConnections(); ++i) {
      INDENT(indent + 1, f);
     _ftprintf(f, _T("type=%c id=%u local=%s:%d remote=%s:%d online=%d idle=%d/%d in=%d/%d out=%d/%d msg=%d/%d\n"),
	      ConnectionType(i), ConnectionID(i),
	      LmSockAddrInet::AddressString(LocalIP(i)), LocalPort(i),
	      LmSockAddrInet::AddressString(RemoteIP(i)), RemotePort(i),
	      Online(i), IdleIn(i), IdleOut(i), MessagesIn(i), BytesIn(i),
	      MessagesOut(i), BytesOut(i), MinMessageType(i), MaxMessageType(i));
    }
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}

////
// SetNumConnections
////

void SMsg_ConnStatus::SetNumConnections(int num_conns)
{
  if (RANGE_OK(num_conns, 0, MAX_CONNS)) {
    data_.num_conns = num_conns;
  }
  else {
    data_.num_conns = 0;
  }
  calc_size();
}

////
// calc_size
////

void SMsg_ConnStatus::calc_size()
{
  // initial size: overall size minus variable-length field
  int size = sizeof(data_t) - sizeof(data_.conns);
  // add space for items
  size += NumConnections() * sizeof(ci_t);
  SetMessageSize(size);
}

bool SMsg_ConnStatus::conn_num_ok(int conn_num) const
{
    return INDEX_OK(conn_num, 0, MAX_CONNS);
}

int SMsg_ConnStatus::NumConnections() const
{
    return data_.num_conns;
}

int SMsg_ConnStatus::ConnectionType(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].conn_type : 'U';
}

int SMsg_ConnStatus::LocalPort(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].local_port : 0;
}

int SMsg_ConnStatus::RemotePort(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].remote_port : 0;
}

int SMsg_ConnStatus::ConnectionID(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].conn_id : 0;
}

unsigned long SMsg_ConnStatus::RemoteIP(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].remote_ip : 0;
}

unsigned long SMsg_ConnStatus::LocalIP(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].local_ip : 0;
}

int SMsg_ConnStatus::Online(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].online : 0;
}

int SMsg_ConnStatus::IdleIn(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].idle_in : 0;
}

int SMsg_ConnStatus::IdleOut(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].idle_out : 0;
}

int SMsg_ConnStatus::MessagesIn(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].msgs_in : 0;
}

int SMsg_ConnStatus::BytesIn(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].bytes_in : 0;
}

int SMsg_ConnStatus::MessagesOut(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].msgs_out : 0;
}

int SMsg_ConnStatus::BytesOut(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].bytes_out : 0;
}

int SMsg_ConnStatus::MinMessageType(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].min_msg : 0;
}

int SMsg_ConnStatus::MaxMessageType(int conn_num) const
{
    return conn_num_ok(conn_num) ? data_.conns[conn_num].max_msg : 0;
}

void SMsg_ConnStatus::SetConnectionType(int conn_num, int conn_type)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].conn_type = conn_type;
}

void SMsg_ConnStatus::SetConnectionID(int conn_num, int conn_id)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].conn_id = conn_id;
}

void SMsg_ConnStatus::SetLocalIP(int conn_num, unsigned long conn_ip)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].local_ip = conn_ip;
}

void SMsg_ConnStatus::SetRemoteIP(int conn_num, unsigned long conn_ip)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].remote_ip = conn_ip;
}

void SMsg_ConnStatus::SetOnline(int conn_num, int online)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].online = online;
}

void SMsg_ConnStatus::SetLocalPort(int conn_num, int port)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].local_port = port;
}

void SMsg_ConnStatus::SetRemotePort(int conn_num, int port)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].remote_port = port;
}

void SMsg_ConnStatus::SetIdleIn(int conn_num, int idle_in)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].idle_in = idle_in;
}

void SMsg_ConnStatus::SetIdleOut(int conn_num, int idle_out)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].idle_out = idle_out;
}

void SMsg_ConnStatus::SetMessagesIn(int conn_num, int msgs_in)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].msgs_in = msgs_in;
}

void SMsg_ConnStatus::SetBytesIn(int conn_num, int bytes_in)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].bytes_in = bytes_in;
}

void SMsg_ConnStatus::SetMessagesOut(int conn_num, int msgs_out)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].msgs_out = msgs_out;
}

void SMsg_ConnStatus::SetBytesOut(int conn_num, int bytes_out)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].bytes_out = bytes_out;
}

void SMsg_ConnStatus::SetMinMessageType(int conn_num, int min_msg)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].min_msg = min_msg;
}

void SMsg_ConnStatus::SetMaxMessageType(int conn_num, int max_msg)
{
    if (conn_num_ok(conn_num)) data_.conns[conn_num].max_msg = max_msg;
}
