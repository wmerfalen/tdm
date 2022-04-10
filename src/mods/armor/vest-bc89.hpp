#ifndef __MENTOC_MODS_ARMOR_VEST_BC89_HEADER__
#define __MENTOC_MODS_ARMOR_VEST_BC89_HEADER__

#include "../armor.hpp"
#include "../player.hpp"
#include "../pqxx-types.hpp"
#include "unique-armor.hpp"

namespace mods::armor {
	struct bc89 : public mods::armor::unique_armor {
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
			    "| Acronym: [B]allistic [C]astle Version [89]                          |\r\n"
			    "|                                                                     |\r\n"
			    "| Description:                                                        |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Known stats ]-----------------------------------------------------|\r\n"
			    "|                                                                     |\r\n"
			    "+-[ R: 001 ]---------------------------------------[ Docu-gen v8.1.0 ]+\r\n";
			struct stats {
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

