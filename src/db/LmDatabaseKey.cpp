// LmDatabaseKey.cpp  -*- C++ -*-
// $Id: LmDatabaseKey.cpp,v 1.7 1997-08-20 01:34:36-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmDatabaseKey.h"
#endif

#include <stdio.h>
#include <string.h>

#include "../../include/DB/LmDatabaseKey.h"
#include "../../include/Core/LyraDefs.h"

// static members
const wchar_t* LmDatabaseKey::DEFAULT_KEYTYPE = _T("");
const wchar_t* LmDatabaseKey::DEFAULT_FIELD = _T("");

////
// Constructor
////

LmDatabaseKey::LmDatabaseKey()
{
  Init(DEFAULT_KEYTYPE, DEFAULT_FIELD, DEFAULT_INST, DEFAULT_SUBINST);
}

LmDatabaseKey::LmDatabaseKey(const TCHAR* type, const TCHAR* field, int instance, int subinstance)
{
  Init(type, field, instance, subinstance);
}

////
// Destructor
////

LmDatabaseKey::~LmDatabaseKey()
{
  // empty
}

////
// Init
////

void LmDatabaseKey::Init(const TCHAR* type, const TCHAR* field, int instance, int subinstance)
{
  SetType(type);
  SetField(field);
  SetInstance(instance);
  SetSubInstance(subinstance);
}

void LmDatabaseKey::SetType(const TCHAR* type)
{
  strcpy((char*)(type_), (const char*)type);
  TRUNC(type_, sizeof(type_));
}

void LmDatabaseKey::SetField(const TCHAR* field)
{
  strcpy((char*)(field_), (const char*)field);
  TRUNC(field_, sizeof(field_));
}

const TCHAR* LmDatabaseKey::KeyString() const
{
  // possible valid keys:
  // 1 type = "T", field = "F", instance = "1", subinstance = "2" --> T_1_2.F
  // 2 type = "T", field = "F", instance = "1", subinstance = "0" --> T_1.F
  // 3 type = "T", field = "F", instance = "0", subinstance = "0" --> T.F
  // 4 type = "",  field = "F", instance = "0", subinstance = "0" --> F
  // (note: field always used)

  // case 1
  if ((strlen((char*)type_) > 0) && (instance_ != 0) && (subinstance_ != 0)) {
   sprintf((char*) realkey_, ("%s_%d_%d.%s"), type_, instance_, subinstance_, field_);
  }
  // case 2
  else if ((strlen((char*)type_) > 0) && (instance_ != 0)) {
   sprintf((char*) realkey_, ("%s_%d.%s"), type_, instance_, field_);
  }
  // case 3
  else if (strlen((char*)type_) > 0) {
   sprintf((char*) realkey_, ("%s.%s"), type_, field_);
  }
  // case 4
  else {
   sprintf((char*) realkey_, ("%s"), field_);
  }
  return realkey_;
}

////
// Dump
////

void LmDatabaseKey::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LmDatabaseKey[%p,%d]: type='%s' field='%s' instance=%d sub=%d>\n"),
	  this, sizeof(LmDatabaseKey),
	  Type(), Field(), Instance(), SubInstance());
}

////
// Dump1
////

void LmDatabaseKey::Dump1(FILE* f) const
{
 _ftprintf(f, _T("[%s]"), KeyString());
}

const TCHAR* LmDatabaseKey::Type() const
{
	return type_;
}

const TCHAR* LmDatabaseKey::Field() const
{
	return field_;
}

int LmDatabaseKey::Instance() const
{
	return instance_;
}

int LmDatabaseKey::SubInstance() const
{
	return subinstance_;
}

void LmDatabaseKey::SetInstance(int instance)
{
	instance_ = instance;
}

void LmDatabaseKey::SetSubInstance(int subinstance)
{
	subinstance_ = subinstance;
}