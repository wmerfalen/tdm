#include "medic.hpp"
#include "../bugs-fixtures.hpp"

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
	int16_t medic::new_player(player_ptr_t &player, primary_choice_t primary_choice){
		auto db_id = m_orm.initialize_row(player,primary_choice);
		if(db_id == 0){
			report({"medic::new_player. failed to initialize_row for player:",player->name().c_str()});
			return -2;
		}
		player->set_db_id(db_id);
		load_by_player(player);
		return 0;
	}
	int16_t medic::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		return result;
		if(result < 0){
			report({"unable to load medic class by player id: ",std::to_string(player->db_id()),".. return status: ",std::to_string(result),"player:",player->name().c_str()});
			return -100 - result;
		}
		obj_ptr_t primary = nullptr;
		switch(m_orm.primary_type()){
			case primary_choice_t::AUGPARA:
				primary = create_object(ITEM_RIFLE,"augpara.yml");
				break;
			case primary_choice_t::TAR21:
				primary = create_object(ITEM_RIFLE,"tar21.yml");
				break;
			default:
				report({"Unknown medic primary type detected...",player->name().c_str()});
				primary = nullptr;
				return -200;
		}
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
		return result;
	}
	medic::medic(){
	}
	medic::medic(player_ptr_t p){
		load_by_player(p);
	}
	player_ptr_t 	medic::player(){
		return m_player;
	}
};
