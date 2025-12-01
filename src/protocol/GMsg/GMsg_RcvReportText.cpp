// GMsg_RcvReportText.cpp  -*- C++ -*-
// $Id: GMsg_RcvReportText.cpp,v 1.7 1998-02-03 23:33:38-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// message implementation

#ifdef __GNUC__
#pragma implementation "GMsg_RcvReportText.h"
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

#include "../../../include/Protocol/GMsg/GMsg_RcvReportText.h"
#include "../../../include/Core/LyraDefs.h"
#include "../../../include/Protocol/GMsg/GMsg.h"

////
// constructor
////

GMsg_RcvReportText::GMsg_RcvReportText()
  : LmMesg(GMsg::RCVREPORTTEXT, sizeof(data_t), sizeof(data_t), &data_)
{
  // initialize default message data values
  Init(Lyra::ID_UNKNOWN, Lyra::ID_UNKNOWN, 0, 0, _T("Creator"), _T("Recipient"), _T("Text"));
  SetCreatorID(Lyra::ID_UNKNOWN);
  SetRecipientID(Lyra::ID_UNKNOWN);
}

////
// destructor
////

GMsg_RcvReportText::~GMsg_RcvReportText()
{
  // empty
}

////
// Init
////

void GMsg_RcvReportText::Init(lyra_id_t reportid, lyra_id_t goalid, int awardxp, int flags,
			      const TCHAR* creator, const TCHAR* recipient, const TCHAR* reporttext)
{
  SetReportID(reportid);
  SetGoalID(goalid);
  SetAwardXP(awardxp);
  SetFlags(flags);
  SetCreator(creator);
  SetRecipient(recipient);
  SetReportText(reporttext);
}

////
// hton
////

void GMsg_RcvReportText::hton()
{
  HTONL(data_.reportid);
  HTONL(data_.goalid);
  HTONL(data_.awardxp);
  HTONL(data_.flags);
  // not converted: Creator
  // not converted: Recipient
  // not converted: ReportText
}

////
// ntoh
////

void GMsg_RcvReportText::ntoh()
{
  NTOHL(data_.reportid);
  NTOHL(data_.goalid);
  NTOHL(data_.awardxp);
  NTOHL(data_.flags);
  // not converted: Creator
  // not converted: Recipient
  // not converted: ReportText
  calc_size(); // variable-size message
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void GMsg_RcvReportText::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<GMsg_RcvReportText[%p,%d]: "), this, sizeof(GMsg_RcvReportText));
  if (ByteOrder() == ByteOrder::HOST) {
   _ftprintf(f, _T("id=%u goal=%u, xp=%d, flags=%d, creator(%u)='%s' recipient(%u)='%s' text='%s'>\n"),
	    ReportID(), GoalID(), AwardXP(), Flags(), CreatorID(), Creator(), RecipientID(), Recipient(), ReportText());
  }
  else {
   _ftprintf(f, _T("(network order)>\n"));
  }
  // print out base class
  LmMesg::Dump(f, indent + 1);
}
#endif /* USE_DEBUG */

////
// SetCreator
////

void GMsg_RcvReportText::SetCreator(const TCHAR* creator)
{
 _tcsnccpy(data_.creator, creator, sizeof(data_.creator));
  TRUNC(data_.creator, sizeof(data_.creator));
}

////
// SetRecipient
////

void GMsg_RcvReportText::SetRecipient(const TCHAR* recipient)
{
 _tcsnccpy(data_.recipient, recipient, sizeof(data_.recipient));
  TRUNC(data_.recipient, sizeof(data_.recipient));
}

////
// SetReportText
////

void GMsg_RcvReportText::SetReportText(const TCHAR* reporttext)
{
 _tcsnccpy(data_.reporttext, reporttext, sizeof(data_.reporttext));
  TRUNC(data_.reporttext, sizeof(data_.reporttext));
  calc_size();
}

////
// calc_size
////

void GMsg_RcvReportText::calc_size()
{
  // initial size: whole structure minus variable-length text
  int size = sizeof(data_t) - sizeof(data_.reporttext);
  // add string length, plus 1 for null
  size += (_tcslen(data_.reporttext) + 1);
  SetMessageSize(size);
}

#ifndef USE_DEBUG
void GMsg_RcvReportText::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

lyra_id_t GMsg_RcvReportText::ReportID() const
{
	return data_.reportid;
}

lyra_id_t GMsg_RcvReportText::GoalID() const
{
	return data_.goalid;
}

int GMsg_RcvReportText::AwardXP() const
{
	return data_.awardxp;
}

int GMsg_RcvReportText::Flags() const
{
	return data_.flags;
}

const TCHAR* GMsg_RcvReportText::Creator() const
{
	return data_.creator;
}

const TCHAR* GMsg_RcvReportText::Recipient() const
{
	return data_.recipient;
}

const TCHAR* GMsg_RcvReportText::ReportText() const
{
	return data_.reporttext;
}

void GMsg_RcvReportText::SetReportID(lyra_id_t reportid)
{
	data_.reportid = reportid;
}

void GMsg_RcvReportText::SetGoalID(lyra_id_t goalid)
{
	data_.goalid = goalid;
}

void GMsg_RcvReportText::SetAwardXP(int awardxp)
{
	data_.awardxp = awardxp;
}

void GMsg_RcvReportText::SetFlags(int flags)
{
	data_.flags = flags;
}

lyra_id_t GMsg_RcvReportText::CreatorID() const
{
	return creatorid_;
}

lyra_id_t GMsg_RcvReportText::RecipientID() const
{
	return recipientid_;
}

void GMsg_RcvReportText::SetCreatorID(lyra_id_t creatorid)
{
	creatorid_ = creatorid;
}

void GMsg_RcvReportText::SetRecipientID(lyra_id_t recipientid)
{
	recipientid_ = recipientid;
}