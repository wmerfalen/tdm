#include "sniper-rifle-psg1.hpp"
/** db.h for read_object() */
#include "../../db.h"

namespace mods::weapons::sniper_rifle {
	obj_data_ptr_t psg1::make(){
		return feed_by_file("psg1.yml");
	}
	obj_data_ptr_t psg1::feed_by_file(std::string_view file){
		return std::move(create_object(ITEM_RIFLE,file.data()));
	}
	void psg1::feed(const mods::pq::row_type & row){
		bound_to = row["bound_to"].as<uint16_t>();
		id = row["id"].as<uint16_t>();
		m_obj = psg1::make();
	}
	obj_data_ptr_t psg1::obj(){
		return m_obj;
	}
	psg1::~psg1(){
		m_obj.reset();
	}

	psg1::psg1(){
		bound_to = 0;
		id = 0;
		m_obj = this->make();
	}
};
