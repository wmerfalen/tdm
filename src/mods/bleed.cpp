#include "bleed.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

#ifdef __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::bleed::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::bleed::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::bleed {
	static uint32_t bleed_id = 0;
	struct bleed_damage_t {
		uint32_t id;
		uuid_t attacker;
		uuid_t target;
		int16_t damage;
		bool cleanup;
		inline bool operator==(const bleed_damage_t& lhs) {
			if(lhs.id == id) {
				return true;
			}
			return false;
		}
		std::string report() {
			return CAT("id:",id,"\nattacker:",attacker,"\ntarget:",target,"\ndamage:",damage,"\n");
		}
		bleed_damage_t() = delete;
		bleed_damage_t(const uuid_t& atk,
		               const uuid_t& t,
		               const int16_t& d
		              )
			: id(++bleed_id),
			  attacker(atk), target(t),damage(d),cleanup(false) {
		}
	};
	using bleed_list_t = std::forward_list<bleed_damage_t>;

	static bleed_list_t& bleed_player_list() {
		static bleed_list_t list;
		return list;
	}

	void cause_glass_damage_to_feet(player_ptr_t victim, int damage) {
		auto chance_to_hurt = 100;
		auto boots = victim->equipment(WEAR_FEET);
		auto ticks = 0;
		if(boots) {
			if(mods::armor::is_basic_protection(boots)) {
				chance_to_hurt = BASIC_BOOTS_DAMAGE_PERCENT();
				ticks = BASIC_BOOTS_TICKS_DURATION();
			}
			if(mods::armor::is_advanced_protection(boots)) {
				chance_to_hurt = ADVANCED_BOOTS_DAMAGE_PERCENT();
				ticks = ADVANCED_BOOTS_TICKS_DURATION();
			}
			if(mods::armor::is_elite_protection(boots)) {
				chance_to_hurt = ELITE_BOOTS_DAMAGE_PERCENT();
				ticks = ELITE_BOOTS_TICKS_DURATION();
			}
		}
		if(mods::rand::chance(chance_to_hurt)) {
			mods::affects::affect_player_for({mods::affects::affect_t::BLEED},victim,ticks);
		}
	}

	void bleed_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item,int damage) {
		//auto shield = victim->equipment(WEAR_SHIELD);
		//mods::weapons::damage_types::deal_hp_damage(victim,damage);
		//mods::weapons::elemental::perform_elemental_damage(attacker,victim,damage,ELEM_CORROSIVE);
		//mods::weapons::damage_types::update_and_process_death(attacker,victim);
		auto ticks = DEFAULT_BLEED_TICKS();
		/** Legs and boots are the main culprits for bleeding */
		auto boots = victim->equipment(WEAR_FEET);
		auto leg_armor = victim->equipment(WEAR_LEGS);
		float tick_reduce = 0.0;
		if(victim->position() != POS_DEAD) {
			m_debug("Victim is not dead, so doing bleed logic");
			if(leg_armor && leg_armor->has_armor()) {
				switch((mods::yaml::durability_profile_type_t)leg_armor->armor()->attributes->durability_profile_enum) {
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
			}
		}
		if(boots && boots->has_armor()) {
			if(boots->armor()->attributes->classification.compare("BASIC") == 0) {
				tick_reduce += 0.05;
			}
			if(boots->armor()->attributes->classification.compare("ADVANCED") == 0) {
				tick_reduce += 0.75;
			}
			if(boots->armor()->attributes->classification.compare("ELITE") == 0) {
				tick_reduce += 0.98;
			}
			switch(boots->armor()->attributes->durability_profile_enum) {
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
		}
		m_debug("ticks now:" << ticks);
		ticks = ticks - (tick_reduce * ticks);
		m_debug("After calculation:" << ticks);
		if(ticks <= 0) {
			m_debug("ticks is less than zero! not bleeding!");
		} else {
			m_debug("ticks is positive number (after calculation):" << ticks);
			mods::affects::affect_player_for({mods::affects::affect_t::BLEED},victim,ticks);
			m_debug("calling queue bleed player");
			mods::bleed::queue_bleed_player(item,victim,damage);
		}

		/**
		 * bleed damage against a player has the following effects:
		 * 	- melee attackers take 2 turns to move instead of 1
		 * 	- chance of melee attackers to drop their weapon
		 * 	- ranged attacks do N percent less damage
		 *  - continuous damage over N seconds
		 *  - blindness over N seconds
		 */
	}
	void process_players() {
		std::vector<bleed_damage_t> player_removals;
		for(const auto& entry : bleed_player_list()) {
			if(entry.cleanup) {
				m_debug("cleanup requested for entry: " << entry.id);
				player_removals.emplace_back(entry);
				continue;
			}
			m_debug("victim: " << entry.target);
			auto victim = ptr_by_uuid(entry.target);
			if(!victim || victim->position() == POS_DEAD || !victim->get_affect_dissolver().has_affect(mods::affects::affect_t::BLEED)) {
				player_removals.emplace_back(entry);
				continue;
			}
			/*
			auto percent = victim->bleed_resistance_percent();
			auto damage = mods::rand::roll(entry.dice_count,entry.dice_sides);
			int dam = damage - ((percent * 0.01) * damage);
			*/
			//auto dam = mods::rand::roll(entry.dice_count,entry.dice_sides);
			auto dam = entry.damage;
			auto attacker = ptr_by_uuid(entry.attacker);
			if(attacker) {
				attacker->sendln(CAT("{grn}[+",dam,"] bleed damage to ",victim->name(),"{/grn}"));
			}
			victim->sendln(CAT("{grn}[-",dam,"] bleed damage.{/grn}"));
			victim->hp() -= dam;
			mods::weapons::damage_types::update_and_process_death(attacker,victim);
		}
		for(const auto& entry : player_removals) {
			bleed_player_list().remove(entry);
		}
	}
	void process_bleed() {
		m_debug("process_bleed");
		process_players();
	}
	void bleed_clears_up(player_ptr_t victim) {
		return;
	}
	void queue_bleed_player(obj_ptr_t device,player_ptr_t& victim,const int16_t& damage) {
		bleed_player_list().emplace_front(device->get_owner(),
		                                  victim->uuid(),
		                                  damage
		                                 );
	}
	void unqueue_player(const uuid_t& player_uuid) {
		for(auto& entry : bleed_player_list()) {
			if(entry.target == player_uuid) {
				entry.cleanup = true;
			}
		}
	}

#undef m_error
#undef m_debug
};
