#include "object-utils.hpp"
#include "weapon-types.hpp"
#include "rifle-attachments.hpp"

namespace mods::object_utils {
	bool is_rifle(const obj_ptr_t& item) {
		auto rifle_attachment = mods::rifle_attachments::by_uuid(item->uuid);
		if(rifle_attachment) {
			return true;
		}
		return item->has_rifle();
	}
	bool is_melee(const obj_ptr_t& item) {
		return item->has_melee();
	}
	bool can_wield_in_secondary(const obj_ptr_t& item) {
		if(item->has_rifle() == false) {
			return false;
		}
		if(item->has_melee()) {
			return true;
		}
		switch(item->rifle()->attributes->type) {
			default:
			case mw_rifle::SNIPER:
			case mw_rifle::LIGHT_MACHINE_GUN:
			case mw_rifle::ASSAULT_RIFLE:
				return false;
			case mw_rifle::MACHINE_PISTOL:
			case mw_rifle::PISTOL:
			case mw_rifle::SHOTGUN:
			case mw_rifle::SUB_MACHINE_GUN:
				return true;
		}
	}
	bool check_rifle_type(const auto& item, const mw_rifle& type) {
		auto rifle_attachment = mods::rifle_attachments::by_uuid(item->uuid);
		if(rifle_attachment) {
			return rifle_attachment->base_object->rifle()->attributes->type == type;
		}
		return item->has_rifle() && item->rifle()->attributes->type == type;
	}
	bool is_shotgun_underbarrel(const obj_ptr_t& item) {
		return item->has_attachment() && item->attachment()->attributes->type == mw_attachment::UNDER_BARREL &&
		       item->attachment()->attributes->underbarrel_launcher_type.compare("SHOTGUN") == 0;
	}
	bool is_corpse(const obj_ptr_t& item) {
		return item->is_corpse;
	}
	bool is_corpse_explosion(const obj_ptr_t& item) {
		return item->has_explosive() && std::string(item->feed_file().data()).compare("corpse-charge.yml") == 0;
	}
	bool is_consumable(const obj_ptr_t& item) {
		return item->has_consumable();
	}
	bool is_smg(const obj_ptr_t& item) {
		return check_rifle_type(item,mw_rifle::SUB_MACHINE_GUN);
	}
	bool is_hellfire_corpse_charge(const obj_ptr_t& device) {
		return device->has_explosive() && std::string(device->feed_file().data()).compare("hellfire-corpse-charge.yml") == 0;
	}
	bool is_shrapnel_corpse_charge(const obj_ptr_t& device) {
		return device->has_explosive() && std::string(device->feed_file().data()).compare("shrapnel-corpse-charge.yml") == 0;
	}
	bool is_corrosive_claymore(const obj_ptr_t& item) {
		return item->explosive()->attributes->alternate_explosion_type.compare("CORROSIVE") == 0;
	}
	bool is_shrapnel_claymore(const obj_ptr_t& item) {
		return item->explosive()->attributes->alternate_explosion_type.compare("SHRAPNEL") == 0;
	}
	bool is_shotgun(const obj_ptr_t& weapon) {
		return check_rifle_type(weapon,mw_rifle::SHOTGUN);
	}
	bool is_assault_rifle(const obj_ptr_t& weapon) {
		return check_rifle_type(weapon,mw_rifle::ASSAULT_RIFLE);
	}
	bool is_sniper_rifle(const obj_ptr_t& weapon) {
		return check_rifle_type(weapon,mw_rifle::SNIPER);
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
