// LmRoomDB.cpp  -*- C++ -*-
// $Id: LmRoomDB.cpp,v 1.12 1997-11-04 23:02:47-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmRoomDB.h"
#endif

#include <stdio.h>
#include <string.h>

#include "../../include/DB/LmRoomDB.h"
#include "../../include/Core/LyraDefs.h"
#include "../../include/DB/LmItemGenDB.h"

// static objects
const TCHAR* LmRoomDB::DEFAULT_ROOMNAME = _T("");
const TCHAR* LmRoomDB::DEFAULT_DESCRIPTION = _T("");

////
// Constructor
////

LmRoomDB::LmRoomDB()
  : roomid_(DEFAULT_ROOMID),
    maxplayers_(0),
    num_generators_(0),
    generators_(0),
    num_portals_(0),
    portals_(0),
    entry_(false),
    no_reap_items_(false)
{
  _tcscpy(roomname_, DEFAULT_ROOMNAME);
  TRUNC(roomname_, sizeof(roomname_));
  _tcscpy(description_, DEFAULT_DESCRIPTION);
  TRUNC(description_, sizeof(description_));
}

////
// Destructor
////

LmRoomDB::~LmRoomDB()
{
	// of course, this causes a memory leak, but gamed's are 
	// coredumping in this destructor, and the destructor is 
	// called only when a process (gamed/leveld) is exiting
	//if (num_generators_ > 0)
		//LmDELETEARRAY(generators_);
	//if (num_portals_ > 0)
		//LmDELETEARRAY(portals_);
}

////
// HasPortal: return true if room has portal to given level/room
////

bool LmRoomDB::HasPortal(lyra_id_t levelid, lyra_id_t roomid) const
{
  // linear search
  for (int i = 0; i < NumPortals(); ++i) {
    if ((levelid == PortalLevelID(i)) && (roomid == PortalRoomID(i))) {
      return true;
    }
  }
  // not found
  return false;
}

////
// Init
////

void LmRoomDB::Init(lyra_id_t room_id, TCHAR* room_name, int entry, int max_players, int no_reap)
{
  _tcscpy(roomname_, room_name);
  roomid_ = room_id;
  entry_ = entry;
  maxplayers_ = max_players;
  no_reap_items_ = no_reap;

}

void LmRoomDB::SetNumGens(int num_generators)
{
  num_generators_ = num_generators;
  if (num_generators_ > 0)
	generators_ = LmNEW(LmItemGenDB[num_generators_]);
}

void LmRoomDB::SetNumPortals(int num_portals)
{
  num_portals_ = num_portals;
  if (num_portals_ > 0)
	portals_ = LmNEW(portal_t[num_portals_]);
}

void LmRoomDB::InitPortal(int index, lyra_id_t levelid, lyra_id_t roomid)
{
  portals_[index].levelid = levelid;
  portals_[index].roomid = roomid;
}

void LmRoomDB::SetDescription(TCHAR* description)
{
	_tcscpy(description_, description);
}


////
// Dump
////

void LmRoomDB::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
  _ftprintf(f, _T("<LmRoomDB[%p,%d]: id=%u name='%s' entry=%d noreap=%d maxplayers=%d itemgens=%d>\n"), this, sizeof(LmRoomDB),
	  RoomID(), RoomName(), EntryArea(), NoReapItems(), MaxPlayers(), NumGenerators());
  INDENT(indent + 1, f);
  _ftprintf(f, _T("portals(%d): "), NumPortals());
  int i;
  for (i = 0; i < NumPortals(); ++i) {
    _ftprintf(f, _T("%u:%u "), PortalLevelID(i), PortalRoomID(i));
  }
  _ftprintf(f, _T("\n"));
  for (i = 0; i < NumGenerators(); ++i) {
    Generator(i).Dump(f, indent + 1);
  }
}

lyra_id_t LmRoomDB::RoomID() const
{
    return roomid_;
}

const TCHAR* LmRoomDB::RoomName() const
{
    return roomname_;
}

bool LmRoomDB::EntryArea() const
{
    return (bool)entry_;
}

int LmRoomDB::MaxPlayers() const
{
    return maxplayers_;
}

int LmRoomDB::NumGenerators() const
{
    return num_generators_;
}

const LmItemGenDB& LmRoomDB::Generator(int index) const
{
    return generators_[index];
}

int LmRoomDB::NumPortals() const
{
    return num_portals_;
}

lyra_id_t LmRoomDB::PortalLevelID(int index) const
{
    return portals_[index].levelid;
}

lyra_id_t LmRoomDB::PortalRoomID(int index) const
{
    return portals_[index].roomid;
}

bool LmRoomDB::NoReapItems() const
{
    return ((bool)no_reap_items_);
}

const TCHAR* LmRoomDB::RoomDescription() const
{
    return description_;
}

