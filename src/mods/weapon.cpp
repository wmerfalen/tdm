#include "weapon.hpp"
#include "../../db.h"
namespace mods::weapon {
	mw_rifle rifle(obj_data_ptr_t& object){
		return static_cast<type::rifle>(object->weapon()->type);
	}
	mw_explosive explosive(obj_data_ptr_t& object){
		return static_cast<type::explosive>(object->weapon()->type);
	}
	mw_drone drone(obj_data_ptr_t& object){
		return static_cast<type::drone>(object->weapon()->type);
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
		obj->obj_flags.type_flag = ITEM_WEAPON_ATTACHMENT; /* Type of item			    */
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
		obj->uuid = mods::globals::obj_uuid();

		return std::move(obj);

	}
	obj_data_ptr_t base_rifle_object(){
		auto obj = blank_object();
		//obj->holds_ammo = 1;

		obj->item_number = 0;	/* Where in data-base			*/
		obj->in_room = 0;		/* In what room -1 when conta/carr	*/
		obj->obj_flags;/* Object information               */
		memset(obj->obj_flags.value,0,sizeof(obj->obj_flags.value));
		obj->obj_flags.type_flag = ITEM_WEAPON; /* Type of item			    */
		obj->obj_flags.ammo_max = 0;//FIXME load me
		obj->obj_flags.ammo = 0; //FIXME load me 
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
		obj->uuid = 0;
		return std::move(obj);
	}

	obj_data_ptr_t base_explosive_object(){
		auto obj = blank_object();
		//obj->holds_ammo = 1;

		obj->item_number = 0;	/* Where in data-base			*/
		obj->in_room = 0;		/* In what room -1 when conta/carr	*/
		obj->obj_flags;/* Object information               */
		memset(obj->obj_flags.value,0,sizeof(obj->obj_flags.value));
		obj->obj_flags.type_flag = ITEM_WEAPON; /* Type of item			    */
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

		/** TODO: fill the uuid in */
		obj->uuid = mods::globals::obj_uuid();
		//obj->weapon(1);
		//obj->weapon()->ammo = 0;
		//obj->weapon()->ammo_max = 0;
		//obj->weapon()->type = obj->obj_flags.weapon_flags = 0;

		obj->obj_flags.clip_size = 0;
		obj->ex_description = static_cast<extra_descr_data*>(calloc(1,sizeof(extra_descr_data)));
		obj->ex_description->next = nullptr;
		obj->ex_description->keyword =  strdup("keyword");
		return std::move(obj);
	}
	obj_data_ptr_t psg1(){
		auto obj = base_rifle_object();
		/** TODO: fill the uuid in */
		obj->uuid = mods::globals::obj_uuid();
		obj->weapon(1);

		/** TODO: determine clip size */
		obj->weapon()->ammo = 12;
		/** TODO: needs to be whatever the rifle's max is */
		obj->weapon()->ammo_max = 75;
		obj->obj_flags.weapon_flags = mods::weapon::SNIPE;
		obj->obj_flags.clip_size = 8;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name = strdup("PSG-1 sniper rifle");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description = strdup("A debilitating PSG-1 sniper rifle is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description = strdup("PSG-1 sniper rifle <short>");
		obj->action_description = strdup("action desc");      /* What to write when used          */
		obj->ex_description = static_cast<extra_descr_data*>(calloc(1,sizeof(extra_descr_data)));
		obj->ex_description->next = nullptr;
		obj->ex_description->keyword =  strdup("keyword");
		obj->weapon()->type = mw_rifle::SNIPER;

		/** damage/hit roll modifications */
		return std::move(obj);
	}

	obj_data_ptr_t new_frag_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::FRAG_GRENADE);
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name = strdup("a frag grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description = strdup("A fragmentation grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		/** [ USED_WHEN ]: when you "hold frag", it will say "You hold frag grenade <short>" */
		/**   |------------> obj->short_description = strdup("frag grenade <short>");        */
		obj->short_description = strdup("an ACME Industries Frag grenade");
		/** [ APPEARS ]: when you "examine frag", it will say exactly this  */
		obj->action_description = strdup("frag fragmentation grenade nade");
		obj->explosive()->type = mw_explosive::FRAG_GRENADE;
		return std::move(obj);
	}

	obj_data_ptr_t new_incendiary_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::INCENDIARY_GRENADE);
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name = strdup("An incendiary grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description = strdup("An incendiary grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description = strdup("incendiary grenade <short>");
		obj->action_description = strdup("incendiary grenade <action>");      /* What to write when used          */
		obj->explosive()->type = mw_explosive::INCENDIARY_GRENADE;
		return std::move(obj);
	}

	obj_data_ptr_t new_emp_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::EMP_GRENADE);
		obj->explosive()->type = mw_explosive::EMP_GRENADE;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name = strdup("An E.M.P. grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description = strdup("An E.M.P. grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description = strdup("emp e.m.p. electro magnetic pulse grenade <short>");
		obj->action_description = strdup("emp e.m.p. electro magnetic pulse grenade <action>");      /* What to write when used          */
		return std::move(obj);
	}

	obj_data_ptr_t new_smoke_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::SMOKE_GRENADE);
		obj->explosive()->type = mw_explosive::SMOKE_GRENADE;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name = strdup("A smoke grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description = strdup("A smoke grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description = strdup("smoke grenade <short>");
		obj->action_description = strdup("smoke grenade <action>");      /* What to write when used          */
		return std::move(obj);
	}

	obj_data_ptr_t new_flashbang_grenade_object(){
		auto obj = base_explosive_object();
		obj->explosive(mw_explosive::FLASHBANG_GRENADE);
		obj->explosive()->type = mw_explosive::FLASHBANG_GRENADE;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name = strdup("A flashbang grenade");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description = strdup("A flashbang grenade is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description = strdup("flash grenade <short>");
		obj->action_description = strdup("flash grenade <action>");      /* What to write when used          */
		return std::move(obj);
	}

	obj_data_ptr_t new_sniper_rifle_object(){
		auto obj = base_rifle_object();
		obj->weapon(1);
		/** TODO: fill the uuid in */
		obj->uuid = mods::globals::obj_uuid();

		/** TODO: determine clip size */
		obj->weapon()->ammo = 12;
		/** TODO: needs to be whatever the rifle's max is */
		obj->weapon()->ammo_max = 75;
		obj->weapon()->type = obj->obj_flags.weapon_flags = mods::weapon::SNIPE;
		obj->obj_flags.clip_size = 8;
		/** [ APPEARS ]: when you 'look sniper' or 'examine sniper' @act.informative.cpp */
		obj->name = strdup("PSG-1 sniper rifle");
		/** [ APPEARS ]: when you drop it and it's laying on the floor */
		obj->description = strdup("A debilitating PSG-1 sniper rifle is lying here.");
		/** [ APPEARS ]: when you type inv */
		obj->short_description = strdup("PSG-1 sniper rifle <short>");
		obj->action_description = strdup("action desc");      /* What to write when used          */
		obj->ex_description = static_cast<extra_descr_data*>(calloc(1,sizeof(extra_descr_data)));
		obj->ex_description->next = nullptr;
		obj->ex_description->keyword =  strdup("keyword");
		obj->weapon()->type = mw_rifle::SNIPER;
		return std::move(obj);
	}


};
