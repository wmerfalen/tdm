#include "shotgun-sasg12.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::shotgun {
	obj_data_ptr_t sasg12::make(){
		auto obj = mods::weapon::base_rifle_object();
		obj->rifle("base_shotgun.yml");

		/** TODO: determine clip size */
		obj->obj_flags.ammo = 8;
		/** TODO: needs to be whatever the rifle's max is */
		obj->obj_flags.ammo_max = 500;
		obj->obj_flags.weapon_flags = mw_rifle::SUB_MACHINE_GUN;
		obj->obj_flags.clip_size = 30;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("SASG12 shotgun");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A debilitating SASG12 shotgun is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("SASG12 shotgun <short>");
		obj->action_description.assign("SASG12");      /* What to write when used          */
		obj->rifle()->type = mw_rifle::SNIPER;
		obj->ex_description.emplace_back("SASG12 shotgun",obj->description.c_str());
		mods::weapon::feed_caps(obj, { cap_t::CQC, cap_t::RELOAD, cap_t::AIM, cap_t::SHOOT });

		/** damage/hit roll modifications */
		return std::move(obj);
	}

	void sasg12::feed(const mods::pq::row_type & row){
		bound_to = row["bound_to"].as<uint64_t>();
		id = row["id"].as<uint64_t>();
	}
	sasg12::~sasg12(){
		obj.reset();
	}

	sasg12::sasg12(){
		bound_to = 0;
		id = 0;
	}
};
