#include "player-scaffolding.hpp"

using descriptor_list_t = std::deque<mods::descriptor_data>;
extern descriptor_list_t descriptor_list;		/* master desc list */
namespace mods {
	int player_scaffolding::set_players(std::string_view main_player,
			std::string_view target_player){
		/* TODO: create a function that loads an initializes an entire player
		 * object just by giving the function the player's name
		 */
		return 0;
	}
	std::vector<player_ptr_t>& player_scaffolding::set_players(std::string_view main_player,
			std::string_view target_player,const std::vector<std::string_view>& optional_other_players){
		/* TODO: create a function that loads and initializes an entire player
		 * object just by giving the function the player's name
		 */

		return m_players;
	}
	std::vector<std::string>* player_scaffolding::get_last_error(){
		return &m_errors;
	}
	int player_scaffolding::set_players(player_ptr_t ch,
			player_ptr_t target_player){
		if(m_players.size() < 2){
			m_players.resize(2);
		}
		m_players[player_scaffolding::MAIN_PLAYER_INDEX] = ch;
		m_players[player_scaffolding::TARGET_PLAYER_INDEX] = target_player;
		return 0;
	};


	player_ptr_t player_scaffolding::create_muted_player_target() {
		auto player = std::make_shared<mods::player>(mods::player::player_type_enum_t::PLAYER_MUTED_DESCRIPTOR);
		mods::globals::player_list.emplace_back(
				player
		);
		return player;
	}

	void player_scaffolding::reset_points(player_ptr_t player,std::map<std::string_view,int> * points_map){
		if(points_map != nullptr){
			for(auto & pair : *points_map){
				if(pair.first.compare("mana") == 0){ player->mana() = static_cast<sh_int>(pair.second); continue; }
				if(pair.first.compare("max_mana") == 0){ player->max_mana() = static_cast<sh_int>(pair.second); continue; }
				if(pair.first.compare("hp") == 0){ player->hp() = static_cast<sh_int>(pair.second); continue; }
				if(pair.first.compare("max_hp") == 0){ player->max_hp() = static_cast<sh_int>(pair.second); continue; }
				if(pair.first.compare("move") == 0){ player->move() = static_cast<sh_int>(pair.second); continue; }
				if(pair.first.compare("max_move") == 0){ player->max_move() = static_cast<sh_int>(pair.second); continue; }
				if(pair.first.compare("armor") == 0){ player->armor() = static_cast<sh_int>(pair.second); continue; }
				if(pair.first.compare("gold") == 0){ player->gold() = static_cast<int>(pair.second); continue; }
				if(pair.first.compare("bank_gold") == 0){ player->bank_gold() = static_cast<int>(pair.second); continue; }
				if(pair.first.compare("exp") == 0){ player->exp() = static_cast<int>(pair.second); continue; }
				if(pair.first.compare("hitroll") == 0){ player->hitroll() = static_cast<sbyte>(pair.second); continue; }
				if(pair.first.compare("damroll") == 0){ player->damroll() = static_cast<sbyte>(pair.second); continue; }
			}
		}else{
			player->move() = 9999;
			player->max_move() = 9999;
			player->hp() = 500;
			player->max_hp() = 500;
			player->mana() = 500;
			player->max_mana() = 500;
			player->armor() = 100;
			player->gold() = 100;
			player->bank_gold() = 100;
			player->exp() = 100;
			player->hitroll() = 3;
			player->damroll() = 3;
		}
	}


	player_ptr_t player_scaffolding::create_player() {
		auto player = std::make_shared<mods::player>(mods::player::player_type_enum_t::PLAYER);
		mods::globals::player_list.emplace_back(
				player
				);
		return player;
	}

