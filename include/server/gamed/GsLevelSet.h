// GsLevelSet.h  -*- C++ -*-
// $Id: GsLevelSet.h,v 1.5 1997-09-25 16:31:38-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved. 
//
// levels in world

#ifndef INCLUDED_GsLevelSet
#define INCLUDED_GsLevelSet

#ifdef __GNUC__
#pragma interface
#endif

#include <stdio.h>
#include <list>

#include "..\..\Core\LyraDefs.h"
#include "..\..\Core\PThMutex.h"

// class forward declarations


class LmLevelDBC;

// the class

class GsLevelSet {

public:

  GsLevelSet();
  ~GsLevelSet();
  //public accessor
  static GsLevelSet* Instance() { return s_instance; }
  const LmLevelDBC* LevelDBC(lyra_id_t levelid);

  bool CanGoto(lyra_id_t from_levelid, lyra_id_t from_roomid, lyra_id_t to_levelid, lyra_id_t to_roomid);

  void Dump(FILE* f, int indent = 0) const;

private:
	//private accessor
	static GsLevelSet* s_instance;
  // not implemented
  GsLevelSet(const GsLevelSet&);
  //operator=(const GsLevelSet&);

  LmLevelDBC* get_level(lyra_id_t levelid);
  LmLevelDBC* load_level(lyra_id_t levelid) const;


  mutable PThMutex lock_;
  typedef std::list<LmLevelDBC*> llist_t;
  llist_t levels_;

};

#endif /* INCLUDED_GsLevelSet */
