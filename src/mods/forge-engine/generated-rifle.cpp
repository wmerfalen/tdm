#include "generated-rifle.hpp"
#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "util.hpp"
#include "../orm/rifle-index.hpp"

#define __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
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


	void generated_rifle_t::init() {
		m_force_type = false;
		m_player = {};
		m_type = RIFLE_TYPE_SNIPER;
		m_requirements = {};
		m_attributes.clear();
		m_elemental_damages.clear();
		m_stat_boosts.clear();
		m_instance = nullptr;
	}
	rifle_types_t to_rifle_type_from_string(std::string_view t) {
		if(t.compare("SHOTGUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_SHOTGUN;
		}
		if(t.compare("ASSAULT_RIFLE") == 0) {
			return rifle_types_t::RIFLE_TYPE_ASSAULT_RIFLE;
		}
		if(t.compare("SUB_MACHINE_GUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_SUB_MACHINE_GUN;
		}
		if(t.compare("SNIPER") == 0) {
			return rifle_types_t::RIFLE_TYPE_SNIPER;
		}
		if(t.compare("HANDGUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_HANDGUN;
		}
		if(t.compare("PISTOL") == 0) {
			return rifle_types_t::RIFLE_TYPE_PISTOL;
		}
		if(t.compare("MACHINE_PISTOL") == 0) {
			return rifle_types_t::RIFLE_TYPE_MACHINE_PISTOL;
		}
		if(t.compare("LIGHT_MACHINE_GUN") == 0) {
			return rifle_types_t::RIFLE_TYPE_LIGHT_MACHINE_GUN;
		}
		return rifle_types_t::RIFLE_TYPE_SNIPER;
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

	generated_rifle_t::generated_rifle_t (kill_t& kill,std::string_view type) {
		this->init();
		m_force_type = true;
		m_type = to_rifle_type_from_string(type);
		m_player = kill;
		load_from_sql();
	}
	generated_rifle_t::generated_rifle_t (kill_t& kill) {
		this->init();
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

	/**
	 * #########################################################################
	 * Giant Rifle attributes randomization code
	 * #########################################################################
	 */
	static const std::vector<rifle_attributes_t> higher_is_better = {
		RIFLE_ATTRIBUTES_AMMO_MAX,
		RIFLE_ATTRIBUTES_CHANCE_TO_INJURE,
		RIFLE_ATTRIBUTES_CLIP_SIZE,
		RIFLE_ATTRIBUTES_CRITICAL_CHANCE,
		RIFLE_ATTRIBUTES_BASE_DAMAGE,
		RIFLE_ATTRIBUTES_DISORIENT_AMOUNT,
		RIFLE_ATTRIBUTES_HEADSHOT_BONUS,
		RIFLE_ATTRIBUTES_RANGE_MULTIPLIER,
		RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE,
		RIFLE_ATTRIBUTES_MUZZLE_VELOCITY,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES
	};

	static const std::vector<rifle_attributes_t> lower_is_better = {
		RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS,
		RIFLE_ATTRIBUTES_CRITICAL_RANGE,
		RIFLE_ATTRIBUTES_RELOAD_TIME,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,
		RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES
	};

	static const std::vector<rifle_attributes_t> balanced_is_better = {
		RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE,
		RIFLE_ATTRIBUTES_MAX_RANGE,
	};



	static const std::vector<attribute_limits_t> pistol_limits = {
		{
			RIFLE_ATTRIBUTES_AMMO_MAX,3,16,80
		},
		{
			RIFLE_ATTRIBUTES_CHANCE_TO_INJURE,0,80,100
		},
		{
			RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS,6,12,3
		},
		{
			RIFLE_ATTRIBUTES_CLIP_SIZE,6,21,40
		},
		{
			RIFLE_ATTRIBUTES_CRITICAL_CHANCE,0,120,250
		},
		{
			RIFLE_ATTRIBUTES_CRITICAL_RANGE,0,3,0
		},
		{
			RIFLE_ATTRIBUTES_BASE_DAMAGE,10,250,350
		},
		{
			RIFLE_ATTRIBUTES_DISORIENT_AMOUNT,0,80,120
		},
		{
			RIFLE_ATTRIBUTES_HEADSHOT_BONUS,0,50,150
		},
		{
			RIFLE_ATTRIBUTES_MAX_RANGE,0,4,20
		},
		{
			RIFLE_ATTRIBUTES_RANGE_MULTIPLIER,0,3,10
		},
		{
			RIFLE_ATTRIBUTES_RELOAD_TIME,3,15,0
		},
		{
			RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE,20,180,250
		},
		{
			/** TBA */
			RIFLE_ATTRIBUTES_MUZZLE_VELOCITY,40,40,180
		},
		{
			RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE,0,5,0
		},
		{
			RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,3,10,25
		},
		{
			RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES,6,30,80
		}
	};

	static const std::vector<attribute_limits_t> sniper_limits = {
		{
			RIFLE_ATTRIBUTES_AMMO_MAX,6,16,35
		},
		{
			RIFLE_ATTRIBUTES_CHANCE_TO_INJURE,0,90,580
		},
		{
			RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS,9,21,3
		},
		{
			RIFLE_ATTRIBUTES_CLIP_SIZE,1,12,32
		},
		{
			RIFLE_ATTRIBUTES_CRITICAL_CHANCE,0,100,950
		},
		{
			RIFLE_ATTRIBUTES_CRITICAL_RANGE,3,9,1
		},
		{
			RIFLE_ATTRIBUTES_BASE_DAMAGE,50,550,1950
		},
		{
			RIFLE_ATTRIBUTES_DISORIENT_AMOUNT,0,80,220
		},
		{
			RIFLE_ATTRIBUTES_HEADSHOT_BONUS,0,450,2350
		},
		{
			RIFLE_ATTRIBUTES_MAX_RANGE,6,12,35
		},
		{
			RIFLE_ATTRIBUTES_RANGE_MULTIPLIER,0,6,12
		},
		{
			RIFLE_ATTRIBUTES_RELOAD_TIME,6,21,0
		},
		{
			RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE,6,60,120
		},
		{
			/** TBA */
			RIFLE_ATTRIBUTES_MUZZLE_VELOCITY,40,140,980
		},
		{
			RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE,3,9,3
		},
		{
			RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,6,30,85
		},
		{
			RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES,20,80,380
		}
	};
	static const attribute_limits_t no_attribute = {__NO_ATTRIBUTE__,0,0,0};
	bool is_no_attribute(const auto& m) {
		return m.attribute == __NO_ATTRIBUTE__;
	}
	attribute_limits_t fetch_attribute_limit(const std::vector<attribute_limits_t>& limits, const rifle_attributes_t& which) {
		for(const auto& limit : limits) {
			if(limit.attribute == which) {
				return limit;
			}
		}
		return no_attribute;
	}
	attribute_limits_t generated_rifle_t::fetch_limits(const rifle_attributes_t& which) {
		switch(m_type) {
			case rifle_types_t::RIFLE_TYPE_SHOTGUN:
				break;
			case rifle_types_t::RIFLE_TYPE_ASSAULT_RIFLE:
				break;
			case rifle_types_t::RIFLE_TYPE_SUB_MACHINE_GUN:
				break;
			case rifle_types_t::RIFLE_TYPE_SNIPER:
				return fetch_attribute_limit(sniper_limits,which);
			case rifle_types_t::RIFLE_TYPE_HANDGUN:
			case rifle_types_t::RIFLE_TYPE_PISTOL:
				return fetch_attribute_limit(pistol_limits,which);
			case rifle_types_t::RIFLE_TYPE_MACHINE_PISTOL:
				break;
			case rifle_types_t::RIFLE_TYPE_LIGHT_MACHINE_GUN:
				break;
			default:
				break;
		}
		return no_attribute;
	}

	using generated_attributes_t = std::vector<std::pair<rifle_attributes_t, std::variant<uint32_t, float>>>;
	generated_attributes_t generated_rifle_t::generate_rifle_attributes() {
		//auto victim = kill.victim;
		value_scaler scale(m_player.killer);
		m_attributes = {};
		if(valid_rifle_attributes.size() == 0) {
			return m_attributes;
		}
		uint8_t i = std::clamp(roll_xoroshiro<uint8_t>(), (uint8_t)0, (uint8_t)(scale.max_stats));
		if(!i) {
			return m_attributes;
		}
		while(i-- > 0) {
			const auto& attr = valid_rifle_attributes.at(roll_xoroshiro<uint16_t>() % valid_rifle_attributes.size());
			auto limit = this->fetch_limits(attr);
			auto low = limit.low;
			auto high = limit.high;
			auto op = limit.overpowered;

			if(roll_xoroshiro<bool>()) {
				auto float_roll = roll_float(scale.stat_low,scale.stat_high);
				if(is_no_attribute(limit)) {
					m_attributes.emplace_back(attr,float_roll);
					continue;
				}
				bool is_overpowered = roll_xoroshiro<bool>();
				if(is_overpowered) {
					m_attributes.emplace_back(attr,roll_float(scale.stat_low,roll_float(scale.stat_high,op)));
				} else {
					m_attributes.emplace_back(attr,std::clamp(float_roll,(float)low,(float)high));
				}
			} else {
				auto int_roll = roll_between<uint32_t> (scale.uint_low,scale.uint_high);
				if(is_no_attribute(limit)) {
					m_attributes.emplace_back(attr,int_roll);
					continue;
				}
				bool is_overpowered = roll_xoroshiro<bool>();
				if(is_overpowered) {
					m_attributes.emplace_back(attr,roll_between<uint32_t>(scale.uint_low,roll_between(scale.uint_high,(uint32_t)op)));
				} else {
					m_attributes.emplace_back(attr,std::clamp(int_roll,(uint32_t)low,(uint32_t)high));
				}
			}
		}
		return m_attributes;
	}

	/**
	 * #########################################################################
	 * Giant Rifle attributes randomization code
	 * #########################################################################
	 */

	obj_ptr_t generated_rifle_t::roll() {

		if(!compute_weapon_cost()) {
			log("WARNING: rolling for a piece of loot without sufficient cost adjustments");
		}

		if(!m_force_type) {
			m_type = mods::forge_engine::item_generator.random_rifle_type();
		}
		m_requirements = mods::forge_engine::item_generator.generate_requirements(m_player);
		m_attributes = this->generate_rifle_attributes();
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
