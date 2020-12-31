#ifndef __MENTOC_MODS_ORM_skill_trees_HEADER__
#define __MENTOC_MODS_ORM_skill_trees_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view skill_trees_table_name = "skill_trees";
	struct skill_trees_record_t {
		uint64_t id;
		std::string skill_name;
		std::string skill_category;
		std::string skill_player_class;
		uint32_t skill_parent_id;
		uint16_t skill_order;
		std::string skill_description;
		strmap_t export_class() {
			strmap_t v;
			v["skill_name"] = skill_name;
			v["skill_category"] = skill_category;
			v["skill_player_class"] = skill_player_class;
			v["skill_parent_id"] = std::to_string(skill_parent_id);
			v["skill_order"] = std::to_string(skill_order);
			v["skill_description"] = skill_description;
			return std::move(v);
		}
		std::string table_name() const {
			return skill_trees_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct skill_trees : public mods::orm::orm_base<skill_trees,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return skill_trees_table_name.data();
		}
		std::string column_prefix() const {
			return "skill_";
		}
		std::string id_column() const {
			return "id";
		}
		skill_trees() {
			this->init();
			loaded = 0;
		}
		~skill_trees() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		void load_multi(const pqxx::result::reference&);
		void populate(std::map<uint16_t,uint16_t>* data);
		std::tuple<int16_t,std::string> delete_by_player_class(std::string_view player_class);
		void init();

		std::tuple<int16_t,std::string> load_all();
		std::tuple<int16_t,std::string> load_by_class(std::string_view);
		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();

		std::vector<skill_trees_record_t> rows;
		bool loaded;
	};
	std::tuple<int16_t,std::string> load_player_class_skill_trees(std::string_view player_class,std::vector<skill_trees_record_t>* storage_ptr);
};

#endif
