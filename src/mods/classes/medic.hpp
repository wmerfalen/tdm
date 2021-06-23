#ifndef  __MENTOC_MODS_CLASSES_MEDIC_HEADER__
#define  __MENTOC_MODS_CLASSES_MEDIC_HEADER__

#include "../orm/medic.hpp"
#include "base.hpp"

using medic_orm_t = mods::orm::medic;

namespace mods::classes {
	struct medic : public base {
			types kind() {
				return types::MEDIC;
			}
			using primary_choice_t = mods::weapon::medic::primary_choice_t;
			using secondary_choice_t = mods::weapon::medic::secondary_choice_t;
			constexpr static uint8_t DEFAULT_STARTING_STIM_PISTOL_AMMO = 4;
			enum ability_t {
				NONE = 0,

				/** MEDICAL */
				STIM_SHOT,
				/** description:
				 * shoot your allies with a stim pistol which restores HP
				 */
				PARASITIC_AMMO,
				/**
				 * description:
				 * causes your next 3 shots to have parisite: damage dealt is
				 * added to your hp
				 */
				ADRENALINE_SHOT,
				/**
				 * description:
				 * shoot an adrenaline shot at you or your teammates.
				 * Adrenaline boosts HP, Accuracy, and reduces damage taken.
				 */

				/** ELECTRONICS */
				DRONE_HEALER,
				/** description:
				 * spawns a drone that will heal you and your allies
				 */
				HEARTBEAT_SENSOR,
				/**
				 * description:
				 * reveal enemy positions on your overhead map
				 */


				/** STRATEGY */
				EMERGENCY_EVAC,
				/**
				 * description:
				 * calls in a helicopter to your position. pulls you out of your
				 * current room and moves you to a random safe area.
				 */



				/** DEMOLITIONS */
				DEUTERIUM_SHOTGUN_UNDERBARREL,
				/**
				 * description:
				 * adds a deuterium (radiation+incendiary) underbarrel to the non-pistol
				 * rifle that you are currently using.
				 * Tier 1: 1 shot
				 * Tier 2: 3 shots
				 * Tier 3: 6 shots
				 */
				CORROSIVE_AGENT,
				/**
				 * description:
				 * fires a blast of corrossive chemicals which causes armor to
				 * work less effectively. Corrossive spray also blinds the target
				 * for a random amount of time. As proficiency gets better, the
				 * effectiveness and duration get better and longer.
				 */




				SEAL_ROOM,	// meh
				/** description:
				 * Creates a temporary barrier to entry to the current room
				 * that the medic resides in. While in this room, the medic
				 * can perform operations which may take a long time.
				 */

				/** WEAPON_HANDLING */


				/** SNIPING */
				SHOTGUN_SNIPER,
				/**
				 * description:
				 * turns a shotgun into a sniper for N rounds based on tier
				 * Tier 1: 3 rounds
				 * Tier 2: 9 rounds
				 * Tier 3: 18 rounds
				 */
				HAZARDOUS_WASTE_SHOT,
				/**
				 * description:
				 * turns your shots into hazardous waste which can have random
				 * effects on your target that could potentially lead to
				 * fatal wounds.
				 */
				SEDATIVE_HYPNOTIC,
				/**
				 * description:
				 * your next shots cause the target to shoot less and slows down their
				 * movement. Their accuracy suffers and there's a chance of falling
				 * asleep.
				 * Tier 1: 1 shot -> add 10 ticks per reload/cooldown
				 * Tier 2: 1 shot -> add 20 ticks per reload/cooldown 15% chance of falling asleep
				 * Tier 3: 2 shots -> add 40 ticks to reload/cooldown 25% chance of falling asleep
				 */

				/** ARMOR */
				ARMOR_BONUS,
				/**
				 * description:
				 * causes you to gain bonus protection from worn armor.
				 * Tier 1: chance to get 25HP absorbed
				 * Tier 2: chance to get 85HP absorbed
				 * Tier 3: chance to get 150HP absorbed
				 */
				NO_FACTOR_ARMOR,
				/**
				 * description:
				 * basic, advanced, elite armor takes less away from movement points
				 */

				/** WEAPON_HANDLING */
				SURGICAL_PRECISION,
				/**
				 * description:
				 * As a doctor, you understand the body and where things connect. Using surgical precision,
				 * your next shots have a chance of causing fatal wounds based on tier.
				 * Tier 1: 1 shot 25% chance to cut off arm, 10% chance to decapitate, 5% chance to penetrate lungs/heart
				 * Tier 2: 1 shot 35% chance to cut off arm, 15% .. , 10%
				 * Tier 3: 2 shots 45%, .. 25%, .. 20%
				 */

			};



			ability_list_t& get_abilities() override {
				return m_abilities;
			}

			player_ptr_t get_player_ptr() override {
				return m_player;
			}

			/* constructors and destructors */
			medic();
			medic(player_ptr_t);
			~medic() = default;

			/** orm wrappers */
			int16_t load_by_player(player_ptr_t&);
			player_ptr_t 	player();
			int16_t	new_player(player_ptr_t&, primary_choice_t primary);
			uint64_t initialize_row(player_ptr_t&);
			int16_t save();
			void replenish();

			/** class abilities */
			void heal_player(player_ptr_t& target);
			uint8_t& stim_pistol_ammo();
			void init();
		protected:
			uint8_t m_stim_pistol_ammo;

		private:
			player_ptr_t m_player;
			medic_orm_t m_orm;
			ability_list_t m_abilities;

			/** medical */
			skill_t m_stim_shot;
			skill_t m_parasitic_ammo;
			skill_t m_adrenaline_shot;

			/** Electronics */
			skill_t m_drone_healer;
			skill_t m_heartbeat_sensor;

			/** strategy */
			skill_t m_emergency_evac;

			/** demolitions */
			skill_t m_deuterium_shotgun_ub;
			skill_t m_corrosive_agent;
			skill_t m_seal_room;

			/** sniping */
			skill_t m_shotgun_sniper;
			skill_t m_hazardous_waste_shot;
			skill_t m_sedative_hypnotic;

			/** armor */
			skill_t m_armor_bonus;
			skill_t m_no_factor_armor;

			/** weapon_handling */
			skill_t m_surgical_precision;
	};

	std::shared_ptr<mods::classes::medic> create_medic(player_ptr_t& player);
};

#endif
