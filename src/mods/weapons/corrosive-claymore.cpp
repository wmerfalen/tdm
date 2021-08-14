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
		auto shield = victim->equipment(WEAR_SHIELD);
		auto e = mods::weapons::damage_calculator::calculate_explosive_damage(victim,item);
		if(e.injured) {
			mods::injure::injure_player(victim);
			msg::youre_injured(victim);
		}
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
		auto attacker = ptr_by_uuid(item->get_owner());

		mods::weapons::damage_types::deal_hp_damage(victim,damage);
		mods::weapons::elemental::perform_elemental_damage(attacker,victim,damage,ELEM_CORROSIVE);
		mods::weapons::damage_types::update_and_process_death(attacker,victim);
		if(victim->position() != POS_DEAD) {
			auto ticks = CORROSIVE_CLAYMORE_CORRODE_TICKS();
			mods::affects::affect_player_for({mods::affects::affect_t::CORRODE},victim,ticks);
			m_debug("calling queue corrode player");
			mods::corrosive::queue_corrode_player(item,victim,damage);
			auto goggles = victim->equipment(WEAR_GOGGLES);
			auto head = victim->equipment(WEAR_HEAD);
			float tick_reduce = 0.0;
			int blind_ticks = ticks;
			if(goggles && goggles->has_armor()) {
				if(goggles->armor()->attributes->classification.compare("BASIC") == 0) {
					tick_reduce = 0.05;
				}
				if(goggles->armor()->attributes->classification.compare("ADVANCED") == 0) {
					tick_reduce = 0.75;
				}
				if(goggles->armor()->attributes->classification.compare("ELITE") == 0) {
					tick_reduce = 0.98;
				}
				switch((mods::yaml::durability_profile_type_t)goggles->armor()->attributes->durability_profile_enum) {
					case mods::yaml::durability_profile_type_t::FLIMSY:
						tick_reduce += 0.01;
						break;
					case mods::yaml::durability_profile_type_t::DECENT:
						tick_reduce += 0.05;
						break;
					case mods::yaml::durability_profile_type_t::DURABLE:
						tick_reduce += 0.15;
						break;
					case mods::yaml::durability_profile_type_t::HARDENED:
						tick_reduce += 0.23;
						break;
					case mods::yaml::durability_profile_type_t::INDUSTRIAL_STRENGTH:
						tick_reduce += 0.75;
						break;
					case mods::yaml::durability_profile_type_t::GODLIKE:
						tick_reduce += 0.87;
						break;
					case mods::yaml::durability_profile_type_t::INDESTRUCTIBLE:
						tick_reduce += 0.98;
						break;
					default:
						break;
				}
				switch((mw_armor)goggles->armor()->attributes->type) {
					case mw_armor::EYEWEAR:
					case mw_armor::GOGGLES:
						tick_reduce += 0.95;
						break;
					default:
						break;
				}
			}
			if(head && head->has_armor() && blind_ticks > 0) {
				if(head->armor()->attributes->classification.compare("BASIC") == 0) {
					tick_reduce += 0.05;
				}
				if(head->armor()->attributes->classification.compare("ADVANCED") == 0) {
					tick_reduce += 0.75;
				}
				if(head->armor()->attributes->classification.compare("ELITE") == 0) {
					tick_reduce += 0.98;
				}
				switch(head->armor()->attributes->durability_profile_enum) {
					case mods::yaml::durability_profile_type_t::FLIMSY:
						tick_reduce += 0.01;
						break;
					case mods::yaml::durability_profile_type_t::DECENT:
						tick_reduce += 0.05;
						break;
					case mods::yaml::durability_profile_type_t::DURABLE:
						tick_reduce += 0.15;
						break;
					case mods::yaml::durability_profile_type_t::HARDENED:
						tick_reduce += 0.23;
						break;
					case mods::yaml::durability_profile_type_t::INDUSTRIAL_STRENGTH:
						tick_reduce += 0.75;
						break;
					case mods::yaml::durability_profile_type_t::GODLIKE:
						tick_reduce += 0.87;
						break;
					case mods::yaml::durability_profile_type_t::INDESTRUCTIBLE:
						tick_reduce += 0.98;
						break;
					default:
						break;
				}
				switch((mw_armor)head->armor()->attributes->type) {
					case mw_armor::HELMET:
						tick_reduce += 0.85;
						break;
					case mw_armor::MASK:
						tick_reduce += head->armor()->attributes->balistic_resistance_percent * 0.01;
						break;
					default:
					case mw_armor::HAT:
						break;
				}
			}
			blind_ticks = ticks - (tick_reduce * ticks);
			if(blind_ticks > 0) {
				mods::affects::affect_player_for({mods::affects::affect_t::BLIND},victim,ticks);
			}
		}
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
