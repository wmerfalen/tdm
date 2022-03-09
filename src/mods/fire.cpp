#include "fire.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"
#include "levels.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"
#include "projectile.hpp"
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

#define __MENTOC_SHOW_MODS_FIRE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_FIRE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::fire::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::fire::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif

extern void object_list_new_owner(struct obj_data *list, char_data *ch);
extern void	obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
extern void obj_from_room(obj_ptr_t in_object);

namespace mods::fire {
	static constexpr uint8_t FIRE_HITS_MINIMUM = 3;
	static constexpr uint8_t FIRE_HITS_MAXIMUM = 9;
	bool deploy_fire_damage_to_victim_via_device(player_ptr_t& victim,obj_ptr_t& device) {
		feedback_t f;
		f.from_direction = NORTH;
		f.hits = rand_number(FIRE_HITS_MINIMUM,FIRE_HITS_MAXIMUM);
		f.damage = roll(device->explosive());
		f.attacker = device->get_owner();
		f.damage_event = HIT_BY_INCENDIARY_DAMAGE;
		victim->damage_event(f);

		update_pos(victim);

		f.damage_event = YOU_INFLICTED_INCENDIARY_DAMAGE;
		auto attacker = ptr_by_uuid(f.attacker);
		if(attacker) {
			attacker->damage_event(f);
		}

		return victim->is_dead();
	}
	void set_fire_to_room_via_device(const room_rnum& room,obj_ptr_t& device) {
		m_debug("Setting fire to room(" << room << ") via device(" << device->name.str() << ")");
		mods::rooms::start_fire_dissolver(room);
	}

	void init() {

	}

#undef m_error
#undef m_debug
};
