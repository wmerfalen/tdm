#include "hq.hpp"

namespace mods::orm {
	bool hq::destroy() {
		auto status = this->remove();
		return ORM_SUCCESS(status);
	}
	uint64_t hq::initialize_row(std::string_view affiliation, room_vnum rvnum,uint16_t level) {
		init();
		this->hq_affiliation = affiliation;
		this->hq_level = level;
		this->hq_room_vnum = rvnum;
		auto status = this->create<hq>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	strmap_t hq::export_class() {
		strmap_t values;
		values["hq_affiliation"] = hq_affiliation;
		values["hq_level"] = std::to_string(hq_level);
		values["hq_room_vnum"] = std::to_string(hq_room_vnum);
		return std::move(values);
	}
	int16_t hq::load_by_profile(std::string_view affiliation) {
		loaded = 0;
		created_at = updated_at = 0;
		id = 0;
		return std::get<0>(this->read<hq>(this,"hq_affiliation",affiliation));
	}
	int16_t hq::feed(const pqxx::result::reference& row) {
		init();
		loaded = 0;
		id = row["id"].as<uint64_t>();
		hq_affiliation = row["hq_affiliation"].c_str();
		hq_level = row["hq_level"].as<uint64_t>();
		hq_room_vnum = row["hq_room_vnum"].as<uint32_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void hq::init() {
		id = 0;
		hq_affiliation.clear();
		hq_level = 0;
		hq_room_vnum = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
