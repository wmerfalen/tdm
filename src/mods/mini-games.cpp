#include "mini-games.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "rand.hpp"

namespace mods::mini_games {
	static std::map<room_vnum,std::deque<line_up>> devices;
	line_up& game(room_vnum vnum,int id){
		return devices[vnum][id];
	}
	void init(){
		devices[128].emplace_back();
		devices[128].emplace_back();
		devices[128].emplace_back();
		devices[128].emplace_back();
		devices[128].emplace_back();
		int ctr=0;
		for(auto & game : devices[128]){
			game.set_device_id(ctr++);
		}
	}
	int line_up::device_id(){
		return m_device_id;
	}
	void line_up::set_device_id(int i){
		m_device_id = i;
	}

	bool device_exists(room_vnum vnum,int device_id){
		auto found = devices.find(vnum);
		if(found == devices.end()){
			return false;
		}
		return devices[vnum].size() > device_id;
	}
	void list_devices(player_ptr_t& player){
		if(devices.find(player->vnum()) == devices.end()){
			player->sendln("No devices in this room");
			return;
		}
		player->send("Listing devices...\r\n");
		for(auto & hackable : devices[player->vnum()]){
			player->sendln(CAT("device #", std::to_string(hackable.device_id()), "\r\n"));
		}
		player->sendln("Done listing devices.");
	}
	int random_odd_number(int from,int to){
		int r = rand_number(from,to);
		if((r % 2)){
			return r;
		}
		return random_odd_number(from,to);
	}
	int random_even_number(int from,int to){
		int r = rand_number(from,to);
		if(!(r % 2)){
			return r;
		}
		return random_even_number(from,to);
	}
	int line_up::row_count(){
		return solutions.size();
	}
	line_up::line_up() {
		seed();
		needs_regen = true;
		highlight_strategy = HIGHLIGHT_ODD_NUMBER;
	}
	void line_up::seed(){
		int from = FROM;
		int to=TO;
		position_count = rand_number(from,to);

		from = 1;
		to = 255;
		solutions.clear();
		positions.clear();
		rows.clear();
		first_values.clear();
		for(int i=0; i < position_count;i++){
			solutions.emplace_back(rand_number(1,position_count));
		}
		for(int p = 0; p < position_count; p++){
			std::vector<uint8_t> row;
			row.reserve(position_count);
			int odd_number = random_odd_number(from,to);
			row.emplace_back(odd_number);
			for(int i=1; i < position_count; i++){
				row.emplace_back(random_even_number(from,to));
			}
			int current_solution = solutions[p];
			int odd_position = -1;
			std::random_device rd;
			std::mt19937 g(rd());
			do {
				std::shuffle(row.begin(),row.end(),g);
				int ictr = 1;
				for(auto c : row){
					if((c % 2) == 1){
						odd_position = ictr;
						break;
					}
					ictr++;
				}
			}while(odd_position == current_solution);
			first_values.emplace_back(row[0]);
			rows.emplace_back(row);
		}
		original_rows = rows;
	}
	bool line_up::should_highlight(int row, uint8_t value){
		switch(highlight_strategy){
			case HIGHLIGHT_EVEN_NUMBER:
				return !(value % 2);
			case HIGHLIGHT_FIRST_NUMBER:
				return first_values[row] == value;
			case HIGHLIGHT_ODD_NUMBER:
				return (value % 2);
			default:
				return false;
		}
	}
	std::string line_up::get_body(){
		if(!needs_regen){
			return body_content;
		}
		body_content = "[solutions]\r\n";
		for(auto & s : solutions){
			body_content += "[";
			body_content += std::to_string(s);
			body_content += "]";
		}

		body_content += "\r\n";
		int ctr = 1;
		int row_counter = 0;
		for(auto & row : rows){
			body_content += "[row ";
			body_content += std::to_string(ctr++) + "]->";
			for(auto n : row){
				if(should_highlight(row_counter,n)){
					body_content += "{grn}";
				}
				body_content += "[";
				body_content += std::to_string(n);
				body_content += "] ";
				if(should_highlight(row_counter,n)){
					body_content += "{/grn}";
				}
			}
			++row_counter;
			body_content += "\r\n";
		}
		needs_regen = false;
		return body_content;
	}
	std::string line_up::invalid_row(){
		return std::string("{red}Invalid row{/red}\r\n") + get_body().data();
	}
	std::string line_up::rotate_left(int in_row){
		if(in_row == 0){
			in_row = 1;
		}
		int row = in_row -1;
		if(row < 0){
			return invalid_row();
		}
		if(row >= position_count){
			return invalid_row();
		}
		std::vector<uint8_t> items;
		items.reserve(position_count);
		bool first = true;
		for(auto & i : rows[row]){
			if(first){
				first = false;
				continue;
			}
			items.emplace_back(i);
		}
		items.emplace_back(rows[row].front());
		rows[row] = items;
		needs_regen = true;
		return get_body();
	}
	void line_up::reset(){
		rows = original_rows;
		needs_regen = true;
		get_body();
	}
	std::string line_up::rotate_right(int in_row){
		if(in_row == 0){
			in_row = 1;
		}
		int row = in_row -1;
		if(row < 0){
			return invalid_row();
		}
		if(row >= position_count){
			return invalid_row();
		}
		std::vector<uint8_t> items;
		items.reserve(position_count);
		items.emplace_back(rows[row].back());
		for(auto & i : rows[row]){
			items.emplace_back(i);
		}
		items.pop_back();
		rows[row] = items;
		needs_regen = true;
		return get_body();
	}

