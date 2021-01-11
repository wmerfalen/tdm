#ifndef __MENTOC_MODS_ORM_player_skill_usage_HEADER__
#define __MENTOC_MODS_ORM_player_skill_usage_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view player_skill_usage_table_name = "player_skill_usage";
	struct player_skill_usage_record_t {
		uint64_t id;
		uint64_t skill_id;
		uint16_t usage_count;
		uint64_t player_id;
		strmap_t export_class() {
			strmap_t v;
			v["ps_skill_id"] = std::to_string(this->skill_id);
			v["ps_usage_count"] = std::to_string(this->usage_count);
			v["ps_player_id"] = std::to_string(this->player_id);
			return std::move(v);
		}
		std::string table_name() const {
			return player_skill_usage_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct player_skill_usage : public mods::orm::orm_base<player_skill_usage,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return player_skill_usage_table_name.data();
		}
		std::string column_prefix() const {
			return "ps_";
		}
		std::string id_column() const {
			return "id";
		}
		player_skill_usage() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~player_skill_usage() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value();

		void load_multi(const pqxx::result::reference&);
		void populate(const uint64_t& player_id, const std::map<uint64_t,uint16_t>& data);
		std::map<uint64_t,uint16_t> get_player_levels(const uint64_t& player_id, std::string_view player_class);
		void init();

		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();

		std::tuple<int16_t,std::string> delete_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player(const uint64_t& player_id);

		std::vector<player_skill_usage_record_t> rows;
		bool loaded;

		uint64_t primary_key_id;
		uint64_t skill_id;
		uint16_t usage_count;
		uint64_t player_id;
	};
	std::tuple<int16_t,std::string> load_player_skill_usage_data(player_ptr_t& player, std::map<uint64_t,uint16_t>* data);
	std::tuple<int16_t,std::string> sync_player_skill_usage(const uint64_t& player_id, std::map<uint64_t,uint16_t>& usage);
	void player_skill_usage_upkeep(player_ptr_t& player);
	void player_skill_usage_upkeep(const uint64_t& player_id, std::string_view c);
};

#endif
