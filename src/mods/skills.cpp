#include "skills.hpp"

ACMD(do_skills){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() >= 2 && ICMP(vec_args[0],"help")){
		for(unsigned i = 1; i < vec_args.size();++i){
			for(auto & skillset : mods::skills::proficiencies::list) {
				for(auto & prof : std::get<1>(skillset)){
					if(ICMP(prof.name.c_str(),vec_args[i])){
						player->send("{grn}skill-name: '{yel}%s{/yel}'\r\n", prof.name.c_str());
						player->send("{grn}skill-description: '{yel}%s{/yel}'\r\n", prof.description.c_str());
						player->send("{grn}minium-proficiency: {yel}%d{/yel}\r\n", prof.minimum_proficiency);
						continue;
					}
				}
			}
		}
		return;
	}

	for(auto & skillset : mods::skills::proficiencies::list) {
		player->send("{grn}-[ %s ]:----------------------------------------{/grn}\r\n",std::get<0>(skillset).c_str());
		for(auto & prof : std::get<1>(skillset)){
			player->send("{gld}[%d]\t\t[%s]\t[%d]\r\n{/gld}",
					mods::skills::get_player_level(player,prof.e_name),
					prof.name.c_str(),
					prof.minimum_proficiency
			);
		}
	}
	player->sendln("To see a detailed description of a skill, type: {grn}skills help <skill>{/grn}");
	player->sendln("Example: {yel}skills help spray-chance{/yel}");
	player->sendln("NOTE: you can specify multiple skills");
	player->sendln("Example: {yel}skills help spray-chance basic-armor mold{/yel}");
	player->sendln("this documentation was written on 2020-07-06.");
}
namespace mods::skills {
	constexpr static const char* DB_PREFIX = "skills";
	static std::vector<std::string> skill_name_list;

