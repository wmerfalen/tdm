#ifndef __MENTOC_MODS_COMMANDS_HEADER__
#define __MENTOC_MODS_COMMANDS_HEADER__

//#define __MENTOC_MODS_MOBS_HELPERS_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_HELPERS_SHOW_DEBUG_OUTPUT__
	#define mmh_debug(a) mentoc_prefix_debug("mods::commands") << a << "\n";
#else
	#define mmh_debug(a) ;;
#endif
namespace mods::commands {
	template <typename T>
	static inline void report(player_ptr_t& player,T s) {
		if(!std::get<0>(s)) {
			player->errorln(std::get<1>(s));
			return;
		}
		player->sendln(std::get<1>(s));
	}
};//end namespace
#endif
