#ifndef __MENTOC_MODS_ORM_ROOM_HEADER__
#define __MENTOC_MODS_ORM_ROOM_HEADER__

#include <iostream>
#include <vector>
#include "../../structs.h"
#include "base.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using sql_save_map_t = std::map<std::string,std::string>;

	struct room : public base {
		constexpr static const char* primary_key = "id";
		std::string primary_key_name() const {
			return "id";
		}
		std::string primary_key_value() {
			return std::to_string(id);
		}
		std::string table_name() {
			return "room";
		}

		room() : base(), id(0) {
			this->init();
			loaded = 0;
		}
		room(const pqxx::result::reference&);
		~room() = default;
		uint64_t initialize_row();
		int16_t feed(const pqxx::result::reference&);
		void init();
		std::string dump_cols(std::vector<std::string> columns);
		std::string dump_cols();
		sql_save_map_t export_class();
		sql_save_map_t export_insert();

		int16_t				load_by_vnum(room_vnum);

		uint64_t id;
		room_vnum room_number;
		zone_vnum zone;
		uint32_t sector_type;
		std::string name;
		std::string description;
		std::string ex_keyword;
		std::string ex_description;
		uint64_t light;
		uint64_t room_flag;
		std::string nickname;
	};

	void foobar();
};

#endif
