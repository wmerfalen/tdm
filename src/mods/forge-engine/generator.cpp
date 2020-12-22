#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"

namespace mods::forge_engine {
	static mods::forge_engine::generator item_generator;
	static constexpr float ATTRIBUTE_LOW = 1.0;
	static constexpr float ATTRIBUTE_HIGH= 10000.0;
	static constexpr uint8_t MAX_ATTRIBUTES = 35;
	static constexpr float STAT_BOOST_LOW = 1.0;
	static constexpr float STAT_BOOST_HIGH= 10000.0;
	static constexpr uint8_t MAX_RIFLE_STAT_BOOSTS = 35;

	template <>
	bool generator::roll<bool>(){
		return rand_xoroshiro() & 1;
	}
	generator::generator(){
		this->init();
	}
	void generator::init(){
		std::cerr << "init ";
		m_player_level = 10;
	}
	generator::~generator(){
		std::cout << "[generator::~generator]\n";
	}
	std::string to_string(attachment_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(ATTACHMENT_TYPE_SIGHT);
			M_LAZY(ATTACHMENT_TYPE_UNDER_BARREL);
			M_LAZY(ATTACHMENT_TYPE_GRIP);
			M_LAZY(ATTACHMENT_TYPE_BARREL);
			M_LAZY(ATTACHMENT_TYPE_MUZZLE);
			M_LAZY(ATTACHMENT_TYPE_MAGAZINE);
			M_LAZY(ATTACHMENT_TYPE_STOCK);
			M_LAZY(ATTACHMENT_TYPE_STRAP);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	std::string to_string(explosive_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(EXPLOSIVE_TYPE_FRAG_GRENADE);
			M_LAZY(EXPLOSIVE_TYPE_INCENDIARY_GRENADE);
			M_LAZY(EXPLOSIVE_TYPE_REMOTE_EXPLOSIVE);
			M_LAZY(EXPLOSIVE_TYPE_REMOTE_CHEMICAL);
			M_LAZY(EXPLOSIVE_TYPE_EMP_GRENADE);
			M_LAZY(EXPLOSIVE_TYPE_CLAYMORE_MINE);
			M_LAZY(EXPLOSIVE_TYPE_SMOKE_GRENADE);
			M_LAZY(EXPLOSIVE_TYPE_C4);
			M_LAZY(EXPLOSIVE_TYPE_BREACH_CHARGE);
			M_LAZY(EXPLOSIVE_TYPE_THERMITE_CHARGE);
			M_LAZY(EXPLOSIVE_TYPE_FLASHBANG_GRENADE);
			M_LAZY(EXPLOSIVE_TYPE_SENSOR_GRENADE);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}
	std::string to_string(armor_wear_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
M_LAZY(ARMOR_ITEM_WEAR_FINGER);
M_LAZY(ARMOR_ITEM_WEAR_NECK);
M_LAZY(ARMOR_ITEM_WEAR_BODY);
M_LAZY(ARMOR_ITEM_WEAR_HEAD);
M_LAZY(ARMOR_ITEM_WEAR_LEGS);
M_LAZY(ARMOR_ITEM_WEAR_FEET);
M_LAZY(ARMOR_ITEM_WEAR_HANDS);
M_LAZY(ARMOR_ITEM_WEAR_ARMS);
M_LAZY(ARMOR_ITEM_WEAR_SHIELD);
M_LAZY(ARMOR_ITEM_WEAR_ABOUT);
M_LAZY(ARMOR_ITEM_WEAR_WAIST);
M_LAZY(ARMOR_ITEM_WEAR_WRIST);
M_LAZY(ARMOR_ITEM_WEAR_WIELD);
M_LAZY(ARMOR_ITEM_WEAR_HOLD);
M_LAZY(ARMOR_ITEM_WEAR_SECONDARY);
M_LAZY(ARMOR_ITEM_WEAR_SHOULDERS);
M_LAZY(ARMOR_ITEM_WEAR_VEST_PACK);
M_LAZY(ARMOR_ITEM_WEAR_ELBOW);
M_LAZY(ARMOR_ITEM_WEAR_BACKPACK);
M_LAZY(ARMOR_ITEM_WEAR_GOGGLES);
M_LAZY(ARMOR_ITEM_WEAR_WEAPON_ATTACHMENT);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}
	std::string to_string(elemental_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(ELEM_INCENDIARY);
			M_LAZY(ELEM_EXPLOSIVE);
			M_LAZY(ELEM_SHRAPNEL);
			M_LAZY(ELEM_CORROSIVE);
			M_LAZY(ELEM_CRYOGENIC);
			M_LAZY(ELEM_RADIOACTIVE);
			M_LAZY(ELEM_EMP);
			M_LAZY(ELEM_SHOCK);
			M_LAZY(ELEM_ANTI_MATTER);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}
	std::string to_string(stat_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(SKILL_STR);
			M_LAZY(SKILL_STR_ADD );
			M_LAZY(SKILL_INTEL );
			M_LAZY(SKILL_WIS );
			M_LAZY(SKILL_DEX );
			M_LAZY(SKILL_CON );
			M_LAZY(SKILL_CHA );
			M_LAZY(SKILL_ELECTRONICS );
			M_LAZY(SKILL_ARMOR );
			M_LAZY(SKILL_MARKSMANSHIP );
			M_LAZY(SKILL_SNIPING );
			M_LAZY(SKILL_DEMOLITIONS );
			M_LAZY(SKILL_CHEMISTRY );
			M_LAZY(SKILL_WEAPON_HANDLING );
			M_LAZY(SKILL_STRATEGY );
			M_LAZY(SKILL_MEDICAL );
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	std::string to_string(item_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(TYPE_ITEM_RIFLE);
			M_LAZY(TYPE_ITEM_EXPLOSIVE);
			M_LAZY(TYPE_ITEM_GADGET);
			M_LAZY(TYPE_ITEM_DRONE);
			M_LAZY(TYPE_ITEM_WEAPON);
			M_LAZY(TYPE_ITEM_ATTACHMENT);
			M_LAZY(TYPE_ITEM_ARMOR);
			M_LAZY(TYPE_ITEM_CONSUMABLE);
			M_LAZY(TYPE_ITEM_TRAP);
#undef M_LAZY
			default:
			std::cerr << "unknown: " << (int)t << "\n";
			return "<unknown>";
		}
	}
	std::string to_string(rifle_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(RIFLE_TYPE_SHOTGUN);
			M_LAZY(RIFLE_TYPE_ASSAULT_RIFLE);
			M_LAZY(RIFLE_TYPE_SUB_MACHINE_GUN);
			M_LAZY(RIFLE_TYPE_SNIPER);
			M_LAZY(RIFLE_TYPE_HANDGUN);
			M_LAZY(RIFLE_TYPE_PISTOL);
			M_LAZY(RIFLE_TYPE_MACHINE_PISTOL);
			M_LAZY(RIFLE_TYPE_LIGHT_MACHINE_GUN);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	std::string to_string(rifle_attributes_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(RIFLE_ATTRIBUTES_AMMO_MAX);
			M_LAZY(RIFLE_ATTRIBUTES_CHANCE_TO_INJURE);
			M_LAZY(RIFLE_ATTRIBUTES_CLIP_SIZE);
			M_LAZY(RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS);
			M_LAZY(RIFLE_ATTRIBUTES_CRITICAL_CHANCE);
			M_LAZY(RIFLE_ATTRIBUTES_CRITICAL_RANGE);
			M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_PER_SECOND);
			M_LAZY(RIFLE_ATTRIBUTES_DISORIENT_AMOUNT);
			M_LAZY(RIFLE_ATTRIBUTES_HEADSHOT_BONUS);
			M_LAZY(RIFLE_ATTRIBUTES_MAX_RANGE);
			M_LAZY(RIFLE_ATTRIBUTES_RANGE_MULTIPLIER);
			M_LAZY(RIFLE_ATTRIBUTES_RELOAD_TIME);
			M_LAZY(RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE);
			M_LAZY(RIFLE_ATTRIBUTES_MUZZLE_VELOCITY);
			M_LAZY(RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE);
			M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT);
			M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	rifle_types_t generator::random_rifle_type(){
		uint32_t size = std::distance(valid_rifle_types.begin(),valid_rifle_types.end());
		return valid_rifle_types.at(this->roll<uint32_t>() % size);
	}
	rifle_attributes_t generator::random_rifle_attribute(){
		uint32_t size = std::distance(valid_rifle_attributes.begin(),valid_rifle_attributes.end());
		return valid_rifle_attributes.at(this->roll<uint32_t>() % size);
	}

	std::vector<std::pair<rifle_attributes_t,std::variant<uint32_t,float>>> generator::generate_rifle_attributes(player_ptr_t& player){
		/** TODO: game balancing: these values need to be tweaked */
		float level = (float)player->level();
		float low = 1.0;
		float high = level * 0.9;
		float low_low = level / 3.5;
		float stat_low = std::clamp(low_low, low,high);
		float stat_high = std::clamp(level,low+1,high+2);
		uint32_t low_level = player->level() / 3;
		uint32_t high_level = player->level() / 1.3;
		uint32_t low_low_clamp = 1, low_high_clamp = 4 + player->level();
		uint32_t high_low_clamp = 2, high_high_clamp = 5 + player->level();
		uint32_t uint_low = std::clamp(low_level, low_low_clamp, low_high_clamp);
		uint32_t uint_high = std::clamp(high_level, high_low_clamp, high_high_clamp);
		uint8_t max_rifle_stats = player->level() + 10;
		player->send("stat_low: %f\nstat_high: %f\nuint_low: %d\nuint_high: %d\nmax_stats: %d\n\n",stat_low,stat_high,uint_low,uint_high, max_rifle_stats);

		return generate_random_mixed<rifle_attributes_t,uint32_t>(valid_rifle_attributes,stat_low,stat_high,uint_low,uint_high,max_rifle_stats);
	}

	std::vector<std::pair<stat_types_t,uint32_t>> generator::generate_rifle_requirements(player_ptr_t& player){
		/** TODO: game balancing: these values need to be tweaked */
		uint8_t max_rifle_reqs = std::clamp((uint8_t)(player->level() / 30),(uint8_t)1,(uint8_t)(player->level() / 10));
		player->send("max rifle reqs: %d\n", max_rifle_reqs);
		return generate_random<stat_types_t,uint32_t,2>(valid_rifle_requirements_based_on,max_rifle_reqs);
	}

	std::vector<std::pair<stat_types_t,std::variant<uint32_t,float>>> generator::generate_rifle_stat_boosts(player_ptr_t& player){
		/** TODO: game balancing: these values need to be tweaked */
		float level = (float)player->level();
		float low = 1.0;
		float high = level * 0.9;
		float low_low = level / 3.5;
		float stat_low = std::clamp(low_low, low,high);
		float stat_high = std::clamp(level,low+1,high+2);
		uint32_t low_level = player->level() / 3;
		uint32_t high_level = player->level() / 1.3;
		uint32_t low_low_clamp = 1, low_high_clamp = 4 + player->level();
		uint32_t high_low_clamp = 2, high_high_clamp = 5 + player->level();
		uint32_t uint_low = std::clamp(low_level, low_low_clamp, low_high_clamp);
		uint32_t uint_high = std::clamp(high_level, high_low_clamp, high_high_clamp);
		uint8_t max_rifle_stats = player->level() + 10;
		player->send("stat_low: %f\nstat_high: %f\nuint_low: %d\nuint_high: %d\nmax_stats: %d\n\n",stat_low,stat_high,uint_low,uint_high, max_rifle_stats);

		return generate_random_mixed<stat_types_t,uint32_t>(valid_rifle_stats_boosts,stat_low,stat_high,uint_low,uint_high,max_rifle_stats);
	}

	std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> generator::generate_rifle_elemental_boosts(player_ptr_t& player){
		/** TODO: game balancing: these values need to be tweaked */
		float level = (float)player->level();
		float low = 1.0;
		float high = level * 0.9;
		float low_low = level / 3.5;
		float stat_low = std::clamp(low_low, low,high);
		float stat_high = std::clamp(level,low+1,high+2);
		uint32_t low_level = player->level() / 3;
		uint32_t high_level = player->level() / 1.3;
		uint32_t low_low_clamp = 1, low_high_clamp = 4 + player->level();
		uint32_t high_low_clamp = 2, high_high_clamp = 5 + player->level();
		uint32_t uint_low = std::clamp(low_level, low_low_clamp, low_high_clamp);
		uint32_t uint_high = std::clamp(high_level, high_low_clamp, high_high_clamp);
		uint8_t max_rifle_stats = 1;
		player->send("stat_low: %f\nstat_high: %f\nuint_low: %d\nuint_high: %d\nmax_stats: %d\n\n",stat_low,stat_high,uint_low,uint_high, max_rifle_stats);
		return generate_random_mixed<elemental_types_t,uint32_t>(valid_elemental_types, stat_low,stat_high,uint_low,uint_high,max_rifle_stats);

	}
	item_types_t generator::random_item_type(){
		uint32_t size = std::distance(active_item_types.begin(),active_item_types.end());
		return active_item_types.at(this->roll<uint32_t>() % size);
	}

	/** admin utility function */
	ACMD(do_reward){
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("reward");
		static constexpr const char* usage = "usage: reward <type> <level>...<level N>\r\n"
			"valid types:\r\n"
			"\t- rifle\r\n"
			"\t- explosive\r\n"
			"\t- drone\r\n"
			"\t- armor\r\n"
			"example: reward rifle 1 10 30\r\n"
			"\t this example will spin the forge engine for 3 rifles at levels 1, 10, and 30.\r\n";
		auto vec_args = PARSE_ARGS();
		std::vector<std::string> screen;
		if(vec_args.size() < 2){
			player->sendln(usage);
			return;
		}
		using namespace mods::forge_engine;
		auto saved_level = player->level();
		if(vec_args[0].compare("explosive") == 0){
			for(uint8_t i = 1; i < vec_args.size(); ++i){
				int level = mods::util::stoi(vec_args[i]).value_or(-1);
				if(level <= 0){
					player->error(CAT("Invalid numeric value encountered at string: '",vec_args[i],"'\r\n"));
					continue;
				}
				player->level() = level;
				auto explosive_type = mods::forge_engine::item_generator.random_explosive_type();
				auto elem = mods::forge_engine::item_generator.random_elemental_type();
				player->send("Explosive type: %s, elemental: %s\r\n",to_string(explosive_type).c_str(), to_string(elem).c_str());
			}
			player->level() = saved_level;
		}
		if(vec_args[0].compare("armor") == 0){
			for(uint8_t i = 1; i < vec_args.size(); ++i){
				int level = mods::util::stoi(vec_args[i]).value_or(-1);
				if(level <= 0){
					player->error(CAT("Invalid numeric value encountered at string: '",vec_args[i],"'\r\n"));
					continue;
				}
				player->level() = level;
				auto armor_type = mods::forge_engine::item_generator.random_armor_type();
				auto elem = mods::forge_engine::item_generator.random_elemental_resistance(armor_type);
				player->send("Armor type: %s, elemental: %s\r\n",to_string(armor_type).c_str(), to_string(elem).c_str());
			}
			player->level() = saved_level;
		}
		if(vec_args[0].compare("rifle") == 0){
			std::vector<std::pair<stat_types_t,std::variant<uint32_t,float>>> stats;
			std::vector<std::pair<stat_types_t,uint32_t>> requirements;
			std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> elementals;
			std::vector<std::pair<rifle_attributes_t,std::variant<uint32_t,float>>> attributes;
			for(uint8_t i = 1; i < vec_args.size(); ++i){
				int level = mods::util::stoi(vec_args[i]).value_or(-1);
				if(level <= 0){
					player->error(CAT("Invalid numeric value encountered at string: '",vec_args[i],"'\r\n"));
					continue;
				}
				player->level() = level;
				auto rifle_type = mods::forge_engine::item_generator.random_rifle_type();
				player->send("random rifle type: %s\r\n", to_string(rifle_type).c_str());
				attributes = mods::forge_engine::item_generator.generate_rifle_attributes(player);
				for(auto & req : attributes){
					player->send("attribute: %s: %d\r\n",to_string(req.first).c_str(),req.second);
				}
				stats = mods::forge_engine::item_generator.generate_rifle_stat_boosts(player);
				for(auto & req : stats){
					if(std::holds_alternative<float>(req.second)){
						player->send("stat_boost: %s: %f\r\n",to_string(req.first).c_str(),std::get<float>(req.second));
					}else if(std::holds_alternative<uint32_t>(req.second)){
						player->send("stat_boost: %s: %d\r\n",to_string(req.first).c_str(),std::get<uint32_t>(req.second));
					}
				}
				elementals = mods::forge_engine::item_generator.generate_rifle_elemental_boosts(player);
				for(auto & req : elementals){
					if(std::holds_alternative<float>(req.second)){
						player->send("stat_boost: %s: %f\r\n",to_string(req.first).c_str(),std::get<float>(req.second));
					}else if(std::holds_alternative<uint32_t>(req.second)){
						player->send("stat_boost: %s: %d\r\n",to_string(req.first).c_str(),std::get<uint32_t>(req.second));
					}
				}
				requirements = mods::forge_engine::item_generator.generate_rifle_requirements(player);
				for(auto & req : requirements){
					player->send("requirement: %s: %d\r\n",to_string(req.first).c_str(),req.second);
				}
			}
		}
		player->level() = saved_level;
	}

	/** game init */
	void init(){
		mods::interpreter::add_command("reward", POS_RESTING, do_reward, LVL_BUILDER,0);
	}
};
