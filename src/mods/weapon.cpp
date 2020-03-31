#include "weapon.hpp"
#include "../../db.h"
/** FIXME: need to assign vnum/ids to objects */
namespace mods::weapon {
	std::vector<cap_t> get_caps(mw_gadget type){
		switch(type){
			case mw_gadget::GRAPPLING_HOOK:
				return { cap_t::SHOOT, cap_t::AIM, cap_t::GRAPPLE };

			case mw_gadget::BARBED_WIRE:
				return { cap_t::AFFECT_MOVE, cap_t::PREVENT_RUN };

			case mw_gadget::CAMERA:
				return { cap_t::REMOTELY_VIEW, cap_t::INSTALL, cap_t::SCAN  };

			case mw_gadget::THERMAL_CAMERA:
				return { cap_t::REMOTELY_VIEW, cap_t::INSTALL, cap_t::THERMAL_VISION, cap_t::SCAN  };

			case mw_gadget::NIGHT_VISION_CAMERA:
				return { cap_t::REMOTELY_VIEW, cap_t::INSTALL, cap_t::NIGHT_VISION, cap_t::SCAN  };

			case mw_gadget::MOTION_SENSOR:
				return { cap_t::INSTALL, cap_t::TRIGGER_ALERT, cap_t::NOTIFY_OWNER, cap_t::SENSE_MOTION };

			case mw_gadget::TRIPWIRE:
				return { cap_t::TRIGGER_EXPLOSION };

			case mw_gadget::DEPLOYABLE_SHIELD:
				return { cap_t::INSTALL, cap_t::ABSORB_DAMAGE };
			default:
				return {};
		}
	}
	std::vector<cap_t> get_caps(mw_rifle type){
		switch(type){
			case mw_rifle::SNIPER:
				return { cap_t::RELOAD, cap_t::RANGED_ATTACK, cap_t::AIM, cap_t::SHOOT, cap_t::SNIPE, cap_t::HAS_CLIP };

			case mw_rifle::SUB_MACHINE_GUN:
				return { cap_t::CQC, cap_t::RELOAD, cap_t::RANGED_ATTACK, cap_t::AIM, cap_t::SHOOT, cap_t::HAS_CLIP };

			case mw_rifle::SHOTGUN:
				return { cap_t::CQC, cap_t::RELOAD, cap_t::AIM, cap_t::SHOOT, cap_t::HIP_FIRE };

			case mw_rifle::HANDGUN:
			case mw_rifle::PISTOL:
				return { cap_t::CQC, cap_t::RELOAD, cap_t::AIM, cap_t::SHOOT, cap_t::HIP_FIRE, cap_t::HAS_CLIP };

			case mw_rifle::MACHINE_PISTOL:
				return { cap_t::CQC, cap_t::RELOAD, cap_t::AIM, cap_t::SHOOT, cap_t::HIP_FIRE, cap_t::HAS_CLIP, cap_t::SPRAY_BULLETS };

			case mw_rifle::ASSAULT_RIFLE:
				return { cap_t::CQC, cap_t::RELOAD, cap_t::SPRAY_BULLETS, cap_t::HIP_FIRE, cap_t::RANGED_ATTACK, cap_t::AIM, cap_t::SHOOT, cap_t::HAS_CLIP };

			default:
				return {};
		}
	}

	std::vector<cap_t> get_caps(mw_explosive type){
		return {};
	}
	std::vector<cap_t> get_caps(mw_armor type){
		return {};
	}
	std::vector<cap_t> get_caps(mw_attachment type){
		return {};
	}
	std::vector<cap_t> get_caps(mw_drone type){
		return {};
	}
	std::vector<cap_t> get_caps(mw_consumable type){
		return {};
	}
	std::vector<cap_t> get_caps(mw_trap type){
		return {};
	}

