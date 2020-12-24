#include "generated-rifle.hpp"
#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "util.hpp"
#include "../orm/rifle-index.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generated_rifle]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {
	extern generator item_generator;
	static bool refresh_rifle_index = true;
	static std::vector<std::string> sub_machine_guns;
	static std::vector<std::string> sniper_rifles;
	static std::vector<std::string> light_machine_guns;
	static std::vector<std::string> pistols;
	static std::vector<std::string> assault_rifles;
	static std::vector<std::string> shotguns;
	static std::vector<std::string> machine_pistols;
	static std::vector<std::string> empty;

	void rifle_index_changed() {
		refresh_rifle_index = true;
	}

	void generated_rifle_t::load_from_sql() {
		static std::map<std::string,std::vector<std::string>> data;
		if(refresh_rifle_index) {
			mods::orm::load_all_rifle_index_data(&data);
			refresh_rifle_index = false;
		}
		for(auto& row : data) {
			if(row.first.compare("ar") == 0) {
				assault_rifles = row.second;
			}
			if(row.first.compare("smg") == 0) {
				sub_machine_guns = row.second;
			}
			if(row.first.compare("sniper") == 0) {
				sniper_rifles = row.second;
			}
			if(row.first.compare("lmg") == 0) {
				light_machine_guns = row.second;
			}
			if(row.first.compare("pistol") == 0) {
				pistols = row.second;
			}
			if(row.first.compare("shotgun") == 0) {
				shotguns = row.second;
			}
			if(row.first.compare("mp") == 0) {
				machine_pistols = row.second;
			}
		}
	}
	const std::vector<std::string>& generated_rifle_t::yaml_list(rifle_types_t t) {
		switch(t) {
			case RIFLE_TYPE_SHOTGUN:
				return shotguns;
			case RIFLE_TYPE_ASSAULT_RIFLE:
				return assault_rifles;
			case RIFLE_TYPE_SUB_MACHINE_GUN:
				return  sub_machine_guns;
			case RIFLE_TYPE_SNIPER:
				return sniper_rifles;
			case RIFLE_TYPE_HANDGUN:
			case RIFLE_TYPE_PISTOL:
				return pistols;
			case RIFLE_TYPE_MACHINE_PISTOL:
				return machine_pistols;
			case RIFLE_TYPE_LIGHT_MACHINE_GUN:
				return light_machine_guns;
			default:
				std::cerr << "[WARNING]: unknown rifle type at '" << __FILE__ << ":" << __LINE__ << ": type: '" << std::to_string(t) << "'\n";
				return empty;
		}
	}


	void generated_rifle_t::fill(obj_ptr_t& obj) {
		fill_attributes(obj);
		fill_elemental(obj);
		fill_stats(obj);
	}

	void generated_rifle_t::fill_stats(obj_ptr_t& obj) {
		fill_stats_generic(obj,m_stat_boosts,obj->rifle()->attributes.get());
	}
	void generated_rifle_t::fill_attributes(obj_ptr_t& obj) {
		fill_rifle_attributes_generic(obj,m_attributes,obj->rifle()->attributes.get());
	}
	void generated_rifle_t::fill_elemental(obj_ptr_t& obj) {
		fill_elemental_damage_generic(obj,m_elemental_damages,obj->rifle()->attributes.get());
	}

	generated_rifle_t::generated_rifle_t (player_ptr_t& player) {
		m_player = player;
		load_from_sql();
	}

	obj_ptr_t generated_rifle_t::roll() {
		m_type = mods::forge_engine::item_generator.random_rifle_type();
		m_requirements = mods::forge_engine::item_generator.generate_requirements(m_player);
		m_attributes = mods::forge_engine::item_generator.generate_rifle_attributes(m_player);
		m_elemental_damages = mods::forge_engine::item_generator.generate_rifle_elemental_boosts(m_player);
		m_stat_boosts = mods::forge_engine::item_generator.generate_rifle_stat_boosts(m_player);
		m_instance = create_object(ITEM_RIFLE,random_yaml(yaml_list(m_type)));
		this->fill(m_instance);
		return m_instance;
	}

	void generated_rifle_t::send_stats_to_player(player_ptr_t& player) {
		player->send("%s\r\n", get_dump().c_str());
	}

	std::string generated_rifle_t::get_dump() {
		std::string output;
		output += "---- start of dump ----\r\n";
		output += CAT("random rifle type: ", to_string(m_type), "\r\n");

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
