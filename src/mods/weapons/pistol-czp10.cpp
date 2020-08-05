#include "pistol-czp10.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::pistol {
	obj_data_ptr_t czp10::make(){
		std::cerr << "[czp10::make] entry\n";
		return czp10::feed_by_file("czp10.yml");
	}
	obj_data_ptr_t czp10::feed_by_file(std::string_view file){
		std::cerr << "[czp10::feed_by_file] entry\n";
		return std::move(create_object(ITEM_RIFLE,file.data()));
	}

	void czp10::feed(const mods::pq::row_type & row){
		std::cerr << "[czp10::feed] -> feeding by rifle_file... \n";
		obj = this->feed_by_file(row["rifle_type"].as<std::string>());
		std::cerr << "[czp10::feed] -> DONE feeding by rifle_file... \n";
		bound_to = 0; //row["bound_to"].as<uint64_t>();
		id = row["rifle_id"].as<uint64_t>();
	}
};
