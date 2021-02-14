#include "object-utils.hpp"
#include "weapon-types.hpp"

namespace mods::object_utils {
	std::vector<std::string> object_types() {
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
			"container",
			"melee",
			"vehicle"
		};
	}
	int get_yaml_type(std::string& yaml) {
		auto exploded = EXPLODE(yaml.c_str(),'/');
		return mods::util::yaml_string_to_int(exploded[0]);
	}
	bool assert_sane_object(std::string_view yaml) {
		auto exploded = EXPLODE(yaml.data(),'/');
		if(exploded.size() < 2) {
			return false;
		}

		for(auto t : object_types()) {
			if(exploded[0].compare(t) == 0) {
				return true;
			}
		}
		return false;
	}
	std::string get_yaml_file(std::string& yaml) {
		std::string current = "";
		bool save = false;
		for(auto ch : yaml) {
			if(ch == '/') {
				save = true;
				continue;
			}
			if(ch == '{') {
				return current;
			}
			if(save) {
				current += ch;
			}
		}
		return current;
	}
	void report_yaml_message(std::string_view msg) {
		yaml_feed_exceptions.emplace_back(CAT(mods::util::time_string(),"->",msg.data()));
	}

	void change_visibility(obj_ptr_t& object, int amount) {
		object->visibility += amount;
	}
	bool is_wield_position(int where) {
		switch(where) {
			case WEAR_SHIELD:
			case WEAR_SECONDARY_WEAPON:
			case WEAR_PRIMARY:
				return true;
			default:
				return false;
		}
	}
};//End namespace
