#include "sniper-rifle.hpp"
/** db.h for read_object() */
#include "../../db.h"

namespace mods::weapons {
	//constexpr static int RIFLE_TYPE = mods::weapon::type_t::RIFLE;
	//constexpr static mw_rifle SNIPER_TYPE = mw_rifle::SNIPER;

	/** commented out because this is a namespace now */
	//obj_data_ptr_t psg1(obj_data_ptr_t obj) {
	//	/** TODO: fill the uuid in */
	//	obj->uuid = mods::globals::obj_uuid();
	//	obj->rifle(1);

	//	/** TODO: determine clip size */
	//	obj->obj_flags.ammo = 12;
	//	/** TODO: needs to be whatever the rifle's max is */
	//	obj->obj_flags.ammo_max = 75;
	//	obj->obj_flags.weapon_flags = mods::weapon::SNIPE;
	//	obj->obj_flags.clip_size = 8;
	//	obj->name.assign("PSG-1 sniper rifle");
	//	/** [ APPEARS ]: when you drop it and it's laying on the floor */
	//	obj->description.assign("A debilitating PSG-1 sniper rifle is lying here.");
	//	/** [ APPEARS ]: when you type inv */
	//	obj->short_description.assign("PSG-1 sniper rifle");
	//	obj->action_description.assign("action desc");      /* What to write when used          */
	//	obj->ex_description.emplace_back("psg1 sniper rifle",obj->description.view());
	//	obj->obj_flags.type = mw_rifle::SNIPER;
	//	return std::move(obj);
	//}
	//void sniper_rifle::feed(const mods::pq::row_type & row){
	//	bound_to = row["bound_to"].as<uint16_t>();
	//	id = row["id"].as<uint16_t>();
	//	if(std::string(row["model"].c_str()).compare("psg1") == 0){
	//		m_obj = psg1(m_obj);
	//	}
	//}
	//obj_data_ptr_t sniper_rifle::obj(){
	//	return m_obj;
	//}
	//sniper_rifle::~sniper_rifle(){
	//	m_obj.reset();
	//}

	//sniper_rifle::sniper_rifle(){
	//	bound_to = 0;
	//	id = 0;
	//	m_obj = std::move(mods::weapon::new_sniper_rifle_object());
	//}
};
