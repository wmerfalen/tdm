#include "adrenaline-shot.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../../globals.hpp"
#include "../interpreter.hpp"
#include "../classes/ghost.hpp"

#ifdef __MENTOC_SHOW_ADRENALINE_SHOT_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[adrenaline-shot]:") << a << "\n";
#else
#define m_debug(a)
#endif

namespace mods::boosters {
	std::tuple<bool,std::string> adrenaline_shot::inject(player_ptr_t& injector) {
		m_active = true;
		mods::globals::defer_queue->push_ticks_event(m_ticks, injector->uuid(),m_deferred_event);
		injector->move() += m_movement_boost;
		injector->add_damage_nerf(m_damage_nerf);
		return {1,"You inject an adrenaline shot directly into your arm..."};
	}
	const bool& adrenaline_shot::active() const {
		return m_active;
	}
	void adrenaline_shot::shot_wears_off(player_ptr_t& injector) {
		injector->add_damage_nerf(- m_damage_nerf);
		m_active = false;
		injector->sendln("The affects of your adrenaline shot wear off...");
	}
	adrenaline_shot::adrenaline_shot() {
		m_debug("adrenaline_shot default constructor");
		set_level(adrenaline_shot::level_t::ADSHOT_SMALL);
	}

	adrenaline_shot::adrenaline_shot(const adrenaline_shot::level_t level) {
		set_level(level);
		m_active = false;
	}

	void adrenaline_shot::set_level(const adrenaline_shot::level_t level) {
		m_level = level;
		switch(m_level) {
			default:
			case adrenaline_shot::level_t::ADSHOT_SMALL:
				m_movement_boost = adrenaline_shot::movement_boost_t::MOVBOOST_SMALL;
				m_damage_nerf = adrenaline_shot::damage_nerf_t::DAMNERF_SMALL;
				m_ticks = adrenaline_shot::damage_ticks_duration_t::DTD_SMALL;
				m_deferred_event = adrenaline_shot::deferred_event_t::DEF_EVENT_SMALL;
				break;
			case adrenaline_shot::level_t::ADSHOT_MEDIUM:
				m_movement_boost = adrenaline_shot::movement_boost_t::MOVBOOST_MEDIUM;
				m_damage_nerf = adrenaline_shot::damage_nerf_t::DAMNERF_MEDIUM;
				m_ticks = adrenaline_shot::damage_ticks_duration_t::DTD_MEDIUM;
				m_deferred_event = adrenaline_shot::deferred_event_t::DEF_EVENT_MEDIUM;
				break;
			case adrenaline_shot::level_t::ADSHOT_LARGE:
				m_movement_boost = adrenaline_shot::movement_boost_t::MOVBOOST_LARGE;
				m_damage_nerf = adrenaline_shot::damage_nerf_t::DAMNERF_LARGE;
				m_ticks = adrenaline_shot::damage_ticks_duration_t::DTD_LARGE;
				m_deferred_event = adrenaline_shot::deferred_event_t::DEF_EVENT_LARGE;
				break;
		}
	}
	adrenaline_shot::level_t adrenaline_shot::get_level() const {
		return m_level;
	}
	namespace adrenaline_shot_init {
		SUPERCMD(do_adshot_test) {
			ADMIN_REJECT();
			DO_HELP("adshot_test");
			if(!player->ghost()) {
				player->sendln("You cannot inject an adrenaline shot!");
				return;
			}
			player->sendln(std::get<1>(player->ghost()->inject_adrenaline_shot()));
			ADMIN_DONE();
		}
		void init() {
			mods::interpreter::add_command("ads", POS_RESTING, do_adshot_test, LVL_BUILDER,0);
		}
	};

};
#undef m_debug
