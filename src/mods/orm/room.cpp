#include "room.hpp"
#include <vector>
#include <string>

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	uint64_t room::initialize_row(){
		this->init();
		return 0;
	}
	sql_save_map_t room::export_class() {
		sql_save_map_t values;
		values["id"] = std::to_string(id);
		values["room_number"] = std::to_string(room_number);
		values["zone"]= std::to_string(zone);
		values["sector_type"] = std::to_string(sector_type);
		values["name"] = name;
		values["description"] = description;
		values["ex_keyword"] = ex_keyword;
		values["ex_description"] = ex_description;
		values["light"] = std::to_string(light);
		values["room_flag"] = std::to_string(room_flag);
		return std::move(values);
	}
	int16_t room::feed(const pqxx::result::reference & row){
		this->init();
		this->loaded = 0;
		this->id = row["id"].as<uint64_t>();
		this->room_number = row["room_number"].as<room_vnum>();
		this->zone = row["zone"].as<zone_vnum>();
		this->sector_type = row["sector_type"].as<uint32_t>();
		this->name = row["name"].c_str();
		this->description = row["description"].c_str();
		this->ex_keyword =  row["ex_keyword"].c_str();
		this->ex_description =  row["ex_description"].c_str();
		this->light = row["light"].as<uint64_t>();
		this->room_flag = row["room_flag"].as<uint64_t>();
		return 0;
	}
	void room::init(){
		id = 0;
		room_number = 0;
		loaded = 0;
		zone = 0;
		sector_type = 0;
		name.clear();
		description.clear();
		ex_keyword.clear();
		ex_description.clear();
		light = 0;
		room_flag = 0;
	}


};

