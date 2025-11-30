// GMsg_RcvGoalDetails.cpp  -*- C++ -*-
// $Id: GMsg_RcvGoalDetails.cpp,v 1.10 1998-02-04 19:03:35-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_RcvGoalDetails.h"
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

#include "../../../include/Protocol/GMsg/GMsg_RcvGoalDetails.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_RcvGoalDetails::GMsg_RcvGoalDetails()
  : LmMesg(GMsg::RCVGOALDETAILS, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("keywords"));
  // init acceptees to 0
  for (int i = 0; i < Lyra::MAX_ACCEPTS; ++i) {
    SetAccepteeID(i, Lyra::ID_UNKNOWN);
    SetAcceptee(i, _T("(unknown)"));
  }
}

////
// destructor
////

GMsg_RcvGoalDetails::~GMsg_RcvGoalDetails()
{
  // empty
}

////
// Init
////

void GMsg_RcvGoalDetails::Init(lyra_id_t goalid, short level_num, short maxacceptances,
			       short expirationtime, short numberyes, short numberno,
			       short voteexpiration, short statusflags, short otherflags,
			       int num_acceptees, short num_completees, unsigned short graphic, 
				   unsigned char charges, unsigned char color1, 
				   unsigned char color2, unsigned char item_type, unsigned int field1, 
				   unsigned int field2, unsigned int field3,  
				   unsigned int quest_xp, const TCHAR* keywords)
{
  SetGoalID(goalid);
  SetLevel(level_num);
  SetMaxAcceptances(maxacceptances);
  SetExpirationTime(expirationtime);
  SetNumberYes(numberyes);
  SetNumberNo(numberno);
  SetVoteExpiration(voteexpiration);
  SetStatusFlags(statusflags);
  SetOtherFlags(otherflags);
  SetNumAcceptees(num_acceptees);
  SetNumCompletees(num_completees);
  SetGraphic(graphic);
  SetCharges(charges);
  SetColor1(color1);
  SetColor2(color2);
  SetItemType(item_type);
  SetField1(field1);
  SetField2(field2);
  SetField3(field3);
  SetQuestXP(quest_xp);
  SetKeywords(keywords);
}

////
// hton
////

void GMsg_RcvGoalDetails::hton()
{
  HTONL(data_.goalid);
  HTONS(data_.level_num);
  HTONS(data_.maxacceptances);
  HTONS(data_.num_completees);
  HTONS(data_.expirationtime);
  HTONS(data_.numberyes);
  HTONS(data_.numberno);
  HTONS(data_.voteexpiration);
  HTONS(data_.statusflags);
  HTONS(data_.otherflags);
  HTONS(data_.graphic);
  HTONL(data_.field1);
  HTONL(data_.field2);
  HTONL(data_.field3);
  HTONL(data_.quest_xp);

  // not converted: charges
  // not converted: color1
  // not converted: color2
  // not converted: item_type
  // not converted: acceptees
}

////
// ntoh
////

