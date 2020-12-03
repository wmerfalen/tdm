#include "marine.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	marine::marine(){
	}
	void marine::replenish(){

	}
	marine::marine(player_ptr_t p){
		load_by_player(p);
	}
	player_ptr_t 	marine::player(){
		return m_player;
	}

	int16_t marine::new_player(player_ptr_t &player, primary_choice_t primary_choice){
		if(primary_choice == primary_choice_t::NONE){
			report("marine::new_player. got primary_choice of zero. defaulting to M16A4");
			primary_choice = primary_choice_t::M16A4;
		}
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0){
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t marine::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			report(CAT("unable to load marine class by player id: ",(player->db_id()),".. return status: ",(result),"player:",player->name().c_str()));
			return -100 - result;
		}
		obj_ptr_t primary = nullptr;
		/** TODO: create catchy name using the deep object parser */
		primary = create_object(ITEM_RIFLE,"m4.yml");
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"magnum-revolver.yml"),WEAR_SECONDARY);
		auto fatal = create_object(ITEM_EXPLOSIVE,"frag-grenade.yml");
		player->carry(fatal);
		return result;
	}
	int16_t marine::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	std::shared_ptr<marine> create_marine(player_ptr_t &in_player){
		return std::move(std::make_shared<marine>(in_player));
	}
};
