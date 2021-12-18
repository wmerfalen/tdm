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

extern std::deque<char_data> mob_proto;

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

	generated_rifle_t::generated_rifle_t (kill_t& kill) {
		m_player = kill;
		load_from_sql();
	}
	bool generated_rifle_t::compute_weapon_cost() {
		m_player.find_mob();
		/**
		 * Find the mob by the VNUM
		 */
		auto opt_mob = m_player.cd();
		if(opt_mob.has_value() == false) {
			log("SYSERR: couldn't find mob[vnum:'%d'] to reward player[name:'%s']!", m_player.victim, m_player.killer->name().c_str());
			m_player.killer->sendln(
			    CAT("{red}######################################################{/red}\r\n",
			        "{red}Not rewarding a weapon because we couldn't find  {/red}\r\n",
			        "{red}the NPC you killed!                              {/red}\r\n",
			        "{red}Please report this to an admin.                  {/red}\r\n",
			        "{red}NPC vnum:",m_player.victim,"{/red}\r\n",
			        "{red}######################################################{/red}\r\n"
			       )
			);
			return false;
		}
		char_data& mob = opt_mob.value();
		/**
		 * Compute the cost of the weapon
		 */
		/**
		 * Player level multiplied by 10 is the base cost in MP
		 */
		m_player.set_cost(m_player.killer->level() * 10);

		/**
		 * How much exp does the NPC have?
		 */
		m_player.add_cost(mob.mob_specials.experience);

		/**
		 * How much MP does the NPC have?
		 */
		m_player.add_cost(mob.mob_specials.mp);

		/**
		 * How many hitpoints does the NPC have?
		 */
		m_player.add_cost(mob.points.max_hit / m_player.killer->max_hp());

		/**
		 * How high of a level is the NPC to the killer?
		 */
		m_player.add_cost(mob.player.level - m_player.killer->level());

		/**
		 * How much mana does the NPC have compared to the killer?
		 */
		m_player.add_cost(mob.points.max_mana - m_player.killer->max_mana());

		return true;
	}

	obj_ptr_t generated_rifle_t::roll() {

		if(!compute_weapon_cost()) {
			log("WARNING: rolling for a piece of loot without sufficient cost adjustments");
		}

		m_type = mods::forge_engine::item_generator.random_rifle_type();
		m_requirements = mods::forge_engine::item_generator.generate_requirements(m_player);
		m_attributes = mods::forge_engine::item_generator.generate_rifle_attributes(m_player);
		m_elemental_damages = mods::forge_engine::item_generator.generate_rifle_elemental_boosts(m_player);
		m_stat_boosts = mods::forge_engine::item_generator.generate_rifle_stat_boosts(m_player);
		m_instance = create_object(ITEM_RIFLE,random_yaml(yaml_list(m_type)));
		this->fill(m_instance);
		m_instance->forged = true;
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
