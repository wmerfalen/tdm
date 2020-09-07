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
	ATTACKER_NARROWLY_MISSED_YOU_EVENT,
	YOU_ARE_INJURED_EVENT,
	TARGET_DEAD_EVENT,
	YOU_MISSED_YOUR_TARGET_EVENT,
	HIT_BY_RIFLE_ATTACK,
	HIT_BY_SPRAY_ATTACK,
	YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK,
	YOU_DEALT_HEADSHOT_WITH_SPRAY_ATTACK,
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
	YOU_INFLICTED_AR_SHRAPNEL
};
#include <functional>
using uuid_t = uint64_t;
using damage_info_t = std::tuple<int,uuid_t>;
struct feedback_t {
	damage_event_t damage_event;
	int damage;
	uint16_t hits;
	std::vector<uuid_t> injured;
	std::vector<std::tuple<uuid_t,int,uint32_t>> damage_info;
	uint8_t from_direction;
	uuid_t attacker;
};
using damage_event_callback_t = std::function<void(feedback_t,uuid_t)>;

#endif
