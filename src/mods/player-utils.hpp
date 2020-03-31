#ifndef __MENTOC_MODS_PLAYER_UTILS_HEADER__
#define __MENTOC_MODS_PLAYER_UTILS_HEADER__
#include "deferred.hpp"

namespace mods::player_utils {
	constexpr static bitvector_t STATUS_INSTALLING = (1 << 0);
	template <typename T>
	static inline bool is_cancel_command(T& arg){
		return !strcmp(arg,"cancel");
	}
	template <>
		inline bool is_cancel_command(const std::string& arg){
			return !arg.compare("cancel");
		}
	template <>
		inline bool is_cancel_command(const std::string_view& arg){
			return !arg.compare("cancel");
		}
	template <typename T>
		static inline bool is_installing(T& player){
			 return player->current_block() == mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION;
		}
};

#endif
