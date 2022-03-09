#ifndef __MENTOC_MODS_FORGE_ENGINE_kill_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_kill_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include <memory>
#include "../object-utils.hpp"
#include "../rand.hpp"
#include "value-scaler.hpp"
#include "requirements.hpp"
#include "../elemental.hpp"
#include "../weapon.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::kill]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

extern std::deque<char_data> mob_proto;

namespace mods::forge_engine {
	struct kill_t {
			using mw_rifle = mods::weapon::type::rifle;
			mob_vnum victim;
			player_ptr_t killer;
			kill_t();
			kill_t(player_ptr_t& p,mob_vnum v) : victim(v), killer(p), m_cost(0)  {
				find_mob();
			}
			kill_t(const kill_t& copy);

			~kill_t() = default;
			void find_mob();

			std::optional<char_data> cd();
			int32_t cost();
			void set_cost(int32_t c);
			void add_cost(int32_t c);
			void consume_cost(int32_t c);

		private:
			std::optional<char_data> m_proto;
			bool m_mob_found;
			int32_t m_cost;
	};

};

#endif
