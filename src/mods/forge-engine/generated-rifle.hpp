#ifndef __MENTOC_MODS_FORGE_ENGINE_GENERATED_RIFLE_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_GENERATED_RIFLE_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include "value-scaler.hpp"
#include "requirements.hpp"
#include "generator.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generated_rifle]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif


namespace mods::forge_engine {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	void rifle_index_changed();
	using kill_t = mods::forge_engine::kill_t;

	struct generated_rifle_t {
			generated_rifle_t() = delete;
			generated_rifle_t (kill_t& player);
			obj_ptr_t roll();
			void send_stats_to_player(player_ptr_t& player);
			std::string get_dump();
			void fill(obj_ptr_t& rifle);
			void fill_attributes(obj_ptr_t& rifle);
			void fill_elemental(obj_ptr_t& rifle);
			void fill_stats(obj_ptr_t& rifle);
			const std::vector<std::string>& yaml_list(rifle_types_t);
			void load_from_sql();
		private:
			kill_t m_player;
			rifle_types_t m_type;
			requirements_t m_requirements;
			std::vector<std::pair<rifle_attributes_t,std::variant<uint32_t,float>>> m_attributes;
			std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> m_elemental_damages;
			std::vector<std::pair<stat_types_t,std::variant<uint32_t,float>>> m_stat_boosts;
			obj_ptr_t m_instance;
			bool compute_weapon_cost();
	};

};

#undef m_debug
#endif
