#include "immortal.hpp"
#include "quests.hpp"
#include "pq.hpp"
#include <stdlib.h> //For itoa

namespace mods::immortal{

};
ACMD(do_rnumtele){
	MENTOC_PREAMBLE();	
	constexpr unsigned int max_char = 11;
    std::array<char,max_char> room_id;
	std::fill(room_id.begin(),room_id.end(),0);
	one_argument(argument,&room_id[0],max_char);
	try{
		int r = std::stoi(&room_id[0]);
		char_from_room(player->cd());
		char_to_room(player->cd(),r);
		command_interpreter(player->cd(),"l");
	}catch(const std::exception &e){
		*player << "{red}Invalid room number{/red}\r\n";
	}
}

ACMD(do_rnumlist){
	MENTOC_PREAMBLE();	
	player->pager_start();
	for(unsigned i =0; i < mods::globals::room_list.size();i++){
		if(world[i].name){
			std::string line = "{red}[";
			line += std::to_string(i);
			line += "]{/red}	";
			line += "{gld}{";
			line += world[i].name;
			line += "}{/gld}->";
			line += std::to_string(world[i].number);
			line += "::";
			line += std::to_string(world[i].zone) + "\r\n";
			*player << line;
		}
	}
	player->pager_end();
	player->page(0);
}
