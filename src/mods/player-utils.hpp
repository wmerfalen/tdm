#ifndef __MENTOC_MODS_PLAYER_UTILS_HEADER__
#define __MENTOC_MODS_PLAYER_UTILS_HEADER__
#include "deferred.hpp"
#include "affects.hpp"

namespace mods::player_utils {
	constexpr static bitvector_t STATUS_INSTALLING = (1 << 0);
	template <typename T>
	static inline bool is_cancel_command(T& arg) {
		return !strcasecmp(arg,"cancel");
	}
	template <>
	inline bool is_cancel_command(const std::string& arg) {
		return !strcasecmp(arg.c_str(),"cancel");
	}
	template <>
	inline bool is_cancel_command(const std::string_view& arg) {
		return !strcasecmp(arg.data(),"cancel");
	}
	template <typename T>
	static inline bool is_installing(T& player) {
		return player->current_block() == mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION;
	}
	template <typename T>
	static inline bool is_scanned(T& player) {
		return player->get_affect_dissolver().has_affect(AFF(SCANNED));
	}
	template <typename T>
	static inline void change_visibility(T& player,int amount) {
		player->visibility() += amount;
	}
	std::pair<uint16_t,uint16_t> get_thermal_vision_range(player_ptr_t& player);
};

#endif
