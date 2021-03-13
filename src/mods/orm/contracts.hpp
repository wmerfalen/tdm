#ifndef __MENTOC_MODS_ORM_contracts_HEADER__
#define __MENTOC_MODS_ORM_contracts_HEADER__

#include "orm-base.hpp"
#include "../contract-types.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view contracts_table_name = "contracts";
	using contract_vnum_t = uint32_t;

	struct contracts : public mods::orm::orm_base<contracts,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return contracts_table_name.data();
		}
		std::string column_prefix() const {
			return "c_";
		}
		std::string id_column() const {
			return "id";
		}
		contracts() {
			init();
			loaded = 0;
			id = 0;
		}
		contracts(const pqxx::row& row) {
			init();
			id = row["id"].as<uint64_t>();
			c_vnum = row["c_vnum"].as<contract_vnum_t>();
			c_description = row["c_description"].c_str();
			c_title = row["c_title"].c_str();
			created_at = row["created_at"].as<long>();
			updated_at = row["updated_at"].as<long>();
			loaded = true;
		}
		~contracts() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value();
		std::tuple<int16_t,std::string> delete_by_contract_vnum(const contract_vnum_t& c_vnum);

		void init();
		void feed_multi(pqxx::result&);

		strmap_t export_class() {
			strmap_t v;
			v["c_vnum"] = std::to_string(c_vnum);
			v["c_description"] = c_description;
			v["c_title"] = c_title;
			return std::move(v);
		}
		int16_t save();

		bool loaded;

		uint64_t id;
		contract_vnum_t c_vnum;
		std::string c_description;
		std::string c_title;
		long created_at;
		long updated_at;
	};
	std::tuple<int16_t,std::string> load_all_contracts(std::deque<std::shared_ptr<mods::contracts::contract>>* list);
};

#endif
