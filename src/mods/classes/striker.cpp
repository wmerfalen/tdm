#include "striker.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
		striker::striker(player_ptr_t p){
			load_by_player(p);
		}
		player_ptr_t 	striker::player(){
			return m_player;
		}
		void striker::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t striker::new_player(player_ptr_t &player){
			auto db_id = m_orm.initialize_row(player,0);
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t striker::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				report(CAT("striker::load_by_player unable to load striker class by player id: ",(player->db_id()),".. return status: ",(result)));
				return result;
			}
			set_player(player);
			m_teep_level = static_cast<teep_levels_t>(m_orm.striker_teep_level);
			m_xray_shot_level = static_cast<xray_shot_levels_t>(m_orm.striker_xray_shot_level);
			m_stealth_level = static_cast<stealth_levels_t>(m_orm.striker_stealth_level);
			m_feign_death_level = static_cast<feign_death_levels_t>(m_orm.striker_feign_death_level);
			m_summon_extraction_level = static_cast<summon_extraction_levels_t>(m_orm.striker_summon_extraction_level);

			player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
			return result;
		}
		void striker::apply_stealth_to(obj_ptr_t& target){
			int stealth = 0;
			switch(m_stealth_level) {
				default:
				case stealth_levels_t::STRIKER_STEALTH_NONE:
					m_player->sendln("It looks like you still need to train that skill");
					return;
				case stealth_levels_t::STRIKER_STEALTH_INITIATE:
					stealth = dice(1, 8) + 1 + (m_player->level() / 4);
					break;

				case stealth_levels_t::STRIKER_STEALTH_FAMILIAR:
					stealth = dice(3, 8) + 3 + (m_player->level() / 4);
					break;

				case stealth_levels_t::STRIKER_STEALTH_MASTER:
					stealth = 100 + dice(3, 8);
					break;
			}
			target->visibility -= stealth;
		}
		void striker::apply_stealth_to_player(player_ptr_t& target){

		}
		void striker::feign_death_done(){
			m_player->set_position(POS_STANDING);
			m_player->sendln("You stand up and dust yourself off.");
		}
		std::pair<int16_t,std::string> striker::teep(direction_t direction){
			switch(m_teep_level){
				default:
				case teep_levels_t::STRIKER_TEEP_NONE:
				case teep_levels_t::STRIKER_TEEP_INITIATE:
					break;
				case teep_levels_t::STRIKER_TEEP_FAMILIAR:
					break;
				case teep_levels_t::STRIKER_TEEP_MASTER:
					break;
			}
			return {1, MSG_YOU_FEIGN_DEATH()};
		}
		std::pair<int16_t,std::string> striker::summon_extraction(room_rnum room){
			return {0,""};
		}
		/** requires drone assisted sniping mode */
		std::pair<int16_t,std::string> striker::xray_shot(){

			return {0,""};
		}
		/* constructors and destructors */
		striker::striker(){
			m_player = nullptr;
		}
		std::shared_ptr<striker> create_striker(player_ptr_t &in_player){
			return std::move(std::make_shared<striker>(in_player));
		}
};
ACMD(do_teep){
	PLAYER_CAN("striker.teep");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 1){
		player->sendln("Which direction?!");
		return;
	}
	auto s = to_direction(vec_args[0]);
	if(!s.first){
		player->sendln("Which direction?!");
		return;
	}
	auto msg = std::get<1>(player->striker()->teep(s.second));
	player->sendln(msg);
}
