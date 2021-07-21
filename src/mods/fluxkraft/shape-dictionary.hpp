#ifndef __MENTOC_MODS_FLUXKRAFT_shape_dictionary_HEADER__
#define __MENTOC_MODS_FLUXKRAFT_shape_dictionary_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include <memory>
#include "../object-utils.hpp"
#include "../rand.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::fluxkraft::shape_dictionary]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::fluxkraft {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using random_number_type_t = uint64_t;

	struct shape_dictionary {
			using exponential_distr_type_t = std::exponential_distribution<double>;
			using distr_type_t = std::uniform_int_distribution<uint32_t>;

			shape_dictionary();
			~shape_dictionary();

		private:
			void init();
	};
	void generate_horizonal_line_at(player_ptr_t player,int x, int y, int z, std::size_t length);
};

#undef m_debug
#endif
