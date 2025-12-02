// gen_item.cpp  -*- C++ -*-
// $Id: gen_item.cpp,v 1.4 1997-12-12 17:03:02-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// test item generators

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/core/LyraDefs.h"
#include "../../include/core/LmRand.h"
#include "../../include/game/LmItem.h"
#include "../../include/game/LmItemHdr.h"
#include "../../include/game/LmItemDefs.h"
#include "../../include/core/SharedConstants.h"
#include "../../include/game/LmItemGen.h"

int _tmain()
{
#ifdef UL_POSIX
  pth_init();
#endif
  LmItem item;

  LmRand::InitSeed();

  for (int gt = LmItemGen::MIN_GENTYPE; gt <= LmItemGen::MAX_GENTYPE; ++gt) {
   _tprintf(("*** Generator Type %d ***\n\n"), gt);
    // print out N items of each type, for each generator type
    int N = 100;
    int i;

#if 0
   _tprintf(_T("Change Stat:\n\n"));
    for (i = 0; i < N; ++i) {
      LmItemGen::GenerateItem(gt, LmItemGen::ITEM_CHANGESTAT, item);
      item.Dump(stdout);
     _tprintf(_T("\n"));
    }

   _tprintf(_T("Armor:\n\n"));
    for (i = 0; i < N; ++i) {
      LmItemGen::GenerateItem(gt, LmItemGen::ITEM_ARMOR, item);
      item.Dump(stdout);
     _tprintf(_T("\n"));
    }

   _tprintf(_T("Effect Player:\n\n"));
    for (i = 0; i < N; ++i) {
      LmItemGen::GenerateItem(gt, LmItemGen::ITEM_EFFECTPLAYER, item);
      item.Dump(stdout);
     _tprintf(_T("\n"));
    }
#endif

   _tprintf(("Missile:\n\n"));
    for (i = 0; i < N; ++i) {
      LmItemGen::GenerateItem(gt, LmItemGen::ITEM_MISSILE, item);
      item.Dump(stdout);
     _tprintf(("\n"));
    }

  }
#ifdef UL_POSIX
  pth_kill();
#endif
  return 0;
}

