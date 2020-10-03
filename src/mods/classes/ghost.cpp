#include "ghost.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	ghost::ghost(){
		m_player = nullptr;
	}
	ghost::ghost(player_ptr_t p){
		m_player = nullptr;
		load_by_player(p);
	}
	player_ptr_t 	ghost::player(){
		return m_player;
	}
	int16_t ghost::new_player(player_ptr_t &player,primary_choice_t primary_choice){
		auto db_id = m_orm.initialize_row(player,primary_choice);
		if(db_id == 0){
			report(CAT("unable to initialize row for player (ghost::new_player) ",(player->db_id()),".. player's database id is zero!"));
			return -2;
		}
		player->set_db_id(db_id);
		load_by_player(player);
		return 0;
	}
	int16_t ghost::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			report(CAT("unable to load ghost class by player id: " , player->db_id() , ".. return status: " , result));
			return -100 - result;
		}
		obj_ptr_t primary = nullptr;
		primary = create_object(ITEM_RIFLE,mods::weapon::yaml_file(m_orm.primary_type()));
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
		return result;
	}
	int16_t ghost::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	std::shared_ptr<ghost> create_ghost(player_ptr_t &in_player){
		return std::move(std::make_shared<ghost>(in_player));
	}
};
