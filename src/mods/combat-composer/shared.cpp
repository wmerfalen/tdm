#include "shared.hpp"
#include "../rifle.hpp"
#include "../scan.hpp"
#include "../player.hpp"
#include "../ranged-combat-totals.hpp"
#include "../damage-event.hpp"
#include "../rooms.hpp"
#include "../object-utils.hpp"
#include "../player-utils.hpp"
#include "../calc-visibility.hpp"
#include "../classes/ghost.hpp"
#include "../classes/includes.hpp"
#include "../levels.hpp"

#include "../../fight.hpp"
#include "../mobs/damage-event.hpp"
#include "../weapons/elemental.hpp"
#include "../weapons/legacy-combat.hpp"
#include "../interpreter.hpp"
#include "skill-increment.hpp"
#include "shared.hpp"

namespace mods::combat_composer {
	namespace ammunition {
		uint16_t reduce_ammo(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    uint16_t wants_to_deduct
		) {
			if(weapon->has_rifle() == false) {
				return 0;/** TODO: make sure this doesnt screw something up */
			}
			uint16_t ammo_reduction = wants_to_deduct;
			auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
			if(!rifle_attachment) {
				return wants_to_deduct;
			}
			if(mods::rand::chance(rifle_attachment->free_ammo_chance)) {
				m_debug("Player rolled free ammo " << green_str("SUCCESS") << "Chances were: " << rifle_attachment->free_ammo_chance << "%");
				ammo_reduction = 0;
			}
			if(mods::rand::chance(rifle_attachment->regenerate_ammo_chance)) {
				m_debug("Player rolled regenerated ammo " << green_str("SUCCESS") << "Chances were: " << rifle_attachment->regenerate_ammo_chance << "%");
				auto regen = rand_number(
				                 mods::values::REGENERATED_AMMO_LOW(),
				                 mods::values::REGENERATED_AMMO_HIGH()
				             );
				m_debug("Random amount of regenerated ammo: " << regen);
				return ammo_reduction - regen;
			}
			return ammo_reduction;
		}
	};
	void decrease_single_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) ;
	namespace phases {
		/**
		 * @brief subtracts 1 from weapon ammo
		 *
		 * @param weapon
		 */
		void decrease_spray_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) {
			/** TODO: if weapon has a bullet printer mod, calculate ammo */
			if(weapon->rifle_instance->ammo == 0) {
				return;
			}
			int16_t deduct = 0;
			switch((mw_rifle)weapon->rifle()->attributes->type) {
				case mw_rifle::LIGHT_MACHINE_GUN:
					deduct = mods::values::SPRAY_SHOT_LIGHT_MACHINE_GUN();
					break;

				case mw_rifle::SUB_MACHINE_GUN:
					deduct = mods::values::SPRAY_SHOT_SUB_MACHINE_GUN();
					break;

				case mw_rifle::SHOTGUN:
					deduct = mods::values::SPRAY_SHOT_SHOTGUN();
					break;

				case mw_rifle::SNIPER:
					deduct = mods::values::SPRAY_SHOT_SNIPER();
					break;

				case mw_rifle::ASSAULT_RIFLE:
					deduct = mods::values::SPRAY_SHOT_ASSAULT_RIFLE();
					break;
				case mw_rifle::HANDGUN:
				case mw_rifle::PISTOL:
					deduct = mods::values::SPRAY_SHOT_HANDGUN();
					break;
				case mw_rifle::MACHINE_PISTOL:
					deduct = mods::values::SPRAY_SHOT_MACHINE_PISTOL();
					break;
				default:
					deduct = 1;
					log("SYSERR: warning, no rifle type given for decrease_single_shot_ammo, default to 1");
					break;
			}
			sub_clamp(weapon->rifle_instance->ammo,mods::combat_composer::ammunition::reduce_ammo(attacker,weapon,deduct));
		}
		/**
		 * @brief subtracts 1 from weapon ammo
		 *
		 * @param weapon
		 */
		void decrease_single_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) {
			/** TODO: if weapon has a bullet printer mod, calculate ammo */
			if(weapon->rifle_instance->ammo == 0) {
				return;
			}
			int16_t deduct = 0;
			switch((mw_rifle)weapon->rifle()->attributes->type) {
				case mw_rifle::LIGHT_MACHINE_GUN:
					deduct = mods::values::SINGLE_SHOT_LIGHT_MACHINE_GUN();
					break;

				case mw_rifle::SUB_MACHINE_GUN:
					deduct = mods::values::SINGLE_SHOT_SUB_MACHINE_GUN();
					break;

				case mw_rifle::SHOTGUN:
					deduct = mods::values::SINGLE_SHOT_SHOTGUN();
					break;

				case mw_rifle::SNIPER:
					deduct = mods::values::SINGLE_SHOT_SNIPER();
					break;

				case mw_rifle::ASSAULT_RIFLE:
					deduct = mods::values::SINGLE_SHOT_ASSAULT_RIFLE();
					break;
				case mw_rifle::HANDGUN:
				case mw_rifle::PISTOL:
					deduct = mods::values::SINGLE_SHOT_HANDGUN();
					break;
				case mw_rifle::MACHINE_PISTOL:
					deduct = mods::values::SINGLE_SHOT_MACHINE_PISTOL();
					break;
				default:
					deduct = 1;
					log("SYSERR: warning, no rifle type given for decrease_single_shot_ammo, default to 1");
					break;
			}
			int ammo = weapon->rifle_instance->ammo - ammunition::reduce_ammo(attacker,weapon,deduct);
			if(ammo < 0) {
				weapon->rifle_instance->ammo = 0;
			} else {
				weapon->rifle_instance->ammo = ammo;
			}
		}
		/**
		 * @brief subtracts 1 from weapon ammo
		 *
		 * @param weapon
		 */
		void decrease_same_room_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) {
			/** TODO: if weapon has a bullet printer mod, calculate ammo */
			if(weapon->rifle_instance->ammo == 0) {
				return;
			}
			int16_t deduct = 0;
			switch((mw_rifle)weapon->rifle()->attributes->type) {
				case mw_rifle::LIGHT_MACHINE_GUN:
					deduct = mods::values::SAME_ROOM_SHOT_LIGHT_MACHINE_GUN();
					break;

				case mw_rifle::SUB_MACHINE_GUN:
					deduct = mods::values::SAME_ROOM_SHOT_SUB_MACHINE_GUN();
					break;

				case mw_rifle::SHOTGUN:
					deduct = mods::values::SAME_ROOM_SHOT_SHOTGUN();
					break;

				case mw_rifle::SNIPER:
					deduct = mods::values::SAME_ROOM_SHOT_SNIPER();
					break;

				case mw_rifle::ASSAULT_RIFLE:
					deduct = mods::values::SAME_ROOM_SHOT_ASSAULT_RIFLE();
					break;
				case mw_rifle::HANDGUN:
				case mw_rifle::PISTOL:
					deduct = mods::values::SAME_ROOM_SHOT_HANDGUN();
					break;
				case mw_rifle::MACHINE_PISTOL:
					deduct = mods::values::SAME_ROOM_SHOT_MACHINE_PISTOL();
					break;
				default:
					deduct = 1;
					log("SYSERR: warning, no rifle type given for decrease_single_shot_ammo, default to 1");
					break;
			}
			sub_clamp(weapon->rifle_instance->ammo,ammunition::reduce_ammo(attacker,weapon,deduct));
		}
		bool attack_disorients(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    player_ptr_t& victim
		) {
			if(!weapon) {
				return false;
			}
			if(weapon->has_rifle()) {
				auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
				if(rifle_attachment && mods::rand::chance(rifle_attachment->disorient_amount)) {
					/** TODO: calculate disorient resistance of victim */
					m_debug("Attack disorients player!");
					return true;
				}
				m_debug("Attack fails to disorient player");
				return false;
			} else if(weapon->has_melee()) {
				/** TODO */
				if(mods::rand::chance(weapon->melee()->attributes->disorient_amount)) {
					/** TODO: calculate disorient resistance of victim */
					m_debug("Attack disorients player!");
					return true;
				}
			}
			return false;
		}
		uint16_t disorient_ticks(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    player_ptr_t& victim
		) {
			return mods::values::DEFAULT_DISORIENT_TICKS_FROM_RIFLE_ATTACK();
		}
		void remember_event(player_ptr_t& victim,player_ptr_t& attacker) {
			if(IS_NPC(victim->cd())) {
				remember(victim->cd(),attacker->cd());
			}
		}


		int calculate_weapon_cooldown(player_ptr_t& attacker,obj_ptr_t& attackers_weapon) {
			return attackers_weapon->rifle()->attributes->cooldown_between_shots;
		}

		void set_player_weapon_cooldown(player_ptr_t& attacker,obj_ptr_t& attackers_weapon) {
			auto cooldown = attackers_weapon->rifle()->attributes->cooldown_between_shots;
			if(cooldown <= 0) {
				attacker->weapon_cooldown_clear();
			} else {
				attacker->weapon_cooldown_start(cooldown);
			}
		}
		void report_headshot(player_ptr_t& attacker, player_ptr_t& victim, int dam, direction_t& direction) {
			feedback_t feedback;
			feedback.hits = 1;
			feedback.damage = dam;
			feedback.attacker = attacker->uuid();
			feedback.damage_event = de::YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK;
			attacker->damage_event(feedback);

			feedback.from_direction = OPPOSITE_DIR(direction);
			feedback.damage_event =de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK;
			victim->damage_event(feedback);

			attacker->send(MSG_HEADSHOT().c_str());
		}
		void report_crit(player_ptr_t& attacker, player_ptr_t& victim, int dam, direction_t& direction) {
			feedback_t feedback;
			attacker->send(MSG_CRITICAL().c_str());
			feedback.damage = dam;
			feedback.hits = 1;

			feedback.damage_event = de::YOU_DEALT_CRITICAL_RIFLE_ATTACK;
			attacker->damage_event(feedback);

			feedback.from_direction = OPPOSITE_DIR(direction);
			feedback.damage_event =de::HIT_BY_CRITICAL_RIFLE_ATTACK;
			victim->damage_event(feedback);
		}

	};//end namespace phases

};//end namespace combat_composer
