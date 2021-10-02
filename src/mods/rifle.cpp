#include "rifle.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"
#include "levels.hpp"
#include "calc-visibility.hpp"

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

#define __MENTOC_SHOW_MODS_RIFLE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_RIFLE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::rifle::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::rifle::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif

namespace mods::rifle {
	bool is_rifle_attachment(obj_data* obj) {
		if(!obj->has_rifle()) {
			return false;
		}
		return !!mods::rifle_attachments::by_uuid(obj->uuid);
	}
	bool is_rifle(const obj_ptr_t& weapon) {
		return weapon->has_rifle();
	}
	float get_zoom_magnification(obj_ptr_t& weapon) {
		return 0;
	}
	int16_t get_max_range(obj_ptr_t& weapon) {
		return 0;
	}
	int16_t get_critical_range(obj_ptr_t& weapon) {
		return 0;
	}
	float get_range_multiplier(obj_ptr_t& weapon) {
		return 0;
	}
	int16_t get_effective_firing_range(obj_ptr_t& weapon) {
		return 0;
	}

	ACMD(do_rifle_me) {
		player->sendln("Creating rifle");
		player->sendln("There you go");
	}
	void init() {
		mods::interpreter::add_user_command("rifle_me", do_rifle_me);

	}

#undef m_error
#undef m_debug
};
