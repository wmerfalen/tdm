#include "support.hpp"
#include "../weapon.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	support::support(){
	}
	support::support(player_ptr_t p){
		load_by_player(p);
	}
	player_ptr_t 	support::player(){
		return m_player;
	}

	int16_t support::new_player(player_ptr_t &player, primary_choice_t primary_choice){
		if(primary_choice == primary_choice_t::NONE){
			mods::bugs::fixtures("support::new_player. got primary_choice of zero. defaulting to MK46");
			primary_choice = primary_choice_t::MK46;
		}
		auto db_id = m_orm.initialize_row(player,primary_choice);
		if(db_id == 0){
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t support::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			mods::bugs::fixtures(CAT("support::load_by_player.",(result)));
		}
		obj_ptr_t primary = nullptr;
		primary = create_object(ITEM_RIFLE,mods::weapon::yaml_file(m_orm.primary_type()));
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
		return result;
	}
	int16_t support::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	std::shared_ptr<support> create_support(player_ptr_t &in_player){
		return std::move(std::make_shared<support>(in_player));
	}
};
