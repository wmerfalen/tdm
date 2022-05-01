#ifndef __MENTOC_MODS_ARMOR_GAUNTLETS_XTV89_HEADER__
#define __MENTOC_MODS_ARMOR_GAUNTLETS_XTV89_HEADER__

#include "../armor.hpp"
#include "../player.hpp"
#include "../pqxx-types.hpp"
#include "unique-armor.hpp"
<<<<<<< HEAD
#include "../weapons/shotgun-charge.hpp"
#include "../user-feedback.hpp"
#include <stack>

namespace mods::armor {
	struct xtv89 : public mods::armor::unique_armor {
			using shotgun_charge_t = mods::weapons::shotgun_charge;
			using shotgun_charge_list_t = std::stack<shotgun_charge_t>;
=======

namespace mods::armor {
	struct xtv89 : public mods::armor::unique_armor {
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)
			static const std::vector<std::string>& get_usage_screen();
			static constexpr std::string_view YAML_FILE = "xtv89-gauntlets.yml";
			static constexpr std::string_view description =
			    "  ______________________________________________________________________\r\n"
			    " /                                                                     /\r\n"
			    "/                                                                     /\r\n"
			    "| .  .  .  .  .  .  .  . .  .  .  . .  .  .  . .  .  .  . .  .  .  . /,\r\n"
			    "| Armor: XTV89 Gauntlets                                              |\r\n"
			    "|                                                                     |\r\n"
<<<<<<< HEAD
			    "+-[ Type: GAUNTLETS ]--:[ MOD_TYPE: DAMAGE INTAKE REBUKE ]:-----------+\r\n"
			    "|                                                                     |\r\n"
			    "| Manufacturer: Xorg TECH                                             |\r\n"
			    "| Acronym: [X]13 [T]ECH [V]ulcan Gauntlets version [8.9]              |\r\n"
=======
			    "+-[ Type: GAUNTLETS ]--:[ MOD_TYPE: SHOTGUN CHARGE ]:-----------------+\r\n"
			    "|                                                                     |\r\n"
			    "| Manufacturer: Xorg TECH                                             |\r\n"
			    "| Acronym: [X]org [T]ECH [V]ulcan Gauntlets version [8.9]             |\r\n"
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)
			    "|                                                                     |\r\n"
			    "| Description:                                                        |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Known stats ]-----------------------------------------------------|\r\n"
			    "| 3d4T5 Create a shotgun charge                                       |\r\n"
			    "| 10d15 Base damage per shotgun charge                                |\r\n"
			    "| 9d15 Corrosive damage per shotgun charge                            |\r\n"
			    "| 2d8T6 deflect ammunition at attacker                                |\r\n"
<<<<<<< HEAD
			    "| 3 max shotgun charges                                               |\r\n"
=======
			    "|                                                                     |\r\n"
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)
			    "+-----------------[ Known Configuration Synergies ]-------------------|\r\n"
			    "|                                                                     |\r\n"
			    "|+[ BC89 Ballistic Vest ]                                             |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ R: 001 ]---------------------------------------[ Docu-gen v8.1.0 ]+\r\n";
			struct stats {
<<<<<<< HEAD
				static constexpr std::array<uint16_t,3> create_shotgun_charge_chance = {3,4,5};
				static constexpr std::array<uint16_t,3> deflect_chance = {2,8,6};
				static constexpr uint8_t max_shotgun_charges = 3;
=======
				static constexpr std::array<uint16_t,3> deflect_chance = {2,8,6};
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)
			};

			xtv89() = delete;
			xtv89(obj_ptr_t& obj);
			~xtv89();


			const uuid_t& uuid() {
				return m_object->uuid;
			}

			const event_list_t& get_subscriptions() override {
				static event_list_t subs = {
					damage_event_t::YOU_HIT_ARMOR,
					damage_event_t::YOU_HIT_FLESH,
				};
				return subs;
			}
			void dispatch_event(damage_event_t event, player_ptr_t attacker) override;
<<<<<<< HEAD
			void dispatch_bc89_synergy(damage_event_t event, player_ptr_t attacker, obj_ptr_t& vest);
=======
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)

			const room_rnum& player_room();
			std::string_view examine();

<<<<<<< HEAD
			user_feedback add_shotgun_charge();
			user_feedback consume_shotgun_charge();

=======
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)
		protected:
			player_ptr_t player_upkeep();
			player_ptr_t m_player;
			void hits_armor_event(player_ptr_t victim);
			std::vector<uuid_t> m_damaged_enemy_list;
<<<<<<< HEAD
			shotgun_charge_list_t m_shotgun_charges;
=======
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)
	};
	using xtv89_ptr_t = std::shared_ptr<mods::armor::xtv89>;
	xtv89_ptr_t make_xtv89(player_ptr_t& wielder);
	xtv89_ptr_t make_xtv89(obj_ptr_t& object);
	xtv89_ptr_t find_xtv89(const uuid_t& uuid) ;
	void erase_xtv89(const uuid_t& uuid);
};
#endif

