#ifndef __MENTOC_MODS_WEAPONS_SHOTGUN_DST7A_HEADER__
#define __MENTOC_MODS_WEAPONS_SHOTGUN_DST7A_HEADER__

#include "../weapon.hpp"
#include "../player.hpp"
#include "../pqxx-types.hpp"
#include "unique-weapons.hpp"

namespace mods::weapons::shotgun {
	struct dst7A : public mods::weapons::unique_ranged_weapon {
			static constexpr std::string_view description =
			    "  ______________________________________________________________________\r\n"
			    " /                                                                     /\r\n"
			    "/                                                                     /\r\n"
			    "| .  .  .  .  .  .  .  . .  .  .  . .  .  .  . .  .  .  . .  .  .  . /,\r\n"
			    "| Weapon: The DST7A                                                   |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Type: SHOTGUN ]--:[ MOD_TYPE: Radiation+Incendiary ]:-------------+\r\n"
			    "|                                                                     |\r\n"
			    "| Manufacturer: HARM TECH                                             |\r\n"
			    "| Acronym: [D]e[s][t]abilize [A]lpha ([7] round clips)                |\r\n"
			    "|                                                                     |\r\n"
			    "| Description:                                                        |\r\n"
			    "| The DST7A was designed with cruelty in mind. The DST7A is a semi-   |\r\n"
			    "| automatic shotgun that deals more than just shrapnel damage to it's |\r\n"
			    "| intended targets. A proprietary mechanism within the barrell of the |\r\n"
			    "| shotgun causes a chemical reaction which coats each shell as it     |\r\n"
			    "| travels towards it's target. A cruel mixture of radioactive isotopes|\r\n"
			    "| and components used in pyrotechnics coat the shrapnel as it leaves  |\r\n"
			    "| the barrell. Upon hitting flesh, the mixture causes the flesh and   |\r\n"
			    "| bone of the target to effectively melt away into uselessness.       |\r\n"
			    "|                                                                     |\r\n"
			    "| Initial testing of the DST7A caused many QA engineers to fall ill to|\r\n"
			    "| radiation sickness. As of yet, the DST7A is considered HIGHLY EXPER-|\r\n"
			    "| IMENTAL. While production of the DST7A halted, the blueprints have  |\r\n"
			    "| since been copied and the shotgun has survived in the black market. |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Known stats ]-----------------------------------------------------|\r\n"
			    "| 850 + 20d40 melting damage                                          |\r\n"
			    "| 2d4T6 move points on forestock movement                             |\r\n"
			    "| 2d2T2 mana points on forestock movement                             |\r\n"
			    "| 1d6T3 blind target when ammunition hits target                      |\r\n"
			    "+-[ R: 001 ]---------------------------------------[ Docu-gen v8.1.0 ]+\r\n";
			/** TODO: 450 - Bone marrow damage every 3 ticks  */
			/** TODO: 780 - Kevlar damage every 12 ticks */
			struct stats {
				static constexpr uint16_t base_damage = 850;
				static constexpr uint16_t base_damage_dice_additional [2] = {20,40};
				static constexpr uint16_t kevlar_damage = 780;
				static constexpr std::array<uint16_t,3> move_points_per_outward_pump_buff = {2,4,6};
				static constexpr std::array<uint16_t,3> mana_points_per_inward_pump_buff = {2,2,2};
				static constexpr std::array<uint16_t,3> blind_target_chance = {1,6,3};
			};

			dst7A() = delete;
			dst7A(obj_ptr_t& obj);
			~dst7A();


			const uuid_t& uuid() {
				return m_object->uuid;
			}

			const event_list_t& get_subscriptions() override {
				static event_list_t subs = {
					damage_event_t::SHOTGUN_PUMP_INWARD,
					damage_event_t::SHOTGUN_PUMP_OUTWARD,
					damage_event_t::RELOAD_SINGLE_SHOT_INTO_SHOTGUN,
					damage_event_t::YOU_HIT_ARMOR,
					damage_event_t::YOU_HIT_FLESH,
				};
				return subs;
			}
			void dispatch_event(damage_event_t event, player_ptr_t victim) override;

			std::vector<uuid_t>& damaged_enemy_list() override;
			const room_rnum& player_room();
			std::string_view examine();
			void blind_target(player_ptr_t& victim);

		protected:
			player_ptr_t player_upkeep();
			player_ptr_t m_player;
			void reload_event();
			void pump_inward_event();
			void hits_flesh_event(player_ptr_t victim);
			void pump_outward_event();
			std::vector<uuid_t> m_damaged_enemy_list;
	};
	using dst7A_ptr_t = std::shared_ptr<mods::weapons::shotgun::dst7A>;
	dst7A_ptr_t make_dst7a(player_ptr_t& wielder);
	dst7A_ptr_t make_dst7a(obj_ptr_t& object);
	dst7A_ptr_t find_dst7a(const uuid_t& uuid) ;
	void erase_dst7a(const uuid_t& uuid);
};
#endif

