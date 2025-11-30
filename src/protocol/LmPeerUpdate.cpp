// LmPeerUpdate.cpp  -*- C++ -*-
// $Id: LmPeerUpdate.cpp,v 1.10 1997-11-06 18:21:32-08 jason Exp $
// Copyright 1996-1997 Lyra LLC, All rights reserved.
//
// implementation

#ifdef __GNUC__
#pragma implementation "LmPeerUpdate.h"
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

#include "../../include/Protocol/LmPeerUpdate.h"
#include "../../include/Core/LyraDefs.h"

// default object
const LmPeerUpdate LmPeerUpdate::DEFAULT_INSTANCE;

////
// Constructor
////

LmPeerUpdate::LmPeerUpdate()
{
  Init(Lyra::ID_UNKNOWN, 0, 0, 0, 0, 0, 0);
}

////
// Init
////

void LmPeerUpdate::Init(short realtimeid, unsigned char soundid, short x, short y, short z, int u1, int u2)
{
  SetRealtimeID(realtimeid);
  SetSoundID(soundid);
  SetPosition(x, y, z);
  u_.u1.SetValue(u1);
  u_.u2.SetValue(u2);
}

void LmPeerUpdate::Init(const lyra_peer_update_t& update)
{
  SetUpdate(update);
}

////
// ConvertToNetwork
////

void LmPeerUpdate::ConvertToNetwork()
{
  HTONS(u_.realtime_id);
  HTONS(u_.x);
  HTONS(u_.y);
  u_.u1.ConvertToNetwork();
  u_.u2.ConvertToNetwork();
}

////
// ConvertToHost
////

void LmPeerUpdate::ConvertToHost()
{
  NTOHS(u_.realtime_id);
  NTOHS(u_.x);
  NTOHS(u_.y);
  u_.u1.ConvertToHost();
  u_.u2.ConvertToHost();
}

////
// Dump: print to FILE stream
////

#ifdef USE_DEBUG
void LmPeerUpdate::Dump(FILE* f, int indent) const
{
  INDENT(indent, f);
	_ftprintf(f, _T("<LmPeerUpdate[%p,%d]: real time id=%u sound id=%u pos=(%d,%d) u1=%u u2=%u>\n"), this, sizeof(LmPeerUpdate),
	  RealtimeID(), SoundID(), X(), Y(), U1(), U2());
  // print u1 stuff
  INDENT(indent + 1, f);
  //char str[80];
  //u_.u1.UnParse(str, sizeof(str));
  ///_ftprintf(f, _T("u1 := %s\n"), str);
  //INDENT(indent + 1, f);
	_ftprintf(f, _T("hit=%d weapon_damage=%d flags=0x%x attack_bits=0x%x angle=%u\n"),
	  HitBits(), WeaponDamage(), Flags(), AttackBits(), Angle());
  // print u2 stuff
  INDENT(indent + 1, f);
  //u_.u2.UnParse(str, sizeof(str));
  //_ftprintf(f, _T("u2 := %s\n"), str);
  //INDENT(indent + 1, f);
	_ftprintf(f, _T("local=%u wpn_effect=%d ht_delta=%d wpn_vel=%d wpn_bmap=%d harmful=%d color=%d/%d wave=%u\n"),
	  Local(), WeaponEffect(), HeightDelta(), WeaponVelocity(), WeaponBitmap(), Harmful(),
	  PrimaryColor(), SecondaryColor(), Wave());
}
#endif /* USE_DEBUG */

#include "SharedConstants.h" // need bitmap constants

#ifndef USE_DEBUG
void LmPeerUpdate::Dump(FILE*, int) const
{
	// empty
}
#endif /* !USE_DEBUG */

const lyra_peer_update_t& LmPeerUpdate::Update() const
{
	return u_;
}

void LmPeerUpdate::SetUpdate(const lyra_peer_update_t& update)
{
	u_ = update;
}

short LmPeerUpdate::RealtimeID() const
{
	return u_.realtime_id;
}

unsigned char LmPeerUpdate::SoundID() const
{
	return u_.sound_id;
}

int LmPeerUpdate::X() const
{
	return u_.x;
}

int LmPeerUpdate::Y() const
{
	return u_.y;
}

int LmPeerUpdate::Z() const
{
	return u_.z;
}

unsigned int LmPeerUpdate::Angle() const
{
	return u_.u1.GetBits(ANGLE_START, ANGLE_WIDTH);
}

unsigned int LmPeerUpdate::AttackBits() const
{
	return u_.u1.GetBits(ATTACKBITS_START, ATTACKBITS_WIDTH);
}

unsigned int LmPeerUpdate::Flags() const
{
	return u_.u1.GetBits(FLAGS_START, FLAGS_WIDTH);
}

bool LmPeerUpdate::FlagSet(int flag) const
{
	return (Flags() & flag) ? true : false;
}

unsigned int LmPeerUpdate::WeaponDamage() const
{
	return u_.u1.GetBits(WEAPONDAMAGE_START, WEAPONDAMAGE_WIDTH);
}

unsigned int LmPeerUpdate::WeaponBitmap() const
{
	unsigned int normalized_bitmap =
		(u_.u2.GetBits(WEAPONBITMAP_START, WEAPONBITMAP_WIDTH) + LyraBitmap::MINIMUM_MISSILE_BITMAP);
	return normalized_bitmap;
}

unsigned int LmPeerUpdate::HitBits() const
{
	return u_.u2.GetBits(HITBITS_START, HITBITS_WIDTH);
}

