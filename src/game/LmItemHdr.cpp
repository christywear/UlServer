// LmItemHdr.cpp  -*- C++ -*-
// $Id: LmItemHdr.cpp,v 1.12 1997-12-05 14:51:26-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmItemHdr.h"
#endif

#ifdef WIN32
#define STRICT
#include "unix.h"
#include <winsock2.h>
#else
#include <sys/types.h>
#include <netinet/in.h> //linux
#endif
#include <stdio.h>
#include <string.h>

#include "../../include/Game/LmItemHdr.h"
#include "../../include/Core/LyraDefs.h"
#include "../../include/Game/LmItemDefs.h"


// default instance
const LmItemHdr LmItemHdr::DEFAULT_INSTANCE;

////
// constructor
////

LmItemHdr::LmItemHdr()
{
  Init(Lyra::ID_UNKNOWN, Lyra::ID_UNKNOWN, DEFAULT_SERIAL);
}

////
// Init: re-construct item
////

void LmItemHdr::Init(lyra_id_t itemid, lyra_id_t h2, int serial)
{
  SetItemID(itemid, h2);
  SetSerial(serial);
}

////
// ConvertToNetwork
////

void LmItemHdr::ConvertToNetwork()
{
  item_.itemid.ConvertToNetwork();
  item_.h2.ConvertToNetwork();
  HTONL(item_.serial);
}

////
// ConvertToHost
////

void LmItemHdr::ConvertToHost()
{
  item_.itemid.ConvertToHost();
  item_.h2.ConvertToHost();
  NTOHL(item_.serial);
}

////
// Parse: read from string; return 0 if successful, -1 otherwise
//   format: id:serial
////

int LmItemHdr::Parse(const TCHAR* str)
{
  lyra_id_t itemid, h2;
  int serial;
  if (_stscanf(str, _T("%u:%u:%u"), &itemid, &h2, &serial) != 3) {
    return -1;
  }
  Init(itemid, h2, serial);
  return 0;
}

////
// UnParse: write to string
////

void LmItemHdr::UnParse(TCHAR* str, int /* strlen */) const
{
 _stprintf(str, _T("%u:%u:%u"), ItemHdr1(), ItemHdr2(), Serial());
}

////
// Dump: print to FILE stream, on an output line of its own
////

#ifdef USE_DEBUG
void LmItemHdr::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LmItemHdr[%p,%d]: h1=%u, h2=%u (flags=0x%x, fmt=%u, graphic=%u, colors=%u/%u) serial=%u>\n"),
	  this, sizeof(LmItemHdr),
	  ItemHdr1(), ItemHdr2(), Flags(), StateFormat(), Graphic(), Color1(), Color2(), Serial());
  INDENT(indent + 1, f);
  // do a bit more in-depth reporting
  int len1 = LyraItem::FieldLength(StateFormat(), 0);
  int len2 = LyraItem::FieldLength(StateFormat(), 1);
  int len3 = LyraItem::FieldLength(StateFormat(), 2);
 _ftprintf(f, _T("lengths=(%d,%d,%d) flags: "), len1, len2, len3);
  if (Flags() == 0) {
   _ftprintf(f, _T("(none)"));
  }
  else {
    if (FlagSet(LyraItem::FLAG_NOREAP))        _ftprintf(f, _T("NOREAP "));
    if (FlagSet(LyraItem::FLAG_SENDSTATE))     _ftprintf(f, _T("SENDSTATE "));
    if (FlagSet(LyraItem::FLAG_IMMUTABLE))     _ftprintf(f, _T("IMMUTABLE "));
    if (FlagSet(LyraItem::FLAG_CHANGE_CHARGES))_ftprintf(f, _T("CHANGE_CHARGES "));
    if (FlagSet(LyraItem::FLAG_ALWAYS_DROP))   _ftprintf(f, _T("ALWAYS_DROP "));
    if (FlagSet(LyraItem::FLAG_HASDESCRIPTION))_ftprintf(f, _T("HASDESCRIPTION "));
  }
 _ftprintf(f, _T("\n"));
}
#endif /* USE_DEBUG */

////
// Dump1: print to FILE stream, in _T("shorthand") format, no trailing newline
////

#ifdef USE_DEBUG
void LmItemHdr::Dump1(FILE* f) const
{
 _ftprintf(f, _T("[%u:%u:%u]"), ItemHdr1(), ItemHdr2(), Serial());
}
#endif /* USE_DEBUG */

#ifdef WIN32
#pragma warning( disable : 4800 )
#endif

#ifndef USE_DEBUG
void LmItemHdr::Dump(FILE*, int) const
{
    // empty
}

void LmItemHdr::Dump1(FILE*) const
{
    // empty
}
#endif /* !USE_DEBUG */

lyra_id_t LmItemHdr::ItemHdr1() const
{
    return item_.itemid.Value();
}

lyra_id_t LmItemHdr::ItemHdr2() const
{
    return item_.h2.Value();
}

int LmItemHdr::Serial() const
{
    return item_.serial;
}

int LmItemHdr::Flags() const
{
    return item_.h2.GetBits(FLAGS_START, FLAGS_WIDTH);
}

int LmItemHdr::StateFormat() const
{
    return item_.itemid.GetBits(FORMAT_START, FORMAT_WIDTH);
}

int LmItemHdr::Graphic() const
{
    return item_.itemid.GetBits(GRAPHIC_START, GRAPHIC_WIDTH);
}

int LmItemHdr::Color1() const
{
    return item_.itemid.GetBits(COLOR1_START, COLOR1_WIDTH);
}

int LmItemHdr::Color2() const
{
    return item_.itemid.GetBits(COLOR2_START, COLOR2_WIDTH);
}

bool LmItemHdr::FlagSet(int flag) const
{
    return (Flags() & flag);
}

void LmItemHdr::SetItemID(lyra_id_t itemid, lyra_id_t i2)
{
    item_.itemid.SetValue(itemid);
    item_.h2.SetValue(i2);
}

void LmItemHdr::SetSerial(int serial)
{
    item_.serial = serial;
}

void LmItemHdr::SetFlags(int flags)
{
    item_.h2.SetBits(FLAGS_START, FLAGS_WIDTH, flags);
}

void LmItemHdr::SetFlag(int flag)
{
    SetFlags(Flags() | flag);
}

void LmItemHdr::ClearFlag(int flag)
{
    SetFlags(Flags() & ~flag);
}

void LmItemHdr::SetColor1(int color)
{
    item_.itemid.SetBits(COLOR1_START, COLOR1_WIDTH, color);
}

void LmItemHdr::SetColor2(int color)
{
    item_.itemid.SetBits(COLOR2_START, COLOR2_WIDTH, color);
}

void LmItemHdr::SetGraphic(int graphic)
{
    item_.itemid.SetBits(GRAPHIC_START, GRAPHIC_WIDTH, graphic);
}

void LmItemHdr::SetStateFormat(int format)
{
    item_.itemid.SetBits(FORMAT_START, FORMAT_WIDTH, format);
}

bool LmItemHdr::Equals(const LmItemHdr& item) const
{
    return ((ItemHdr1() == item.ItemHdr1()) && (ItemHdr2() == item.ItemHdr2()) && (Serial() == item.Serial()));
}