#ifndef __MENTOC_COMBAT_COMPOSER_SHARED_HEADER__
#define __MENTOC_COMBAT_COMPOSER_SHARED_HEADER__
#include "snipe-target.hpp"
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

namespace mods::combat_composer {
	using vpd = mods::scan::vec_player_data;
	using de = damage_event_t;
	using namespace mods::object_utils;
	enum attack_type_t {
		RANGED,
		PROXY, /** i.e.: though a drone */
		MELEE_WEAPON,
		CQC, /** fists, elbows, knees, kicks */
		JUDO_THROW,
		SUBMISSION,
		WRESTLING,
		DEMOLITIONS,
		RANGED_MANA, /** i.e.: a magic missile */
		MELEE_MANA, /** same room melee attack */
	};

	namespace ammunition {
		uint16_t reduce_ammo(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    uint16_t wants_to_deduct
		);
	};
	void decrease_single_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) ;
	namespace phases {
		struct calculation_t {
			int16_t max_range;
			int16_t critical_range;
			std::pair<int16_t,int16_t> effective_range;
			uint16_t stat_boosts;
		};

		struct target_t {
			std::string target_name;
			direction_t direction;
			bool is_corpse;
			bool is_object;
			bool is_character;
			target_t(std::string_view _target_name,
			         direction_t _direction,
			         bool _is_corpse,
			         bool _is_object,
			         bool _is_character
			        ) : target_name(_target_name),
				direction(_direction),
				is_corpse(_is_corpse),
				is_object(_is_object),
				is_character(_is_character)
			{}
			target_t(std::string_view _target_name,
			         direction_t _direction) :
				target_name(_target_name),
				direction(_direction),
				is_corpse(0),
				is_object(0),
				is_character(1)
			{}
			target_t() = delete;
			~target_t() = default;
		};

		struct acquired_target_t {
			player_ptr_t target;
			direction_t direction;
			uint16_t distance;
		};

		struct calculated_damage_t {
			direction_t from_direction;
			uint16_t damage;
			uint16_t headshot_damage;
			uint16_t critical_damage;
			uint16_t incendiary_damage;
			uint16_t explosive_damage;
			uint16_t shrapnel_damage;
			uint16_t corrosive_damage;
			uint16_t cryogenic_damage;
			uint16_t radioactive_damage;
			uint16_t emp_damage;
			uint16_t shock_damage;
			uint16_t anti_matter_damage;
			calculated_damage_t() :
				from_direction(NORTH),
				damage(0),
				headshot_damage(0),
				critical_damage(0),
				incendiary_damage(0),
				explosive_damage(0),
				shrapnel_damage(0),
				corrosive_damage(0),
				cryogenic_damage(0),
				radioactive_damage(0),
				emp_damage(0),
				shock_damage(0),
				anti_matter_damage(0) {}
			~calculated_damage_t() = default;
		};
		bool attack_disorients(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    player_ptr_t& victim
		);
		uint16_t disorient_ticks(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    player_ptr_t& victim
		);
		void remember_event(player_ptr_t& victim,player_ptr_t& attacker);


		int calculate_weapon_cooldown(player_ptr_t& attacker,obj_ptr_t& attackers_weapon);

		void set_player_weapon_cooldown(player_ptr_t& attacker,obj_ptr_t& attackers_weapon);

		void report_headshot(player_ptr_t& attacker, player_ptr_t& victim, int dam, direction_t& direction);
		void report_crit(player_ptr_t& attacker, player_ptr_t& victim, int dam, direction_t& direction);


		void decrease_same_room_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon);
		void decrease_spray_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon);
		/**
		 * @brief subtracts 1 from weapon ammo
		 *
		 * @param weapon
		 */
		void decrease_single_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon);
	};//end namespace phases
};//end namespace combat_composer
#endif
