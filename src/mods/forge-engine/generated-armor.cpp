#include "generated-armor.hpp"
#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "util.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generated_armor]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {
	extern generator item_generator;
	static bool refresh_armor_index = true;
	static std::vector<std::string> armor_finger = {};
	static std::vector<std::string> armor_neck = {};
	static std::vector<std::string> armor_shield= {};
	static std::vector<std::string> armor_about = {};
	static std::vector<std::string> armor_wield = {};
	static std::vector<std::string> armor_hold = {};
	static std::vector<std::string> armor_secondary = {};

	static std::vector<std::string> armor_shoulders = {};
	static std::vector<std::string> armor_vest_pack  = {};

	static std::vector<std::string> armor_body = {};
	static std::vector<std::string> armor_head = {};
	static std::vector<std::string> armor_legs = {};
	static std::vector<std::string> armor_feet = {};
	static std::vector<std::string> armor_hands = {};
	static std::vector<std::string> armor_arms = {};
	static std::vector<std::string> armor_waist = {};
	static std::vector<std::string> armor_wrist = {};
	static std::vector<std::string> armor_elbow = {};
	static std::vector<std::string> armor_backpack = {};
	static std::vector<std::string> armor_goggles = {};
	static std::vector<std::string> empty = {};

	void armor_index_changed() {
		refresh_armor_index = true;
	}
	void generated_armor_t::load_from_sql() {
		static std::map<std::string,std::vector<std::string>> data;
		if(refresh_armor_index) {
			mods::orm::load_all_armor_index_data(&data);
			refresh_armor_index = false;
		}
		for(auto& row : data) {
			if(row.first.compare("finger") == 0) {
				armor_finger = row.second;
			}
			if(row.first.compare("neck") == 0) {
				armor_neck = row.second;
			}
			if(row.first.compare("body") == 0) {
				armor_body = row.second;
			}
			if(row.first.compare("head") == 0) {
				armor_head = row.second;
			}
			if(row.first.compare("legs") == 0) {
				armor_legs = row.second;
			}
			if(row.first.compare("feet") == 0) {
				armor_feet = row.second;
			}
			if(row.first.compare("hands") == 0) {
				armor_hands = row.second;
			}
			if(row.first.compare("arms") == 0) {
				armor_arms = row.second;
			}
			if(row.first.compare("shield") == 0) {
				armor_shield = row.second;
			}
			if(row.first.compare("about") == 0) {
				armor_about = row.second;
			}
			if(row.first.compare("waist") == 0) {
				armor_waist = row.second;
			}
			if(row.first.compare("wrist") == 0) {
				armor_wrist = row.second;
			}
			if(row.first.compare("wield") == 0) {
				armor_wield = row.second;
			}
			if(row.first.compare("hold") == 0) {
				armor_hold = row.second;
			}
			if(row.first.compare("secondary") == 0) {
				armor_secondary = row.second;
			}
			if(row.first.compare("shoulders") == 0) {
				armor_shoulders = row.second;
			}
			if(row.first.compare("vestpack") == 0) {
				armor_vest_pack = row.second;
			}
			if(row.first.compare("elbow") == 0) {
				armor_elbow = row.second;
			}
			if(row.first.compare("backpack") == 0) {
				armor_backpack = row.second;
			}
			if(row.first.compare("goggles") == 0) {
				armor_goggles = row.second;
			}
		}
	}
	const std::vector<std::string>& generated_armor_t::yaml_list(armor_types_t t) {
		switch(t) {
			case ARMOR_ITEM_WEAR_FINGER:
				return armor_finger;
			case ARMOR_ITEM_WEAR_NECK:
				return armor_neck;
			case ARMOR_ITEM_WEAR_BODY:
				return armor_body;
			case ARMOR_ITEM_WEAR_HEAD:
				return armor_head;
			case ARMOR_ITEM_WEAR_LEGS:
				return armor_legs;
			case ARMOR_ITEM_WEAR_FEET:
				return armor_feet;
			case ARMOR_ITEM_WEAR_HANDS:
				return armor_hands;
			case ARMOR_ITEM_WEAR_ARMS:
				return armor_arms;
			case ARMOR_ITEM_WEAR_SHIELD:
				return armor_shield;
			case ARMOR_ITEM_WEAR_ABOUT:
				return armor_about;
			case ARMOR_ITEM_WEAR_WAIST:
				return armor_waist;
			case ARMOR_ITEM_WEAR_WRIST:
				return armor_wrist;
			case ARMOR_ITEM_WEAR_WIELD:
				return armor_wield;
			case ARMOR_ITEM_WEAR_HOLD:
				return armor_hold;
			case ARMOR_ITEM_WEAR_SECONDARY:
				return armor_secondary;
			case ARMOR_ITEM_WEAR_SHOULDERS:
				return armor_shoulders;
			case ARMOR_ITEM_WEAR_VEST_PACK:
				return armor_vest_pack;
			case ARMOR_ITEM_WEAR_ELBOW:
				return armor_elbow;
			case ARMOR_ITEM_WEAR_BACKPACK:
				return armor_backpack;
			case ARMOR_ITEM_WEAR_GOGGLES:
				return armor_goggles;
			case ARMOR_ITEM_WEAR_WEAPON_ATTACHMENT:
			default:
				return empty;
		}
	}

	void generated_armor_t::fill(obj_ptr_t& obj) {
		fill_attributes(obj);
		fill_elemental(obj);
		fill_stats(obj);
	}

	void generated_armor_t::fill_stats(obj_ptr_t& obj) {
		fill_stats_generic(obj,m_stat_boosts,obj->armor()->attributes.get());
	}
	void generated_armor_t::fill_attributes(obj_ptr_t& obj) {
		fill_armor_attributes_generic(obj,m_attributes,obj->armor()->attributes.get());
	}
	void generated_armor_t::fill_elemental(obj_ptr_t& obj) {
		fill_elemental_resistances_generic(obj,m_elemental_resistances,obj->armor()->attributes.get());
	}

	generated_armor_t::generated_armor_t (player_ptr_t& player) {
		m_player = player;
		load_from_sql();
	}

	obj_ptr_t generated_armor_t::roll() {

		std::string yaml = "";
		uint8_t max_loops = 100;
		do {
			m_type = mods::forge_engine::item_generator.random_armor_type();
			yaml = random_yaml(yaml_list(m_type));
		} while(yaml.length() == 0 && max_loops-- > 0);
		if(yaml.length() == 0) {
			log("SYSERR: generated_armor_t is continuing with a zero-length yaml file! randomized armor loot will be terrible!");
			exit(-4);
		}

		m_requirements = mods::forge_engine::item_generator.generate_requirements(m_player);
		m_attributes = mods::forge_engine::item_generator.generate_armor_attributes(m_player);
		m_elemental_damages = mods::forge_engine::item_generator.generate_armor_elemental_boosts(m_player);
		m_elemental_resistances = mods::forge_engine::item_generator.generate_armor_elemental_resistances(m_player);
		m_stat_boosts = mods::forge_engine::item_generator.generate_armor_stat_boosts(m_player);
		m_instance = create_object(ITEM_ARMOR, random_yaml(yaml_list(m_type)));
		this->fill(m_instance);
		return m_instance;
	}

	void generated_armor_t::send_stats_to_player(player_ptr_t& player) {
		player->send("%s\r\n", get_dump().c_str());
	}

	std::string generated_armor_t::get_dump() {
		std::string output;
		output += "---- start of dump ----\r\n";
		output += CAT("random armor type: ", to_string(m_type), "\r\n");

		for(auto& req : m_attributes) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("attribute: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else {
				output += CAT("attribute: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		for(auto& req : m_stat_boosts) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else if(std::holds_alternative<uint32_t> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		for(auto& req : m_elemental_damages) {
			if(std::holds_alternative<float> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<float> (req.second)), "\r\n");
			} else if(std::holds_alternative<uint32_t> (req.second)) {
				output += CAT("stat_boost: ", to_string(req.first), ": ", std::to_string(std::get<uint32_t> (req.second)), "\r\n");
			}
		}

		output += CAT("\r\n", get_requirements_string(m_requirements), "\r\n");
		output += "---- End of dump ----\r\n";
		return output;
	}

};//end namespace mods::forge_engine
#undef m_debug
