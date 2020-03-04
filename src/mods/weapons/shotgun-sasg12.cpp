#include "shotgun-sasg12.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::shotgun {
	obj_data_ptr_t sasg12::make(){
		return sasg12::feed_by_file("sasg12.yml");
	}
	obj_data_ptr_t sasg12::feed_by_file(std::string_view file){
		auto obj = mods::weapon::base_rifle_object();
		obj->rifle(file.data());
		obj->obj_flags.ammo = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.ammo_max = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.weapon_flags = mw_rifle::SHOTGUN;
		obj->obj_flags.clip_size =obj->rifle()->attributes->clip_size;
		obj->rifle()->type = mw_rifle::SHOTGUN;
		obj->name.assign(obj->rifle()->attributes->name);
		obj->description.assign("A debilitating SASG12 shotgun is lying here.");
		obj->short_description.assign("SASG12 shotgun <short>");
		obj->action_description.assign("SASG12");      /* What to write when used          */
		obj->ex_description.emplace_back("SASG12 shotgun",obj->description.c_str());
		mods::weapon::feed_caps(obj, { cap_t::CQC, cap_t::RELOAD, cap_t::AIM, cap_t::SHOOT, cap_t::HIP_FIRE });
		obj->extended_item_vnum = obj->rifle()->attributes->vnum;
		return std::move(obj);
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
	}
};
