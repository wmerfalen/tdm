#include "sentinel.hpp"
#include "../orm/inventory.hpp"
#include "../weapon.hpp"
#include "../affects.hpp"
#include "../bugs-fixtures.hpp"

namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
		int16_t destroy(player_ptr_t& player){
			player = nullptr;
			return 0;
		}
		sentinel::sentinel(player_ptr_t p){
			load_by_player(p);
		}
		player_ptr_t 	sentinel::player(){
			return m_player;
		}
		void sentinel::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t sentinel::new_player(player_ptr_t &player, primary_choice_t primary_choice){
			if(primary_choice == primary_choice_t::NONE){
				mods::bugs::fixtures("sentinel::new_player. got primary_choice of zero. defaulting to MP5");
				primary_choice = primary_choice_t::MP5;
			}
			auto db_id = m_orm.initialize_row(player,primary_choice);
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t sentinel::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				mods::bugs::fixtures(CAT("sentinel::load_by_player unable to load sentinel class by player id: ",(player->db_id()),".. return status: ",(result)));
				return result;
			}
			set_player(player);
			m_heal_level = static_cast<cure_levels_t>(m_orm.sentinel_heal_level);
			m_intimidate_level = static_cast<intimidate_levels_t>(m_orm.sentinel_intimidate_level);
			m_human_shield_level = static_cast<human_shield_levels_t>(m_orm.sentinel_human_shield_level);
			m_deny_entry_level = static_cast<deny_entry_levels_t>(m_orm.sentinel_deny_entry_level);
			m_gadget_shield_level = static_cast<gadget_shield_levels_t>(m_orm.sentinel_gadget_shield_level);

			return 0;
		}
		void sentinel::sendln(std::string_view msg){ m_player->sendln(msg); }
		void sentinel::psendln(std::string_view msg){ m_player->psendln(msg); }
		void sentinel::heal(player_ptr_t& target){
			int healing = 0;
			switch(m_heal_level) {
				default:
				case SENTINEL_CURE_NONE:
					psendln("It looks like you still need to train that skill");
					return;
				case SENTINEL_CURE_LIGHT:
					healing = dice(1, 8) + 1 + (m_player->level() / 4);
					target->psendln("You feel better.");
					break;

				case SENTINEL_CURE_CRITIC:
					healing = dice(3, 8) + 3 + (m_player->level() / 4);
					target->psendln("You feel a lot better!");
					break;

				case SENTINEL_HEAL:
					healing = 100 + dice(3, 8);
					target->psendln("Your health increases drastically.");
					break;
			}
			target->hp() += healing;
		}
		void sentinel::intimidate(player_ptr_t& target){
			int intimidate = 0;
			switch(m_intimidate_level) {
				default:
				case SENTINEL_INTIMIDATE_NONE:
					psendln("It looks like you still need to train that skill");
					return;
				case SENTINEL_INTIMIDATE_LIGHT:
					intimidate = dice(1, 8) + 1 + (m_player->level() / 4);
					m_player->send("%s flees in fear!", target->name());
					break;

				case SENTINEL_INTIMIDATE_CRITIC:
					intimidate = dice(3, 8) + 3 + (m_player->level() / 4);
					m_player->send("%s experiences a massive adrenaline dump", target->name());
					break;

				case SENTINEL_INTIMIDATE_HEART_ATTACK:
					intimidate = 100 + dice(3, 8);
					m_player->send("%s grips his chest and vomits uncontrollably", target->name());
					break;
			}
			auto & affects = target->get_affect_dissolver();
			affects.affect(mods::affects::affect_t::INTIMIDATED,intimidate);
		}
		/* constructors and destructors */
		sentinel::sentinel(){
			m_player = nullptr;
		}
		std::shared_ptr<sentinel> create_sentinel(player_ptr_t &in_player){
			return std::move(std::make_shared<sentinel>(in_player));
		}
};
