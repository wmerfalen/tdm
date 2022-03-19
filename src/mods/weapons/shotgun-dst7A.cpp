#include "shotgun-dst7A.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::shotgun {
	obj_data_ptr_t dst7A::make() {
		return dst7A::feed_by_file("dst7A.yml");
	}
	obj_data_ptr_t dst7A::feed_by_file(std::string_view file) {
		return std::move(create_object(ITEM_RIFLE,file.data()));
	}

	void dst7A::feed(const mods::pq::row_type& row) {
		bound_to = 0;//row["bound_to"].as<uint64_t>();
		id = row["rifle_id"].as<uint64_t>();
	}
	dst7A::~dst7A() {
		obj.reset();
	}

	dst7A::dst7A() {
		bound_to = 0;
		id = 0;
		obj = this->make();
	}
};
