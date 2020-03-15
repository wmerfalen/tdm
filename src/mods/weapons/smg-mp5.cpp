#include "smg-mp5.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::smg {
	obj_data_ptr_t mp5::make(){
		return feed_by_file("mp5.yml");
	}
	obj_data_ptr_t mp5::feed_by_file(std::string_view file){
		auto obj = mods::weapon::base_rifle_object();
		obj->rifle(file.data());
		obj->obj_flags.ammo = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.ammo_max = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.weapon_flags = mw_rifle::SUB_MACHINE_GUN;
		obj->obj_flags.clip_size =obj->rifle()->attributes->clip_size;
		obj->rifle()->type = mw_rifle::SUB_MACHINE_GUN;
		obj->name.assign(obj->rifle()->attributes->name);
		obj->description.assign("A debilitating mp5 sub-machine gun is lying here.");
		obj->short_description.assign("MP5 sub-machine gun <short>");
		obj->action_description.assign("MP5");      /* What to write when used          */
		obj->ex_description.emplace_back("mp5 sub-machine gun",obj->description.c_str());
		mods::weapon::feed_caps(obj, (mw_rifle) obj->rifle()->type);
		obj->extended_item_vnum = obj->rifle()->attributes->vnum;
		return std::move(obj);
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
