#include "camera-feed.hpp"

namespace mods::orm {
	uint64_t camera_feed::initialize_row() {
		init();
		auto status = this->create<camera_feed>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = feed_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t camera_feed::export_class() {
		strmap_t values;
		values["feed_id"] = std::to_string(feed_id);
		values["feed_room_vnum"] = feed_room_vnum;
		values["feed_type"] = feed_type;
		values["feed_vnum"] = std::to_string(feed_vnum);
		return std::move(values);
	}

	int16_t camera_feed::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["feed_id"].as<uint64_t>();
		feed_id = row["feed_id"].as<uint64_t>();
		feed_room_vnum = row["feed_room_vnum"].as<room_vnum_t>();
		feed_type = row["feed_type"].c_str();
		feed_vnum = row["feed_vnum"].as<feed_vnum_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void camera_feed::init(){
		id = 0;
		feed_id = 0;
		feed_room_vnum = 0;
		feed_type = "";
		feed_vnum = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
