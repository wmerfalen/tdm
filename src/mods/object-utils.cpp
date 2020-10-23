#include "object-utils.hpp"
#include "weapon-types.hpp"

namespace mods::object_utils {
	void report_yaml_message(std::string_view msg){
		yaml_feed_exceptions.emplace_back(CAT(mods::util::time_string(),"->",msg.data()));
	}

	std::vector<std::string> object_types(){
	/** !!*****************!! */
	/** !!UPDATE_ITEM_TYPES!! */
	/** !!*****************!! */
		return {
"rifle",
"explosive",
"drone",
"gadget",
"attachment",
"armor",
"consumable",
"trap",
"container"
		};
	}
	void change_visibility(obj_ptr_t& object, int amount){
		object->visibility += amount;
	}
	bool is_wield_position(int where){
		switch(where){
			case WEAR_SHIELD:
			case WEAR_SECONDARY_WEAPON:
			case WEAR_PRIMARY:
				return true;
			default:
				return false;
		}
	}
};//End namespace
