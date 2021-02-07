#include "mob-equipment.hpp"

namespace mods::orm {
	bool mob_equipment::destroy() {
		auto status = this->remove();
		return ORM_SUCCESS(status);
	}
	uint64_t mob_equipment::initialize_row(std::string_view profile_name, uint64_t vnum) {
		init();
		this->meq_profile_name = profile_name;
		this->meq_vnum = vnum;
		auto status = this->create<mob_equipment>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	bool mob_equipment::set_slot(std::string_view slot, std::string_view value) {
		if(slot.compare("light") == 0) {
			this->meq_light = value;
			return true;
		}
		if(slot.compare("finger_r") == 0) {
			this->meq_finger_r = value;
			return true;
		}
		if(slot.compare("finger_l") == 0) {
			this->meq_finger_l = value;
			return true;
		}
		if(slot.compare("neck_1") == 0) {
			this->meq_neck_1 = value;
			return true;
		}
		if(slot.compare("neck_2") == 0) {
			this->meq_neck_2 = value;
			return true;
		}
		if(slot.compare("body") == 0) {
			this->meq_body = value;
			return true;
		}
		if(slot.compare("head") == 0) {
			this->meq_head = value;
			return true;
		}
		if(slot.compare("legs") == 0) {
			this->meq_legs = value;
			return true;
		}
		if(slot.compare("feet") == 0) {
			this->meq_feet = value;
			return true;
		}
		if(slot.compare("hands") == 0) {
			this->meq_hands = value;
			return true;
		}
		if(slot.compare("arms") == 0) {
			this->meq_arms = value;
			return true;
		}
		if(slot.compare("shield") == 0) {
			this->meq_shield = value;
			return true;
		}
		if(slot.compare("about") == 0) {
			this->meq_about = value;
			return true;
		}
		if(slot.compare("waist") == 0) {
			this->meq_waist = value;
			return true;
		}
		if(slot.compare("wrist_r") == 0) {
			this->meq_wrist_r = value;
			return true;
		}
		if(slot.compare("wrist_l") == 0) {
			this->meq_wrist_l = value;
			return true;
		}
		if(slot.compare("wield") == 0) {
			this->meq_wield = value;
			return true;
		}
		if(slot.compare("primary") == 0) {
			this->meq_wield = value;
			return true;
		}
		if(slot.compare("hold") == 0) {
			this->meq_hold = value;
			return true;
		}
		if(slot.compare("secondary") == 0) {
			this->meq_secondary = value;
			return true;
		}
		if(slot.compare("shoulders_l") == 0) {
			this->meq_shoulders_l = value;
			return true;
		}
		if(slot.compare("shoulders_r") == 0) {
			this->meq_shoulders_r = value;
			return true;
		}
		if(slot.compare("backpack") == 0) {
			this->meq_backpack = value;
			return true;
		}
		if(slot.compare("goggles") == 0) {
			this->meq_goggles = value;
			return true;
		}
		if(slot.compare("vest_pack") == 0) {
			this->meq_vest_pack = value;
			return true;
		}
		if(slot.compare("elbow_l") == 0) {
			this->meq_elbow_l = value;
			return true;
		}
		if(slot.compare("elbow_r") == 0) {
			this->meq_elbow_r = value;
			return true;
		}
		return false;
	}
	strmap_t mob_equipment::export_class() {
		strmap_t values;
		values["meq_profile_name"] = meq_profile_name;
		values["meq_vnum"] = std::to_string(meq_vnum);
		values["meq_light"] = meq_light;
		values["meq_finger_r"] = meq_finger_r;
		values["meq_finger_l"] = meq_finger_l;
		values["meq_neck_1"] = meq_neck_1;
		values["meq_neck_2"] = meq_neck_2;
		values["meq_body"] = meq_body;
		values["meq_head"] = meq_head;
		values["meq_legs"] = meq_legs;
		values["meq_feet"] = meq_feet;
		values["meq_hands"] = meq_hands;
		values["meq_arms"] = meq_arms;
		values["meq_shield"] = meq_shield;
		values["meq_about"] = meq_about;
		values["meq_waist"] = meq_waist;
		values["meq_wrist_r"] = meq_wrist_r;
		values["meq_wrist_l"] = meq_wrist_l;
		values["meq_wield"] = meq_wield;
		values["meq_hold"] = meq_hold;
		values["meq_secondary"] = meq_secondary;
		values["meq_shoulders_l"] = meq_shoulders_l;
		values["meq_shoulders_r"] = meq_shoulders_r;
		values["meq_backpack"] = meq_backpack;
		values["meq_goggles"] = meq_goggles;
		values["meq_vest_pack"] = meq_vest_pack;
		values["meq_elbow_l"] = meq_elbow_l;
		values["meq_elbow_r"] = meq_elbow_r;
		return std::move(values);
	}
	int16_t mob_equipment::load_by_profile(std::string_view profile_name) {
		loaded = 0;
		created_at = updated_at = 0;
		id = 0;
		return std::get<0>(this->read<mob_equipment>(this,"meq_profile_name",profile_name));
	}
	int16_t mob_equipment::feed(const pqxx::result::reference& row) {
		init();
		loaded = 0;
		id = row["id"].as<uint64_t>();
		meq_profile_name = row["meq_profile_name"].c_str();
		meq_vnum = row["meq_vnum"].as<uint64_t>();
		meq_light = row["meq_light"].c_str();
		meq_finger_r = row["meq_finger_r"].c_str();
		meq_finger_l = row["meq_finger_l"].c_str();
		meq_neck_1 = row["meq_neck_1"].c_str();
		meq_neck_2 = row["meq_neck_2"].c_str();
		meq_body = row["meq_body"].c_str();
		meq_head = row["meq_head"].c_str();
		meq_legs = row["meq_legs"].c_str();
		meq_feet = row["meq_feet"].c_str();
		meq_hands = row["meq_hands"].c_str();
		meq_arms = row["meq_arms"].c_str();
		meq_shield = row["meq_shield"].c_str();
		meq_about = row["meq_about"].c_str();
		meq_waist = row["meq_waist"].c_str();
		meq_wrist_r = row["meq_wrist_r"].c_str();
		meq_wrist_l = row["meq_wrist_l"].c_str();
		meq_wield = row["meq_wield"].c_str();
		meq_hold = row["meq_hold"].c_str();
		meq_secondary = row["meq_secondary"].c_str();
		meq_shoulders_l = row["meq_shoulders_l"].c_str();
		meq_shoulders_r = row["meq_shoulders_r"].c_str();
		meq_backpack = row["meq_backpack"].c_str();
		meq_goggles = row["meq_goggles"].c_str();
		meq_vest_pack = row["meq_vest_pack"].c_str();
		meq_elbow_l = row["meq_elbow_l"].c_str();
		meq_elbow_r = row["meq_elbow_r"].c_str();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void mob_equipment::init() {
		id = 0;
		meq_profile_name.clear();
		meq_vnum = 0;
		meq_light.clear();
		meq_finger_r.clear();
		meq_finger_l.clear();
		meq_neck_1.clear();
		meq_neck_2.clear();
		meq_body.clear();
		meq_head.clear();
		meq_legs.clear();
		meq_feet.clear();
		meq_hands.clear();
		meq_arms.clear();
		meq_shield.clear();
		meq_about.clear();
		meq_waist.clear();
		meq_wrist_r.clear();
		meq_wrist_l.clear();
		meq_wield.clear();
		meq_hold.clear();
		meq_secondary.clear();
		meq_shoulders_l.clear();
		meq_shoulders_r.clear();
		meq_backpack.clear();
		meq_goggles.clear();
		meq_vest_pack.clear();
		meq_elbow_l.clear();
		meq_elbow_r.clear();
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
