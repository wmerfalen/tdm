#ifndef __MENTOC_MODS_DAMAGE_EVENT_HEADER__
#define __MENTOC_MODS_DAMAGE_EVENT_HEADER__

enum damage_event_t : uint16_t {
	ATTACKER_NARROWLY_MISSED_YOU_EVENT,
	YOU_ARE_INJURED_EVENT,
	TARGET_DEAD_EVENT,
	YOU_MISSED_YOUR_TARGET_EVENT,
	HIT_BY_RIFLE_ATTACK,
	HIT_BY_SPRAY_ATTACK,
	NO_PRIMARY_WIELDED_EVENT,
	OUT_OF_AMMO_EVENT,
	COOLDOWN_IN_EFFECT_EVENT,
	COULDNT_FIND_TARGET_EVENT,
	TARGET_IN_PEACEFUL_ROOM_EVENT,
	YOU_INJURED_SOMEONE_EVENT,
};
#include <functional>
using uuid_t = uint64_t;
using damage_event_callback_t = std::function<void(damage_event_t,uuid_t)>;
struct feedback_t {
	damage_event_t damage_event;
	int damage;
	int hits;
	std::vector<uuid_t> injured;
	std::vector<std::tuple<uuid_t,int,uint32_t>> damage_info;
};

#endif
