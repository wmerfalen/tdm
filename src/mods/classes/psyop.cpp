#include "psyop.hpp"
#include "../weapon.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	std::shared_ptr<mods::weapons::sniper_rifle::psg1> psyop::psg1(){ 
			return m_psg1;
	}
	std::shared_ptr<mods::weapons::sniper_rifle::l96aw> psyop::l96aw(){ 
			return m_l96aw;
	}
	psyop::psyop(){
			m_psg1 = nullptr;
			m_l96aw = nullptr;
	}
	psyop::psyop(player_ptr_t p){
		m_psg1 = nullptr;
		m_l96aw = nullptr;
		load_by_player(p);
	}
	player_ptr_t 	psyop::player(){
		return m_player;
	}

	int16_t psyop::new_player(player_ptr_t &player, std::string_view primary_choice){
		using primary = mods::weapon::psyop::primary_choice_t;
		auto pchoice = 0;
		if(!primary_choice.compare("SCARH")){
			pchoice = primary::PSYOP_PRIMARY_SCARH;
		}
		if(!primary_choice.compare("UMP45") == 0){
			pchoice = primary::PSYOP_PRIMARY_UMP45;
		}
		if(pchoice == 0){
			std::cerr << "invalid primary weapon choice for psyop class...\n";
			return -1;
		}
		auto db_id = m_orm.initialize_row(player,static_cast<primary>(pchoice));
		if(db_id == 0){
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t psyop::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			std::cerr << "unable to load psyop class by player id: " << player->db_id() << ".. return status: " << result << "\n";
		}
		return result;
	}
	int16_t psyop::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	std::shared_ptr<psyop> create_psyop(player_ptr_t &in_player){
		return std::move(std::make_shared<psyop>(in_player));
	}
};
