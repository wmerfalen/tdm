#include "integral-object.hpp"

namespace mods::orm {
	uint64_t integral_object::initialize_row() {
		init();
		auto status = this->create<integral_object>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = object_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t integral_object::export_class() {
		strmap_t values;
		values["object_id"] = std::to_string(object_id);
		values["object_room_vnum"] = object_room_vnum;
		values["object_type"] = object_type;
		values["object_vnum"] = std::to_string(object_vnum);
		return std::move(values);
	}
	int16_t integral_object::save_new(){
		id = 0;
		auto status = this->create<integral_object>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}

	int16_t integral_object::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["object_id"].as<uint64_t>();
		object_id = row["object_id"].as<uint64_t>();
		object_room_vnum = row["object_room_vnum"].as<uint64_t>();
		object_type = row["object_type"].c_str();
		object_vnum = row["object_vnum"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void integral_object::init(){
		id = 0;
		object_id = 0;
		object_room_vnum = 0;
		object_type = "";
		object_vnum = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

