// GMsg_ChangeStat.cpp  -*- C++ -*-
// $Id: GMsg_ChangeStat.cpp,v 1.3 1997-07-29 19:32:10-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_ChangeStat.h"
#endif

#include "../../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../../include/Core/unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //??
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/GMsg/GMsg_ChangeStat.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_ChangeStat::GMsg_ChangeStat()
  : LmMesg(GMsg::CHANGESTAT, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0);
}

////
// destructor
////

GMsg_ChangeStat::~GMsg_ChangeStat()
{
  // empty
}

////
// Init
////

void GMsg_ChangeStat::Init(int num_changes)
{
  SetNumChanges(num_changes);
}

////
// hton
////

void GMsg_ChangeStat::hton()
{
  for (int i = 0; i < NumChanges(); ++i) {
    HTONS(data_.changes[i].requesttype);
    HTONS(data_.changes[i].stat);
    HTONL(data_.changes[i].value);
  }
}

////
// ntoh
////

void GMsg_ChangeStat::ntoh()
{
  calc_changes();
  for (int i = 0; i < NumChanges(); ++i) {
    NTOHS(data_.changes[i].requesttype);
    NTOHS(data_.changes[i].stat);
    NTOHL(data_.changes[i].value);
  }
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_ChangeStat::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_ChangeStat[%p,%d]: changes=%d>\n_T("), this, sizeof(GMsg_ChangeStat), NumChanges());
  if (ByteOrder() == ByteOrder::HOST) {
    for (int i = 0; i < NumChanges(); ++i) {
      INDENT(indent + 1, f);
     _ftprintf(f, _T("change %d: req=%d stat=%d val=%d\n_T("), i, RequestType(i), Stat(i), Value(i));
    }
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

////
// SetNumChanges
////

void GMsg_ChangeStat::SetNumChanges(int num_changes)
{
  // avoid bad range
  if (change_num_ok(num_changes)) {
    num_changes_ = num_changes;
  }
  else {
    num_changes_ = 0;
  }
  calc_size();
}

////
// calc_changes - calculate number of changes based on message size
////

void GMsg_ChangeStat::calc_changes()
{
  int msgsize = MessageSize();
  // determine portion of message size related to fixed fields
  int fixed_size = sizeof(data_t) - sizeof(data_.changes);
  // subtract this from the overall message size; result is the
  // size attributable to the variable-sized field
  msgsize -= fixed_size;
  // calculate number of changes
  SetNumChanges(msgsize / sizeof(change_t));
}

////
// calc_size - calculate message size based on number of changes
////

void GMsg_ChangeStat::calc_size()
{
  // initial size: overall size minus variable-length field
  int size = sizeof(data_t) - sizeof(data_.changes);
  // add space for changes
  size += NumChanges() * sizeof(change_t);
  SetMessageSize(size);
}

#ifndef USE_DEBUG
void GMsg_ChangeStat::Dump(FILE*, int) const
{
    // empty
}
#endif /* !USE_DEBUG */

bool GMsg_ChangeStat::change_num_ok(int change_num)
{
    return INDEX_OK(change_num, 0, MAX_CHANGES);
}

int GMsg_ChangeStat::NumChanges() const
{
    return num_changes_;
}

int GMsg_ChangeStat::RequestType(int change_num) const
{
    return change_num_ok(change_num) ? data_.changes[change_num].requesttype : 0;
}

int GMsg_ChangeStat::Stat(int change_num) const
{
    return change_num_ok(change_num) ? data_.changes[change_num].stat : 0;
}

int GMsg_ChangeStat::Value(int change_num) const
{
    return change_num_ok(change_num) ? data_.changes[change_num].value : 0;
}

void GMsg_ChangeStat::SetRequestType(int change_num, int requesttype)
{
    if (change_num_ok(change_num)) {
        data_.changes[change_num].requesttype = requesttype;
    }
}

void GMsg_ChangeStat::SetStat(int change_num, int stat)
{
    if (change_num_ok(change_num)) {
        data_.changes[change_num].stat = stat;
    }
}

void GMsg_ChangeStat::SetValue(int change_num, int value)
{
    if (change_num_ok(change_num)) {
        data_.changes[change_num].value = value;
    }
}

void GMsg_ChangeStat::InitChange(int change_num, int requesttype, int stat, int value)
{
    if (change_num_ok(change_num)) {
        data_.changes[change_num].requesttype = requesttype;
        data_.changes[change_num].stat = stat;
        data_.changes[change_num].value = value;
    }
}
