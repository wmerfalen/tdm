#include "mob-equipment-map.hpp"

namespace mods::orm {
	bool mob_equipment_map::destroy() {
		auto status = this->remove(this);
		return ORM_SUCCESS(status);
	}
	uint64_t mob_equipment_map::initialize_row(const uint64_t& mob_vn, const uint64_t& equipment_vnum) {
		init();
		this->mmap_mob_vnum = mob_vn;
		this->mmap_mob_equipment_vnum = equipment_vnum;
		auto status = this->create<mob_equipment_map>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}

	strmap_t mob_equipment_map::export_class() {
		strmap_t values;
		values["mmap_mob_vnum"] = std::to_string(mmap_mob_vnum);
		values["mmap_mob_equipment_vnum"] = std::to_string(mmap_mob_equipment_vnum);
		return std::move(values);
	}
	int16_t mob_equipment_map::load_by_mob_vnum(const uint64_t& mob_vnum) {
		loaded = 0;
		created_at = updated_at = 0;
		id = 0;
		return std::get<0>(this->read<mob_equipment_map>(this,"mmap_mob_vnum",std::to_string(mob_vnum)));
	}
	int16_t mob_equipment_map::feed(const pqxx::result::reference& row) {
		init();
		loaded = 0;
		id = row["id"].as<uint64_t>();
		mmap_mob_vnum = row["mmap_mob_vnum"].as<uint64_t>();
		mmap_mob_equipment_vnum = row["mmap_mob_equipment_vnum"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void mob_equipment_map::init() {
		destroyed = false;
		id = 0;
		mmap_mob_vnum = 0;
		mmap_mob_equipment_vnum = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

	std::deque<std::shared_ptr<mods::orm::mob_equipment_map>>& mob_equipment_map_list() {
		static std::deque<std::shared_ptr<mods::orm::mob_equipment_map>> list;
		return list;
	}
};
