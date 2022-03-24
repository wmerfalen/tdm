#ifndef __MENTOC_MODS_WEAPONS_SMG_VC88_HEADER__
#define __MENTOC_MODS_WEAPONS_SMG_VC88_HEADER__

#include "../weapon.hpp"
#include "../player.hpp"
#include "../pqxx-types.hpp"
#include "unique-weapons.hpp"

namespace mods::weapons::smg {
	struct vc88 : public mods::weapons::unique_ranged_weapon {
			std::vector<std::string> effects_and_behaviour = {
				"10d15 shrapnel damage",
				"chance of ammunition exploding in powdery haze that blinds and disorients target",
				"upon reload, gain 10% of your hp back",
				"last round in magazine deals 10d20 corrosive damage",
				"chance to ensnare enemy",

				"each burst fires one corrosive shotgun shell",
				"ammunition latches onto target and reduces resting heal amounts by 50 percent",
				"25% chance of target turning 180 degrees and spraying/sniping targets in that area",
				"at high damage amounts, target explodes in flesh and bone",

				"every fully emptied clip gives you a single sniper rifle charge",
				"switch to sniper rifle charge mode and you can use your VC88 as a sniper rifle",
			};
			static constexpr std::string_view description =
			    "  ______________________________________________________________________\r\n"
			    " /                                                                     /\r\n"
			    "/                                                                     /\r\n"
			    "| .  .  .  .  .  .  .  . .  .  .  . .  .  .  . .  .  .  . .  .  .  . /,\r\n"
			    "| Weapon: VC88                                                        |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Type: SMG ]--:[ MOD_TYPE: Ballistic ]:==--------------------------+\r\n"
			    "|                                                                     |\r\n"
			    "| Manufacturer: M10-X LLC                                             |\r\n"
			    "| Acronym: [V]ector [C]urse ([88] round clip)                         |\r\n"
			    "|                                                                     |\r\n"
			    "| Description:                                                        |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Known stats ]-----------------------------------------------------|\r\n"
			    "| 10d10                                                               |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ R: 001 ]---------------------------------------[ Docu-gen v8.1.0 ]+\r\n";
			struct stats {
				static constexpr uint16_t base_damage = 850;
				static constexpr uint16_t base_damage_dice_additional [2] = {20,40};
				static constexpr uint16_t incendiary_damage_percent = 150;
				static constexpr uint16_t kevlar_damage = 780;
			};

			vc88() = delete;
			vc88(obj_ptr_t& obj);
			~vc88();


			const uuid_t& uuid() {
				return m_object->uuid;
			}
			void increment_shot();

			const event_list_t& get_subscriptions() override {
				static event_list_t subs = {
					damage_event_t::YOU_HIT_FLESH,
					damage_event_t::YOUR_CLIP_IS_DEPLETED,
					damage_event_t::PULL_CLIP_OUT,
					damage_event_t::PUSH_CLIP_IN,
					damage_event_t::AIM_AT_TARGET,
					damage_event_t::PULL_TRIGGER,

				};
				return subs;
			}
			void dispatch_event(damage_event_t event, player_ptr_t victim) override;

			std::vector<uuid_t>& damaged_enemy_list() override;
			const room_rnum& player_room();

		protected:
			player_ptr_t player_upkeep();
			player_ptr_t m_player;
			void reload_event();
			void hits_flesh_event(player_ptr_t victim);
			std::vector<uuid_t> m_damaged_enemy_list;
	};
	using vc88_ptr_t = std::shared_ptr<mods::weapons::smg::vc88>;
	vc88_ptr_t make_vc88(player_ptr_t& wielder);
	vc88_ptr_t make_vc88(obj_ptr_t& object);
	vc88_ptr_t find_vc88(const uuid_t& uuid) ;
	void erase_vc88(const uuid_t& uuid);
};
#endif