int LmPeerUpdate::WeaponVelocity() const
{
	int retval = u_.u2.GetBits(WEAPONVELOCITY_START, WEAPONVELOCITY_WIDTH);
	if (retval > 8) {
		retval -= 8;
		retval = -retval;
	}
	return retval;
}

int LmPeerUpdate::HeightDelta() const
{
	return u_.u2.GetBits(HEIGHTDELTA_START, HEIGHTDELTA_WIDTH);
}

unsigned int LmPeerUpdate::WeaponEffect() const
{
	return u_.u2.GetBits(WEAPONEFFECT_START, WEAPONEFFECT_WIDTH);
}

unsigned int LmPeerUpdate::Local() const
{
	return u_.u2.GetBits(LOCAL_START, LOCAL_WIDTH);
}

unsigned int LmPeerUpdate::Harmful() const
{
	return u_.u2.GetBits(HARMFUL_START, HARMFUL_WIDTH);
}

unsigned int LmPeerUpdate::PrimaryColor() const
{
	return u_.u2.GetBits(PRIMARY_COLOR_START, PRIMARY_COLOR_WIDTH);
}

unsigned int LmPeerUpdate::SecondaryColor() const
{
	return u_.u2.GetBits(SECONDARY_COLOR_START, SECONDARY_COLOR_WIDTH);
}

unsigned int LmPeerUpdate::Wave() const
{
	return u_.u2.GetBits(WAVE_START, WAVE_WIDTH);
}

unsigned int LmPeerUpdate::Flying() const
{
	return u_.u1.GetBits(FLIGHT_START, FLIGHT_WIDTH);
}

unsigned int LmPeerUpdate::U1() const
{
	return u_.u1.Value();
}

unsigned int LmPeerUpdate::U2() const
{
	return u_.u2.Value();
}

void LmPeerUpdate::SetRealtimeID(short realtimeid)
{
	u_.realtime_id = realtimeid;
}

void LmPeerUpdate::SetSoundID(unsigned char soundid)
{
	u_.sound_id = soundid;
}

void LmPeerUpdate::SetPosition(int x, int y, int z)
{
	u_.x = x;
	u_.y = y;
	u_.z = z;
}

void LmPeerUpdate::SetAngle(unsigned int angle)
{
	u_.u1.SetBits(ANGLE_START, ANGLE_WIDTH, angle);
}

void LmPeerUpdate::SetAttackBits(unsigned int attack_bits)
{
	u_.u1.SetBits(ATTACKBITS_START, ATTACKBITS_WIDTH, attack_bits);
}

void LmPeerUpdate::SetFlags(unsigned int flags)
{
	u_.u1.SetBits(FLAGS_START, FLAGS_WIDTH, flags);
}

void LmPeerUpdate::SetWeaponDamage(unsigned int weapon_damage)
{
	u_.u1.SetBits(WEAPONDAMAGE_START, WEAPONDAMAGE_WIDTH, weapon_damage);
}

void LmPeerUpdate::SetHitBits(unsigned int hit_bits)
{
	u_.u2.SetBits(HITBITS_START, HITBITS_WIDTH, hit_bits);
}

void LmPeerUpdate::SetWeaponBitmap(unsigned int weapon_bitmap)
{	// to reduce the # of bits used, we normalize the weapon bitmap
	unsigned int normalized_bitmap = weapon_bitmap - LyraBitmap::MINIMUM_MISSILE_BITMAP;
	u_.u2.SetBits(WEAPONBITMAP_START, WEAPONBITMAP_WIDTH, normalized_bitmap);
}

void LmPeerUpdate::SetWeaponVelocity(int weapon_velocity)
{
	if (weapon_velocity < 0) {
		weapon_velocity = (-weapon_velocity) + 8;
	}
	u_.u2.SetBits(WEAPONVELOCITY_START, WEAPONVELOCITY_WIDTH, weapon_velocity);
}

void LmPeerUpdate::SetHeightDelta(int height_delta)
{
	u_.u2.SetBits(HEIGHTDELTA_START, HEIGHTDELTA_WIDTH, height_delta);
}

void LmPeerUpdate::SetWeaponEffect(unsigned int weapon_effect)
{
	u_.u2.SetBits(WEAPONEFFECT_START, WEAPONEFFECT_WIDTH, weapon_effect);
}

void LmPeerUpdate::SetHarmful(unsigned int harmful)
{
	u_.u2.SetBits(HARMFUL_START, HARMFUL_WIDTH, harmful);
}

void LmPeerUpdate::SetPrimaryColor(unsigned int primary)
{
	u_.u2.SetBits(PRIMARY_COLOR_START, PRIMARY_COLOR_WIDTH, primary);
}

void LmPeerUpdate::SetSecondaryColor(unsigned int secondary)
{
	u_.u2.SetBits(SECONDARY_COLOR_START, SECONDARY_COLOR_WIDTH, secondary);
}

void LmPeerUpdate::SetLocal(unsigned int local)
{
	u_.u2.SetBits(LOCAL_START, LOCAL_WIDTH, local);
}

void LmPeerUpdate::SetWave(unsigned int wave)
{
	u_.u2.SetBits(WAVE_START, WAVE_WIDTH, wave);
}

void LmPeerUpdate::SetFlying(unsigned int flying)
{
	u_.u1.SetBits(FLIGHT_START, FLIGHT_WIDTH, flying);
}
