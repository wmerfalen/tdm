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

			player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
			return result;
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
		/* constructors and destructors */
		striker::striker(){
			m_player = nullptr;
		}
		std::shared_ptr<striker> create_striker(player_ptr_t &in_player){
			return std::move(std::make_shared<striker>(in_player));
		}
};
ACMD(do_teep){
	player->sendln("Command not implemented.");
	return;
#if 0
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
#endif
}
