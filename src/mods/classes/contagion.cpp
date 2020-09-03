#include "contagion.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	contagion::contagion(){
	}
	contagion::contagion(player_ptr_t p){
		load_by_player(p);
	}
	player_ptr_t 	contagion::player(){
		return m_player;
	}

	int16_t contagion::new_player(player_ptr_t &player, primary_choice_t primary_choice){
		if(primary_choice == primary_choice_t::NONE){
			report("contagion::new_player. got primary_choice of zero. defaulting to FAMAS");
			primary_choice = primary_choice_t::FAMAS;
		}
		auto db_id = m_orm.initialize_row(player,primary_choice);
		if(db_id == 0){
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t contagion::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			report(CAT("unable to load contagion class by player id: ",player->db_id(), ".. return status: " ,result));
		}
		obj_ptr_t primary = nullptr;
		primary = create_object(ITEM_RIFLE,mods::weapon::yaml_file(m_orm.primary_type()));
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
		return result;
	}
	int16_t contagion::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	std::shared_ptr<contagion> create_contagion(player_ptr_t &in_player){
		return std::move(std::make_shared<contagion>(in_player));
	}
};
