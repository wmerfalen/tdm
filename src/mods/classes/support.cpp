#include "support.hpp"
#include "../weapon.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	std::shared_ptr<mods::weapons::sniper_rifle::psg1> support::psg1(){ 
			return m_psg1;
	}
	std::shared_ptr<mods::weapons::sniper_rifle::l96aw> support::l96aw(){ 
			return m_l96aw;
	}
	support::support(){
			m_psg1 = nullptr;
			m_l96aw = nullptr;
	}
	support::support(player_ptr_t p){
		m_psg1 = nullptr;
		m_l96aw = nullptr;
		load_by_player(p);
	}
	player_ptr_t 	support::player(){
		return m_player;
	}

	int16_t support::new_player(player_ptr_t &player, std::string_view primary_choice){
		using primary = mods::weapon::support::primary_choice_t;
		auto pchoice = 0;
		if(!primary_choice.compare("MK46")){
			pchoice = primary::SUPPORT_PRIMARY_MK46;
		}
		if(!primary_choice.compare("HK21") == 0){
			pchoice = primary::SUPPORT_PRIMARY_HK21;
		}
		if(pchoice == 0){
			std::cerr << "invalid primary weapon choice for support class...\n";
			return -1;
		}
		auto db_id = m_orm.initialize_row(player,static_cast<primary>(pchoice));
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
			std::cerr << "unable to load support class by player id: " << player->db_id() << ".. return status: " << result << "\n";
		}
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
