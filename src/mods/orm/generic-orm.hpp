#ifndef __MENTOC_MODS_ORM_generic_orm_HEADER__
#define __MENTOC_MODS_ORM_generic_orm_HEADER__

#define __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using sql_response_t = std::tuple<int16_t,std::string>;

	struct where_clause_t {
		std::string field;
		std::string op;
		std::string value;
	};
	struct and_clause_t {
		static constexpr bool and_clause = true;
		static constexpr bool or_clause = false;
	};
	struct or_clause_t {
		static constexpr bool or_clause = true;
		static constexpr bool and_clause = false;
	};

	template <typename ...T>
	struct clause_t {
		...T;
	};

	struct player_associated_orm_t {
		static constexpr std::array<clause_t,3> update_points_criteria = {
			where_clause_t{"n_player_id","=","n_player_id"},
			and_clause_t{},
			where_clause_t{"n_name","=","n_name"},
		};
		std::map<std::string,std::string>& export_class();
		void update_using(const criteria_t& crit);
		player_associated_orm_t(std::string_view player_id_column);
#if 0
		static constexpr std::string_view table_name = "notch";
		static constexpr std::string_view column_prefix = "n_";
		// Should be able to be omitted
		static constexpr std::string_view id_column = "id";
		static constexpr std::string_view player_id_column = "n_player_id";
		static constexpr std::array<std::string_view,1> string_columns = {
			"n_name"
		};
		static constexpr std::array<std::string_view,3> uint64_columns = {
			"id",
			"n_points",
			"n_player_id",
		};
		static constexpr std::array<std::string_view,0> bool_columns = {
		};

		static constexpr std::array<std::string_view,2> date_columns = {
			"created_at",
			"updated_at",
		};

		sql_response_t run_create_table();


		/** Accept uint64_t, player_ptr_t, pqxx::result (accesses n_player_id) */
		std::tuple<int16_t,std::string> delete_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player_and_notch(const uint64_t& player_id,std::string_view notch);

		bool loaded;

		std::string n_name;
		uint16_t n_points;
		uint64_t n_player_id;
		uint64_t id;
#endif
	};
};

#endif
