#ifndef __MENTOC_MODS_COMMANDS_HEADER__
#define __MENTOC_MODS_COMMANDS_HEADER__

//#define __MENTOC_MODS_MOBS_HELPERS_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_HELPERS_SHOW_DEBUG_OUTPUT__
	#define mmh_debug(a) mentoc_prefix_debug("mods::player_helpers") << a << "\n";
#else
	#define mmh_debug(a) ;;
#endif
namespace mods::commands {
	void report(player_ptr_t& player,std::tuple<bool,std::string> s) {
		if(!std::get<0>(s)) {
			player->errorln(std::get<1>(s));
			return;
		}
		player->sendln(std::get<1>(s));
	}
};//end namespace
#endif
