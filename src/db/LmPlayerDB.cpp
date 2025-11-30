// LmPlayerDB.cpp  -*- C++ -*-
// $Id: LmPlayerDB.cpp,v 1.25 1998/04/17 02:02:41 jason Exp jason $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmPlayerDB.h"
#endif

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "../../include/platform/Platform.h" //temp fix for old style legacy defines
#ifndef WIN32
#include <stdlib.h>
#include <unistd.h> //linux
#endif

#include "../../include/DB/LmPlayerDB.h"

// default objects
const TCHAR* LmPlayerDB::DEFAULT_PLAYERNAME = _T("");
const TCHAR* LmPlayerDB::DEFAULT_PASSWORD = _T("");

////
// Constructor
////

LmPlayerDB::LmPlayerDB()
  : goalbook_(Lyra::MAX_ACTIVE_GOALS)
{
  Init();
}

////
// Destructor
////

LmPlayerDB::~LmPlayerDB()
{
  // empty
}

////
// Init: initialize to unused default state
////

void LmPlayerDB::Init()
{
  playerid_ = Lyra::ID_UNKNOWN;
  billing_id_ = Lyra::ID_UNKNOWN;
 _tcscpy(playername_, DEFAULT_PLAYERNAME);
 _tcscpy(password_, DEFAULT_PASSWORD);
 _tcscpy(avatar_descrip_, _T("(none)"));
 _tcscpy(email_, _T("(unknown)"));
 _tcscpy(realname_, _T("(unknown)"));
  stats_.Init();
  avatar_.Init(0, 0);
  arts_.Init();
  goalbook_.Empty();
  inv_.RemoveAll();
  acct_type_ = ACCT_LOCKED;
  last_login_ = 0;
  time_online_ = 0;
  num_logins_ = 0;
  xp_gain_ = 0;
  xp_loss_ = 0;
  newly_alert_ = 0;
  x_ = y_ = 0;
  level_id_ = 0;
//  tcp_only_ = 0;
  for (int i = 0; i < NUM_GUILDS; ++i) {
    initiators_[i] = Lyra::ID_UNKNOWN;
  }
//  player_modified_ = false;
}

////
// PeggedBelowSphere: return true if player is 1 xp away from next sphere, ie. cannot 
//   earn any more xp
////

bool LmPlayerDB::PeggedBelowSphere() const
{
  bool retval = true;
  int xp = stats_.XP();
  int next_base = LmStats::SphereXPBase(stats_.Sphere() + 1);
  if (xp != (next_base - 1)) { // not 1 point under next sphere base
    retval = false;
  }
  return retval;
}

////
// CheckPassword: return true if password matches, false otherwise
////

bool LmPlayerDB::CheckPassword(const TCHAR* password) const
{
  return (_tcsncmp(password, password_, sizeof(password_)) == 0);
}

////
// CheckAvatar: check given avatar such that any house shields, sphere, or teacher indicators are valid;
//   return > 0 if error, 0 if not
////

int LmPlayerDB::CheckAvatar(const LmAvatar& avatar, int acct_type) const
{
  int retval = 0;
  if (avatar.AvatarType() >= Avatars::MIN_NIGHTMARE_TYPE) {
    return retval;
  }
  // check house shield
  if (avatar.ShowGuild() && (Stats().GuildRank(avatar.GuildID()) != avatar.GuildRank())) {
    retval = 1;
  }
  // check sphere
  if (avatar.ShowSphere() && (Stats().Sphere() != avatar.Sphere())) {
    retval = 2;
  }
  // check teacher
  if (avatar.Teacher() && (Arts().Skill(Arts::TRAIN) == 0)) {
    retval = 3;
  }

  // check teacher
  if (avatar.Teacher() && (Arts().Skill(Arts::TRAIN) == 0)) {
    retval = 3;
  }

  if (avatar.Dreamstrike() && (Arts().Skill(Arts::DREAMSTRIKE) == 0)) {
    retval = 4;
  }

  if (avatar.WordSmith() && (Arts().Skill(Arts::WORDSMITH_MARK) == 0)) {
    retval = 5;
  }

  if (avatar.DreamSmith() && (Arts().Skill(Arts::DREAMSMITH_MARK) == 0)) {
    retval = 6;
  }

  if (avatar.NPSymbol() && (Arts().Skill(Arts::NP_SYMBOL) == 0)) {
    retval = 7;
  }

  return retval;
}

