#include "sniper-rifle-l96aw.hpp"
/** db.h for read_object() */
#include "../../db.h"

namespace mods::weapons::sniper_rifle {
	obj_data_ptr_t l96aw::make(){
		return create_object(ITEM_RIFLE,"l96aw.yml");
	}
	void l96aw::feed(const mods::pq::row_type & row){
		bound_to = row["bound_to"].as<uint16_t>();
		id = row["id"].as<uint16_t>();
		m_obj = l96aw::make();
	}
	obj_data_ptr_t l96aw::obj(){
		return m_obj;
	}
	l96aw::~l96aw(){
		m_obj.reset();
	}

	l96aw::l96aw(){
		bound_to = 0;
		id = 0;
		m_obj = this->make();
	}
};
