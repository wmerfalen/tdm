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
"trap"
		};
	}
	void change_visibility(obj_ptr_t& object, int amount){
		object->visibility += amount;
	}
};//End namespace
