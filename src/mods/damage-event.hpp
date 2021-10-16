#ifndef __MENTOC_MODS_DAMAGE_EVENT_HEADER__
#define __MENTOC_MODS_DAMAGE_EVENT_HEADER__

/**
 * To be consistent and to have a centralized place where all these
 * damage events are emitted, no other file other than weapons/damage-types.cpp
 * should emit these events. Likewise, no other file other than player.cpp
 * should handle these events.
 * -will, 2020-09-06
 */
enum damage_event_t : uint16_t {
	DAMAGE_EVENT_NOTHING = 0,
	ATTACKER_NARROWLY_MISSED_YOU_EVENT,
	YOU_ARE_INJURED_EVENT,
	TARGET_DEAD_EVENT,
	YOU_MISSED_YOUR_TARGET_EVENT,
	HIT_BY_RIFLE_ATTACK,
	HIT_BY_MELEE_ATTACK,
	HIT_BY_BLADED_MELEE_ATTACK,
	HIT_BY_BLUNT_MELEE_ATTACK,

	HIT_BY_INCENDIARY_DAMAGE,
	HIT_BY_RADIOACTIVE_DAMAGE,
	HIT_BY_ANTI_MATTER_DAMAGE,
	HIT_BY_CORROSIVE_DAMAGE,
	HIT_BY_EMP_DAMAGE,
	HIT_BY_EXPLOSIVE_DAMAGE,
	HIT_BY_SHRAPNEL_DAMAGE,
	HIT_BY_CRYOGENIC_DAMAGE,
	HIT_BY_SHOCK_DAMAGE,
	HIT_BY_BONUS_INNATE_SNIPER_RIFLE_ATTACK,
	HIT_BY_SHOTGUN_BLAST,

	HIT_BY_TEETH_AND_BONES,
	YOU_INFLICTED_CORPSE_EXPLOSION_DAMAGE,

	HIT_BY_CQC_DAMAGE,
	YOU_INFLICTED_CQC_DAMAGE,

	YOU_INFLICTED_BONUS_INNATE_SNIPER_RIFLE_ATTACK,
	YOU_INFLICTED_MELEE_ATTACK,
	YOU_INFLICTED_BLADED_MELEE_ATTACK,
	YOU_INFLICTED_BLUNT_MELEE_ATTACK,

	YOU_INFLICTED_INCENDIARY_DAMAGE,
	YOU_INFLICTED_RADIOACTIVE_DAMAGE,
	YOU_INFLICTED_ANTI_MATTER_DAMAGE,
	YOU_INFLICTED_CORROSIVE_DAMAGE,
	YOU_INFLICTED_EMP_DAMAGE,
	YOU_INFLICTED_EXPLOSIVE_DAMAGE,
	YOU_INFLICTED_SHRAPNEL_DAMAGE,
	YOU_INFLICTED_CRYOGENIC_DAMAGE,
	YOU_INFLICTED_SHOCK_DAMAGE,

	YOU_INFLICTED_SHOTGUN_BLAST,
	YOU_INFLICTED_SPRAY_DAMAGE,
	YOU_DEALT_HEADSHOT_WITH_SPRAY_ATTACK,

	TARGET_IS_OUT_OF_RANGE,

	HIT_BY_SPRAY_ATTACK,
	YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK,
	YOU_DEALT_CRITICAL_RIFLE_ATTACK,
	YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK,
	YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK,
	HIT_BY_CRITICAL_SPRAY_ATTACK,
	HIT_BY_CRITICAL_RIFLE_ATTACK,
	NO_PRIMARY_WIELDED_EVENT,
	OUT_OF_AMMO_EVENT,
	COOLDOWN_IN_EFFECT_EVENT,
	COULDNT_FIND_TARGET_EVENT,
	TARGET_IN_PEACEFUL_ROOM_EVENT,
	YOURE_IN_PEACEFUL_ROOM,
	YOU_INJURED_SOMEONE_EVENT,
	YOU_GOT_HIT_BY_REFLECTED_MUNITIONS_EVENT,
	YOU_REFLECTED_MUNITIONS_EVENT,
	YOU_GOT_HIT_BY_AR_SHRAPNEL,
	YOU_INFLICTED_AR_SHRAPNEL,
	YOU_GOT_HIT_BY_INCENDIARY_AMMO,
	YOU_INFLICTED_INCENDIARY_AMMO,
	YOU_DISORIENTED_SOMEONE_EVENT,
	YOU_ARE_DISORIENTED_EVENT,

	YOU_WERE_INFLICTED_WITH_BAD_LUCK,
	GUNFIRE_WHIZZED_BY_FROM,
	YOU_INFLICTED_SNIPE_DAMAGE,
};
#include <functional>
using uuid_t = uint64_t;
using damage_info_t = std::tuple<int,uuid_t>;
struct feedback_t {
	feedback_t() : damage_event(DAMAGE_EVENT_NOTHING),
		damage(0),
		hits(0),
		from_direction(0),
		attacker(0) {}
	feedback_t(damage_event_t e) :
		damage_event(e),
		damage(0),hits(0),from_direction(0),
		attacker(0) {}
	feedback_t(damage_event_t e,uint8_t dir) :
		damage_event(e),
		damage(0),hits(0),from_direction(dir),
		attacker(0) {
	}
	damage_event_t damage_event;
	int damage;
	uint16_t hits;
	std::vector<uuid_t> injured;
	std::vector<std::tuple<uuid_t,int,uint32_t>> damage_info;
	uint8_t from_direction;
	uuid_t attacker;
	std::string dump() {
		using namespace std;
		return string("damage_event:") + to_string(damage_event) + string("\r\n") +
		       string("damage:") + to_string(damage) + string("\r\n") +
		       string("hits:") + to_string(hits) + string("\r\n") +
		       string("from_direction:") + to_string(from_direction) + string("\r\n") +
		       string("attacker:") + to_string(attacker) + string("\r\n");
		/** TODO: dump injured and damage_info */
	}
};
using damage_event_callback_t = std::function<void(const feedback_t&,const uuid_t&)>;

#endif
