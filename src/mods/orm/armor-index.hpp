#ifndef __MENTOC_MODS_ORM_ARMOR_INDEX_HEADER__
#define __MENTOC_MODS_ORM_ARMOR_INDEX_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct armor_index_record_t {
		std::string armor_filename;
		std::string armor_type;
		strmap_t export_class() {
			strmap_t v;
			v["armor_filename"] = armor_filename;
			v["armor_type"] = armor_type;
			return std::move(v);
		}
		std::string table_name() const {
			return "armor_index";
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct armor_index : public mods::orm::orm_base<armor_index,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return "armor_index";
		}
		std::string column_prefix() const {
			return "armor_";
		}
		std::string id_column() const {
			return "id";
		}
		armor_index() : id(0) {
			this->init();
			loaded = 0;
		}
		~armor_index() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		void load_multi(const pqxx::result::reference&);
		void populate(std::string_view in_armor_type, const std::vector<std::string>& in_armor_filenames);
		void init();

		std::tuple<int16_t,std::string> load_all();
		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();
		using id_type_t = uint64_t;
		using room_vnum_t = uint32_t;

		std::tuple<int16_t,std::string> delete_by_armor_type(std::string_view);

		id_type_t id;
		room_vnum_t event_room_vnum;
		std::string armor_type;
		std::string mob_virtual_number;
		std::vector<armor_index_record_t> rows;
		bool loaded;
	};
	std::tuple<int16_t,std::string> load_all_armor_index_data(std::map<std::string,std::vector<std::string>>* storage_ptr);
};

#endif