	int player_scaffolding::initialize_player(player_ptr_t player,int to_room){
		descriptor_list.emplace_back();
		auto descriptor = descriptor_list.end() -1;
		player->set_desc(descriptor);
		player->desc().character = player->cd();
		/** TODO: !uncertainty! -> question: "Does this need to be uncommented?" */
		player->set_char_on_descriptor(descriptor);
		auto ch = player->cd();
		ch->drone = false;
		/** TODO: Check if we need to manually assign ch->desc */
		//I'm pretty sure this is taken care of by set_char_on_descriptor
		//ch->desc = std::make_shared<mods::descriptor_data>(*descriptor;
		ch->drone_owner = 0;
		ch->player.name.assign("A player_scaffolding");
		//owner->drone_uuid = 0;
		//owner->drone_simulate = false;
		//IN_ROOM(ch) = IN_ROOM();
		ch->points.move = 9999;
		ch->points.max_move = 9999;
		ch->points.hit = 500;
		ch->points.max_hit = 500;
		ch->points.mana = 500;
		ch->points.max_mana = 500;
		ch->points.armor = 100;
		ch->points.gold = 100;
		ch->points.bank_gold = 100;
		ch->points.exp = 100;
		ch->points.hitroll = 3;
		ch->points.damroll = 3;


		//memset(&ch->char_specials,0,sizeof(ch->char_specials));
		ch->char_specials.position = POS_STANDING;
		ch->char_specials.saved.idnum = ch->uuid;
		REMOVE_BIT(MOB_FLAGS(ch),MOB_ISNPC);
		char_from_room(*player);
		char_to_room(*player,to_room);
		//ch->in_room = 0;
		//SET_BIT(ch->char_specials.saved.affected_by,AFF_SNEAK);
		//SET_BIT(ch->player_specials->saved.pref, PRF_AUTOEXIT);
		return 0;
	}
	/*
		 void player_scaffolding::start(struct char_data* owner) {
		 auto player_scaffolding = mods::player_scaffolding::get_existing(owner);

		 if(player_scaffolding == nullptr) {
		 player_scaffolding = mods::player_scaffolding::create(owner);
		 }

		 if(IN_ROOM(player_scaffolding) != NOWHERE) {
		 char_from_room(player_scaffolding);
		 }

		 char_to_room(player_scaffolding,IN_ROOM(owner));
		 owner->player_scaffolding_simulate = true;
		 }
		 void player_scaffolding::stop(struct char_data* owner) {
		 owner->player_scaffolding_simulate = false;
		 }
		 bool player_scaffolding::started(struct char_data* owner) {
		 return owner->player_scaffolding_simulate;
		 }
		 void player_scaffolding::simulate(struct char_data* owner,bool value) {
		 owner->player_scaffolding_simulate = value;
		 }
		 void player_scaffolding::get_player_scaffolding(struct char_data* owner) {
		 auto player_scaffolding = mods::globals::player_list[owner->player_scaffolding_uuid]->cd();

		 if(IN_ROOM(owner) == IN_ROOM(player_scaffolding)) {
		 if(IN_ROOM(player_scaffolding) != NOWHERE) {
		 char_from_room(player_scaffolding);
		 }

		 char_to_room(player_scaffolding,NOWHERE);
		 } else {
		 send_to_char(owner,"Must be in the same room as your player_scaffolding\r\n");
		 }
		 }
		 struct char_data * player_scaffolding::get_existing(struct char_data* owner) {
		 if(owner->player_scaffolding_uuid == 0 || owner->player_scaffolding_uuid > mods::globals::player_list.size() -1) {
		 return nullptr;
		 }

		 return mods::globals::player_list[owner->player_scaffolding_uuid]->cd();
		 }

		 bool player_scaffolding::interpret(struct char_data *owner,const std::string& argument) {
		 auto player_scaffolding = mods::globals::player_list[owner->player_scaffolding_uuid];

		 if(argument.compare("player_scaffolding stop") == 0) {
		 mods::player_scaffolding::stop(owner);
		 return true;
		 }

		 if(argument.compare("scan") == 0) {
		 for(auto & ch : mods::globals::room_list[player_scaffolding->cd()->in_room]){
		 mods::player(ch) << "You have been spotted.\r\n";
		 }
		 return false;
		 }

		 switch(argument[0]) {
		 case 'n':
		 case 'e':
		 case 's':
		 case 'w':
		 case 'N':
		 case 'E':
		 case 'S':
		 case 'W':
		 case 'l':
		 command_interpreter(player_scaffolding->cd(),argument.substr(0,1).c_str());
return false;

default:
return false;
}
}
*/
};

