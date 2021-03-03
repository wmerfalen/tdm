#ifndef __MENTOC_MODS_ORM_player_contract_state_HEADER__
#define __MENTOC_MODS_ORM_player_contract_state_HEADER__

#include "orm-base.hpp"
#include "../contract-types.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view player_contract_state_table_name = "player_contract_state";
	using contract_vnum_t = uint32_t;

	struct player_contract_state : public mods::orm::orm_base<player_contract_state,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return player_contract_state_table_name.data();
		}
		std::string column_prefix() const {
			return "pc_";
		}
		std::string id_column() const {
			return "id";
		}
		player_contract_state() {
			init();
			loaded = 0;
			id = 0;
		}
		player_contract_state(const pqxx::row& row) {
			init();
			id = row["id"].as<uint64_t>();
			pc_contract_vnum = row["pc_contract_vnum"].as<contract_vnum_t>();
			pc_state_data = row["pc_state_data"].c_str();
			pc_player_id = row["pc_player_id"].as<uint64_t>();
			created_at = row["created_at"].as<long>();
			updated_at = row["updated_at"].as<long>();
			loaded = true;
		}
		~player_contract_state() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value();
		std::tuple<int16_t,std::string> delete_by_contract_vnum(const contract_vnum_t& c_vnum);

		void init();
		void feed_multi(pqxx::result&);

		strmap_t export_class() {
			strmap_t v;
			v["pc_contract_vnum"] = std::to_string(pc_contract_vnum);
			v["pc_player_id"] = std::to_string(pc_player_id);
			v["pc_state_data"] = pc_state_data;
			return std::move(v);
		}
		int16_t save();

		bool loaded;

		uint64_t id;
		contract_vnum_t pc_contract_vnum;
		std::string pc_state_data;
		uint64_t pc_player_id;
		long created_at;
		long updated_at;
	};
	std::tuple<int16_t,std::string> load_player_contract_state(player_ptr_t& player,contract_vnum_t c_vnum,std::string& in_buffer);
};

#endif
