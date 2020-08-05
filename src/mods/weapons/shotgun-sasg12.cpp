#include "shotgun-sasg12.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::shotgun {
	obj_data_ptr_t sasg12::make(){
		return sasg12::feed_by_file("sasg12.yml");
	}
	obj_data_ptr_t sasg12::feed_by_file(std::string_view file){
		return std::move(create_object(ITEM_RIFLE,file.data()));
	}

	void sasg12::feed(const mods::pq::row_type & row){
		bound_to = 0;//row["bound_to"].as<uint64_t>();
		id = row["rifle_id"].as<uint64_t>();
	}
	sasg12::~sasg12(){
		obj.reset();
	}

	sasg12::sasg12(){
		bound_to = 0;
		id = 0;
		obj = this->make();
	}
};
