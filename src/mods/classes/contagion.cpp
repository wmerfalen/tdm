#include "contagion.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	contagion::contagion() {
		init();
	}

	void contagion::init() {
		using skillset_t = ability_data_t::skillset_t;
		m_abilities = {
			{PATHOGEN_AMMUNITION,"patho","PATHOGEN_AMMUNITION",skillset_t::INTELLIGENCE,&m_pathogen_ammunition},
			{GRIM_AURA,"aura","Grim Aura",skillset_t::INTELLIGENCE,&m_grim_aura},
			{MELT,"melt","Melt",skillset_t::INTELLIGENCE,&m_melt},
			{SUFFOCATE,"suff","SUFFOCATE",skillset_t::INTELLIGENCE,&m_suffocate},
			{SHREDDED_CANTRIP,"shred","SHREDDED_CANTRIP",skillset_t::INTELLIGENCE,&m_shredded_cantrip},
			{MUSCLE_MEMORY,"mm","MUSCLE_MEMORY",skillset_t::INTELLIGENCE,&m_muscle_memory},
			{HELLFIRE_CIRCLE,"hfc","HELLFIRE_CIRCLE",skillset_t::INTELLIGENCE,&m_hellfire_circle},
			{PARTICLE_DECELERATION,"pardec","PARTICLE_DECELERATION",skillset_t::INTELLIGENCE,&m_particle_deceleration},
			{GHASTLY_DOUBLE,"gd","GHASTLY_DOUBLE",skillset_t::INTELLIGENCE,&m_ghastly_double},
			{MINOR_SHIELDING,"ms","MINOR_SHIELDING",skillset_t::INTELLIGENCE,&m_minor_shielding},
			{CURSED_BALLISTICS,"curb","CURSED_BALLISTICS",skillset_t::INTELLIGENCE,&m_cursed_ballistics},
			{NEUTRON_SHIELD,"ns","NEUTRON_SHIELD",skillset_t::INTELLIGENCE,&m_neutron_shield},
			{BLADED_ARRAY,"ba","BLADED_ARRAY",skillset_t::INTELLIGENCE,&m_bladed_array},
			{ROOTS_OF_MAYHEM,"rom","ROOTS_OF_MAYHEM",skillset_t::INTELLIGENCE,&m_roots_of_mayhem},
			{MORBID_DOUBT,"mord","MORBID_DOUBT",skillset_t::INTELLIGENCE,&m_morbid_doubt},
			{INTIMIDATE,"int","INTIMIDATE",skillset_t::INTELLIGENCE,&m_intimidate},
			{FORCE_OUT,"force","FORCE_OUT",skillset_t::INTELLIGENCE,&m_force_out},
			{LEECH,"leech","LEECH",skillset_t::INTELLIGENCE,&m_leech},
			{PARASITIC_CORPSE_TAP,"pctap","PARASITIC_CORPSE_TAP",skillset_t::INTELLIGENCE,&m_parasitic_corpse_tap},
			{EXTRACT_ORGANS,"extr","EXTRACT_ORGANS",skillset_t::INTELLIGENCE,&m_extract_organs},
			{RECRUIT,"rec","RECRUIT",skillset_t::INTELLIGENCE,&m_recruit},
			{DEMONIC_INCANTATION,"demi","DEMONIC_INCANTATION",skillset_t::INTELLIGENCE,&m_demonic_incantation},
			{SHADOW_SIGHT,"ss","SHADOW_SIGHT",skillset_t::INTELLIGENCE,&m_shadow_sight},
			{MORBID_INSIGHT,"mi","MORBID_INSIGHT",skillset_t::INTELLIGENCE,&m_morbid_insight},
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
	void contagion::replenish_notify(std::string_view v) {
		m_player->sendln(v.data());
	}
	int16_t contagion::save() {
		return m_orm.save();
	}
	std::shared_ptr<contagion> create_contagion(player_ptr_t& in_player) {
		return std::move(std::make_shared<contagion>(in_player));
	}
	/** Contagion class abilities */
	std::pair<int16_t,std::string> contagion::cast_corpse_explosion(obj_ptr_t& corpse) {
		if(m_corpse_explosion.not_learned()) {
			return {0,"It looks like you still need to train that skill"};
		}
		auto s = roll_skill_success(CORPSE_EXPLOSION);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		uint16_t damage = 0;
		if(m_corpse_explosion.awful() || m_corpse_explosion.terrible() || m_corpse_explosion.okay()) {
			damage = dice(10, 28) + 1 + (m_player->level() / 4);
		}
		if(m_corpse_explosion.learned()) {
			damage = dice(20, 28) + 3 + (m_player->level() / 4);
		}

		if(m_corpse_explosion.mastered() || m_corpse_explosion.elite()) {
			damage = 100 + dice(30, 28);
		}
		m_player->sendln(CAT("Damage: ",damage));
		return {1,"You rig a corpse to explode!"};
	}
};