	mw_rifle rifle(obj_data_ptr_t& object){
		return static_cast<type::rifle>(object->rifle()->type);
	}
	mw_explosive explosive(obj_data_ptr_t& object){
		return static_cast<type::explosive>(object->rifle()->type);
	}
	mw_drone drone(obj_data_ptr_t& object){
		return static_cast<type::drone>(object->rifle()->type);
	}
	std::variant<mw_rifle,mw_explosive,mw_drone> get_type(obj_data_ptr_t& o){
		return (mw_rifle)0;
	}
	obj_data_ptr_t attachment(attachment_t type){
		auto obj = blank_object();
		//obj->holds_ammo = 0;

		obj->item_number = 0;	/* Where in data-type			*/
		obj->in_room = 0;		/* In what room -1 when conta/carr	*/
		obj->obj_flags;/* Object information               */
		memset(obj->obj_flags.value,0,sizeof(obj->obj_flags.value));
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->type = obj->obj_flags.type_flag = ITEM_WEAPON_ATTACHMENT; /* Type of item			    */
		obj->obj_flags.ammo_max = 0;
		obj->obj_flags.ammo = 0;
		obj->obj_flags.wear_flags = ITEM_WEAR_TAKE;
		obj->obj_flags.extra_flags = 0;
		obj->obj_flags.weight = 0;
		obj->obj_flags.cost = 0;
		obj->obj_flags.cost_per_day = 0;
		obj->obj_flags.timer = 0;
		obj->obj_flags.bitvector = 0;
		obj->affected[MAX_OBJ_AFFECT];  /* affects */
		obj->carried_by = nullptr;
		obj->worn_by = nullptr;
		obj->worn_on = WEAR_WEAPON_ATTACHMENT;
		obj->in_obj = nullptr;
		obj->contains = nullptr;     /* Contains objects                 */
		obj->next_content = nullptr; /* For 'contains' lists             */
		obj->next = nullptr;         /* For the object list              */
		obj->ai_state = 0;
		obj->type = 0;
		obj->attachment(0);
		obj->ex_description.emplace_back("attachment",obj->description.c_str());

		return std::move(obj);

	}
	obj_data_ptr_t base_rifle_object(){
		auto obj = blank_object();

		obj->item_number = 0;	/* Where in data-base			*/
		obj->in_room = 0;		/* In what room -1 when conta/carr	*/
		obj->obj_flags;/* Object information               */
		memset(obj->obj_flags.value,0,sizeof(obj->obj_flags.value));
		obj->type = obj->obj_flags.type_flag = ITEM_RIFLE; /* Type of item			    */
		obj->obj_flags.holds_ammo = 1;
		obj->obj_flags.ammo_max = 1000;//FIXME load me
		obj->obj_flags.ammo = 100; //FIXME load me 
		obj->obj_flags.wear_flags = ITEM_WEAR_WIELD | ITEM_WEAR_TAKE;
		obj->obj_flags.extra_flags = 0;
		obj->obj_flags.weight = 0;
		obj->obj_flags.cost = 0;
		obj->obj_flags.cost_per_day = 0;
		obj->obj_flags.timer = 0;
		obj->obj_flags.bitvector = 0;
		obj->affected[MAX_OBJ_AFFECT];  /* affects */
		obj->carried_by = nullptr;
		obj->worn_by = nullptr;
		obj->worn_on = WEAR_WIELD;
		obj->in_obj = nullptr;
		obj->contains = nullptr;     /* Contains objects                 */
		obj->next_content = nullptr; /* For 'contains' lists             */
		obj->next = nullptr;         /* For the object list              */
		obj->ai_state = 0;
		//obj->loaded = 0;
		//obj->weapon_type = 0;
		obj->type = 0;
		obj->rifle(0);
		obj->ex_description.emplace_back("rifle",obj->description.c_str());
		return std::move(obj);
	}

