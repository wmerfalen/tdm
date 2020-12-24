#ifndef __MENTOC_MODS_FORGE_ENGINE_GENERATED_ARMOR_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_GENERATED_ARMOR_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include "value-scaler.hpp"
#include "requirements.hpp"
#include "generator.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generated_armor]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	struct generated_armor_t {
			generated_armor_t() = delete;
			generated_armor_t (player_ptr_t& player);
			obj_ptr_t roll();
			void send_stats_to_player(player_ptr_t& player);
			std::string get_dump();
			void fill(obj_ptr_t& armor);
			void fill_attributes(obj_ptr_t& armor);
			void fill_elemental(obj_ptr_t& armor);
			void fill_stats(obj_ptr_t& armor);
		private:
			player_ptr_t m_player;
			armor_types_t m_type;
			requirements_t m_requirements;
			std::vector<std::pair<armor_attributes_t,std::variant<uint32_t,float>>> m_attributes;
			std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> m_elemental_damages;
			std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> m_elemental_resistances;
			std::vector<std::pair<stat_types_t,std::variant<uint32_t,float>>> m_stat_boosts;
			obj_ptr_t m_instance;
	};

};

#undef m_debug
#endif