void GMsg_RcvGoalDetails::ntoh()
{
  // determine number of acceptees
  calc_acceptees();
  NTOHL(data_.goalid);
  NTOHS(data_.level_num);
  NTOHS(data_.maxacceptances);
  NTOHS(data_.num_completees);
  NTOHS(data_.expirationtime);
  NTOHS(data_.numberyes);
  NTOHS(data_.numberno);
  NTOHS(data_.voteexpiration);
  NTOHS(data_.statusflags);
  NTOHS(data_.otherflags);
  NTOHS(data_.graphic);
  NTOHL(data_.field1);
  NTOHL(data_.field2);
  NTOHL(data_.field3);
  NTOHL(data_.quest_xp);

  // not converted: charges
  // not converted: color1
  // not converted: color2
  // not converted: item_type

  // not converted: acceptees
  calc_size(); // variable-length message
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_RcvGoalDetails::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_RcvGoalDetails[%p,%d]: "), this, sizeof(GMsg_RcvGoalDetails));
  if (ByteOrder() == ByteOrder::HOST) {
    // TODO: implement
   _ftprintf(f, _T("goal=%u level=%d maxaccepts=%d expire=%d votes=%d/%d voteexpire=%d status=%d other=%d accepts=%d completes=%d graphic=%u charges=%c color1=%c color2=%c item_type=%c field1=%d field2=%d field3=%d quest_xp=%d>\n"),
	    GoalID(), Level(), MaxAcceptances(), ExpirationTime(), NumberYes(), NumberNo(), VoteExpiration(),
	    StatusFlags(), OtherFlags(), NumAcceptees(), NumCompletees(), Graphic(), Charges(), Color1(), 
		Color2(), ItemType(), Field1(), Field2(), Field3(), QuestXP());
    for (int i = 0; i < NumAcceptees(); ++i) {
      INDENT(indent + 1, f);
     _ftprintf(f, _T("acceptee: id=%u name='%s'\n"), AccepteeID(i), Acceptee(i));
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
// SetNumCompletees
////

void GMsg_RcvGoalDetails::SetNumCompletees(int num_completees)
{
	// note that setting completees increases num_acceptees by 
	// the same amount, as both lists are stored in the same
	// variable length field
 
  // check range
  if (RANGE_OK(num_acceptees_ + num_completees, 0, Lyra::MAX_ACCEPTS)) {
    this->SetNumAcceptees(num_acceptees_ + num_completees);
	data_.num_completees = num_completees;
  }
  else {
    num_completees = 0;
  }
}


////
// SetNumAcceptees
////

void GMsg_RcvGoalDetails::SetNumAcceptees(int num_acceptees)
{
  // check range
  if (RANGE_OK(num_acceptees, 0, Lyra::MAX_ACCEPTS)) {
    num_acceptees_ = num_acceptees;
  }
  else {
    num_acceptees_ = 0;
  }
  calc_size();
}

////
// SetAcceptee
////

void GMsg_RcvGoalDetails::SetAcceptee(int acceptee_num, const TCHAR* name)
{
  if (!INDEX_OK(acceptee_num, 0, Lyra::MAX_ACCEPTS)) {
    return;
  }
  _tcsnccpy(data_.acceptees[acceptee_num], name, sizeof(pname_t));
  TRUNC(data_.acceptees[acceptee_num], sizeof(pname_t));
}

////
// calc_acceptees
////

void GMsg_RcvGoalDetails::calc_acceptees()
{
  int msgsize = MessageSize();
  // determine portion of message size related to fixed fields
  int fixed_size = sizeof(data_t) - sizeof(data_.acceptees);
  // subtract this from the overall message size; result is the
  // size attributable to the variable-sized field
  msgsize -= fixed_size;
  // calculate number of changes
  SetNumAcceptees(msgsize / sizeof(pname_t));
}

////
// SetKeywords
////

void GMsg_RcvGoalDetails::SetKeywords(const TCHAR* keywords)
{
 _tcsnccpy(data_.keywords, keywords, sizeof(data_.keywords));
  TRUNC(data_.keywords, sizeof(data_.keywords));
}

////
// calc_size
////

void GMsg_RcvGoalDetails::calc_size()
{
  // initial size: whole structure minus variable-length field
  int size = sizeof(data_t) - sizeof(data_.acceptees);
  // add acceptees
  size += NumAcceptees() * sizeof(pname_t);
  SetMessageSize(size);
}

#ifndef USE_DEBUG
void GMsg_RcvGoalDetails::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_RcvGoalDetails::GoalID() const
{
	return data_.goalid;
}

short GMsg_RcvGoalDetails::Level() const
{
	return data_.level_num;
}

short GMsg_RcvGoalDetails::MaxAcceptances() const
{
	return data_.maxacceptances;
}

short GMsg_RcvGoalDetails::ExpirationTime() const
{
	return data_.expirationtime;
}

short GMsg_RcvGoalDetails::NumberYes() const
{
	return data_.numberyes;
}

short GMsg_RcvGoalDetails::NumberNo() const
{
	return data_.numberno;
}

short GMsg_RcvGoalDetails::VoteExpiration() const
{
	return data_.voteexpiration;
}

int GMsg_RcvGoalDetails::NumAcceptees() const
{
	return num_acceptees_;
}

short GMsg_RcvGoalDetails::NumCompletees() const
{
	return data_.num_completees;
}

const TCHAR* GMsg_RcvGoalDetails::Acceptee(int acceptee_num) const
{
	return data_.acceptees[acceptee_num];
}

const TCHAR* GMsg_RcvGoalDetails::Keywords() const
{
	return data_.keywords;
}

short GMsg_RcvGoalDetails::StatusFlags() const
{
	return data_.statusflags;
}

short GMsg_RcvGoalDetails::OtherFlags() const
{
	return data_.otherflags;
}

unsigned short GMsg_RcvGoalDetails::Graphic() const
{
	return data_.graphic;
}

unsigned char GMsg_RcvGoalDetails::Charges() const
{
	return data_.charges;
}

unsigned char GMsg_RcvGoalDetails::Color1() const
{
	return data_.color1;
}

unsigned char GMsg_RcvGoalDetails::Color2() const
{
	return data_.color2;
}

unsigned char GMsg_RcvGoalDetails::ItemType() const
{
	return data_.item_type;
}

unsigned int GMsg_RcvGoalDetails::Field1() const
{
	return data_.field1;
}

unsigned int GMsg_RcvGoalDetails::Field2() const
{
	return data_.field2;
}

unsigned int GMsg_RcvGoalDetails::Field3() const
{
	return data_.field3;
}

unsigned int GMsg_RcvGoalDetails::QuestXP() const
{
	return data_.quest_xp;
}

void GMsg_RcvGoalDetails::SetGoalID(lyra_id_t goalid)
{
	data_.goalid = goalid;
}

void GMsg_RcvGoalDetails::SetLevel(short level_num)
{
	data_.level_num = level_num;
}

void GMsg_RcvGoalDetails::SetMaxAcceptances(short maxacceptances)
{
	data_.maxacceptances = maxacceptances;
}

void GMsg_RcvGoalDetails::SetExpirationTime(short expirationtime)
{
	data_.expirationtime = expirationtime;
}

void GMsg_RcvGoalDetails::SetNumberYes(short numberyes)
{
	data_.numberyes = numberyes;
}

void GMsg_RcvGoalDetails::SetNumberNo(short numberno)
{
	data_.numberno = numberno;
}

void GMsg_RcvGoalDetails::SetVoteExpiration(short voteexpiration)
{
	data_.voteexpiration = voteexpiration;
}

void GMsg_RcvGoalDetails::SetStatusFlags(short statusflags)
{
	data_.statusflags = statusflags;
}

void GMsg_RcvGoalDetails::SetOtherFlags(short otherflags)
{
	data_.otherflags = otherflags;
}

lyra_id_t GMsg_RcvGoalDetails::AccepteeID(int acceptee_num) const
{
	return acceptees_[acceptee_num];
}

void GMsg_RcvGoalDetails::SetAccepteeID(int acceptee_num, lyra_id_t playerid)
{
	acceptees_[acceptee_num] = playerid;
}

void GMsg_RcvGoalDetails::SetGraphic(unsigned short graphic)
{
	data_.graphic = graphic;
}

void GMsg_RcvGoalDetails::SetCharges(unsigned char charges)
{
	data_.charges = charges;
}

void GMsg_RcvGoalDetails::SetColor1(unsigned char color1)
{
	data_.color1 = color1;
}

void GMsg_RcvGoalDetails::SetColor2(unsigned char color2)
{
	data_.color2 = color2;
}

void GMsg_RcvGoalDetails::SetItemType(unsigned char item_type)
{
	data_.item_type = item_type;
}

void GMsg_RcvGoalDetails::SetField1(unsigned int field1)
{
	data_.field1 = field1;
}

void GMsg_RcvGoalDetails::SetField2(unsigned int field2)
{
	data_.field2 = field2;
}

void GMsg_RcvGoalDetails::SetField3(unsigned int field3)
{
	data_.field3 = field3;
}

void GMsg_RcvGoalDetails::SetQuestXP(unsigned int quest_xp)
{
	data_.quest_xp = quest_xp;
}
