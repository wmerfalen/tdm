#include "smg-mp5.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::smg {
	obj_data_ptr_t mp5::make(){
		return feed_by_file("mp5.yml");
	}
	obj_data_ptr_t mp5::feed_by_file(std::string_view file){
		return std::move(create_object(ITEM_RIFLE,file.data()));
	}

	void mp5::feed(const mods::pq::row_type & row){
		this->make();
		bound_to = 0;//row["bound_to"].as<uint64_t>();
		id = row["rifle_id"].as<uint64_t>();
	}
	mp5::~mp5(){
		obj.reset();
	}

	mp5::mp5(){
		bound_to = 0;
		id = 0;
	}
};
