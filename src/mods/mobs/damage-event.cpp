#include "damage-event.hpp"
#include "../util.hpp"
#include "mini-gunner.hpp"
#include "lowly-security.hpp"
#include "mp-shotgunner.hpp"
#include "car-thief.hpp"
#include "../behaviour_tree_impl.hpp"
#include "extended-types.hpp"

#ifdef __MENTOC_DAMAGE_EVENTS_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::weapons::damage_event][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

namespace mods::mobs::damage_event {
	using extended_types_t = mods::mobs::extended_types_t;
	void sniped(player_ptr_t& mob,const feedback_t& feedback) {
		auto ch = mob->cd();
		switch(ch->mob_specials.extended_mob_type) {
			default:
			case extended_types_t::NONE:
				dty_debug("no extended mob type for mob: ('" << mob->name().c_str() << "')");
				break;
			case extended_types_t::MP_SHOTGUNNER:
				dty_debug("[ found mp shotgunner ]");
				{
					auto mp = mp_shotgunner_ptr(mob->uuid());
					mp->set_behaviour_tree("mp_shotgunner_engage");
					mp->attacked(feedback);
				}
				break;
			case extended_types_t::LOWLY_SECURITY:
				dty_debug("[ found lowly security guard ]");
				//lowly_security::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER:
				dty_debug("[ found mini gunner ]");
				//mini_gunner::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER_SENTINEL:
				dty_debug("[ found mini gunner sentinel ]");
				//mini_gunner::create(mob_id, "sentinel");
				break;
			case extended_types_t::CAR_THIEF:
				dty_debug("[ found car thief ]");
				{
					auto ct = car_thief_ptr(mob->uuid());
					ct->set_behaviour_tree("car_thief_hostile");
					ct->attacked(feedback);
				}
				break;
		}
		//mods::behaviour_tree_impl::register_mob(ch->uuid);
	}
	void received_elemental_damage(player_ptr_t& mob,const feedback_t& feedback) {

	}
	void disoriented(player_ptr_t& mob,const feedback_t& feedback) {

	}
	void injured(player_ptr_t& mob,const feedback_t& feedback) {

	}
	void meleed(player_ptr_t& mob,const feedback_t& feedback) {
		auto ch = mob->cd();
		switch(ch->mob_specials.extended_mob_type) {
			default:
			case extended_types_t::NONE:
				dty_debug("no extended mob type for mob: ('" << ch->player.name.c_str() << "')");
				break;
			/**! @NEW_BEHAVIOUR_TREE@ !**/
			case extended_types_t::CAR_THIEF:
				dty_debug("[ found car thief ]");
				//mp_shotgunner::create(mob_id, "normal");
				break;
			case extended_types_t::MP_SHOTGUNNER:
				dty_debug("[ found mp shotgunner ]");
				//mp_shotgunner::create(mob_id, "normal");
				break;
			case extended_types_t::LOWLY_SECURITY:
				dty_debug("[ found lowly security guard ]");
				//lowly_security::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER:
				dty_debug("[ found mini gunner ]");
				//mini_gunner::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER_SENTINEL:
				dty_debug("[ found mini gunner sentinel ]");
				//mini_gunner::create(mob_id, "sentinel");
				break;
		}
		//mods::behaviour_tree_impl::register_mob(ch->uuid);
	}
};

