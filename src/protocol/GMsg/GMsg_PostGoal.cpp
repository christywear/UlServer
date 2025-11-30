// GMsg_PostGoal.cpp  -*- C++ -*-
// $Id: GMsg_PostGoal.cpp,v 1.9 1997-11-17 14:04:49-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_PostGoal.h"
#endif

#ifdef WIN32
#define STRICT
#include "unix.h"
#include <winsock.h>
#else /* !WIN32 */
#include <sys/types.h>
#include <netinet/in.h> //?
#endif /* WIN32 */
#include <stdio.h>
#include <string.h>

#include "../../../include/Protocol/GMsg/GMsg_PostGoal.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_PostGoal::GMsg_PostGoal()
  : LmMesg(GMsg::POSTGOAL, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, 0, 0, 0, 0, 0, 0, 0, _T("Summary"), _T("Text"), 0, 
	  0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Keywords"));
}

////
// destructor
////

GMsg_PostGoal::~GMsg_PostGoal()
{
  // empty
}

////
// Init
////

void GMsg_PostGoal::Init(lyra_id_t goalid, short level_num, short guild, 
			 short maxaccepted, short expirationtime, 
			 short sugsphere, short sugstat, short flags,
			 const TCHAR* summary, const TCHAR* goaltext, short voteexpire,
			 unsigned short graphic, unsigned char charges, unsigned char color1, unsigned char color2,
			 unsigned char item_type, unsigned int field1, unsigned int field2, 
			 unsigned int field3, unsigned int quest_xp, const TCHAR* keywords)
	
{
  SetGoalID(goalid);
  SetLevel(level_num);
  SetGuild(guild);
  SetMaxAccepted(maxaccepted);
  SetExpirationTime(expirationtime);
  SetSugSphere(sugsphere);
  SetSugStat(sugstat);
  SetSummary(summary);
  SetGoalText(goaltext);
  SetVoteExpiration(voteexpire);
  SetFlags(flags);
  SetKeywords(keywords);
  SetGraphic(graphic);
  SetCharges(charges);
  SetColor1(color1);
  SetColor2(color2);
  SetItemType(item_type);
  SetField1(field1);
  SetField2(field2);
  SetField3(field3);
  SetQuestXP(quest_xp);
}

////
// hton
////

void GMsg_PostGoal::hton()
{
  HTONL(data_.goalid);
  HTONS(data_.level_num);
  HTONS(data_.guild);
  HTONS(data_.maxaccepted);
  HTONS(data_.voteexpire);
  HTONS(data_.expirationtime);
  HTONS(data_.sugsphere);
  HTONS(data_.sugstat);
  HTONS(data_.flags);
  HTONS(data_.graphic);
  HTONL(data_.field1);
  HTONL(data_.field2);
  HTONL(data_.field3);
  HTONL(data_.quest_xp);

  // not converted: Summary
  // not converted: GoalText
  // not converted: Keywords
  // not converted: charges
  // not converted: color1
  // not converted: color2
  // not converted: item_type

}

////
// ntoh
////

void GMsg_PostGoal::ntoh()
{
  NTOHL(data_.goalid);
  NTOHS(data_.level_num);
  NTOHS(data_.guild);
  NTOHS(data_.maxaccepted);
  NTOHS(data_.voteexpire);
  NTOHS(data_.expirationtime);
  NTOHS(data_.sugsphere);
  NTOHS(data_.sugstat);
  NTOHS(data_.flags);
  NTOHS(data_.graphic);
  NTOHL(data_.field1);
  NTOHL(data_.field2);
  NTOHL(data_.field3);
  NTOHL(data_.quest_xp);

  // not converted: Summary
  // not converted: GoalText
  // not converted: Keywords
  // not converted: charges
  // not converted: color1
  // not converted: color2
  // not converted: item_type

  calc_size(); // variable-size message
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_PostGoal::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_PostGoal[%p,%d]: "), this, sizeof(GMsg_PostGoal));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("goalid=%u level=%d guild=%d maxaccept=%d expire=%d voteexpire=%d sphere=%d stat=%d flags=%d summary='%s' keywords='%s' graphic=%u charges=%c color1=%c color2=%c item_type=%c field1=%d field2=%d field3=%d quest_xp=%d text='%s'>\n"),
	    GoalID(), Level(), Guild(), MaxAccepted(), ExpirationTime(), VoteExpiration(),
	    SugSphere(), SugStat(), Flags(), Summary(), Keywords(), Graphic(), Charges(), Color1(), 
		Color2(), ItemType(), Field1(), Field2(), Field3(), QuestXP(), GoalText());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

////
// SetSummary
////

void GMsg_PostGoal::SetSummary(const TCHAR* summary)
{
 _tcsnccpy(data_.summary, summary, sizeof(data_.summary));
  TRUNC(data_.summary, sizeof(data_.summary));
}

