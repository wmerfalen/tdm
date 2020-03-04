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
		auto obj = blank_object();
		obj->rifle(file.data());
		obj->obj_flags.ammo = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.ammo_max = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.weapon_flags = mw_rifle::PISTOL;
		obj->obj_flags.clip_size =obj->rifle()->attributes->clip_size;
		obj->rifle()->type = mw_rifle::PISTOL;
		obj->name.assign(obj->rifle()->attributes->name);
		obj->description.assign("A debilitating CZP10 pistol is lying here.");
		obj->short_description.assign("CZP10 pistol <short>");
		obj->action_description.assign("CZP10");      /* What to write when used          */
		obj->ex_description.emplace_back("czp10 pistol",obj->description.c_str());
		mods::weapon::feed_caps(obj, { cap_t::CQC, cap_t::RELOAD, cap_t::AIM, cap_t::SHOOT, cap_t::HIP_FIRE, cap_t::HAS_CLIP });
		obj->extended_item_vnum = obj->rifle()->attributes->vnum;
		std::cerr << "[czp10::feed_by_file] returning\n";
		return std::move(obj);
	}

	void czp10::feed(const mods::pq::row_type & row){
		std::cerr << "[czp10::feed] -> feeding by rifle_file... \n";
		this->feed_by_file(row["rifle_type"].as<std::string>());
		std::cerr << "[czp10::feed] -> DONE feeding by rifle_file... \n";
		bound_to = 0; //row["bound_to"].as<uint64_t>();
		id = row["rifle_id"].as<uint64_t>();
	}
};
