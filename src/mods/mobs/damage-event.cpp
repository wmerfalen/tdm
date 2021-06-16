#include "damage-event.hpp"
#include "../util.hpp"
#include "mini-gunner.hpp"
#include "lowly-security.hpp"
#include "mp-shotgunner.hpp"
#include "../behaviour_tree_impl.hpp"
#include "extended-types.hpp"

namespace mods::mobs::damage_event {
	using extended_types_t = mods::mobs::extended_types_t;
	void sniped(player_ptr_t& mob,const feedback_t& feedback) {
		auto ch = mob->cd();
		switch(ch->mob_specials.extended_mob_type) {
			default:
			case extended_types_t::NONE:
				std::cerr << "[extended mob type]: no extended mob type for mob: ('" << mob->name().c_str() << "')\n";
				break;
			case extended_types_t::MP_SHOTGUNNER:
				std::cerr << "[ found mp shotgunner ]\n";
				{
					auto mp = mp_shotgunner_ptr(mob->uuid());
					mp->set_behaviour_tree("mp_shotgunner_engage");
					mp->attacked(feedback);
				}
				break;
			case extended_types_t::LOWLY_SECURITY:
				std::cerr << "[ found lowly security guard ]\n";
				//lowly_security::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER:
				std::cerr << "[ found mini gunner ]\n";
				//mini_gunner::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER_SENTINEL:
				std::cerr << "[ found mini gunner sentinel ]\n";
				//mini_gunner::create(mob_id, "sentinel");
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
				std::cerr << "[extended mob type]: no extended mob type for mob: ('" << ch->player.name.c_str() << "')\n";
				break;
			/**! @NEW_BEHAVIOUR_TREE@ !**/
			case extended_types_t::MP_SHOTGUNNER:
				std::cerr << "[ found mp shotgunner ]\n";
				//mp_shotgunner::create(mob_id, "normal");
				break;
			case extended_types_t::LOWLY_SECURITY:
				std::cerr << "[ found lowly security guard ]\n";
				//lowly_security::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER:
				std::cerr << "[ found mini gunner ]\n";
				//mini_gunner::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER_SENTINEL:
				std::cerr << "[ found mini gunner sentinel ]\n";
				//mini_gunner::create(mob_id, "sentinel");
				break;
		}
		//mods::behaviour_tree_impl::register_mob(ch->uuid);
	}
};

