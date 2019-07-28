#include "smg.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"

namespace mods::weapons {

	//constexpr static mods::weapon::type_t RIFLE_TYPE = mods::weapon::type_t::RIFLE;
	//constexpr static mods::weapon::base::rifle SNIPER_TYPE = mods::weapon::base::rifle::SNIPER;
	void smg::feed(const mods::pq::row_type & row){
		bound_to = row["bound_to"].as<uint64_t>();
		id = row["id"].as<uint64_t>();
	}
	obj_data_ptr_t new_rifle_object(){
		auto obj = blank_object();
		obj->holds_ammo = 1;
		obj->ammo = 12;
		obj->ammo_max = 75;
		obj->item_number;	/* Where in data-base			*/
		obj->in_room;		/* In what room -1 when conta/carr	*/

		obj->obj_flags;/* Object information               */
		obj->affected[MAX_OBJ_AFFECT];  /* affects */

		obj->name = strdup("Your very own smg");
		obj->description = strdup("When in room");		  /* When in room                     */
		obj->short_description = strdup("Your very own smg short");
		obj->action_description = strdup("action desc");      /* What to write when used          */
		obj->ex_description = (extra_descr_data*) calloc(1,sizeof(extra_descr_data));
		obj->ex_description->next = nullptr;
		obj->ex_description->keyword =  nullptr;
		obj->carried_by = nullptr;
		obj->worn_by = nullptr;
		obj->worn_on = WEAR_WIELD;
		obj->in_obj = nullptr;
		obj->contains = nullptr;     /* Contains objects                 */
		obj->next_content = nullptr; /* For 'contains' lists             */
		obj->next = nullptr;         /* For the object list              */
		obj->ai_state = 0;
		obj->uuid = 0;
		obj->loaded = 0;
		obj->weapon_type = 0;
		obj->wpn_type = RIFLE_TYPE;
		obj->wpn_base = SNIPER_TYPE;
		obj->type = 0;
		return std::move(obj);
	}
	smg::~smg(){
		obj.reset();
	}

	smg::smg(){
		bound_to = 0;
		id = 0;
	}
};
