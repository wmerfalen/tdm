#ifndef __MENTOC_MODS_FORGE_ENGINE_VALUE_SCALER_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_VALUE_SCALER_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include <memory>

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::value_scaler]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {

	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using random_number_type_t = uint64_t;
	struct value_scaler {
		player_ptr_t player;
		float level;
		float low;
		float high;
		float low_low;
		float stat_low;
		float stat_high;
		uint32_t low_level;
		uint32_t high_level;
		uint32_t low_low_clamp;
		uint32_t low_high_clamp;
		uint32_t high_low_clamp;
		uint32_t high_high_clamp;
		uint32_t uint_low;
		uint32_t uint_high;
		uint8_t max_stats;
		~value_scaler();
		value_scaler() = delete;
		/** TODO: game balancing: these values need to be tweaked */
		value_scaler(player_ptr_t& player);

		void calculate(player_ptr_t& player);
		void dump();
		std::string get_dump();
	};

};

#undef m_debug
#endif
