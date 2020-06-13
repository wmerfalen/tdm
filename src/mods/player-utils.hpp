#ifndef __MENTOC_MODS_PLAYER_UTILS_HEADER__
#define __MENTOC_MODS_PLAYER_UTILS_HEADER__
#include "deferred.hpp"

namespace mods::player_utils {
	constexpr static bitvector_t STATUS_INSTALLING = (1 << 0);
	template <typename T>
	static inline bool is_cancel_command(T& arg){
		return !strcasecmp(arg,"cancel");
	}
	template <>
		inline bool is_cancel_command(const std::string& arg){
			return !strcasecmp(arg.c_str(),"cancel");
		}
	template <>
		inline bool is_cancel_command(const std::string_view& arg){
			return !strcasecmp(arg.data(),"cancel");
		}
	template <typename T>
		static inline bool is_installing(T& player){
			 return player->current_block() == mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION;
		}
	template <typename T>
	static inline bool is_scanned(T& player){
		return player->get_affect_dissolver().has_affect(AFF(SCANNED));
	}
};

#endif