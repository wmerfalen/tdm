#include "contagion.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	contagion::contagion() {
		init();
	}

	void contagion::init() {
		using skillset_t = ability_data_t::skillset_t;
		m_abilities = {
			{LIFE_TAP,"tap","Life Tap",skillset_t::INTELLIGENCE,&m_life_tap},
			{CORPSE_EXPLOSION,"ce","Corpse Explosion",skillset_t::DEMOLITIONS,&m_corpse_explosion},
			{SHRAPNEL_CORPSE_EXPLOSION,"sce","Shrapnel Corpse Explosion",skillset_t::DEMOLITIONS,&m_shrapnel_corpse_explosion},
			{CORROSIVE_CORPSE_EXPLOSION,"cce","Corrosive Corpse Explosion",skillset_t::DEMOLITIONS,&m_corrosive_corpse_explosion},
			{HELLFIRE_CORPSE_EXPLOSION,"hce","Hellfire Corpse Explosion",skillset_t::DEMOLITIONS,&m_hellfire_corpse_explosion},
			{ATTRACT,"at","Attract",skillset_t::STRATEGY,&m_attract},
			{CONFUSE,"cf","Confuse",skillset_t::STRATEGY,&m_confuse},
			{HELLFIRE_INCANTATION,"hell","Hellfire Incantation",skillset_t::DEMOLITIONS,&m_hellfire_incantation},
			{DETONATE_LIMB,"det","Detonate Limb",skillset_t::DEMOLITIONS,&m_detonate_limb},
		};
	}
	void contagion::set_player(player_ptr_t p) {
		m_player = p;
	}
	contagion::contagion(player_ptr_t p) {
		load_by_player(p);
	}

	int16_t contagion::new_player(player_ptr_t& player) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t contagion::load_by_player(player_ptr_t& player) {
		set_player(player);
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("contagion::load_by_player unable to load contagion class by player id: ",(player->db_id()),".. return status: ",(result)));
			return result;
		}
		initialize_skills_for_player(m_player);
		obj_ptr_t primary = nullptr;
		return result;
	}
	int16_t contagion::save() {
		return m_orm.save();
	}
	std::shared_ptr<contagion> create_contagion(player_ptr_t& in_player) {
		return std::move(std::make_shared<contagion>(in_player));
	}
};
