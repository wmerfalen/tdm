#include "help.hpp"

namespace mods::help {
	bool should_continue(std::string_view from,std::string_view argument, std::shared_ptr<mods::player>& player){
#define M_MATCH(A,SUBJECT) if(matches_many(A,from)){ player->sendln(pages::SUBJECT); return false; }
		if(!mods::util::parse_help(argument)){
			return true;
		}
		M_MATCH("throw,grenade",h_grenade);
		M_MATCH("camera,claymore,install,uninstall",h_install);
		M_MATCH("cancel",h_cancel);
		return true;
#undef M_MATCH
	}
};
