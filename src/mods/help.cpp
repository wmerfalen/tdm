#include "help.hpp"

namespace mods::help {
	bool send_help(std::string_view from, std::shared_ptr<mods::player>& player){
#define M_MATCH(A,SUBJECT) if(matches_many(A,from)){ player->sendln(pages::SUBJECT); return false; }
			M_MATCH("throw,grenade",h_grenade);
			M_MATCH("camera,claymore,install,uninstall",h_install);
			M_MATCH("cancel",h_cancel);
			M_MATCH("set_position",h_set_position);
			M_MATCH("set_npc_position",h_set_npc_position);
			M_MATCH("contract,contracts,quest,quests",h_contract);
#undef M_MATCH
			return true;
	}
	bool should_continue(std::string_view from,std::string_view argument, std::shared_ptr<mods::player>& player,bool zero_is_help){
		bool show = false;
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0 && zero_is_help){
			show = true;
		}
		if(!show && !mods::util::parse_help(argument)){
			return true;
		}
		return send_help(from,player);
	}
};
