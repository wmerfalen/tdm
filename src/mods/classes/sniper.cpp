#include "sniper.hpp"
#include "../weapon.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	psg1_t sniper::psg1(){ 
			return m_psg1;
	}
	l96aw_t sniper::l96aw(){ 
			return m_l96aw;
	}
	sniper::sniper(){
			m_psg1 = nullptr;
			m_l96aw = nullptr;
	}
	sniper::sniper(player_ptr_t p){
		m_psg1 = nullptr;
		m_l96aw = nullptr;
		load_by_player(p);
	}
	player_ptr_t 	sniper::player(){
		return m_player;
	}
	int16_t sniper::new_player(player_ptr_t &player, std::string_view primary_choice){
		using primary = mods::weapon::sniper::primary_choice_t;
		auto pchoice = 0;
		assert(primary_choice.length() > 0);

		if(std::tolower(primary_choice[0]) == 'p'){
			pchoice = primary::SNIPER_PRIMARY_PSG1;
		}

		if(std::tolower(primary_choice[0]) == 'l'){ // "ell"
			pchoice = primary::SNIPER_PRIMARY_L96AW;
		}
		if(pchoice == 0){
			std::cerr << "invalid primary weapon choice for sniper class...\n";
			return -1;
		}
		auto db_id = m_orm.initialize_row(player,static_cast<primary>(pchoice));
		if(db_id == 0){
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t sniper::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			std::cerr << "unable to load sniper class by player id: " << player->db_id() << ".. return status: " << result << "\n";
		}
		return result;
	}
	int16_t sniper::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	std::shared_ptr<sniper> create_sniper(player_ptr_t &in_player){
		return std::move(std::make_shared<sniper>(in_player));
	}
};
