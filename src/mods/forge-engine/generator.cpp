#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generator]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

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
	std::string to_string(explosive_attributes_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(EXPLOSIVE_ATTR_ALTERNATE_EXPLOSION_TYPE);
			M_LAZY(EXPLOSIVE_ATTR_CHANCE_TO_INJURE);
			M_LAZY(EXPLOSIVE_ATTR_CRITICAL_CHANCE);
			M_LAZY(EXPLOSIVE_ATTR_CRITICAL_RANGE);
			M_LAZY(EXPLOSIVE_ATTR_BLAST_RADIUS);
			M_LAZY(EXPLOSIVE_ATTR_DAMAGE_PER_SECOND);
			M_LAZY(EXPLOSIVE_ATTR_DISORIENT_AMOUNT);
			M_LAZY(EXPLOSIVE_ATTR_RANGE_MODIFIER);
			M_LAZY(EXPLOSIVE_ATTR_LOUDNESS_TYPE);
			M_LAZY(EXPLOSIVE_ATTR_INCENDIARY_DAMAGE_DICE_COUNT);
			M_LAZY(EXPLOSIVE_ATTR_INCENDIARY_DAMAGE_DICE_SIDES);
			M_LAZY(EXPLOSIVE_ATTR_RADIATION_DAMAGE_DICE_COUNT);
			M_LAZY(EXPLOSIVE_ATTR_RADIATION_DAMAGE_DICE_SIDES);
			M_LAZY(EXPLOSIVE_ATTR_CHEMICAL_DAMAGE_DICE_COUNT);
			M_LAZY(EXPLOSIVE_ATTR_CHEMICAL_DAMAGE_DICE_SIDES);
			M_LAZY(EXPLOSIVE_ATTR_ELECTRIC_DAMAGE_DICE_COUNT);
			M_LAZY(EXPLOSIVE_ATTR_ELECTRIC_DAMAGE_DICE_SIDES);
			M_LAZY(EXPLOSIVE_ATTR_ARMOR_PENETRATION_DAMAGE_DICE_COUNT);
			M_LAZY(EXPLOSIVE_ATTR_ARMOR_PENETRATION_DAMAGE_DICE_SIDES);
			M_LAZY(EXPLOSIVE_ATTR_DAMAGE_DICE_COUNT);
			M_LAZY(EXPLOSIVE_ATTR_DAMAGE_DICE_SIDES);
			M_LAZY(EXPLOSIVE_ATTR_INCENDIARY_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_EXPLOSIVE_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_SHRAPNEL_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_CORROSIVE_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_CRYOGENIC_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_RADIATION_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_EMP_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_SHOCK_DAMAGE_PERCENT);
			M_LAZY(EXPLOSIVE_ATTR_ANTI_MATTER_DAMAGE_PERCENT);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	std::string to_string(player_class_types_t t){
		switch(t){
				case PLCLASS_SNIPER:
					return "SNIPER";
				case PLCLASS_MARINE:
					return "MARINE";
				case PLCLASS_BREACHER:
					return "BREACHER";
				case PLCLASS_ENGINEER:
					return "ENGINEER";
				case PLCLASS_MEDIC:
					return "MEDIC";
				case PLCLASS_GHOST:
					return "GHOST";
				case PLCLASS_SUPPORT:
					return "SUPPORT";
				default:
					return "<unknown>";
		}
	}
	std::string to_string(player_class_t t){
		switch(t){
				case CLASS_SNIPER:
					return "SNIPER";
				case CLASS_MARINE:
					return "MARINE";
				case CLASS_BREACHER:
					return "BREACHER";
				case CLASS_ENGINEER:
					return "ENGINEER";
				case CLASS_MEDIC:
					return "MEDIC";
				case CLASS_GHOST:
					return "GHOST";
				case CLASS_SUPPORT:
					return "SUPPORT";
				default:
					return "<unknown>";
		}
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
	std::string to_string(armor_types_t t){
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

	std::vector<std::pair<explosive_attributes_t,std::variant<uint32_t,float>>> generator::generate_explosive_attributes(player_ptr_t& player){
		return generate_random_mixed<explosive_attributes_t,uint32_t>(valid_explosive_attributes,player);
	}


	std::vector<std::pair<rifle_attributes_t,std::variant<uint32_t,float>>> generator::generate_rifle_attributes(player_ptr_t& player){
		return generate_random_mixed<rifle_attributes_t,uint32_t>(valid_rifle_attributes,player);
	}

	requirements_t generator::generate_requirements(player_ptr_t& player){
		requirements_t reqs;
		reqs.minimum_player_level = 0;

		/** TODO: game balancing: these values need to be tweaked */
		if(this->roll<bool>()){
			uint8_t max_rifle_reqs = std::clamp((uint8_t)(player->level() / 30),(uint8_t)1,(uint8_t)(player->level() / 10));
			reqs.stats = generate_random<stat_types_t,uint32_t,2>(valid_requirements,max_rifle_reqs);
		}
		if(this->roll<bool>()){
			/** TODO: mess with this a bit */
			reqs.minimum_player_level = player->level();
		}
		if(this->roll<bool>()){
			reqs.player_classes.emplace_back(generate_random_class(player));
		}
		return reqs;
	}

	std::vector<std::pair<stat_types_t,std::variant<uint32_t,float>>> generator::generate_rifle_stat_boosts(player_ptr_t& player){
		return generate_random_mixed<stat_types_t,uint32_t>(valid_rifle_stats_boosts,player);
	}

	std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> generator::generate_rifle_elemental_boosts(player_ptr_t& player){
		return generate_random_mixed<elemental_types_t,uint32_t>(valid_elemental_types, player);

	}
	item_types_t generator::random_item_type(){
		uint32_t size = std::distance(active_item_types.begin(),active_item_types.end());
		return active_item_types.at(this->roll<uint32_t>() % size);
	}
	std::string get_requirements_string(requirements_t& requirements){
		std::string output;
		if(requirements.minimum_player_level){
			output += CAT("minimum player level: ", requirements.minimum_player_level, "\r\n");
		}
		for(auto & pclass : requirements.player_classes){
			output += CAT("class requirement: ",to_string(pclass),"\r\n");
		}
		for(auto & req : requirements.stats){
			output += CAT("requirement: ", to_string(req.first),": ",req.second,"\r\n");
		}
		return std::move(output);
	}

	void send_requirements(requirements_t& requirements, player_ptr_t& player){
		player->send("%s\r\n",get_requirements_string(requirements).c_str());
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
		requirements_t requirements;
		/** generate explosive */
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
				requirements = mods::forge_engine::item_generator.generate_requirements(player);
				send_requirements(requirements,player);
			}
			player->level() = saved_level;
		}


		/** generate armor */
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
				requirements = mods::forge_engine::item_generator.generate_requirements(player);
				send_requirements(requirements,player);
			}
			player->level() = saved_level;
		}


		/** generate rifle */
		if(vec_args[0].compare("rifle") == 0){

			for(uint8_t i = 1; i < vec_args.size(); ++i){
				int level = mods::util::stoi(vec_args[i]).value_or(-1);
				if(level <= 0){
					player->error(CAT("Invalid numeric value encountered at string: '",vec_args[i],"'\r\n"));
					continue;
				}
				player->level() = level;
				generated_rifle_t rifle(player);
				rifle.send_stats_to_player(player);
			}
		}
		player->level() = saved_level;
	}

	generated_rifle_t::generated_rifle_t(player_ptr_t& player){
		m_player = player;
	}
	obj_ptr_t generated_rifle_t::roll(){
		m_type = mods::forge_engine::item_generator.random_rifle_type();
		m_requirements = mods::forge_engine::item_generator.generate_requirements(m_player);
		m_attributes = mods::forge_engine::item_generator.generate_rifle_attributes(m_player);
		m_elemental_damages = mods::forge_engine::item_generator.generate_rifle_elemental_boosts(m_player);
		m_stat_boosts = mods::forge_engine::item_generator.generate_rifle_stat_boosts(m_player);
		m_instance = create_object(ITEM_RIFLE,"g36c.yml");
		return m_instance;
	}
		void generated_rifle_t::send_stats_to_player(player_ptr_t& player){
			player->send("%s\r\n",get_dump().c_str());
		}
		std::string generated_rifle_t::get_dump(){
			std::string output;
			output += "---- start of dump ----\r\n";
			output += CAT("random rifle type: ", to_string(m_type), "\r\n");
			for(auto & req : m_attributes){
				if(std::holds_alternative<float>(req.second)){
					output += CAT("attribute: ", to_string(req.first),": ", std::to_string(std::get<float>(req.second)),"\r\n");
				}else {
					output += CAT("attribute: ", to_string(req.first),": ", std::to_string(std::get<uint32_t>(req.second)),"\r\n");
				}
			}
			for(auto & req : m_stat_boosts){
				if(std::holds_alternative<float>(req.second)){
					output += CAT("stat_boost: ",to_string(req.first),": ",std::to_string(std::get<float>(req.second)),"\r\n");
				}else if(std::holds_alternative<uint32_t>(req.second)){
					output += CAT("stat_boost: ",to_string(req.first),": ",std::to_string(std::get<uint32_t>(req.second)),"\r\n");
				}
			}
			for(auto & req : m_elemental_damages){
				if(std::holds_alternative<float>(req.second)){
					output += CAT("stat_boost: ",to_string(req.first),": ",std::to_string(std::get<float>(req.second)),"\r\n");
				}else if(std::holds_alternative<uint32_t>(req.second)){
					output += CAT("stat_boost: ",to_string(req.first),": ",std::to_string(std::get<uint32_t>(req.second)),"\r\n");
				}
			}
			output += CAT("\r\n", get_requirements_string(m_requirements), "\r\n");
			output += "---- End of dump ----\r\n";
			return output;
		}

	/** game init */
	void init(){
		mods::interpreter::add_command("reward", POS_RESTING, do_reward, LVL_BUILDER,0);
	}
};
#undef m_debug
