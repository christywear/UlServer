// dump_item.cpp  -*- C++ -*-
// $Id: dump_item.cpp,v 1.2 1998-02-27 17:40:14-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// given an item string, dump its contents (as best as we can)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/Core/LyraDefs.h"
#include "../../include/Game/LmItem.h"
#include "../../include/Game/LmItemHdr.h"
#include "../../include/Game/LmItemDefs.h"
#include "../../include/Core/SharedConstants.h"

int _tmain(int argc, TCHAR** argv)
{
#ifdef UL_POSIX
  pth_init();
#endif
  // if there's an argument, it is the item string, otherwise prompt for it
  TCHAR itemstr[80];
  if (argc == 2) {
   _tcsnccpy(itemstr, argv[1], sizeof(itemstr));
  }
  else {
   _tprintf(("Enter item string: "));
   _tscanf(_T("%s"), itemstr);
  }

  LmItem item;
  if (item.Parse(itemstr) < 0) {
   _tprintf(("Error: item string '%s' did not parse.\n"), itemstr);
  }
  else {
   _tprintf(("Item Dump:\n\n"));
    item.Dump(stdout);
  }
#ifdef UL_POSIX
  pth_kill();
#endif
  return 0;
}

