#ifndef __MENTOC_MODS_ORM_RIFLE_INDEX_HEADER__
#define __MENTOC_MODS_ORM_RIFLE_INDEX_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct rifle_index_record_t {
		std::string rifle_filename;
		std::string rifle_type;
		strmap_t export_class() {
			strmap_t v;
			v["rifle_filename"] = rifle_filename;
			v["rifle_type"] = rifle_type;
			return std::move(v);
		}
		std::string table_name() const {
			return "rifle_index";
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct rifle_index : public mods::orm::orm_base<rifle_index,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return "rifle_index";
		}
		std::string column_prefix() const {
			return "rifle_";
		}
		std::string id_column() const {
			return "id";
		}
		rifle_index() : id(0) {
			this->init();
			loaded = 0;
		}
		~rifle_index() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		void load_multi(const pqxx::result::reference&);
		void populate(std::string_view in_rifle_type, const std::vector<std::string>& in_rifle_filenames);
		void init();

		std::tuple<int16_t,std::string> load_all();
		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();
		using id_type_t = uint64_t;
		using room_vnum_t = uint32_t;

		std::tuple<int16_t,std::string> delete_by_rifle_type(std::string_view);
		std::tuple<int16_t,std::string> delete_by_mob_vnum(mob_vnum);

		id_type_t id;
		room_vnum_t event_room_vnum;
		std::string rifle_type;
		std::string mob_virtual_number;
		std::vector<rifle_index_record_t> rows;
		bool loaded;
	};
	std::tuple<int16_t,std::string> load_all_rifle_index_data(std::map<std::string,std::vector<std::string>>* storage_ptr);
};

#endif
