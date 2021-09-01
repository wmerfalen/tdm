#include "armor.hpp"
#include "yaml.hpp"

namespace mods::armor {
	bool is_basic_protection(obj_ptr_t& eq) {
		if(!eq->has_armor()) {
			return false;
		}
		return eq->armor()->attributes->classification_enum == mods::yaml::armor_classification_type_t::BASIC;
	}
	bool is_advanced_protection(obj_ptr_t& eq) {
		if(!eq->has_armor()) {
			return false;
		}
		return eq->armor()->attributes->classification_enum == mods::yaml::armor_classification_type_t::ADVANCED;
	}
	bool is_elite_protection(obj_ptr_t& eq) {
		if(!eq->has_armor()) {
			return false;
		}
		return eq->armor()->attributes->classification_enum == mods::yaml::armor_classification_type_t::ELITE;
	}
};
