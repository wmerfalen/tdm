#include "wires.hpp"
#include "../help.hpp"
#include "../super-users.hpp"
#include "../rand.hpp"
#include "../orm/mini-game.hpp"
#include "../util.hpp"

namespace mods::mini_games {
	int wires::device_id(){
		return m_device_id;
	}
	void wires::set_device_id(int i){
		m_device_id = i;
	}

	wires::wires() {
		seed();
		needs_regen = true;
		current_row = 0;
	}
	void wires::next_row(){
		++current_row;
		if(current_row == ROWS){
			current_row = 0;
		}
		needs_regen = true;
		get_body();
	}
	void wires::seed(){
		int first_solution = rand_number(FROM,TO);
		int second_solution = rand_number(FROM,TO);
		solution = {first_solution,second_solution};
		std::vector<uint8_t> beeps_row_1 = {0,1,2};
		std::vector<uint8_t> beeps_row_2 = {0,1,2};
		mods::util::shuffle(beeps_row_1);
		mods::util::shuffle(beeps_row_2);
		plugged_in.clear();
		for(int i = 0; i < TO; i++){
			beeps.emplace_back(beeps_row_1[i]);
			plugged_in.emplace_back(0);
		}
		for(int i = 0; i < TO; i++){
			beeps.emplace_back(beeps_row_2[i]);
			plugged_in.emplace_back(0);
		}
		needs_regen = 1;
		get_body();
	}
	std::string wires::get_body(){
		if(!needs_regen){
			return body_content;
		}
		body_content = HEADER.data();
		body_content += "\r\n";
		body_content += "[PORTS]\r\n";
		int iCtr = 0;
		for(int i ='A';i < 'F';i++, iCtr++){
			if(plugged_in[iCtr]){
				body_content += "[ * -- ";
			}else{
				body_content += "[ O -- ";
			}
			body_content += (char)i;
			body_content += "]";
			if(plugged_in[iCtr]){
				body_content += "[plugged in]";
			}else{
				body_content += "[open]";
			}
			body_content += "{/grn}\r\n";
		}
		body_content += "\r\n";
		body_content += DISPLAY.data();
		body_content += CAT("[",std::to_string(std::get<0>(solution)),",",std::to_string(std::get<1>(solution)),"]\r\n");
		body_content += CAT("[current row:",std::to_string(1 + current_row), "]\r\n");
		body_content += CAT(FOOTER.data(),"\r\n");
		needs_regen = false;
		return body_content;
	}
	std::string invalid_plug(){
		return "That plug doesn't exist. Choose a valid plug.\r\n";
	}
	std::string wires::plug_into(char port){
		uint8_t plug = ((uint8_t)'A') - (uint8_t)(port);
		if(plug > ROWS * COLUMNS){
			return invalid_plug();
		}
		return get_body();
	}
	void wires::unplug_all() {
		std::fill(plugged_in.begin(),plugged_in.end(),0);
	}
	void wires::reset(){
		current_row = 0;
		unplug_all();
		needs_regen = true;
		get_body();
	}

	void plug_cable(player_ptr_t& player){
		auto t = player->currently_hacking();
		auto id = std::get<0>(t);
		auto row = std::get<1>(t);
		if(!mods::mini_games::device_exists(player->vnum(),id)){
			player->sendln("There is no device to hack in this room that goes by that ID");
			return;
		}
		player->send("Current row value: %d/%d\r\n", row,mods::mini_games::game(player->vnum(),id).row_count());
		player->sendln(mods::mini_games::game(player->vnum(),id).rotate_left(row));
	}
};

ACMD(do_plug_cable){
	DO_HELP("plug_cable");
	mods::mini_games::plug_cable(player);
}
