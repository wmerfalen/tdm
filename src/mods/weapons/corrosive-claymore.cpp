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
#include "../rooms.hpp"

#define __MENTOC_SHOW_CORROSIVE_CLAYMORE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_CORROSIVE_CLAYMORE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[corrosive-claymore]:") << a << "\n";
#else
#define m_debug(a)
#endif
void obj_from_room(obj_ptr_t in_object);
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
				mods::globals::dispose_object(obj_uuid);
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
		auto attacker = ptr_by_uuid(item->get_owner());
		auto damage_feedback = mods::weapons::damage_calculator::calculate_explosive_damage(victim,item);
		if(damage_feedback.injured) {
			mods::injure::injure_player(victim);
			msg::youre_injured(victim);
		}
		int damage = damage_feedback.damage;

		if(damage_feedback.critical) {
			damage += damage_feedback.critical;
			victim->sendln("{red}*** [CRITICAL] ***{/red} -- ");
		}

		if(damage_feedback.chemical) {
			damage += damage_feedback.chemical;
			victim->sendln("A chemical weapons explosion causes you to take damage!");
		}
		if(damage_feedback.fire) {
			damage += damage_feedback.fire;
			victim->sendln("An incendiary explosion causes you to take damage!");
		}
		if(damage_feedback.radiation) {
			damage += damage_feedback.radiation;
			victim->sendln("A radiation explosion causes you to take damage!");
		}
		if(damage_feedback.electric) {
			damage += damage_feedback.electric;
			victim->sendln("An electric explosion causes you to take damage!");
		}
		if(damage_feedback.armor_pen) {
			damage += damage_feedback.armor_pen;
			victim->sendln("The explosion shreds through your armor!");
		}
		damage = mods::corrosive::corrode_damage(attacker,victim,item,damage);
		obj_from_room(item);
		corrosive_claymore_exploded(item->uuid);
		return damage;
	}

	void corrosive_claymore_perform_blast_radius(room_rnum room_id,obj_ptr_t device) {
		for(auto& victim : mods::globals::room_list[room_id]) {
			corrosive_claymore_explode(victim, device);
		}
	}

	obj_ptr_t corrosive_claymore::create(player_ptr_t& planter) {
		auto s = std::make_shared<corrosive_claymore>();
		s->set_planter(planter);
		corrosive_claymore_list().emplace_front(s);
		return s->obj();
	}


	std::tuple<bool,std::string> corrosive_claymore::install(player_ptr_t& attacker,const room_rnum& room,const direction_t& direction) {
		attacker->sendln("install debug");
		m_installer = attacker->uuid();
		m_claymore_room  = {room,direction};
		mods::demolitions::plant_claymore(attacker,direction,m_obj);
		return {true,"You begin installing a {grn}corrosive{/grn} claymore mine..."};
	}

	obj_ptr_t corrosive_claymore::obj() {
		return m_obj;
	}

	void corrosive_claymore::exploded() {

	}
	void corrosive_claymore::finished_installing() {
		auto installer = ptr_by_uuid(m_installer);
		if(installer) {
			mods::rooms::broadcast_claymore_plant(m_claymore_room.first,installer,m_claymore_room.second,m_obj);
		}
	}

	corrosive_claymore::corrosive_claymore() {
		m_obj = nullptr;
		m_claymore_room = {0,0};
		m_installed = 0;
		m_installer = 0;
		m_uuid = 0;
		m_installer = 0;
		m_obj = create_object(ITEM_EXPLOSIVE,"corrosive-claymore-mine.yml");
		m_uuid = m_obj->uuid;
	}

	namespace corrosive_claymore_memory {
		std::string get() {
			std::string s = CAT("corrosive_claymores: ",std::distance(corrosive_claymore_list().cbegin(),corrosive_claymore_list().cend()));
			return s;
		}
	};
};
#undef m_debug
