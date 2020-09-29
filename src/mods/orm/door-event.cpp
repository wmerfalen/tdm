#include "door-event.hpp"

namespace mods::orm {
	uint64_t door_event::initialize_row() {
		init();
		auto status = this->create<door_event>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = event_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t door_event::export_class() {
		strmap_t values;
		values["event_id"] = std::to_string(event_id);
		values["event_room_vnum"] = event_room_vnum;
		values["event_type"] = event_type;
		values["event_vnum"] = std::to_string(event_vnum);
		return std::move(values);
	}

	int16_t door_event::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["event_id"].as<uint64_t>();
		event_id = row["event_id"].as<uint64_t>();
		event_room_vnum = row["event_room_vnum"].as<room_vnum_t>();
		event_type = row["event_type"].c_str();
		event_vnum = row["event_vnum"].as<event_vnum_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void door_event::init(){
		id = 0;
		event_id = 0;
		event_room_vnum = 0;
		event_type = "";
		event_vnum = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