	void refresh_skill_name_list() {
		skill_name_list.clear();
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : std::get<1>(skillset)){
				skill_name_list.emplace_back(prof.name);
			}
		}
	}

	/** PLAYER BOOTSTRAP */
	void init_player_levels(player_ptr_t& player){
		init_player_levels(player->name().c_str());
	}
	void init_player_levels(std::string_view player_name){
		strmap_t m;
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : std::get<1>(skillset)){
				m[prof.name.str()] = "0";
			}
		}
		put_player_map(player_name.data(),DB_PREFIX,m);
	}
	static std::map<std::string,int> mappings;
	static std::map<int,std::string> to_string_mappings;
	static std::map<int,proficiencies::proficiency_t> e_name_to_proficiency;
	void load_from_values(){
		using proficiency_t = mods::skills::proficiencies::proficiency_name_t;
			e_name_to_proficiency[proficiency_t::CAMERA_PLACEMENT].minimum_proficiency = SKILL_CAMERA_PLACEMENT();/** 50 */
			e_name_to_proficiency[proficiency_t::DETECT_CAMERAS].minimum_proficiency = SKILL_DETECT_CAMERAS();/** 50 */
			e_name_to_proficiency[proficiency_t::CAMERA_DESTRUCTION].minimum_proficiency = SKILL_CAMERA_DESTRUCTION();/** 100 */
			e_name_to_proficiency[proficiency_t::ELECTRIFIED_DAMAGE].minimum_proficiency = SKILL_ELECTRIFIED_DAMAGE();/** 100 */
			e_name_to_proficiency[proficiency_t::PROTECT_FROM_EMP].minimum_proficiency = SKILL_PROTECT_FROM_EMP();/** 200 */
			e_name_to_proficiency[proficiency_t::CREATE_EMP_WEAPONS].minimum_proficiency = SKILL_CREATE_EMP_WEAPONS();/** 200 */
			e_name_to_proficiency[proficiency_t::COUNTER_SHOCK].minimum_proficiency = SKILL_COUNTER_SHOCK();/** 4 */
			e_name_to_proficiency[proficiency_t::TOGGLE_THERMAL].minimum_proficiency = SKILL_TOGGLE_THERMAL();/** 2050 */
			e_name_to_proficiency[proficiency_t::TOGGLE_NIGHT_VISION].minimum_proficiency = SKILL_TOGGLE_NIGHT_VISION();/** 2050 */
			e_name_to_proficiency[proficiency_t::HEADGEAR_INTROSPECTION].minimum_proficiency = SKILL_HEADGEAR_INTROSPECTION();/** 3050 */
			e_name_to_proficiency[proficiency_t::PROXIMITY_ALARM].minimum_proficiency = SKILL_PROXIMITY_ALARM();/** 3050 */
			e_name_to_proficiency[proficiency_t::DEPLOYABLE_TURRET].minimum_proficiency = SKILL_DEPLOYABLE_TURRET();/** 3050 */
			e_name_to_proficiency[proficiency_t::DEPLOYABLE_FIRE_TURRET].minimum_proficiency = SKILL_DEPLOYABLE_FIRE_TURRET();/** 3050 */
			e_name_to_proficiency[proficiency_t::DOOR_CAMERA].minimum_proficiency = SKILL_DOOR_CAMERA();/** 3050 */
			e_name_to_proficiency[proficiency_t::BASIC_ARMOR].minimum_proficiency = SKILL_BASIC_ARMOR();/** 50 */
			e_name_to_proficiency[proficiency_t::ADVANCED_ARMOR].minimum_proficiency = SKILL_ADVANCED_ARMOR();/** 50 */
			e_name_to_proficiency[proficiency_t::ELITE_ARMOR].minimum_proficiency = SKILL_ELITE_ARMOR();/** 50 */
			e_name_to_proficiency[proficiency_t::THIEF].minimum_proficiency = SKILL_THIEF();/** 200 */
			e_name_to_proficiency[proficiency_t::MOLD].minimum_proficiency = SKILL_MOLD();/** 350 */
			e_name_to_proficiency[proficiency_t::NO_FACTOR_BASIC].minimum_proficiency = SKILL_NO_FACTOR_BASIC();/** 400 */
			e_name_to_proficiency[proficiency_t::NO_FACTOR_ADVANCED].minimum_proficiency = SKILL_NO_FACTOR_ADVANCED();/** 850 */
			e_name_to_proficiency[proficiency_t::NO_FACTOR_ELITE].minimum_proficiency = SKILL_NO_FACTOR_ELITE();/** 900 */
			e_name_to_proficiency[proficiency_t::EXPLOSIVE_RESISTANCE].minimum_proficiency = SKILL_EXPLOSIVE_RESISTANCE();/** 2050 */
			e_name_to_proficiency[proficiency_t::INJURE_RESISTANCE].minimum_proficiency = SKILL_INJURE_RESISTANCE();/** 3050 */
			e_name_to_proficiency[proficiency_t::MUNITIONS_REFLECTOR].minimum_proficiency = SKILL_MUNITIONS_REFLECTOR();/** 4 */
			e_name_to_proficiency[proficiency_t::SENTINEL_DISCIPLINE].minimum_proficiency = SKILL_SENTINEL_DISCIPLINE();/** 3 */
			e_name_to_proficiency[proficiency_t::DEPLOYABLE_SHIELD].minimum_proficiency = SKILL_DEPLOYABLE_SHIELD();/** 8050 */
			e_name_to_proficiency[proficiency_t::SPRAY_CHANCE].minimum_proficiency = SKILL_SPRAY_CHANCE();/** 150 */
			e_name_to_proficiency[proficiency_t::HEADSHOT_CHANCE].minimum_proficiency = SKILL_HEADSHOT_CHANCE();/** 250 */
			e_name_to_proficiency[proficiency_t::LIMB_CHANCE].minimum_proficiency = SKILL_LIMB_CHANCE();/** 350 */
			e_name_to_proficiency[proficiency_t::BETTER_SNIPING_ACCURACY].minimum_proficiency = SKILL_BETTER_SNIPING_ACCURACY();/** 450 */
			e_name_to_proficiency[proficiency_t::TARGET_LIMB].minimum_proficiency = SKILL_TARGET_LIMB();/** 850 */
			e_name_to_proficiency[proficiency_t::ACCURACY].minimum_proficiency = SKILL_ACCURACY();/** 150 */
			e_name_to_proficiency[proficiency_t::VIP].minimum_proficiency = SKILL_VIP();/** 250 */
			e_name_to_proficiency[proficiency_t::TRACKER].minimum_proficiency = SKILL_TRACKER();/** 350 */
			e_name_to_proficiency[proficiency_t::QUIETER_SNIPER_SHOTS].minimum_proficiency = SKILL_QUIETER_SNIPER_SHOTS();/** 350 */
			e_name_to_proficiency[proficiency_t::FASTER_BREACHES].minimum_proficiency = SKILL_FASTER_BREACHES();/** 50 */
			e_name_to_proficiency[proficiency_t::FURTHER_C4S].minimum_proficiency = SKILL_FURTHER_C4S();/** 4 */
			e_name_to_proficiency[proficiency_t::STRONGER_FRAG_ARM].minimum_proficiency = SKILL_STRONGER_FRAG_ARM();/** 50 */
			e_name_to_proficiency[proficiency_t::BIGGER_CLAYMORES].minimum_proficiency = SKILL_BIGGER_CLAYMORES();/** 50 */
			e_name_to_proficiency[proficiency_t::FASTER_THERMITES].minimum_proficiency = SKILL_FASTER_THERMITES();/** 50 */
			e_name_to_proficiency[proficiency_t::BIGGER_FIRE_NADES].minimum_proficiency = SKILL_BIGGER_FIRE_NADES();/** 50 */
			e_name_to_proficiency[proficiency_t::CRAFTY_C4].minimum_proficiency = SKILL_CRAFTY_C4();/** 4 */
			e_name_to_proficiency[proficiency_t::BETTER_EXPLOSIONS].minimum_proficiency = SKILL_BETTER_EXPLOSIONS();/** 33 */
			e_name_to_proficiency[proficiency_t::RETROFIT_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_GRENADE_LAUNCHER();/** 50 */
			e_name_to_proficiency[proficiency_t::RETROFIT_FIRE_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_FIRE_GRENADE_LAUNCHER();/** 50 */
			e_name_to_proficiency[proficiency_t::RETROFIT_SMOKE_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_SMOKE_GRENADE_LAUNCHER();/** 50 */
			e_name_to_proficiency[proficiency_t::RETROFIT_SENSOR_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_SENSOR_GRENADE_LAUNCHER();/** 50 */
			e_name_to_proficiency[proficiency_t::RETROFIT_STUN_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_STUN_GRENADE_LAUNCHER();/** 50 */
			e_name_to_proficiency[proficiency_t::STEROID_DEALER].minimum_proficiency = SKILL_STEROID_DEALER();/** 50 */
			e_name_to_proficiency[proficiency_t::HGH_DEALER].minimum_proficiency = SKILL_HGH_DEALER();/** 50 */
			e_name_to_proficiency[proficiency_t::NOXIOUS_NADE_CRAFTER].minimum_proficiency = SKILL_NOXIOUS_NADE_CRAFTER();/** 50 */
			e_name_to_proficiency[proficiency_t::DUTCH_OVEN].minimum_proficiency = SKILL_DUTCH_OVEN();/** 50 */
			e_name_to_proficiency[proficiency_t::FIRE_NADE_DEALER].minimum_proficiency = SKILL_FIRE_NADE_DEALER();/** 50 */
			e_name_to_proficiency[proficiency_t::PRO_WEAPON_HANDLER].minimum_proficiency = SKILL_PRO_WEAPON_HANDLER();/** 50 */
			e_name_to_proficiency[proficiency_t::BETTER_WEAPON_ACCURACY].minimum_proficiency = SKILL_BETTER_WEAPON_ACCURACY();/** 50 */
			e_name_to_proficiency[proficiency_t::FASTER_TRIGGER_FINGER].minimum_proficiency = SKILL_FASTER_TRIGGER_FINGER();/** 50 */
			e_name_to_proficiency[proficiency_t::FASTER_RELOADING].minimum_proficiency = SKILL_FASTER_RELOADING();/** 50 */
			e_name_to_proficiency[proficiency_t::BASIC_PISTOL].minimum_proficiency = SKILL_BASIC_PISTOL();/** 150 */
			e_name_to_proficiency[proficiency_t::BASIC_ASSAULT_RIFLE].minimum_proficiency = SKILL_BASIC_ASSAULT_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::BASIC_SHOTGUN].minimum_proficiency = SKILL_BASIC_SHOTGUN();/** 150 */
			e_name_to_proficiency[proficiency_t::BASIC_SNIPER_RIFLE].minimum_proficiency = SKILL_BASIC_SNIPER_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::BASIC_SUB_MACHINE_GUN].minimum_proficiency = SKILL_BASIC_SUB_MACHINE_GUN();/** 150 */
			e_name_to_proficiency[proficiency_t::INTERMEDIATE_PISTOL].minimum_proficiency = SKILL_INTERMEDIATE_PISTOL();/** 150 */
			e_name_to_proficiency[proficiency_t::INTERMEDIATE_ASSAULT_RIFLE].minimum_proficiency = SKILL_INTERMEDIATE_ASSAULT_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::INTERMEDIATE_SHOTGUN].minimum_proficiency = SKILL_INTERMEDIATE_SHOTGUN();/** 150 */
			e_name_to_proficiency[proficiency_t::INTERMEDIATE_SNIPER_RIFLE].minimum_proficiency = SKILL_INTERMEDIATE_SNIPER_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::INTERMEDIATE_SUB_MACHINE_GUN].minimum_proficiency = SKILL_INTERMEDIATE_SUB_MACHINE_GUN();/** 150 */
			e_name_to_proficiency[proficiency_t::ADVANCED_PISTOL].minimum_proficiency = SKILL_ADVANCED_PISTOL();/** 150 */
			e_name_to_proficiency[proficiency_t::ADVANCED_ASSAULT_RIFLE].minimum_proficiency = SKILL_ADVANCED_ASSAULT_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::ADVANCED_SHOTGUN].minimum_proficiency = SKILL_ADVANCED_SHOTGUN();/** 150 */
			e_name_to_proficiency[proficiency_t::ADVANCED_SNIPER_RIFLE].minimum_proficiency = SKILL_ADVANCED_SNIPER_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::ADVANCED_SUB_MACHINE_GUN].minimum_proficiency = SKILL_ADVANCED_SUB_MACHINE_GUN();/** 150 */
			e_name_to_proficiency[proficiency_t::EXPERT_PISTOL].minimum_proficiency = SKILL_EXPERT_PISTOL();/** 150 */
			e_name_to_proficiency[proficiency_t::EXPERT_ASSAULT_RIFLE].minimum_proficiency = SKILL_EXPERT_ASSAULT_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::EXPERT_SHOTGUN].minimum_proficiency = SKILL_EXPERT_SHOTGUN();/** 150 */
			e_name_to_proficiency[proficiency_t::EXPERT_SNIPER_RIFLE].minimum_proficiency = SKILL_EXPERT_SNIPER_RIFLE();/** 150 */
			e_name_to_proficiency[proficiency_t::EXPERT_SUB_MACHINE_GUN].minimum_proficiency = SKILL_EXPERT_SUB_MACHINE_GUN();/** 150 */
			e_name_to_proficiency[proficiency_t::ARMOR_PENETRATION_SHOT].minimum_proficiency = SKILL_ARMOR_PENETRATION_SHOT();/** 150 */
			e_name_to_proficiency[proficiency_t::NONSTOP_PENETRATION_SHOT].minimum_proficiency = SKILL_NONSTOP_PENETRATION_SHOT();/** 150 */
			e_name_to_proficiency[proficiency_t::INCREASED_INJURE_CHANCE].minimum_proficiency = SKILL_INCREASED_INJURE_CHANCE();/** 150 */
			e_name_to_proficiency[proficiency_t::ASSAULT_RIFLE_SHRAPNEL].minimum_proficiency = SKILL_ASSAULT_RIFLE_SHRAPNEL();/** 150 */
			e_name_to_proficiency[proficiency_t::ENTRY_DENIAL].minimum_proficiency = SKILL_ENTRY_DENIAL();/** 2050 */
			e_name_to_proficiency[proficiency_t::MISDIRECTION].minimum_proficiency = SKILL_MISDIRECTION();/** 2050 */
			e_name_to_proficiency[proficiency_t::SNIPER_SUPPORT].minimum_proficiency = SKILL_SNIPER_SUPPORT();/** 2050 */
			e_name_to_proficiency[proficiency_t::REDUCED_DETECTION_CHANCE].minimum_proficiency = SKILL_REDUCED_DETECTION_CHANCE();/** 4 */
			e_name_to_proficiency[proficiency_t::INCREASED_AWARENESS].minimum_proficiency = SKILL_INCREASED_AWARENESS();/** 2050 */
			e_name_to_proficiency[proficiency_t::TRICK_MAGAZINE].minimum_proficiency = SKILL_TRICK_MAGAZINE();/** 2050 */
			e_name_to_proficiency[proficiency_t::CHAINABLE_BREACHING].minimum_proficiency = SKILL_CHAINABLE_BREACHING();/** 1 */
			e_name_to_proficiency[proficiency_t::HGH_MORE_POWERFUL].minimum_proficiency = SKILL_HGH_MORE_POWERFUL();/** 2050 */
			e_name_to_proficiency[proficiency_t::BASIC_HP_RECOVERY].minimum_proficiency = SKILL_BASIC_HP_RECOVERY();/** 2050 */
			e_name_to_proficiency[proficiency_t::PARASITIC_HP_RECOVERY].minimum_proficiency = SKILL_PARASITIC_HP_RECOVERY();/** 2050 */
			e_name_to_proficiency[proficiency_t::SUTURE].minimum_proficiency = SKILL_SUTURE();/** 2050 */
			e_name_to_proficiency[proficiency_t::ADRENALINE_BOOST].minimum_proficiency = SKILL_ADRENALINE_BOOST();/** 2050 */
	}

	/** called by game initialization sequence */
	void game_init(){
		load_from_values();
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : std::get<1>(skillset)){
				mappings[prof.name.c_str()] = prof.e_name;
				to_string_mappings[prof.e_name] = prof.name.c_str();
				e_name_to_proficiency[prof.e_name] = prof;
				skill_name_list.emplace_back(prof.name.str());
			}
		}
	}
	int get_enum_by_name(std::string_view name){
		return mappings[name.data()];
	}
	std::string to_string(int e_name){
		return to_string_mappings[e_name];
	}
	uint16_t get_minimum_proficiency(int e_name){
		return e_name_to_proficiency[e_name].minimum_proficiency;
	}
	void load_player_levels(player_ptr_t& player){
		strmap_t skills;
		if(0 == get_player_map(player->name(),DB_PREFIX,skills)){
			init_player_levels(player->name().c_str());
			get_player_map(player->name(),DB_PREFIX,skills);
		}
		for(auto & pair : skills){
			int e_name = get_enum_by_name(pair.first);
			if(e_name == -1){
				continue;
			}
			auto level_opt = mods::util::stoi_optional<int>(pair.second.c_str());
			int level = 0;
			if(level_opt.has_value() == false){
				level = 0;
			}
			player->set_skill(e_name,level);
		}
	}
	uint16_t get_player_level(player_ptr_t& player,int skill){
		return player->skill(skill);
	}
	uint16_t get_player_level(player_ptr_t& player,std::string_view skill){
		return player->skill(get_enum_by_name(skill));
	}
	bool npc_can(player_ptr_t& player,int e_name){
		return true;/** TODO */
	}
	bool player_can(player_ptr_t& player,int e_name){
		if(player->is_npc()){
			return npc_can(player,e_name);
		}
		return player->skill(e_name) >= get_minimum_proficiency(e_name);
	}
	std::string pkey(std::string player_name,std::string key){
		return player_key(player_name,DB_PREFIX,"map") + key;
	}
	int update_schema_for_player(std::string player_name) {
		int ctr = 0;
		for(auto & skill_name : skill_name_list){
			if(!mods::db::lmdb_exists(pkey(player_name,skill_name))){
				mods::db::lmdb_put(pkey(player_name,skill_name),"0");
				++ctr;
			}
		}
		return ctr;
	}
};