////
// FixAvatar: check and optionally modify avatar such that any house shields, sphere, or teacher indicators are valid;
//   return > 0 if changes were made, 0 if not
////

int LmPlayerDB::FixAvatar(bool modify)
{
  int retval = 0;
  if (this->AccountType() == LmPlayerDB::ACCT_PMARE)
    {
      if (Avatar().AvatarType() >= Avatars::MIN_NIGHTMARE_TYPE) 
	return 0;
      //      avatar_.SetAvatarType(Avatars::MIN_NIGHTMARE_TYPE);
      //      return 1;
  }
  else if (Avatar().AvatarType() >= Avatars::MIN_NIGHTMARE_TYPE)
    return retval;

  if (modify) {

	avatar_.SetFocus(this->Stats().FocusStat());
	if (this->Arts().Skill(Arts::DREAMSTRIKE) > 0 ) 
		avatar_.SetDreamstrike(1);
	else
		avatar_.SetDreamstrike(0);

	if (this->Arts().Skill(Arts::WORDSMITH_MARK) > 0)
		avatar_.SetWordSmith(1);
	else
		avatar_.SetWordSmith(0);

	if (this->Arts().Skill(Arts::DREAMSMITH_MARK) > 0)
		avatar_.SetDreamSmith(1);
	else
		avatar_.SetDreamSmith(0);

	if (this->Arts().Skill(Arts::NP_SYMBOL) > 0)
		avatar_.SetNPSymbol(1);
	else
		avatar_.SetNPSymbol(0);

  }

  
  // check house shield
  if (Avatar().ShowGuild() && (Stats().GuildRank(Avatar().GuildID()) != Avatar().GuildRank())) {
    // clear it
    if (modify) {
      avatar_.SetShowGuild(0);
      avatar_.SetGuildRank(Guild::NO_RANK);
      avatar_.SetGuildID(Guild::NO_GUILD);
    }
    retval = 2;
  }
  // check sphere
  if (Avatar().ShowSphere() && (Stats().Sphere() != Avatar().Sphere())) {
    if (modify) {
      avatar_.SetShowSphere(0);
      avatar_.SetSphere(Stats().Sphere());
    }
    retval = 3;
  }
  // check teacher
  if (Avatar().Teacher() && (Arts().Skill(Arts::TRAIN) == 0)) {
    if (modify) {
      avatar_.SetTeacher(0);
    }
    retval = 4;
  }
  // check master teacher
  if (Avatar().MasterTeacher() && (Arts().Skill(Arts::TRAIN_SELF) == 0)) {
    if (modify) {
      avatar_.SetMasterTeacher(0);
    }
    retval = 5;
  }

  // check GM invisibility, Show Lyran
  if (this->AccountType() != LmPlayerDB::ACCT_ADMIN)
  {
	  if ((avatar_.Hidden())  || (avatar_.ShowLyran())) {
		  if (modify) {
			avatar_.SetHidden(0);
			avatar_.SetShowLyran(0);
		  }
		retval = 6;
	  }
  }

  // GM Invis for dreamers only
  if ((Avatar().AvatarType() >= Avatars::MIN_NIGHTMARE_TYPE) &&
		Avatar().Hidden()) {
	  if (modify)
		  avatar_.SetHidden(0);
	retval = 7;
  }


  // GM Invis for %INVIS% real named accounts only
  if (avatar_.Hidden())
  {
	if ((NULL == _tcsstr(realname_, _T("INVIS"))) && 
		(NULL == _tcsstr(realname_, _T("invis"))) &&
		(NULL == _tcsstr(realname_, _T("Invis"))))
	{
		 if (modify)
			 avatar_.SetHidden(0);
		retval = 8;
	}
  }



  return retval;
}


////
// Dump
////