	obj_data_ptr_t base_explosive_object(){
		auto obj = blank_object();

		obj->item_number = 0;	/* Where in data-base			*/
		obj->in_room = 0;		/* In what room -1 when conta/carr	*/
		obj->obj_flags;/* Object information               */
		memset(obj->obj_flags.value,0,sizeof(obj->obj_flags.value));
		obj->type = obj->obj_flags.type_flag = ITEM_EXPLOSIVE; /* Type of item			    */
		obj->obj_flags.ammo_max = 0;//FIXME load me
		obj->obj_flags.ammo = 0; //FIXME load me 
		obj->obj_flags.wear_flags = ITEM_WEAR_HOLD | ITEM_WEAR_TAKE;
		obj->obj_flags.extra_flags = 0;
		obj->obj_flags.weight = 0;
		obj->obj_flags.cost = 0;
		obj->obj_flags.cost_per_day = 0;
		obj->obj_flags.timer = 0;
		obj->obj_flags.bitvector = 0;
		obj->affected[MAX_OBJ_AFFECT];  /* affects */
		memset(obj->affected,0,sizeof(obj->affected));
		obj->carried_by = nullptr;
		obj->worn_by = nullptr;
		obj->worn_on = WEAR_HOLD;
		obj->in_obj = nullptr;
		obj->contains = nullptr;     /* Contains objects                 */
		obj->next_content = nullptr; /* For 'contains' lists             */
		obj->next = nullptr;         /* For the object list              */
		obj->ai_state = 0;

		obj->obj_flags.clip_size = 0;
		obj->explosive(0);
		obj->ex_description.emplace_back("explosive",obj->description.c_str());
		return std::move(obj);
	}

	/** !example !howto */
	/** This function is a prime example of how to populate a obj_ptr_t */
	obj_data_ptr_t psg1(){
		auto obj = base_rifle_object();
		obj->rifle("base_sniper.yml");

		/** TODO: determine clip size */
		obj->obj_flags.ammo = 12;
		/** TODO: needs to be whatever the rifle's max is */
		obj->obj_flags.ammo_max = 75;
		obj->obj_flags.weapon_flags = mods::weapon::SNIPE;
		obj->obj_flags.clip_size = 8;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("PSG-1 sniper rifle");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A debilitating PSG-1 sniper rifle is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("PSG-1 sniper rifle <short>");
		obj->action_description.assign("action desc");      /* What to write when used          */
		obj->rifle()->type = mw_rifle::SNIPER;
		obj->ex_description.emplace_back("sniper rifle psg1",obj->description.c_str());

		/** damage/hit roll modifications */
		return std::move(obj);
	}

