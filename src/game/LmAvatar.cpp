// LmAvatar.cpp  -*- C++ -*-
// $Id: LmAvatar.cpp,v 1.8 1998-04-03 17:01:13-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#include "../../include/platform/Platform.h"

#ifdef WIN32
#define STRICT
#include "../../include/Core/unix.h"
#include <winsock.h>
#else
#include <sys/types.h>
#include <netinet/in.h> //linux
#endif
#include <stdio.h>
#include <string.h>

#include "../../include/Core/SharedConstants.h"
#include "../../include/Game/LmAvatar.h"

// default object
const LmAvatar LmAvatar::DEFAULT_INSTANCE;

////
// constructor
////

LmAvatar::LmAvatar()
{
	Init(Avatars::MALE, 0, 0, 0, 0, 0, Guild::NO_GUILD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

////
// Init
////

void LmAvatar::Init(unsigned int avatar_type, unsigned int color0, 
		    unsigned int color1, unsigned int color2,
		    unsigned int color3, unsigned int color4,
			unsigned int guild_id, unsigned int guild_rank,
			unsigned int show_guild, unsigned int head_type, 
			unsigned int sphere, unsigned int show_sphere, 
			unsigned int teacher, unsigned int master_teacher, 
			unsigned int show_lyran, unsigned int dreamsmith,
			unsigned int hidden, unsigned int extra_damage,
			unsigned int wordsmith, unsigned int dreamstrike,
			unsigned int focus, unsigned int np_symbol, unsigned int apprentice
			)
{
  SetAvatarType(avatar_type);
  SetColor0(color0);
  SetColor1(color1);
  SetColor2(color2);
  SetColor3(color3);
  SetColor4(color4);
  SetGuildID(guild_id);
  SetGuildRank(guild_rank);
  SetShowGuild(show_guild);
  SetHead(head_type);
  SetSphere(sphere);
  SetShowSphere(show_sphere);
  SetTeacher(teacher);
  SetMasterTeacher(master_teacher);
  SetShowLyran(show_lyran);
  SetDreamSmith(dreamsmith);
  SetAccountType(LmAvatar::ACCT_DREAMER); // default to dreamer - set by server
  SetHidden(hidden);
  SetExtraDamage(extra_damage);
  SetWordSmith(wordsmith);
  SetDreamstrike(dreamstrike);
  SetFocus(focus);
  SetNPSymbol(np_symbol);
  SetApprentice(apprentice);
}

void LmAvatar::Init(const lyra_avatar_t& avatar)
{
  SetAvatar(avatar);
}

void LmAvatar::Init(int avatar1, int avatar2)
{
  SetAvatar1(avatar1);
  SetAvatar2(avatar2);
}

////
// ConvertToHost
////

void LmAvatar::ConvertToHost()
{
  avatar_.avatar1.ConvertToHost();
  avatar_.avatar2.ConvertToHost();
}

////
// ConvertToNetwork
////

void LmAvatar::ConvertToNetwork()
{
  avatar_.avatar1.ConvertToNetwork();
  avatar_.avatar2.ConvertToNetwork();
}

////
// Parse
////

int LmAvatar::Parse(const TCHAR* str)
{
  int av1, av2;
  if (_stscanf(str, _T("%u:%u"), &av1, &av2) != 2) {
    return -1;
  }
  SetAvatar1(av1);
  SetAvatar2(av2);
  return 0;
}

////
// UnParse
////

void LmAvatar::UnParse(TCHAR* str, int /* strlen */) const
{
 _stprintf(str, _T("%u:%u"), Avatar1(), Avatar2());
}

////
// Dump
////

#ifdef USE_DEBUG
void LmAvatar::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
 _ftprintf(f, _T("<LmAvatar[%p,%d]: av=[%u:%u] bm=%u colors=(%u,%u,%u,%u,%u) guild_id=%u guild_rank=%u show_guild=%u head=%u sphere=%u show_sphere=%u teacher=%u master teacher=%u show lyran=%u dreamsmith=%u acct_type=%u hidden=%u extra_damage=%u wordsmith=%u dreamstrike=%u focus=%u np_symbol=%u apprentice=%u>\n"),
	  this, sizeof(LmAvatar),
	  Avatar1(), Avatar2(), AvatarType(), Color0(), Color1(), Color2(), 
	  Color3(), Color4(), GuildID(), GuildRank(), ShowGuild(), Head(), 
	  Sphere(), ShowSphere(), Teacher(), MasterTeacher(), ShowLyran(), 
	  DreamSmith(), AccountType(), Hidden(), ExtraDamage(), WordSmith(), 
	  Dreamstrike(), Focus(), NPSymbol(), Apprentice());
}
#endif /* USE_DEBUG */

////
// Dump1
////

#ifdef USE_DEBUG
void LmAvatar::Dump1(FILE* f) const
{
 _ftprintf(f, _T("[%u:%u]"), Avatar1(), Avatar2());
}
#endif /* USE_DEBUG */

#include "SharedConstants.h"

#ifndef USE_DEBUG
void LmAvatar::Dump(FILE*, int) const
{
    // empty
}

void LmAvatar::Dump1(FILE*) const
{
    // empty
}
#endif /* !USE_DEBUG */

// returns base 4dx bitmap id for this avatar
int LmAvatar::BitmapID() const
{
    switch (this->AvatarType()) {
    case Avatars::MALE:
        return LyraBitmap::MALE_AVATAR;

    case Avatars::FEMALE:
        return LyraBitmap::FEMALE_AVATAR;

    case Avatars::EMPHANT:
        return LyraBitmap::EMPHANT_AVATAR;

    case Avatars::BOGROM:
        return LyraBitmap::BOGROM_AVATAR;

    case Avatars::AGOKNIGHT:
        return LyraBitmap::AGOKNIGHT_AVATAR;

    case Avatars::SHAMBLIX:
        return LyraBitmap::SHAMBLIX_AVATAR;

    case Avatars::HORRON:
        return LyraBitmap::HORRON_AVATAR;

    default:
        return LyraBitmap::MALE_AVATAR;
    }
}

int LmAvatar::EssenceBitmap() const
{
    switch (this->AvatarType()) {
    case Avatars::MALE:
        return LyraBitmap::M_AVATAR_ESSENCE;

    case Avatars::FEMALE:
        return LyraBitmap::F_AVATAR_ESSENCE;

    case Avatars::EMPHANT:
        return LyraBitmap::EMPHANT_ESSENCE;

    case Avatars::BOGROM:
        return LyraBitmap::BOGROM_ESSENCE;

    case Avatars::AGOKNIGHT:
        return LyraBitmap::AGOKNIGHT_ESSENCE;

    case Avatars::SHAMBLIX:
    case Avatars::HORRON: // no Horron essence yet
        return LyraBitmap::SHAMBLIX_ESSENCE;

    default:
        return LyraBitmap::M_AVATAR_ESSENCE;
    }
}

// converts 4dx bitmap id's into avatar types
void LmAvatar::SetBitmapID(unsigned int bitmap_id)
{
    switch (bitmap_id) {
    case LyraBitmap::MALE_AVATAR:
        this->SetAvatarType(Avatars::MALE);
        break;

    case LyraBitmap::FEMALE_AVATAR:
        this->SetAvatarType(Avatars::FEMALE);
        break;

    case LyraBitmap::EMPHANT_AVATAR:
        this->SetAvatarType(Avatars::EMPHANT);
        break;

    case LyraBitmap::BOGROM_AVATAR:
        this->SetAvatarType(Avatars::BOGROM);
        break;

    case LyraBitmap::AGOKNIGHT_AVATAR:
        this->SetAvatarType(Avatars::AGOKNIGHT);
        break;

    case LyraBitmap::SHAMBLIX_AVATAR:
        this->SetAvatarType(Avatars::SHAMBLIX);
        break;

        /*
          case LyraBitmap::HORRON_AVATAR:
            this->SetAvatarType(Avatars::SHAMBLIX);
            break;
            */

    default:
        this->SetAvatarType(Avatars::EMPHANT);
        break;
    }
}

const lyra_avatar_t& LmAvatar::Avatar() const
{
    return avatar_;
}

int LmAvatar::Avatar1() const
{
    return avatar_.avatar1.Value();
}

int LmAvatar::Avatar2() const
{
    return avatar_.avatar2.Value();
}

void LmAvatar::SetAvatar1(int avatar1)
{
    avatar_.avatar1.SetValue(avatar1);
}

void LmAvatar::SetAvatar2(int avatar2)
{
    avatar_.avatar2.SetValue(avatar2);
}

unsigned int LmAvatar::AvatarType() const
{
    return avatar_.avatar1.GetBits(AVATAR_START, AVATAR_WIDTH);
}

unsigned int LmAvatar::Color0() const
{
    return avatar_.avatar1.GetBits(COLOR0_START, COLOR_WIDTH);
}

unsigned int LmAvatar::Color1() const
{
    return avatar_.avatar1.GetBits(COLOR1_START, COLOR_WIDTH);
}

unsigned int LmAvatar::Color2() const
{
    return avatar_.avatar1.GetBits(COLOR2_START, COLOR_WIDTH);
}

unsigned int LmAvatar::Color3() const
{
    return avatar_.avatar1.GetBits(COLOR3_START, COLOR_WIDTH);
}

unsigned int LmAvatar::Color4() const
{
    return avatar_.avatar1.GetBits(COLOR4_START, COLOR_WIDTH);
}

unsigned int LmAvatar::GuildID() const
{
    return avatar_.avatar1.GetBits(GUILD_ID_START, GUILD_ID_WIDTH);
}

unsigned int LmAvatar::GuildRank() const
{
    return avatar_.avatar1.GetBits(GUILD_RANK_START, GUILD_RANK_WIDTH);
}

unsigned int LmAvatar::ShowGuild() const
{
    return avatar_.avatar1.GetBits(SHOW_GUILD_START, SHOW_GUILD_WIDTH);
}

unsigned int LmAvatar::Head() const
{
    return avatar_.avatar2.GetBits(HEAD_START, HEAD_WIDTH);
}

unsigned int LmAvatar::Sphere() const
{
    return avatar_.avatar2.GetBits(SPHERE_START, SPHERE_WIDTH);
}

unsigned int LmAvatar::ShowSphere() const
{
    return avatar_.avatar2.GetBits(SHOW_SPHERE_START, SHOW_SPHERE_WIDTH);
}

unsigned int LmAvatar::Teacher() const
{
    return avatar_.avatar2.GetBits(TEACHER_START, TEACHER_WIDTH);
}

unsigned int LmAvatar::Focus() const
{
    return avatar_.avatar2.GetBits(FOCUS_START, FOCUS_WIDTH);
}

unsigned int LmAvatar::MasterTeacher() const
{
    return avatar_.avatar2.GetBits(MASTER_TEACHER_START, MASTER_TEACHER_WIDTH);
}

unsigned int LmAvatar::ShowLyran() const
{
    return avatar_.avatar2.GetBits(SHOW_LYRAN_START, SHOW_LYRAN_WIDTH);
}

unsigned int LmAvatar::DreamSmith() const
{
    return avatar_.avatar2.GetBits(DREAMSMITH_START, DREAMSMITH_WIDTH);
}

unsigned int LmAvatar::WordSmith() const
{
    return avatar_.avatar2.GetBits(WORDSMITH_START, WORDSMITH_WIDTH);
}

unsigned int LmAvatar::Dreamstrike() const
{
    return avatar_.avatar2.GetBits(DREAMSTRIKE_START, DREAMSTRIKE_WIDTH);
}

unsigned int LmAvatar::NPSymbol() const
{
    return avatar_.avatar2.GetBits(NP_SYMBOL_START, NP_SYMBOL_WIDTH);
}

unsigned int LmAvatar::Apprentice() const
{
    return avatar_.avatar2.GetBits(APPRENTICE_START, APPRENTICE_WIDTH);
}

unsigned int LmAvatar::AccountType() const
{
    return avatar_.avatar2.GetBits(ACCOUNT_START, ACCOUNT_WIDTH);
}

unsigned int LmAvatar::Hidden() const
{
    return avatar_.avatar2.GetBits(HIDDEN_START, HIDDEN_WIDTH);
}

unsigned int LmAvatar::ExtraDamage() const
{
    return avatar_.avatar2.GetBits(EXTRA_DAMAGE_START, EXTRA_DAMAGE_WIDTH);
}

void LmAvatar::SetAvatar(const lyra_avatar_t& avatar)
{
    avatar_ = avatar;
}

void LmAvatar::SetAvatarType(unsigned int type)
{
    avatar_.avatar1.SetBits(AVATAR_START, AVATAR_WIDTH, type);
}

void LmAvatar::SetColor0(unsigned int color)
{
    avatar_.avatar1.SetBits(COLOR0_START, COLOR_WIDTH, color);
}

void LmAvatar::SetColor1(unsigned int color)
{
    avatar_.avatar1.SetBits(COLOR1_START, COLOR_WIDTH, color);
}

void LmAvatar::SetColor2(unsigned int color)
{
    avatar_.avatar1.SetBits(COLOR2_START, COLOR_WIDTH, color);
}

void LmAvatar::SetColor3(unsigned int color)
{
    avatar_.avatar1.SetBits(COLOR3_START, COLOR_WIDTH, color);
}

void LmAvatar::SetColor4(unsigned int color)
{
    avatar_.avatar1.SetBits(COLOR4_START, COLOR_WIDTH, color);
}

void LmAvatar::SetGuildID(unsigned int id)
{
    avatar_.avatar1.SetBits(GUILD_ID_START, GUILD_ID_WIDTH, id);
}

void LmAvatar::SetGuildRank(unsigned int rank)
{
    avatar_.avatar1.SetBits(GUILD_RANK_START, GUILD_RANK_WIDTH, rank);
}

void LmAvatar::SetShowGuild(unsigned int show)
{
    avatar_.avatar1.SetBits(SHOW_GUILD_START, SHOW_GUILD_WIDTH, show);
}

void LmAvatar::SetHead(unsigned int id)
{
    avatar_.avatar2.SetBits(HEAD_START, HEAD_WIDTH, id);
}

void LmAvatar::SetSphere(unsigned int sphere)
{
    avatar_.avatar2.SetBits(SPHERE_START, SPHERE_WIDTH, sphere);
}

void LmAvatar::SetShowSphere(unsigned int show)
{
    avatar_.avatar2.SetBits(SHOW_SPHERE_START, SHOW_SPHERE_WIDTH, show);
}

void LmAvatar::SetTeacher(unsigned int teacher)
{
    avatar_.avatar2.SetBits(TEACHER_START, TEACHER_WIDTH, teacher);
}

void LmAvatar::SetFocus(unsigned int focus)
{
    avatar_.avatar2.SetBits(FOCUS_START, FOCUS_WIDTH, focus);
}

void LmAvatar::SetMasterTeacher(unsigned int master_teacher)
{
    avatar_.avatar2.SetBits(MASTER_TEACHER_START, MASTER_TEACHER_WIDTH, master_teacher);
}

void LmAvatar::SetShowLyran(unsigned int show_lyran)
{
    avatar_.avatar2.SetBits(SHOW_LYRAN_START, SHOW_LYRAN_WIDTH, show_lyran);
}

void LmAvatar::SetDreamSmith(unsigned int dreamsmith)
{
    avatar_.avatar2.SetBits(DREAMSMITH_START, DREAMSMITH_WIDTH, dreamsmith);
}

void LmAvatar::SetWordSmith(unsigned int wordsmith)
{
    avatar_.avatar2.SetBits(WORDSMITH_START, WORDSMITH_WIDTH, wordsmith);
}

void LmAvatar::SetDreamstrike(unsigned int dreamstrike)
{
    avatar_.avatar2.SetBits(DREAMSTRIKE_START, DREAMSTRIKE_WIDTH, dreamstrike);
}

void LmAvatar::SetNPSymbol(unsigned int np_symbol)
{
    avatar_.avatar2.SetBits(NP_SYMBOL_START, NP_SYMBOL_WIDTH, np_symbol);
}

void LmAvatar::SetApprentice(unsigned int apprentice)
{
    avatar_.avatar2.SetBits(APPRENTICE_START, APPRENTICE_WIDTH, apprentice);
}

void LmAvatar::SetAccountType(unsigned int acct_type)
{
    avatar_.avatar2.SetBits(ACCOUNT_START, ACCOUNT_WIDTH, acct_type);
}

void LmAvatar::SetHidden(unsigned int hidden)
{
    avatar_.avatar2.SetBits(HIDDEN_START, HIDDEN_WIDTH, hidden);
}

void LmAvatar::SetExtraDamage(unsigned int extra_damage)
{
    avatar_.avatar2.SetBits(EXTRA_DAMAGE_START, EXTRA_DAMAGE_WIDTH, extra_damage);
}

