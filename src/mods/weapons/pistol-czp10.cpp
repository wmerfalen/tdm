#include "pistol-czp10.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons::pistol {
	/** This function is a prime example of how to populate a obj_ptr_t */
	obj_data_ptr_t czp10::make(){
		auto obj = mods::weapon::new_pistol_object();
		obj->rifle(".yml");

		/** TODO: determine clip size */
		obj->obj_flags.ammo = 9;
		/** TODO: needs to be whatever the pistol's max is */
		obj->obj_flags.ammo_max = obj->obj_flags.ammo * 20;
		obj->obj_flags.weapon_flags = mw_rifle::PISTOL;
		obj->obj_flags.clip_size = 9;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("CZP10 pistol");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A debilitating CZP10 pistol is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("CZP10 pistol <short>");
		obj->action_description.assign("CZP10");      /* What to write when used          */
		obj->rifle()->type = mw_rifle::PISTOL;
		obj->ex_description.emplace_back("czp10 pistol",obj->description.c_str());
		mods::weapon::feed_caps(obj, { cap_t::CQC, cap_t::RELOAD, cap_t::AIM, cap_t::SHOOT, cap_t::HIP_FIRE, cap_t::HAS_CLIP });

		obj->rifle()->attributes->accuracy_map = {80.0,40.0,0.0,0.0};
		obj->rifle()->attributes->ammo_max=75 ;
		obj->rifle()->attributes->ammo_type="PISTOL" ;
		obj->rifle()->attributes->chance_to_injure=20.50100 ;
		obj->rifle()->attributes->clip_size=9 ;
		obj->rifle()->attributes->cooldown_between_shots=0.001301 ;
		obj->rifle()->attributes->critical_chance=5.14 ;
		obj->rifle()->attributes->critical_range=1 ;
		obj->rifle()->attributes->damage_map={50.00, 0.00, 0.00, 0.00 };
		obj->rifle()->attributes->damage_per_second=20.00 ;
		obj->rifle()->attributes->disorient_amount=0.0 ;
		obj->rifle()->attributes->headshot_bonus=33.0 ;
		obj->rifle()->attributes->rounds_per_minute=80 ;
		obj->rifle()->attributes->effective_firing_range=2 ;
		obj->rifle()->attributes->muzzle_velocity=40 ;
		obj->rifle()->attributes->max_range=1 ;
;
		obj->rifle()->attributes->range_multiplier=0.0 ;
;
		obj->rifle()->attributes->reload_time=2.0013 ;
		obj->rifle()->attributes->type= mw_rifle::PISTOL;
		obj->rifle()->attributes->manufacturer="Prophet Industrial";
		obj->rifle()->attributes->name="A CZP10 pistol" ;
		obj->rifle()->attributes->rarity= mods::rarity::COMMON;
		obj->rifle()->attributes->vnum=600;

		/** damage/hit roll modifications */
		return std::move(obj);
	}

	void czp10::feed(const mods::pq::row_type & row){
		bound_to = row["bound_to"].as<uint64_t>();
		id = row["id"].as<uint64_t>();
	}
};
