#include "contagion.hpp"
#include "../weapon.hpp"
#include "../orm/inventory.hpp"
#include "../bugs-fixtures.hpp"

namespace mods::classes {
	std::shared_ptr<mods::weapons::sniper_rifle::psg1> contagion::psg1(){ 
			return m_psg1;
	}
	std::shared_ptr<mods::weapons::sniper_rifle::l96aw> contagion::l96aw(){ 
			return m_l96aw;
	}
	contagion::contagion(){
			m_psg1 = nullptr;
			m_l96aw = nullptr;
	}
	contagion::contagion(player_ptr_t p){
		m_psg1 = nullptr;
		m_l96aw = nullptr;
		load_by_player(p);
	}
	player_ptr_t 	contagion::player(){
		return m_player;
	}

	int16_t contagion::new_player(player_ptr_t &player, primary_choice_t primary_choice){
		if(primary_choice == primary_choice_t::NONE){
			mods::bugs::fixtures("contagion::new_player. got primary_choice of zero. defaulting to FAMAS");
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
			std::cerr << "unable to load contagion class by player id: " << player->db_id() << ".. return status: " << result << "\n";
		}
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