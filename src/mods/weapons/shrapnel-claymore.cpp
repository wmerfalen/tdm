#include "damage-calculator.hpp"
#include "damage-types.hpp"
#include "shrapnel-claymore.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../../globals.hpp"
#include "../projectile.hpp"
#include "../player.hpp"
#include "../demolitions.hpp"
#include "../injure.hpp"

#ifdef __MENTOC_SHOW_CORROSIVE_CLAYMORE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[shrapnel-claymore]:") << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::weapons {
	std::forward_list<std::shared_ptr<shrapnel_claymore>>& shrapnel_claymore_list() {
		static std::forward_list<std::shared_ptr<shrapnel_claymore>> c;
		return c;
	}
	void shrapnel_claymore_exploded(const uuid_t& obj_uuid) {
		for(const auto& claymore : shrapnel_claymore_list()) {
			if(claymore->uuid() == obj_uuid) {
				claymore->exploded();
				shrapnel_claymore_list().remove(claymore);
				return;
			}
		}
	}
	void shrapnel_claymore_installed(const uuid_t& obj_uuid) {
		for(const auto& claymore : shrapnel_claymore_list()) {
			if(claymore->uuid() == obj_uuid) {
				claymore->finished_installing();
				return;
			}
		}
	}
	int shrapnel_claymore_explode(player_ptr_t& victim,obj_ptr_t& item) {
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
		return damage;
	}

	obj_ptr_t shrapnel_claymore::create() {
		shrapnel_claymore_list().emplace_front();
		auto& ref = shrapnel_claymore_list().front();
		return ref->obj();
	}


	std::tuple<bool,std::string> shrapnel_claymore::install(player_ptr_t& attacker,const room_rnum& room,const direction_t& direction) {
		mods::demolitions::plant_claymore(attacker,direction,m_obj);
		return {true,"You begin installing an {yel}enhanced shrapnel{/yel} claymore mine..."};
	}

	obj_ptr_t& shrapnel_claymore::obj() {
		return m_obj;
	}

	void shrapnel_claymore::exploded() {

	}
	void shrapnel_claymore::finished_installing() {

	}

	shrapnel_claymore::shrapnel_claymore() {
		m_obj = nullptr;
		m_claymore_room = {0,0};
		m_installed = 0;
		m_installer = 0;
		m_uuid = 0;
		m_installer = 0;
		m_obj = create_object(ITEM_EXPLOSIVE,"shrapnel-claymore-mine.yml");
	}

};
#undef m_debug
