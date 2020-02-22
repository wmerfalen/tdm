#include "smg-mp5.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::smg {
	/** This function is a prime example of how to populate a obj_ptr_t */
	obj_data_ptr_t mp5::make(){
		auto obj = mods::weapon::base_rifle_object();
		obj->rifle("base_smg.yml");

		/** TODO: determine clip size */
		obj->obj_flags.ammo = 30;
		/** TODO: needs to be whatever the rifle's max is */
		obj->obj_flags.ammo_max = 500;
		obj->obj_flags.weapon_flags = mw_rifle::SUB_MACHINE_GUN;
		obj->obj_flags.clip_size = 30;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("MP5 sub-machine gun");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A debilitating MP5 sub-machine gun is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("MP5 sub-machine gun <short>");
		obj->action_description.assign("MP5");      /* What to write when used          */
		obj->rifle()->type = mw_rifle::SUB_MACHINE_GUN;
		obj->ex_description.emplace_back("mp5 sub-machine gun",obj->description.c_str());
		mods::weapon::feed_caps(obj, { cap_t::CQC, cap_t::RELOAD, cap_t::RANGED_ATTACK, cap_t::AIM, cap_t::SHOOT });

		/** damage/hit roll modifications */
		return std::move(obj);
	}

	void mp5::feed(const mods::pq::row_type & row){
		this->make();
		bound_to = row["bound_to"].as<uint64_t>();
		id = row["id"].as<uint64_t>();
	}
	mp5::~mp5(){
		obj.reset();
	}

	mp5::mp5(){
		bound_to = 0;
		id = 0;
	}
};
