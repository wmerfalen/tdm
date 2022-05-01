#ifndef __MENTOC_MODS_ARMOR_VEST_BC89_HEADER__
#define __MENTOC_MODS_ARMOR_VEST_BC89_HEADER__

#include "../armor.hpp"
#include "../player.hpp"
#include "../pqxx-types.hpp"
#include "unique-armor.hpp"

namespace mods::armor {
	struct bc89 : public mods::armor::unique_armor {
			static const std::vector<std::string>& get_usage_screen();
			static constexpr std::string_view YAML_FILE = "bc89-vest.yml";
			static constexpr std::string_view description =
			    "  ______________________________________________________________________\r\n"
			    " /                                                                     /\r\n"
			    "/                                                                     /\r\n"
			    "| .  .  .  .  .  .  .  . .  .  .  . .  .  .  . .  .  .  . .  .  .  . /,\r\n"
			    "| Armor: BC89 Ballistic Vest                                          |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Type: VEST ]--:[ MOD_TYPE:                         ]:-------------+\r\n"
			    "|                                                                     |\r\n"
			    "| Manufacturer: Xorg TECH                                             |\r\n"
			    "| Acronym: Optimized [B]allistic [C]hest Protection version [8.9]     |\r\n"
			    "|                                                                     |\r\n"
			    "| Description:                                                        |\r\n"
			    "|                                                                     |\r\n"
			    "| The BC89 Ballistic Vest is an oddity in the realm of ballistic prot-|\r\n"
			    "| ection. While it is a heavy piece of armor, the amount of phsyical  |\r\n"
			    "| protection against a wide array of ballistics makes up for it.      |\r\n"
			    "| The manufacturer, Xorg TECH, has released several versions prior to |\r\n"
			    "| BC89 in the hopes of winning several international government contr-|\r\n"
			    "| acts. Each and every iteration prior to the BC89 had failed to gain |\r\n"
			    "| the interest of governments worldwide due to defects of design.     |\r\n"
			    "|                                                                     |\r\n"
			    "| What separates the BC89 from other pieces of armor is it's ability  |\r\n"
			    "| to deflect damage onto it's attackers. Using a highly proprietary   |\r\n"
			    "| and exotic technologoy, the BC89 can deflect and/or absorb ammunit- |\r\n"
			    "| ion. In addition to ammunition denial, the BC89 can be equipped     |\r\n"
			    "| with a pair of Xorg TECH Vulcan Gauntlets allowing the user of the  |\r\n"
			    "| gauntlets to absorb the ammunition from the BC89 and charge them    |\r\n"
			    "| into the gauntlets. Once charged, the gauntlets can be used as a    |\r\n"
			    "| shotgun.                                                            |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Known stats ]-----------------------------------------------------|\r\n"
			    "| 2d8T6 deflect ammunition at attacker                                |\r\n"
			    "| 15d18 base damage per deflection                                    |\r\n"
			    "|                                                                     |\r\n"
			    "+-----------------[ Known Configuration Synergies ]-------------------|\r\n"
			    "|                                                                     |\r\n"
			    "|+[ Xorg TECH Vulcan Gauntlets ]                                      |\r\n"
			    "| 3d4T5 Create a shotgun charge                                       |\r\n"
			    "| 10d15 Base damage per shotgun charge                                |\r\n"
			    "| 9d15 Corrosive damage per shotgun charge                            |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ R: 001 ]---------------------------------------[ Docu-gen v8.1.0 ]+\r\n";
			struct stats {
				static constexpr std::array<uint16_t,3> deflect_chance = {2,8,6};
				static constexpr std::array<uint16_t,2> deflect_dice = {15,18};
				static constexpr std::array<uint16_t,2> deflect_dice = {15,18};
				static constexpr std::array<uint16_t,3> blind_target_chance = {1,6,3};
			};

			bc89() = delete;
			bc89(obj_ptr_t& obj);
			~bc89();


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

			const room_rnum& player_room();
			std::string_view examine();

		protected:
			player_ptr_t player_upkeep();
			player_ptr_t m_player;
			void hits_flesh_event(player_ptr_t victim);
			void hits_armor_event(player_ptr_t victim);
			std::vector<uuid_t> m_damaged_enemy_list;
	};
	using bc89_ptr_t = std::shared_ptr<mods::armor::bc89>;
	bc89_ptr_t make_bc89(player_ptr_t& wielder);
	bc89_ptr_t make_bc89(obj_ptr_t& object);
	bc89_ptr_t find_bc89(const uuid_t& uuid) ;
	void erase_bc89(const uuid_t& uuid);
};
#endif

