#include "melt.hpp"
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
#include "weapons/shotgun-dst7A.hpp"

#define __MENTOC_SHOW_MODS_MELT_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_MELT_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::melt::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::melt::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::melt {
	inline bool melt_damage_t::operator==(const melt_damage_t& lhs) {
		if(lhs.id == id) {
			return true;
		}
		return false;
	}
	std::string melt_damage_t::report() {
		return CAT("id:",id,"\nattacker:",attacker,"\ntarget:",target,"\ndamage:",damage,"\n");
	}
	using melt_list_t = std::forward_list<melt_damage_t>;

	static melt_list_t& melt_player_list() {
		static melt_list_t list;
		return list;
	}

	void melt_damage(
	    melt_damage_t melt
	) {
		auto attacker = ptr_by_uuid(melt.attacker);
		auto victim = ptr_by_uuid(melt.target);
		if(!attacker || !victim) {
			return;
		}
		auto ticks = DEFAULT_MELT_TICKS();
		/** Legs and boots are the main culprits for melting */
		auto goggles = victim->equipment(WEAR_GOGGLES);
		auto head_armor = victim->equipment(WEAR_HEAD);
		float tick_reduce = 0.0;
		if(victim->position() != POS_DEAD) {
			m_debug("Victim is not dead, so doing melt logic");
			if(head_armor && head_armor->has_armor()) {
				switch((mods::yaml::durability_profile_type_t)head_armor->armor()->attributes->durability_profile_enum) {
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
		if(goggles && goggles->has_armor()) {
			if(goggles->armor()->attributes->classification.compare("BASIC") == 0) {
				tick_reduce += 0.02;
			}
			if(goggles->armor()->attributes->classification.compare("ADVANCED") == 0) {
				tick_reduce += 0.10;
			}
			if(goggles->armor()->attributes->classification.compare("ELITE") == 0) {
				tick_reduce += 0.20;
			}
			switch(goggles->armor()->attributes->durability_profile_enum) {
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
			m_debug("ticks is less than zero! not melting!");
		} else {
			m_debug("ticks is positive number (after calculation):" << ticks);
			mods::affects::affect_player_for({mods::affects::affect_t::MELT},victim,ticks);
			m_debug("calling queue melt player");
			melt_player_list().emplace_front(melt);
		}

		/**
		 * melt damage against a player has the following effects:
		 * 	- melee attackers take 2 turns to move instead of 1
		 * 	- chance of melee attackers to drop their weapon
		 * 	- ranged attacks do N percent less damage
		 *  - continuous damage over N seconds
		 *  - blindness over N seconds
		 */
	}
	void process_players() {
		std::vector<melt_damage_t> player_removals;
		for(const auto& entry : melt_player_list()) {
			if(entry.cleanup) {
				m_debug("cleanup requested for entry: " << entry.id);
				player_removals.emplace_back(entry);
				continue;
			}
			m_debug("victim: " << entry.target);
			auto victim = ptr_by_uuid(entry.target);
			if(!victim || victim->position() == POS_DEAD || !victim->get_affect_dissolver().has_affect(mods::affects::affect_t::MELT)) {
				player_removals.emplace_back(entry);
				continue;
			}

			auto attacker = ptr_by_uuid(entry.attacker);
			/**
			 * Melt damage consists of:
			 * - radioactive damage
			 * - incendiary damage
			 */
			auto dam = entry.damage;
			dam += dice(entry.dice);
			if(attacker) {
				attacker->sendln(CAT("{grn}[+",dam,"] melt damage to ",victim->name(),"{/grn}"));
			}
			mods::weapons::elemental::incendiary_damage(
			    attacker,
			    victim,
			    dice(entry.dice)
			);
			mods::weapons::elemental::radioactive_damage(
			    attacker,
			    victim,
			    dice(entry.dice)
			);
			act("$n screams in AGONY as parts of $s skin melts off the bone!!! ", false, victim->cd(), 0, 0, TO_ROOM);

			victim->sendln(CAT("{red}[-",dam,"] melt damage.{/red}"));
			victim->hp() -= dam;
			mods::weapons::damage_types::update_and_process_death(attacker,victim);
		}
		for(const auto& entry : player_removals) {
			melt_player_list().remove(entry);
		}
	}
	void process_melt() {
#ifdef __MENTOC_SHOW_PROCESS_MELT_ENTRY__
		m_debug("process_melt");
#endif
		process_players();
	}
	void melt_clears_up(player_ptr_t victim) {
		return;
	}
	void unqueue_player(const uuid_t& player_uuid) {
		for(auto& entry : melt_player_list()) {
			if(entry.target == player_uuid) {
				entry.cleanup = true;
			}
		}
	}
	ACMD(do_melt_me) {
		player->sendln("Creating claymore");
		melt_damage({
			player->uuid(),
			player->uuid(),
			mods::weapons::shotgun::dst7A::stats::base_damage_dice_additional,
			mods::weapons::shotgun::dst7A::stats::base_damage
		});
	}
	void init() {
		mods::interpreter::add_command("melt_me", POS_RESTING, do_melt_me, 0, 0);

	}

#undef m_error
#undef m_debug
};
