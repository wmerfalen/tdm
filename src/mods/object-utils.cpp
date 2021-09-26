#include "object-utils.hpp"
#include "weapon-types.hpp"
#include "rifle-attachments.hpp"

namespace mods::object_utils {
	bool is_corpse(const obj_ptr_t& item) {
		return item->is_corpse;
	}
	bool is_corpse_explosion(const obj_ptr_t& item) {
		return item->has_explosive() && std::string(item->feed_file().data()).compare("corpse-charge.yml") == 0;
	}
	bool is_corrosive_claymore(const obj_ptr_t& item) {
		return item->explosive()->attributes->alternate_explosion_type.compare("CORROSIVE") == 0;
	}
	bool is_shrapnel_claymore(const obj_ptr_t& item) {
		return item->explosive()->attributes->alternate_explosion_type.compare("SHRAPNEL") == 0;
	}
	bool is_assault_rifle(const obj_ptr_t& weapon) {
		auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
		if(rifle_attachment) {
			return rifle_attachment->base_object->rifle()->attributes->type == mw_rifle::ASSAULT_RIFLE;
		}
		return weapon->rifle()->attributes->type == mw_rifle::ASSAULT_RIFLE;
	}
	bool is_sniper_rifle(const obj_ptr_t& weapon) {
		auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
		if(rifle_attachment) {
			return rifle_attachment->base_object->rifle()->attributes->type == mw_rifle::SNIPER;
		}
		return weapon->rifle()->attributes->type == mw_rifle::SNIPER;
	}
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