	obj_data_ptr_t new_frag_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::FRAG_GRENADE);
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("a frag grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A fragmentation grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		/** [ USED_WHEN ]: when you "hold frag", it will say "You hold frag grenade <short>" */
		/**   |------------> obj->short_description.assign("frag grenade <short>");        */
		obj->short_description.assign("an ACME Industries Frag grenade");
		/** [ APPEARS ]: when you "examine frag", it will say exactly this  */
		obj->action_description.assign("frag fragmentation grenade nade");
		obj->explosive()->type = mw_explosive::FRAG_GRENADE;
		obj->ex_description.emplace_back("frag grenade",obj->description.c_str());
		return std::move(obj);
	}

	obj_data_ptr_t new_sensor_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive("sensor_grenade.yml");
		obj->explosive()->type = mw_explosive::SENSOR_GRENADE;
		obj->explosive()->attributes->type = mw_explosive::SENSOR_GRENADE;
		obj->explosive()->attributes = std::make_unique<mods::yaml::explosive_description_t>();
		obj->explosive()->attributes->chance_to_injure = 0.0;
		obj->explosive()->attributes->critical_chance = 0.0;
		obj->explosive()->attributes->critical_range = 0;
		obj->explosive()->attributes->blast_radius = 2;
		obj->explosive()->attributes->damage_per_second = 0;
		obj->explosive()->attributes->disorient_amount = 0.0;
		obj->explosive()->attributes->alternate_explosion_type = ALTEX_SCAN;
		obj->ex_description.emplace_back("sensor grenade",obj->description.c_str());
		return std::move(obj);
	}

	obj_data_ptr_t new_incendiary_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::INCENDIARY_GRENADE);
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("An incendiary grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("An incendiary grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("incendiary grenade <short>");
		obj->action_description.assign("incendiary grenade <action>");      /* What to write when used          */
		obj->explosive()->type = mw_explosive::INCENDIARY_GRENADE;
		obj->ex_description.emplace_back("incendiary grenade",obj->description.c_str());
		return std::move(obj);
	}

	obj_data_ptr_t new_emp_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::EMP_GRENADE);
		obj->explosive()->type = mw_explosive::EMP_GRENADE;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("An E.M.P. grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("An E.M.P. grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("emp e.m.p. electro magnetic pulse grenade <short>");
		obj->action_description.assign("emp e.m.p. electro magnetic pulse grenade <action>");      /* What to write when used          */
		obj->ex_description.emplace_back("emp grenade",obj->description.c_str());
		return std::move(obj);
	}

	obj_data_ptr_t new_smoke_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::SMOKE_GRENADE);
		obj->explosive()->type = mw_explosive::SMOKE_GRENADE;
		/** [ APPEARS ]: when you 'look smoke' or 'examine smoke' @act.informative.cpp */
		obj->name.assign("Smoke grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A smoke grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("Smoke grenade");
		obj->action_description.assign("smoke grenade");      /* What to write when used          */
		obj->ex_description.emplace_back("smoke grenade",obj->description.c_str());
		return std::move(obj);
	}

	obj_data_ptr_t new_flashbang_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::FLASHBANG_GRENADE);
		obj->explosive()->type = mw_explosive::FLASHBANG_GRENADE;
		/** [ APPEARS ]: when you 'look flashbang' or 'examine flashbang' @act.informative.cpp */
		obj->name.assign("flashbang grenade flash");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A flashbang grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		/** [ APPEARS ]: when you type remove "flash" */
		obj->short_description.assign("a flashbang grenade");
		obj->action_description.assign("a flashbang grenade <action>");      /* What to write when used          */
		return std::move(obj);
	}

	obj_data_ptr_t new_sniper_rifle_object(){
		auto obj = base_rifle_object();
		obj->rifle("base_sniper.yml");

		/** TODO: determine clip size */
		obj->obj_flags.ammo = 12;
		/** TODO: needs to be whatever the rifle's max is */
		obj->obj_flags.ammo_max = 75;
		obj->obj_flags.weapon_flags = mods::weapon::SNIPE;
		obj->rifle()->type = mw_rifle::SNIPER;
		obj->obj_flags.clip_size = 8;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("PSG-1 sniper rifle");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A debilitating PSG-1 sniper rifle is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("PSG-1 sniper rifle <short>");
		obj->action_description.assign("action desc");      /* What to write when used          */
		obj->ex_description.emplace_back("sniper",obj->description.c_str());
		return std::move(obj);
	}

	obj_data_ptr_t new_pistol_object(){
		auto obj = base_rifle_object();
		obj->rifle("base_pistol.yml");

		/** TODO: determine clip size */
		obj->obj_flags.ammo = 7;
		/** TODO: needs to be whatever the rifle's max is */
		obj->obj_flags.ammo_max = 90;
		obj->obj_flags.weapon_flags = mods::weapon::PISTOL;
		obj->obj_flags.clip_size = 7;
		/** [ APPEARS ]: when you 'look pistol' or 'examine pistol' @act.informative.cpp */
		obj->name.assign("Pistol");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A pistol is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("Pistol <short_description>");
		obj->action_description.assign("Pistol action_description");      /* What to write when used          */
		obj->ex_description.emplace_back("pistol",obj->description.c_str());
		obj->rifle()->type = mw_rifle::PISTOL;

		return std::move(obj);
	}

	obj_data_ptr_t desert_eagle(){
		auto obj = new_pistol_object();
		obj->obj_flags.ammo = 7;
		obj->obj_flags.ammo_max = 63; /** 9 clips */
		obj->obj_flags.weapon_flags = mods::weapon::PISTOL;
		obj->obj_flags.clip_size = 7;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name.assign("A Desert Eagle");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description.assign("A Desert Eagle is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description.assign("A Desert Eagle");
		obj->action_description.assign("deagle desert eagle pistol action_description");      /* What to write when used          */
		obj->ex_description.emplace_back("deagle","A devastating Desert Eagle lays here");
		obj->rifle()->type = mw_rifle::PISTOL;
		return std::move(obj);
	}

};
