#include "rifle-attachments.hpp"

namespace mods {
	const std::string example_grammar = "g36c.yml{sight:acog.yml,muzzle:compensator.yml,under_barrel:gm32grenadelauncher.yml}";
	using counter_t = uint16_t;

	void rifle_attachments_t::import_objects(const encoding_t& line) {
		auto map = m_parser.extract_line_items(line,rifle_slot_names);
		m_rifle_yaml_file = map["base"];
		base_object = create_object(ITEM_RIFLE,map["base"]);
		for(auto& pair : map) {
			if(pair.first.compare("sight") == 0) {
				sight = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("under_barrel") == 0) {
				under_barrel = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("grip") == 0) {
				grip = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("barrel") == 0) {
				barrel = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("muzzle") == 0) {
				muzzle = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("magazine") == 0) {
				magazine = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("stock") == 0) {
				stock = create_object(ITEM_ATTACHMENT,pair.second);
			}
			if(pair.first.compare("strap") == 0) {
				strap = create_object(ITEM_ATTACHMENT,pair.second);
			}
		}
	}
	std::string rifle_attachments_t::extract_base_yaml_file(const encoding_t& line) {
		auto map = m_parser.extract_line_items(line,rifle_slot_names);
		return map["base"];
	}
	std::string& rifle_attachments_t::base_yaml_file() {
		return m_rifle_yaml_file;
	}
};
