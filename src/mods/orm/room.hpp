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
		std::string table_name(){ return "room"; }

		room() : base(), id(0) { this->init(); loaded = 0; }
		~room() = default;
		uint64_t initialize_row();
		int16_t feed(const pqxx::result::reference &);
		void init();	
		sql_save_map_t export_class();

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
	};

	static inline void foobar(){
		mods::orm::room r;

		auto status3 = mods::orm::base::insert_returing<mods::orm::room>(r, "id");
		if(std::get<0>(status3) < 0){
			std::cerr << "[error] insert_returning failed!:\n";
			exit(1);
		}
		std::cerr << "returned id:" << std::get<1>(status3) << "\n";

		auto status = mods::orm::base::load_by_pkid(r,std::get<1>(status3));
		if(std::get<0>(status) < 0){
			std::cerr << "[error] load_by_pkid failed!: '" << std::get<1>(status) << "'\n";
			exit(1);
		}
		std::cerr << ":)\n";
		r.id = 1;
		auto status2 = mods::orm::base::delete_from(r);
		if(std::get<0>(status2) < 0){
			std::cerr << "[error] delete_from failed!: '" << std::get<1>(status2) << "'\n";
			exit(1);
		}
		std::cerr << ":)\n";
		exit(0);
	}
};

#endif
