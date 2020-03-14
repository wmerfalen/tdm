#include "sniper-rifle-psg1.hpp"
/** db.h for read_object() */
#include "../../db.h"

namespace mods::weapons::sniper_rifle {
	//constexpr static int RIFLE_TYPE = mods::weapon::type_t::RIFLE;
	//constexpr static mw_rifle SNIPER_TYPE = mw_rifle::SNIPER;

	obj_data_ptr_t psg1::make(){
		return feed_by_file("psg1.yml");
	}
	obj_data_ptr_t psg1::feed_by_file(std::string_view file){
		/** TODO: fill the uuid in */
		auto obj = mods::weapon::base_rifle_object();
		obj->uuid = mods::globals::obj_uuid();
		obj->rifle(file.data());
		obj->obj_flags.ammo = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.ammo_max = obj->rifle()->attributes->ammo_max;
		obj->obj_flags.weapon_flags = mw_rifle::SUB_MACHINE_GUN;
		obj->obj_flags.clip_size =obj->rifle()->attributes->clip_size;
		obj->name.assign(obj->rifle()->attributes->name);
		obj->description.assign("A debilitating PSG-1 sniper rifle is lying here.");
		obj->short_description.assign("PSG-1 sniper rifle");
		obj->action_description.assign("action desc");      /* What to write when used          */
		obj->ex_description.emplace_back("psg1 sniper rifle",obj->description.view());
		obj->extended_item_vnum = obj->rifle()->attributes->vnum;
		obj->rifle()->type = mw_rifle::SNIPER;
		obj->obj_flags.type = mw_rifle::SNIPER;
		mods::weapon::feed_caps(obj, { 
				cap_t::RELOAD, cap_t::RANGED_ATTACK, cap_t::AIM, cap_t::SHOOT,
				cap_t::SNIPE, cap_t::HAS_CLIP 
		});
		obj->extended_item_vnum = obj->rifle()->attributes->vnum;
		return std::move(obj);
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
		m_obj = std::move(mods::weapon::new_sniper_rifle_object());
	}
};