	void rotate_right(player_ptr_t& player){
		auto t = player->currently_hacking();
		auto id = std::get<0>(t); auto row = std::get<1>(t);
		if(!mods::mini_games::device_exists(player->vnum(),id)){
			player->sendln("There is no device to hack in this room that goes by that ID");
			return;
		}
		player->send("Current row value: %d/%d\r\n", row,mods::mini_games::game(player->vnum(),id).row_count());
		player->sendln(mods::mini_games::game(player->vnum(),id).rotate_right(row));
	}
	void rotate_left(player_ptr_t& player){
		auto t = player->currently_hacking();
		auto id = std::get<0>(t); auto row = std::get<1>(t);
		if(!mods::mini_games::device_exists(player->vnum(),id)){
			player->sendln("There is no device to hack in this room that goes by that ID");
			return;
		}
		player->send("Current row value: %d/%d\r\n", row,mods::mini_games::game(player->vnum(),id).row_count());
		player->sendln(mods::mini_games::game(player->vnum(),id).rotate_left(row));
	}
	void reset_hack(player_ptr_t& player){
		auto t = player->currently_hacking();
		auto id = std::get<0>(t);
		if(!mods::mini_games::device_exists(player->vnum(),id)){
			player->sendln("There is no device to hack in this room that goes by that ID");
			return;
		}
		mods::mini_games::game(player->vnum(),id);
	}
	void next_row(player_ptr_t& player){
		auto t = player->currently_hacking();
		auto id = std::get<0>(t);
		if(!mods::mini_games::device_exists(player->vnum(),id)){
			player->sendln("There is no device to hack in this room that goes by that ID");
			return;
		}
		auto row = std::get<1>(t);
		row += 1;
		if(row > mods::mini_games::game(player->vnum(),id).row_count()){
			row = 1;
		}
		player->set_currently_hacking(id,row);
		player->send("Current row value: %d/%d\r\n", row,mods::mini_games::game(player->vnum(),id).row_count());
		player->sendln(mods::mini_games::game(player->vnum(),id).get_body());
	}
};

ACMD(do_install_minigame) {
	DO_HELP("install_minigame");
	ADMIN_REJECT();
	ADMIN_DONE();
}
ACMD(do_uninstall_minigame) {
	DO_HELP("install_minigame");
	ADMIN_REJECT();
	ADMIN_DONE();
}
ACMD(do_list_minigame) {
	DO_HELP("list_minigame");
	ADMIN_REJECT();
	ADMIN_DONE();
}
ACMD(do_hack){
	DO_HELP("hack");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0){
		player->send("Which device would you like to hack?\r\n");
		mods::mini_games::list_devices(player);
		return;
	}
	if (!mods::mini_games::device_exists(player->vnum(), mods::util::stoi(vec_args[0]).value_or(-1))){
		player->sendln("That device doesn't exist in this room.");
		return;
	}
	player->set_currently_hacking(mods::util::stoi(vec_args[0]).value_or(0),0);
}
ACMD(do_rotate_right) {
	DO_HELP("rotate_right");
	mods::mini_games::rotate_right(player);
}
ACMD(do_rotate_left) {
	DO_HELP("rotate_left");
	mods::mini_games::rotate_left(player);
}
ACMD(do_next_row) {
	DO_HELP("next_row");
	mods::mini_games::next_row(player);
}
ACMD(do_reset_hack) {
	DO_HELP("reset_hack");
	mods::mini_games::reset_hack(player);
}