void LmPlayerDB::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LmPlayerDB[%p,%d]: id=%u billing_id=%u name='%s' real='%s' email='%s'>\n"),
	  this, sizeof(LmPlayerDB),
	  PlayerID(), BillingID(), PlayerName(), RealName(), Email());
  INDENT(indent + 1, f);
 _ftprintf(f, _T("logins=%d online=%d laston=%lu acct=%c xpgain=%d xploss=%d x=%d y=%d level_id=%d\n"),
	  NumLogins(), TimeOnline(), LastLogin(), AccountType(), XPGained(), XPLost(), X(), Y(), LevelID());
  avatar_.Dump(f, indent + 1);
  // the following items only matter for non-monsters
  if (AccountType() != ACCT_MONSTER) {
    INDENT(indent + 1, f);
   _ftprintf(f, _T("initiators: "));
    for (int i = 0; i < NUM_GUILDS; ++i) {
      if (Initiator(i) != Lyra::ID_UNKNOWN) {
_ftprintf(f, _T("%d:%u "), i, Initiator(i));
      }
    }
   _ftprintf(f, _T("\n"));
    INDENT(indent + 1, f);
   _ftprintf(f, _T("goalbook: "));
    goalbook_.Dump(f);
    stats_.Dump(f, indent + 1);
    arts_.Dump(f, indent + 1);
    inv_.Dump(f, indent + 1);
  }
}

lyra_id_t LmPlayerDB::PlayerID() const
{
    return playerid_;
}

lyra_id_t LmPlayerDB::BillingID() const
{
    return billing_id_;
}

int LmPlayerDB::PMareBilling() const
{
    return pmare_billing_;
}

const TCHAR* LmPlayerDB::PlayerName() const
{
    return playername_;
}

const TCHAR* LmPlayerDB::Password() const
{
    return password_;
}

const TCHAR* LmPlayerDB::AvatarDescrip() const
{
    return avatar_descrip_;
}

const LmStats& LmPlayerDB::Stats() const
{
    return stats_;
}

const LmArts& LmPlayerDB::Arts() const
{
    return arts_;
}

const LmAvatar& LmPlayerDB::Avatar() const
{
    return avatar_;
}

const LmInventory& LmPlayerDB::Inventory() const
{
    return inv_;
}

time_t LmPlayerDB::LastLogin() const
{
    return last_login_;
}

int LmPlayerDB::TimeOnline() const
{
    return time_online_;
}

int LmPlayerDB::NumLogins() const
{
    return num_logins_;
}

const TCHAR* LmPlayerDB::RealName() const
{
    return realname_;
}

const TCHAR* LmPlayerDB::Email() const
{
    return email_;
}

int LmPlayerDB::AccountType() const
{
    return acct_type_;
}

LmStats& LmPlayerDB::Stats()
{
    return stats_;
}

LmArts& LmPlayerDB::Arts()
{
    return arts_;
}

LmAvatar& LmPlayerDB::Avatar()
{
    return avatar_;
}

LmInventory& LmPlayerDB::Inventory()
{
    return inv_;
}

int LmPlayerDB::XPGained() const
{
    return xp_gain_;
}

int LmPlayerDB::XPLost() const
{
    return xp_loss_;
}

lyra_id_t LmPlayerDB::Initiator(int guild_num) const
{
    return initiators_[guild_num];
}

int LmPlayerDB::NewlyAlert() const
{
    return newly_alert_;
}

int LmPlayerDB::NewlyAwakened() const
{
    return newly_awakened_;
}

short LmPlayerDB::X() const
{
    return x_;
}

short LmPlayerDB::Y() const
{
    return y_;
}

unsigned char LmPlayerDB::LevelID() const
{
    return level_id_;
}

unsigned char LmPlayerDB::Gamesite() const
{
    return gamesite_;
}

lyra_id_t LmPlayerDB::GamesiteID() const
{
    return gamesite_id_;
}


//INLINE int LmPlayerDB::TCPOnly() const
//{  return tcp_only_; }

/*
INLINE bool LmPlayerDB::PlayerModified() const
{
  if (player_modified_ || stats_.XPModified() || avatar_.Modified())
    return true;
  else
    return false;
}

INLINE bool LmPlayerDB::StatModified() const
{
  return stats_.StatsModified();
}

INLINE bool LmPlayerDB::SkillModified() const
{
  return arts_.Modified();
}

INLINE bool LmPlayerDB::GuildPlayerModified() const
{
  return stats_.GuildModified();
}
*/

