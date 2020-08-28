#include "medic.hpp"

namespace mods::classes {
	template <typename TPlayer>
	int scaled_heal(TPlayer& target){
		return (33 * (target->level() * 0.5));
	}
	/*
	int scaled_heal(player_ptr_t& target){
		return (33 * (target->level() * 0.5));
	}
	*/
	void medic::heal_player(player_ptr_t& player_obj,player_ptr_t& target) {
		/*
		if(target->cd()->in_room != player_obj->cd()->in_room) {
			*player_obj << "You can't find your target.\r\n";
			return;
		}

		if(stim_pistol_ammo == 0) {
			*player_obj << "Out of ammo.\r\n";
			return;
		}

		if(target->cd() == player_obj->cd()) {
			const char* self_name = (player_obj->sex() == SEX_MALE ? "himself" : "herself");
			const char* short_self_name = (player_obj->sex() == SEX_MALE ? "his" : "her");
			*player_obj << "You aim your stim pistol at yourself...\r\n";
			send_to_room_except(player_obj->room(),
				{player_obj->cd()},
				"%s aims a stim pistol at %s\r\n",
				player_obj->name().c_str(),self_name);
			*player_obj << "You fire your stim pistol.\r\n";
			send_to_room_except(player_obj->room(),
				{player_obj->cd()},
				"%s fires a stim pistol into %s arm\r\n",
				player_obj->name().c_str(),short_self_name);
			auto amount = scaled_heal(player_obj);
			player_obj->hp() += amount;
			*player_obj << "You gain " << amount << " hit points.\r\n";
			return;
		}

		*player_obj << "You aim your stim pistol at " <<
		            target->name() << " carefully...\r\n";
		const char* gender = (player_obj->cd()->player.sex == SEX_MALE ? "his" : "her");
		send_to_room_except(player_obj->room(),
			{player_obj->cd(),target->cd()},
			"%s aims a stim pistol at %s\r\n",
			player_obj->name().c_str(),
			target->name().c_str()
		);
		*target << player_obj->name() << " aims " << gender <<
		        " stim pistol at you...\r\n";
		*player_obj << "You fire your stim pistol.\r\n";
		*target << player_obj->name() << " fires!\r\n";
		send_to_room_except(player_obj->room(),
			{player_obj->cd(),target->cd()},
			"%s fires a stim pistol at %s\r\n",
			player_obj->name().c_str(),
			target->name().c_str()
		);
		auto amount = scaled_heal(target); 
		target->hp() += amount;
		*target << "You gain " << amount << " hit points.\r\n";
		*/
	}
		std::shared_ptr<medic> create_medic(player_ptr_t &in_player){
			return std::move(std::make_shared<medic>(in_player));
		}
		int16_t medic::new_player(player_ptr_t &player, std::string_view primary_choice){
			using primary = mods::weapon::medic::primary_choice_t;
			auto pchoice = 0;
			if(!primary_choice.compare("AUGPARA")){
				pchoice = primary::MEDIC_PRIMARY_AUGPARA;
			}
			if(!primary_choice.compare("TAR21")){
				pchoice = primary::MEDIC_PRIMARY_TAR21;
			}
			if(pchoice == 0){
				std::cerr << "invalid primary weapon choice for medic class...\n";
				return -1;
			}
			auto db_id = m_orm.initialize_row(player,static_cast<primary>(pchoice));
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
	int16_t medic::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			std::cerr << "unable to load medic class by player id: " << player->db_id() << ".. return status: " << result << "\n";
		}
		return result;
	}
	medic::medic(){
			m_psg1 = nullptr;
			m_l96aw = nullptr;
	}
	medic::medic(player_ptr_t p){
		m_psg1 = nullptr;
		m_l96aw = nullptr;
		load_by_player(p);
	}
	player_ptr_t 	medic::player(){
		return m_player;
	}
};
