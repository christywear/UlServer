// LmBit32.cpp  -*- C++ -*-
// $Id: LmBit32.cpp,v 1.3 1997-06-30 20:21:30-07 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmBit32.h"
#endif

#ifdef WIN32
#define STRICT
#include "unix.h"
#include <winsock2.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //linux?
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../include/Protocol/LmBit32.h"
#include "../../include/Core/LyraDefs.h"


////
// ConvertToHost
////

void LmBit32::ConvertToHost()
{
  NTOHL(x_);
}

////
// ConvertToNetwork
////

void LmBit32::ConvertToNetwork()
{
  HTONL(x_);
}

////
// UnParse
////

void LmBit32::UnParse(char* str, int len) const
{
  // don't bother if string is too short
  if (len < 36) { // 32 + 3 .'s, 1 NULL
    return;
  }
  // print 31 to 0, left to right
  int j = 0;
  for (int i = 31; i >= 0; --i, ++j) {
    if (x_ & (1 << i)) {
      str[j] = '1';
    }
    else {
      str[j] = '0';
    }
    if (i && ((i % 8) == 0)) {
      str[++j] = '.';  // add .'s in between bytes, except for last one
    }
  }
  str[j] = '\0';
}

LmBit32::LmBit32(unsigned int x)
{
    x_ = x;
}

unsigned int LmBit32::Value() const
{
    return x_;
}

unsigned int LmBit32::BitMask(unsigned int len)
{
    return (1 << len) - 1;  // 2^len - 1
}

unsigned int LmBit32::BitMask(unsigned int start, unsigned int len)
{
    return BitMask(len) << start;
}

unsigned int LmBit32::GetBits(unsigned int start, unsigned int len) const
{
    return (x_ >> start) & BitMask(len);
}

void LmBit32::SetValue(unsigned int value)
{
    x_ = value;
}

void LmBit32::ClearBits(unsigned int start, unsigned int len)
{
    x_ &= ~BitMask(start, len);
}

void LmBit32::SetBits(unsigned int start, unsigned int len, unsigned int val)
{
    val &= BitMask(len);    // chop val down to size
    val = val << start;     // shift up to start position
    ClearBits(start, len);  // clear part corresponding to val
    x_ |= val;              // set bits
}