void LmPlayerDB::UpdateStats(const LmStats& stats)
{
    stats_ = stats;
}

void LmPlayerDB::SetArts(const LmArts& arts)
{
    arts_ = arts;
}

void LmPlayerDB::SetLastLogin(time_t time)
{
    last_login_ = time;
}

void LmPlayerDB::SetTimeOnline(int online)
{
    time_online_ = online;
    //  player_modified_ = true;
}

void LmPlayerDB::SetNumLogins(int logins)
{
    num_logins_ = logins;
    //  player_modified_ = true;
}

void LmPlayerDB::SetAvatar(const LmAvatar& avatar)
{
    avatar_ = avatar;
    //  player_modified_ = true;
}

const LmIdSet& LmPlayerDB::GoalBook() const
{
    return goalbook_;
}

LmIdSet& LmPlayerDB::GoalBook()
{
    return goalbook_;
}

int LmPlayerDB::AcceptGoal(lyra_id_t goalid)
{
    int retval = 0;
    if (goalid != 0) {
        retval = goalbook_.Add(goalid);
    }
    return retval;
}

int LmPlayerDB::RemoveGoal(lyra_id_t goalid)
{
    return goalbook_.Remove(goalid);
}

void LmPlayerDB::SetPlayerID(lyra_id_t playerid)
{
    playerid_ = playerid;
}

void LmPlayerDB::SetBillingID(lyra_id_t billing_id)
{
    billing_id_ = billing_id;
}

void LmPlayerDB::SetPMareBilling(int pmare_billing)
{
    pmare_billing_ = pmare_billing;
}

void LmPlayerDB::SetPlayerName(const TCHAR* playername)
{
    _tcscpy(playername_, playername);
}

void LmPlayerDB::SetPassword(const TCHAR* password)
{
    _tcscpy(password_, password);
}

void LmPlayerDB::SetAvatarDescrip(const TCHAR* descrip)
{
    _tcscpy(avatar_descrip_, descrip);
    //  player_modified_ = true;
}

void LmPlayerDB::SetRealName(const TCHAR* realname)
{
    _tcscpy(realname_, realname);
}

void LmPlayerDB::SetEmail(const TCHAR* email)
{
    _tcscpy(email_, email);
}

void LmPlayerDB::SetAccountType(int acct_type)
{
    acct_type_ = acct_type;
}

void LmPlayerDB::SetXPGained(int xp_gain)
{
    xp_gain_ = xp_gain;
    //player_modified_ = true;
}

void LmPlayerDB::SetXPLost(int xp_loss)
{
    xp_loss_ = xp_loss;
    //player_modified_ = true;
}

void LmPlayerDB::SetInitiator(int guild_num, lyra_id_t initiator)
{
    initiators_[guild_num] = initiator;
}

void LmPlayerDB::SetNewlyAlert(int newly_alert)
{
    newly_alert_ = newly_alert;
}

void LmPlayerDB::SetNewlyAwakened(int newly_awakened)
{
    newly_awakened_ = newly_awakened;
}

void LmPlayerDB::SetX(short x)
{
    x_ = x;
}

void LmPlayerDB::SetY(short y)
{
    y_ = y;
}

void LmPlayerDB::SetLevelID(unsigned char level_id)
{
    level_id_ = level_id;
}

void LmPlayerDB::SetGamesite(unsigned char gamesite)
{
    gamesite_ = gamesite;
}

void LmPlayerDB::SetGamesiteID(lyra_id_t gamesite_id)
{
    gamesite_id_ = gamesite_id;
}


//INLINE void LmPlayerDB::SetTCPOnly(int tcp_only)
//{  tcp_only_ = tcp_only; }

/*

INLINE void LmPlayerDB::ClearModified(void)
{
  player_modified_ = false;
  arts_.SetModified(false);
  avatar_.SetModified(false);
  stats_.SetStatsModified(false);
  stats_.SetXPModified(false);
  stats_.SetGuildModified(false);
}
*/