#include "smg.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons {

	//constexpr static mods::weapon::type_t RIFLE_TYPE = mods::weapon::type_t::RIFLE;
	//constexpr static mods::weapon::base::rifle SNIPER_TYPE = mods::weapon::base::rifle::SNIPER;
	/** Ideal code */
	/*
	void smg::feed(const mods::pq::row_type & row){
		bound_to = row["bound_to"].as<uint64_t>();
		id = row["id"].as<uint64_t>();
	}
	smg::~smg(){
		obj.reset();
	}

	smg::smg(){
		bound_to = 0;
		id = 0;
	}
	*/
};
