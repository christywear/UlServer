// GMsg_ChangeAvatar.cpp  -*- C++ -*-
// $Id: GMsg_ChangeAvatar.cpp,v 1.5 1997-07-29 19:32:10-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_ChangeAvatar.h"
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

#include "../../../include/Protocol/GMsg/GMsg_ChangeAvatar.h"
#include "../../../include/game/LmAvatar.h"


////
// constructor
////

GMsg_ChangeAvatar::GMsg_ChangeAvatar()
  : LmMesg(GMsg::CHANGEAVATAR, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(LmAvatar::DEFAULT_INSTANCE, AVATAR_CURRENT);
}

////
// destructor
////

GMsg_ChangeAvatar::~GMsg_ChangeAvatar()
{
  // empty
}

////
// Init
////

void GMsg_ChangeAvatar::Init(const LmAvatar& avatar, int which)
{
  SetAvatar(avatar);
  SetWhich(which);
}

////
// hton
////

void GMsg_ChangeAvatar::hton()
{
  data_.avatar.ConvertToNetwork();
  HTONL(data_.which);
}

////
// ntoh
////

void GMsg_ChangeAvatar::ntoh()
{
  data_.avatar.ConvertToHost();
  NTOHL(data_.which);
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_ChangeAvatar::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_ChangeAvatar[%p,%d]: "), this, sizeof(GMsg_ChangeAvatar));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("which=%c>\n"), Which());
    Avatar().Dump(f, indent + 1);
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

#ifndef USE_DEBUG
void GMsg_ChangeAvatar::Dump(FILE*, int) const
{
    // empty
}
#endif /* !USE_DEBUG */

const LmAvatar& GMsg_ChangeAvatar::Avatar() const
{
    return data_.avatar;
}

int GMsg_ChangeAvatar::Which() const
{
    return data_.which;
}

void GMsg_ChangeAvatar::SetAvatar(const LmAvatar& avatar)
{
    data_.avatar = avatar;
}

void GMsg_ChangeAvatar::SetWhich(int which)
{
    data_.which = which;
}