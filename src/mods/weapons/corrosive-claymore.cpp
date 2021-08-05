#include "damage-calculator.hpp"
#include "elemental.hpp"
#include "damage-types.hpp"
#include "corrosive-claymore.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../../globals.hpp"
#include "../projectile.hpp"
#include "../player.hpp"
#include "../demolitions.hpp"
#include "../injure.hpp"

#ifdef __MENTOC_SHOW_CORROSIVE_CLAYMORE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[corrosive-claymore]:") << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::weapons {
	std::forward_list<std::shared_ptr<corrosive_claymore>>& corrosive_claymore_list() {
		static std::forward_list<std::shared_ptr<corrosive_claymore>> c;
		return c;
	}
	void corrosive_claymore_exploded(const uuid_t& obj_uuid) {
		for(const auto& claymore : corrosive_claymore_list()) {
			if(claymore->uuid() == obj_uuid) {
				claymore->exploded();
				corrosive_claymore_list().remove(claymore);
				return;
			}
		}
	}
	void corrosive_claymore_installed(const uuid_t& obj_uuid) {
		for(const auto& claymore : corrosive_claymore_list()) {
			if(claymore->uuid() == obj_uuid) {
				claymore->finished_installing();
				return;
			}
		}
	}
	int corrosive_claymore_explode(player_ptr_t& victim,obj_ptr_t& item) {
		auto e = mods::weapons::damage_calculator::calculate_explosive_damage(victim,item);
		if(e.injured) {
			mods::injure::injure_player(victim);
			msg::youre_injured(victim);
		}
		/** TODO handle critical range attribute */
		/** TODO handle blast radius attribute */
		/** TODO handle loudness type */
		int damage = e.damage;

		if(e.critical) {
			damage += e.critical;
			victim->sendln("{red}*** [CRITICAL] ***{/red} -- ");
		}

		if(e.chemical) {
			damage += e.chemical;
			victim->sendln("A chemical weapons explosion causes you to take damage!");
		}
		if(e.fire) {
			damage += e.fire;
			victim->sendln("An incendiary explosion causes you to take damage!");
		}
		if(e.radiation) {
			damage += e.radiation;
			victim->sendln("A radiation explosion causes you to take damage!");
		}
		if(e.electric) {
			damage += e.electric;
			victim->sendln("An electric explosion causes you to take damage!");
		}
		if(e.armor_pen) {
			damage += e.armor_pen;
			victim->sendln("The explosion shreds through your armor!");
		}

		mods::weapons::damage_types::deal_hp_damage(victim,damage);
		mods::weapons::elemental::perform_elemental_damage(nullptr,victim,damage,ELEM_CORROSIVE);
		return damage;
	}

	obj_ptr_t corrosive_claymore::create() {
		corrosive_claymore_list().emplace_front();
		auto& ref = corrosive_claymore_list().front();
		return ref->obj();
	}


	std::tuple<bool,std::string> corrosive_claymore::install(player_ptr_t& attacker,const room_rnum& room,const direction_t& direction) {
		mods::demolitions::plant_claymore(attacker,direction,m_obj);
		return {true,"You begin installing a {grn}corrosive{/grn} claymore mine..."};
	}

	obj_ptr_t& corrosive_claymore::obj() {
		return m_obj;
	}

	void corrosive_claymore::exploded() {

	}
	void corrosive_claymore::finished_installing() {

	}

	corrosive_claymore::corrosive_claymore() {
		m_obj = nullptr;
		m_claymore_room = {0,0};
		m_installed = 0;
		m_installer = 0;
		m_uuid = 0;
		m_installer = 0;
		m_obj = create_object(ITEM_EXPLOSIVE,"corrosive-claymore-mine.yml");
	}

};
#undef m_debug
