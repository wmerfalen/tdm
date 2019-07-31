#include "weapon.hpp"
#include "../../db.h"
namespace mods::weapon {
	base::rifle rifle(obj_data_ptr_t& object){
		return static_cast<base::rifle>(object->weapon()->base);
	}
	base::explosive explosive(obj_data_ptr_t& object){
		return static_cast<base::explosive>(object->weapon()->base);
	}
	base::drone drone(obj_data_ptr_t& object){
		return static_cast<base::drone>(object->weapon()->base);
	}
	std::variant<base::rifle,base::explosive,base::drone> type(obj_data_ptr_t& o){
			switch(o->weapon()->type){
				case type_t::EXPLOSIVE:
					return explosive(o);
				case type_t::DRONE:
					return drone(o);
				default:
				case type_t::RIFLE:
					return rifle(o);
			}
		}
	obj_data_ptr_t attachment(attachment_t type){
		auto obj = blank_object();
		//obj->holds_ammo = 0;

		obj->item_number = 0;	/* Where in data-base			*/
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
		obj->uuid = mods::globals::get_uuid();

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

	obj_data_ptr_t psg1(){
		auto obj = base_rifle_object();
		/** TODO: fill the uuid in */
		obj->uuid = mods::globals::get_uuid();
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
		obj->weapon()->base = mods::weapon::type_t::RIFLE;
		obj->weapon()->type = mods::weapon::base::rifle::SNIPER;

		/** damage/hit roll modifications */
		return std::move(obj);
	}

	obj_data_ptr_t new_sniper_rifle_object(){
		auto obj = base_rifle_object();
		obj->weapon(1);
		/** TODO: fill the uuid in */
		obj->uuid = mods::globals::get_uuid();

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
		obj->weapon()->base = mods::weapon::type_t::RIFLE;
		obj->weapon()->type = mods::weapon::base::rifle::SNIPER;
		return std::move(obj);
	}


};