////
// SetGoalText: copy goal text, recalc message size
////

void GMsg_PostGoal::SetGoalText(const TCHAR* goaltext)
{
 _tcsnccpy(data_.goaltext, goaltext, sizeof(data_.goaltext));
  TRUNC(data_.goaltext, sizeof(data_.goaltext));
  calc_size();
}

////
// SetKeywords
////

void GMsg_PostGoal::SetKeywords(const TCHAR* keywords)
{
 _tcsnccpy(data_.keywords, keywords, sizeof(data_.keywords));
  TRUNC(data_.keywords, sizeof(data_.keywords));
}

////
// calc_size
////

void GMsg_PostGoal::calc_size()
{
  // initial size: whole structure minus variable-length text
  int size = sizeof(data_t) - sizeof(data_.goaltext);
  // add string length, plus 1 for null
  size += (_tcslen(GoalText()) + 1);
  SetMessageSize(size);
}

#ifndef USE_DEBUG
void GMsg_PostGoal::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_PostGoal::GoalID() const
{
	return data_.goalid;
}

short GMsg_PostGoal::Level() const
{
	return data_.level_num;
}

short GMsg_PostGoal::Guild() const
{
	return data_.guild;
}

short GMsg_PostGoal::MaxAccepted() const
{
	return data_.maxaccepted;
}

short GMsg_PostGoal::ExpirationTime() const
{
	return data_.expirationtime;
}

short GMsg_PostGoal::VoteExpiration() const
{
	return data_.voteexpire;
}

short GMsg_PostGoal::SugSphere() const
{
	return data_.sugsphere;
}

short GMsg_PostGoal::SugStat() const
{
	return data_.sugstat;
}

short GMsg_PostGoal::Flags() const
{
	return data_.flags;
}

unsigned short GMsg_PostGoal::Graphic() const
{
	return data_.graphic;
}

unsigned char GMsg_PostGoal::Charges() const
{
	return data_.charges;
}

unsigned char GMsg_PostGoal::Color1() const
{
	return data_.color1;
}

unsigned char GMsg_PostGoal::Color2() const
{
	return data_.color2;
}

unsigned char GMsg_PostGoal::ItemType() const
{
	return data_.item_type;
}

unsigned int GMsg_PostGoal::Field1() const
{
	return data_.field1;
}

unsigned int GMsg_PostGoal::Field2() const
{
	return data_.field2;
}

unsigned int GMsg_PostGoal::Field3() const
{
	return data_.field3;
}

unsigned int GMsg_PostGoal::QuestXP() const
{
	return data_.quest_xp;
}

const TCHAR* GMsg_PostGoal::Summary() const
{
	return data_.summary;
}

const TCHAR* GMsg_PostGoal::Keywords() const
{
	return data_.keywords;
}

const TCHAR* GMsg_PostGoal::GoalText() const
{
	return data_.goaltext;
}

void GMsg_PostGoal::SetGoalID(lyra_id_t goalid)
{
	data_.goalid = goalid;
}

void GMsg_PostGoal::SetLevel(short level_num)
{
	data_.level_num = level_num;
}

void GMsg_PostGoal::SetGuild(short guild)
{
	data_.guild = guild;
}

void GMsg_PostGoal::SetMaxAccepted(short maxaccepted)
{
	data_.maxaccepted = maxaccepted;
}

void GMsg_PostGoal::SetExpirationTime(short expirationtime)
{
	data_.expirationtime = expirationtime;
}

void GMsg_PostGoal::SetVoteExpiration(short voteexpire)
{
	data_.voteexpire = voteexpire;
}

void GMsg_PostGoal::SetSugSphere(short sugsphere)
{
	data_.sugsphere = sugsphere;
}

void GMsg_PostGoal::SetSugStat(short sugstat)
{
	data_.sugstat = sugstat;
}

void GMsg_PostGoal::SetFlags(short flags)
{
	data_.flags = flags;
}

void GMsg_PostGoal::SetGraphic(unsigned short graphic)
{
	data_.graphic = graphic;
}

void GMsg_PostGoal::SetCharges(unsigned char charges)
{
	data_.charges = charges;
}

void GMsg_PostGoal::SetColor1(unsigned char color1)
{
	data_.color1 = color1;
}

void GMsg_PostGoal::SetColor2(unsigned char color2)
{
	data_.color2 = color2;
}

void GMsg_PostGoal::SetItemType(unsigned char item_type)
{
	data_.item_type = item_type;
}

void GMsg_PostGoal::SetField1(unsigned int field1)
{
	data_.field1 = field1;
}

void GMsg_PostGoal::SetField2(unsigned int field2)
{
	data_.field2 = field2;
}

void GMsg_PostGoal::SetField3(unsigned int field3)
{
	data_.field3 = field3;
}

void GMsg_PostGoal::SetQuestXP(unsigned int quest_xp)
{
	data_.quest_xp = quest_xp;
}
