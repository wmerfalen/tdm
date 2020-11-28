#include "skills.hpp"
#include "interpreter.hpp"
#include "super-users.hpp"

#ifdef __MENTOC_MODS_SKILLS_SHOW_DEBUG_OUTPUT__
#define m_debug(A) std::cerr << "[mods::skills][debug]:'" << A << "'\n";
#else
#define m_debug(A)
#endif

std::string pad_string(std::string_view str,std::size_t to, std::string with){
	std::string padded;
	padded.resize(to);

	for(unsigned i =0; i < to; i++){
		if(str.length()  > i){
			padded[i] = str[i];
			continue;
		}
		padded[i] = with[0];
	}
	return padded;
}

#define FOREACH_SKILLSET_AS(PROF_NAME) for(auto & skillset : mods::skills::proficiencies::list) for(auto & PROF_NAME : std::get<1>(skillset))
ACMD(do_allow_skill){
	ADMIN_REJECT();
	DO_HELP("allow_skill");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2){
		player->sendln("Invalid argument count");
		return;
	}
	auto found = mods::globals::player_name_map.find(vec_args[0]);
	if(found == mods::globals::player_name_map.end()){
		player->sendln("Couldn't find player by that name.");
		return;
	}

	if(ICMP(vec_args[1],"all")){
		player->send("Setting all skills...");
		mods::skills::allow_player_to_do_everything(found->second);
		player->sendln("done");
		return;
	}
	skill_t s = mods::skills::to_skill_from_user_friendly_string(vec_args[1]);
	player->send("Setting skill...");
	mods::skills::set_player_can_override(found->second,{s});
	player->sendln("done");
	ADMIN_DONE();
}
namespace mods::skills {
	constexpr static const char* DB_PREFIX = "skills";
	static std::vector<std::string> skill_name_list;
	static std::map<player_class_t,std::vector<std::string>> registered_class_skillsets;

	void refresh_skill_name_list() {
		skill_name_list.clear();
		FOREACH_SKILLSET_AS(prof){
			skill_name_list.emplace_back(prof.name);
		}
	}

	/** PLAYER BOOTSTRAP */
	void init_player_levels(player_ptr_t& player){
		init_player_levels(player->name().c_str());
	}
	void init_player_levels(std::string_view player_name){
		strmap_t m;
		FOREACH_SKILLSET_AS(prof){
			m[prof.name.str()] = "0";
		}
		put_player_map(player_name.data(),DB_PREFIX,m);
	}
	static std::map<std::string,int> mappings;
	static std::map<int,std::string> to_string_mappings;
	static std::map<int,proficiencies::proficiency_t> e_name_to_proficiency;
	static constexpr const std::pair<uint8_t,uint8_t> tier_one = {1,9};
	static constexpr const std::pair<uint8_t,uint8_t> tier_two = {10,19};
	static constexpr const std::pair<uint8_t,uint8_t> tier_three = {20,31};
	static std::vector<proficiency_t> tier_one_proficiencies;
	static std::vector<proficiency_t> tier_two_proficiencies;
	static std::vector<proficiency_t> tier_three_proficiencies;
	static std::vector<proficiency_t> up_to_tier_two;
	static std::vector<proficiency_t> up_to_tier_three;
	std::vector<proficiency_t> get_player_tier_skills(player_ptr_t& player){
		auto level = player->level();
		if(tier_one.first <= level && level <= tier_one.second){
			return tier_one_proficiencies;
		}
		if(tier_two.first <= level && level <= tier_two.second){
			return up_to_tier_two;
		}
		if(tier_three.first <= level && level <= tier_three.second){
			return up_to_tier_three;
		}
		return tier_one_proficiencies;
	}
	void refresh_minimum_proficiencies(){
		tier_one_proficiencies.clear();
		tier_two_proficiencies.clear();
		tier_three_proficiencies.clear();
		using p = proficiency_t;
		/** technology usage */
		e_name_to_proficiency[proficiency_t::CAMERA_PLACEMENT].minimum_proficiency = SKILL_CAMERA_PLACEMENT();/** 50 */
		e_name_to_proficiency[proficiency_t::DETECT_CAMERAS].minimum_proficiency = SKILL_DETECT_CAMERAS();/** 50 */
		tier_one_proficiencies.emplace_back(p::CAMERA_PLACEMENT);
		tier_one_proficiencies.emplace_back(p::DETECT_CAMERAS);

		/** destruction of property */
		e_name_to_proficiency[proficiency_t::CAMERA_DESTRUCTION].minimum_proficiency = SKILL_CAMERA_DESTRUCTION();/** 100 */
		e_name_to_proficiency[proficiency_t::ELECTRIFIED_DAMAGE].minimum_proficiency = SKILL_ELECTRIFIED_DAMAGE();/** 100 */
		tier_one_proficiencies.emplace_back(p::CAMERA_DESTRUCTION);
		tier_one_proficiencies.emplace_back(p::ELECTRIFIED_DAMAGE);


		/** defensive technology */
		e_name_to_proficiency[proficiency_t::PROTECT_FROM_EMP].minimum_proficiency = SKILL_PROTECT_FROM_EMP();/** 200 */
		tier_one_proficiencies.emplace_back(p::PROTECT_FROM_EMP);

		/** crafting */
		e_name_to_proficiency[proficiency_t::CREATE_EMP_WEAPONS].minimum_proficiency = SKILL_CREATE_EMP_WEAPONS();/** 200 */
		e_name_to_proficiency[proficiency_t::MOLD].minimum_proficiency = SKILL_MOLD();/** 350 */
		tier_two_proficiencies.emplace_back(p::CREATE_EMP_WEAPONS);
		tier_two_proficiencies.emplace_back(p::MOLD);

		/** defensive offensive */
		e_name_to_proficiency[proficiency_t::COUNTER_SHOCK].minimum_proficiency = SKILL_COUNTER_SHOCK();/** 4 */
		tier_two_proficiencies.emplace_back(p::COUNTER_SHOCK);

		/** sight enhancement */
		e_name_to_proficiency[proficiency_t::TOGGLE_THERMAL].minimum_proficiency = SKILL_TOGGLE_THERMAL();/** 2050 */
		e_name_to_proficiency[proficiency_t::TOGGLE_NIGHT_VISION].minimum_proficiency = SKILL_TOGGLE_NIGHT_VISION();/** 2050 */
		e_name_to_proficiency[proficiency_t::HEADGEAR_INTROSPECTION].minimum_proficiency = SKILL_HEADGEAR_INTROSPECTION();/** 3050 */
		tier_two_proficiencies.emplace_back(p::COUNTER_SHOCK);
		tier_two_proficiencies.emplace_back(p::TOGGLE_THERMAL);
		tier_two_proficiencies.emplace_back(p::TOGGLE_NIGHT_VISION);
		tier_two_proficiencies.emplace_back(p::HEADGEAR_INTROSPECTION);


		/** recon/intel technology */
		e_name_to_proficiency[proficiency_t::PROXIMITY_ALARM].minimum_proficiency = SKILL_PROXIMITY_ALARM();/** 3050 */
		e_name_to_proficiency[proficiency_t::DOOR_CAMERA].minimum_proficiency = SKILL_DOOR_CAMERA();/** 3050 */
		tier_two_proficiencies.emplace_back(p::PROXIMITY_ALARM);
		tier_two_proficiencies.emplace_back(p::DOOR_CAMERA);

		/** defensive technology */
		e_name_to_proficiency[proficiency_t::DEPLOYABLE_TURRET].minimum_proficiency = SKILL_DEPLOYABLE_TURRET();/** 3050 */
		e_name_to_proficiency[proficiency_t::DEPLOYABLE_FIRE_TURRET].minimum_proficiency = SKILL_DEPLOYABLE_FIRE_TURRET();/** 3050 */
		tier_two_proficiencies.emplace_back(p::DEPLOYABLE_TURRET);
		tier_two_proficiencies.emplace_back(p::DEPLOYABLE_FIRE_TURRET);

		/** available from level 1-10 */
		e_name_to_proficiency[proficiency_t::BASIC_ARMOR].minimum_proficiency = SKILL_BASIC_ARMOR();/** 50 */
		e_name_to_proficiency[proficiency_t::BASIC_PISTOL].minimum_proficiency = SKILL_BASIC_PISTOL();/** 150 */
		e_name_to_proficiency[proficiency_t::BASIC_ASSAULT_RIFLE].minimum_proficiency = SKILL_BASIC_ASSAULT_RIFLE();/** 150 */
		e_name_to_proficiency[proficiency_t::BASIC_SHOTGUN].minimum_proficiency = SKILL_BASIC_SHOTGUN();/** 150 */
		e_name_to_proficiency[proficiency_t::BASIC_SNIPER_RIFLE].minimum_proficiency = SKILL_BASIC_SNIPER_RIFLE();/** 150 */
		e_name_to_proficiency[proficiency_t::BASIC_SUB_MACHINE_GUN].minimum_proficiency = SKILL_BASIC_SUB_MACHINE_GUN();/** 150 */

		tier_one_proficiencies.emplace_back(p::BASIC_ARMOR);
		tier_one_proficiencies.emplace_back(p::BASIC_PISTOL);
		tier_one_proficiencies.emplace_back(p::BASIC_ASSAULT_RIFLE);
		tier_one_proficiencies.emplace_back(p::BASIC_SHOTGUN);
		tier_one_proficiencies.emplace_back(p::BASIC_SNIPER_RIFLE);
		tier_one_proficiencies.emplace_back(p::BASIC_SUB_MACHINE_GUN);



		/** available from level 11-21 */
		e_name_to_proficiency[proficiency_t::BASIC_HP_RECOVERY].minimum_proficiency = SKILL_BASIC_HP_RECOVERY();/** 2050 */
		e_name_to_proficiency[proficiency_t::ADVANCED_ARMOR].minimum_proficiency = SKILL_ADVANCED_ARMOR();/** 50 */
		e_name_to_proficiency[proficiency_t::NO_FACTOR_BASIC].minimum_proficiency = SKILL_NO_FACTOR_BASIC();/** 400 */
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

		tier_two_proficiencies.emplace_back(p::BASIC_HP_RECOVERY);
		tier_two_proficiencies.emplace_back(p::ADVANCED_ARMOR);
		tier_two_proficiencies.emplace_back(p::NO_FACTOR_BASIC);
		tier_two_proficiencies.emplace_back(p::INTERMEDIATE_PISTOL);
		tier_two_proficiencies.emplace_back(p::INTERMEDIATE_ASSAULT_RIFLE);
		tier_two_proficiencies.emplace_back(p::INTERMEDIATE_SHOTGUN);
		tier_two_proficiencies.emplace_back(p::INTERMEDIATE_SNIPER_RIFLE);
		tier_two_proficiencies.emplace_back(p::INTERMEDIATE_SUB_MACHINE_GUN);
		tier_two_proficiencies.emplace_back(p::ADVANCED_PISTOL);
		tier_two_proficiencies.emplace_back(p::ADVANCED_ASSAULT_RIFLE);
		tier_two_proficiencies.emplace_back(p::ADVANCED_SHOTGUN);
		tier_two_proficiencies.emplace_back(p::ADVANCED_SNIPER_RIFLE);
		tier_two_proficiencies.emplace_back(p::ADVANCED_SUB_MACHINE_GUN);






		/** available from level 21-31 */
		e_name_to_proficiency[proficiency_t::ELITE_ARMOR].minimum_proficiency = SKILL_ELITE_ARMOR();/** 50 */
		e_name_to_proficiency[proficiency_t::NO_FACTOR_ADVANCED].minimum_proficiency = SKILL_NO_FACTOR_ADVANCED();/** 850 */
		e_name_to_proficiency[proficiency_t::NO_FACTOR_ELITE].minimum_proficiency = SKILL_NO_FACTOR_ELITE();/** 900 */
		e_name_to_proficiency[proficiency_t::EXPERT_PISTOL].minimum_proficiency = SKILL_EXPERT_PISTOL();/** 150 */
		e_name_to_proficiency[proficiency_t::EXPERT_ASSAULT_RIFLE].minimum_proficiency = SKILL_EXPERT_ASSAULT_RIFLE();/** 150 */
		e_name_to_proficiency[proficiency_t::EXPERT_SHOTGUN].minimum_proficiency = SKILL_EXPERT_SHOTGUN();/** 150 */
		e_name_to_proficiency[proficiency_t::EXPERT_SNIPER_RIFLE].minimum_proficiency = SKILL_EXPERT_SNIPER_RIFLE();/** 150 */
		e_name_to_proficiency[proficiency_t::EXPERT_SUB_MACHINE_GUN].minimum_proficiency = SKILL_EXPERT_SUB_MACHINE_GUN();/** 150 */
		e_name_to_proficiency[proficiency_t::PARASITIC_HP_RECOVERY].minimum_proficiency = SKILL_PARASITIC_HP_RECOVERY();/** 2050 */

		tier_three_proficiencies.emplace_back(p::ELITE_ARMOR);
		tier_three_proficiencies.emplace_back(p::NO_FACTOR_ADVANCED);
		tier_three_proficiencies.emplace_back(p::NO_FACTOR_ELITE);
		tier_three_proficiencies.emplace_back(p::EXPERT_PISTOL);
		tier_three_proficiencies.emplace_back(p::EXPERT_ASSAULT_RIFLE);
		tier_three_proficiencies.emplace_back(p::EXPERT_SHOTGUN);
		tier_three_proficiencies.emplace_back(p::EXPERT_SNIPER_RIFLE);
		tier_three_proficiencies.emplace_back(p::EXPERT_SUB_MACHINE_GUN);
		tier_three_proficiencies.emplace_back(p::PARASITIC_HP_RECOVERY);


		/** stealing property */
		e_name_to_proficiency[proficiency_t::THIEF].minimum_proficiency = SKILL_THIEF();/** 200 */
		tier_three_proficiencies.emplace_back(p::THIEF);



		/** ARMOR triad specialties */
		e_name_to_proficiency[proficiency_t::EXPLOSIVE_RESISTANCE].minimum_proficiency = SKILL_EXPLOSIVE_RESISTANCE();/** 2050 */
		e_name_to_proficiency[proficiency_t::INJURE_RESISTANCE].minimum_proficiency = SKILL_INJURE_RESISTANCE();/** 3050 */
		e_name_to_proficiency[proficiency_t::MUNITIONS_REFLECTOR].minimum_proficiency = SKILL_MUNITIONS_REFLECTOR();/** 4 */
		e_name_to_proficiency[proficiency_t::SENTINEL_DISCIPLINE].minimum_proficiency = SKILL_SENTINEL_DISCIPLINE();/** 3 */

		tier_two_proficiencies.emplace_back(p::EXPLOSIVE_RESISTANCE);
		tier_two_proficiencies.emplace_back(p::INJURE_RESISTANCE);
		tier_two_proficiencies.emplace_back(p::MUNITIONS_REFLECTOR);
		tier_two_proficiencies.emplace_back(p::SENTINEL_DISCIPLINE);



		/** weapon speciality - shields */
		e_name_to_proficiency[proficiency_t::DEPLOYABLE_SHIELD].minimum_proficiency = SKILL_DEPLOYABLE_SHIELD();/** 8050 */
		tier_two_proficiencies.emplace_back(p::DEPLOYABLE_SHIELD);



		/** weapons handling any tier */
		e_name_to_proficiency[proficiency_t::TRICK_MAGAZINE].minimum_proficiency = SKILL_TRICK_MAGAZINE();/** 2050 */
		e_name_to_proficiency[proficiency_t::ASSAULT_RIFLE_SHRAPNEL].minimum_proficiency = SKILL_ASSAULT_RIFLE_SHRAPNEL();/** 150 */
		e_name_to_proficiency[proficiency_t::SPRAY_CHANCE].minimum_proficiency = SKILL_SPRAY_CHANCE();/** 150 */
		e_name_to_proficiency[proficiency_t::HEADSHOT_CHANCE].minimum_proficiency = SKILL_HEADSHOT_CHANCE();/** 250 */
		e_name_to_proficiency[proficiency_t::LIMB_CHANCE].minimum_proficiency = SKILL_LIMB_CHANCE();/** 350 */
		e_name_to_proficiency[proficiency_t::ARMOR_PENETRATION_SHOT].minimum_proficiency = SKILL_ARMOR_PENETRATION_SHOT();/** 150 */
		e_name_to_proficiency[proficiency_t::NONSTOP_PENETRATION_SHOT].minimum_proficiency = SKILL_NONSTOP_PENETRATION_SHOT();/** 150 */
		e_name_to_proficiency[proficiency_t::INCREASED_INJURE_CHANCE].minimum_proficiency = SKILL_INCREASED_INJURE_CHANCE();/** 150 */

		tier_one_proficiencies.emplace_back(p::TRICK_MAGAZINE);
		tier_one_proficiencies.emplace_back(p::ASSAULT_RIFLE_SHRAPNEL);
		tier_one_proficiencies.emplace_back(p::SPRAY_CHANCE);
		tier_one_proficiencies.emplace_back(p::HEADSHOT_CHANCE);
		tier_one_proficiencies.emplace_back(p::LIMB_CHANCE);
		tier_one_proficiencies.emplace_back(p::ARMOR_PENETRATION_SHOT);
		tier_one_proficiencies.emplace_back(p::NONSTOP_PENETRATION_SHOT);
		tier_one_proficiencies.emplace_back(p::INCREASED_INJURE_CHANCE);


		/** weapons handling -> third tier (levels 21-31) */
		e_name_to_proficiency[proficiency_t::FIRE_NADE_DEALER].minimum_proficiency = SKILL_FIRE_NADE_DEALER();/** 50 */
		e_name_to_proficiency[proficiency_t::BETTER_WEAPON_ACCURACY].minimum_proficiency = SKILL_BETTER_WEAPON_ACCURACY();/** 50 */
		e_name_to_proficiency[proficiency_t::FASTER_TRIGGER_FINGER].minimum_proficiency = SKILL_FASTER_TRIGGER_FINGER();/** 50 */
		e_name_to_proficiency[proficiency_t::FASTER_RELOADING].minimum_proficiency = SKILL_FASTER_RELOADING();/** 50 */
		e_name_to_proficiency[proficiency_t::PRO_WEAPON_HANDLER].minimum_proficiency = SKILL_PRO_WEAPON_HANDLER();/** 50 */


		tier_three_proficiencies.emplace_back(p::FIRE_NADE_DEALER);
		tier_three_proficiencies.emplace_back(p::BETTER_WEAPON_ACCURACY);
		tier_three_proficiencies.emplace_back(p::FASTER_TRIGGER_FINGER);
		tier_three_proficiencies.emplace_back(p::FASTER_RELOADING);
		tier_three_proficiencies.emplace_back(p::PRO_WEAPON_HANDLER);




		/** weapons handling and sniping */
		e_name_to_proficiency[proficiency_t::BETTER_SNIPING_ACCURACY].minimum_proficiency = SKILL_BETTER_SNIPING_ACCURACY();/** 450 */
		e_name_to_proficiency[proficiency_t::TARGET_LIMB].minimum_proficiency = SKILL_TARGET_LIMB();/** 850 */
		e_name_to_proficiency[proficiency_t::ACCURACY].minimum_proficiency = SKILL_ACCURACY();/** 150 */

		tier_one_proficiencies.emplace_back(p::BETTER_SNIPING_ACCURACY);
		tier_one_proficiencies.emplace_back(p::TARGET_LIMB);
		tier_one_proficiencies.emplace_back(p::ACCURACY);



		/** sniping base attribute */
		e_name_to_proficiency[proficiency_t::VIP].minimum_proficiency = SKILL_VIP();/** 250 */
		e_name_to_proficiency[proficiency_t::TRACKER].minimum_proficiency = SKILL_TRACKER();/** 350 */
		e_name_to_proficiency[proficiency_t::QUIETER_SNIPER_SHOTS].minimum_proficiency = SKILL_QUIETER_SNIPER_SHOTS();/** 350 */

		tier_three_proficiencies.emplace_back(p::VIP);
		tier_two_proficiencies.emplace_back(p::TRACKER);
		tier_two_proficiencies.emplace_back(p::QUIETER_SNIPER_SHOTS);


		/** demolitions specialty */
			/** speed triad helps this one */
		e_name_to_proficiency[proficiency_t::FASTER_BREACHES].minimum_proficiency = SKILL_FASTER_BREACHES();/** 50 */
		e_name_to_proficiency[proficiency_t::FURTHER_C4S].minimum_proficiency = SKILL_FURTHER_C4S();/** 4 */
		e_name_to_proficiency[proficiency_t::STRONGER_FRAG_ARM].minimum_proficiency = SKILL_STRONGER_FRAG_ARM();/** 50 */
		e_name_to_proficiency[proficiency_t::BIGGER_CLAYMORES].minimum_proficiency = SKILL_BIGGER_CLAYMORES();/** 50 */
		e_name_to_proficiency[proficiency_t::FASTER_THERMITES].minimum_proficiency = SKILL_FASTER_THERMITES();/** 50 */
		e_name_to_proficiency[proficiency_t::BIGGER_FIRE_NADES].minimum_proficiency = SKILL_BIGGER_FIRE_NADES();/** 50 */
		e_name_to_proficiency[proficiency_t::CRAFTY_C4].minimum_proficiency = SKILL_CRAFTY_C4();/** 4 */
		e_name_to_proficiency[proficiency_t::BETTER_EXPLOSIONS].minimum_proficiency = SKILL_BETTER_EXPLOSIONS();/** 33 */
		e_name_to_proficiency[proficiency_t::CHAINABLE_BREACHING].minimum_proficiency = SKILL_CHAINABLE_BREACHING();/** 1 */

		tier_three_proficiencies.emplace_back(p::FASTER_BREACHES);
		tier_three_proficiencies.emplace_back(p::FURTHER_C4S);
		tier_three_proficiencies.emplace_back(p::STRONGER_FRAG_ARM);
		tier_three_proficiencies.emplace_back(p::BIGGER_CLAYMORES);
		tier_three_proficiencies.emplace_back(p::FASTER_THERMITES);
		tier_three_proficiencies.emplace_back(p::BIGGER_FIRE_NADES);
		tier_three_proficiencies.emplace_back(p::CRAFTY_C4);
		tier_three_proficiencies.emplace_back(p::BETTER_EXPLOSIONS);
		tier_three_proficiencies.emplace_back(p::CHAINABLE_BREACHING);

		/** weapon handling and demolitions */
		e_name_to_proficiency[proficiency_t::RETROFIT_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_GRENADE_LAUNCHER();/** 50 */
		e_name_to_proficiency[proficiency_t::RETROFIT_FIRE_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_FIRE_GRENADE_LAUNCHER();/** 50 */
		e_name_to_proficiency[proficiency_t::RETROFIT_SMOKE_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_SMOKE_GRENADE_LAUNCHER();/** 50 */
		e_name_to_proficiency[proficiency_t::RETROFIT_SENSOR_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_SENSOR_GRENADE_LAUNCHER();/** 50 */
		e_name_to_proficiency[proficiency_t::RETROFIT_STUN_GRENADE_LAUNCHER].minimum_proficiency = SKILL_RETROFIT_STUN_GRENADE_LAUNCHER();/** 50 */
		e_name_to_proficiency[proficiency_t::NOXIOUS_NADE_CRAFTER].minimum_proficiency = SKILL_NOXIOUS_NADE_CRAFTER();/** 50 */

		tier_three_proficiencies.emplace_back(p::RETROFIT_GRENADE_LAUNCHER);
		tier_three_proficiencies.emplace_back(p::RETROFIT_FIRE_GRENADE_LAUNCHER);
		tier_three_proficiencies.emplace_back(p::RETROFIT_SMOKE_GRENADE_LAUNCHER);
		tier_three_proficiencies.emplace_back(p::RETROFIT_SENSOR_GRENADE_LAUNCHER);
		tier_three_proficiencies.emplace_back(p::RETROFIT_STUN_GRENADE_LAUNCHER);
		tier_three_proficiencies.emplace_back(p::NOXIOUS_NADE_CRAFTER);




		/** medical specialty - any tier */
		e_name_to_proficiency[proficiency_t::STEROID_DEALER].minimum_proficiency = SKILL_STEROID_DEALER();/** 50 */
		e_name_to_proficiency[proficiency_t::HGH_DEALER].minimum_proficiency = SKILL_HGH_DEALER();/** 50 */
		e_name_to_proficiency[proficiency_t::DUTCH_OVEN].minimum_proficiency = SKILL_DUTCH_OVEN();/** 50 */
		e_name_to_proficiency[proficiency_t::HGH_MORE_POWERFUL].minimum_proficiency = SKILL_HGH_MORE_POWERFUL();/** 2050 */
		e_name_to_proficiency[proficiency_t::SUTURE].minimum_proficiency = SKILL_SUTURE();/** 2050 */
		e_name_to_proficiency[proficiency_t::ADRENALINE_BOOST].minimum_proficiency = SKILL_ADRENALINE_BOOST();/** 2050 */

		tier_one_proficiencies.emplace_back(p::STEROID_DEALER);
		tier_one_proficiencies.emplace_back(p::HGH_DEALER);
		tier_one_proficiencies.emplace_back(p::DUTCH_OVEN);
		tier_one_proficiencies.emplace_back(p::HGH_MORE_POWERFUL);
		tier_one_proficiencies.emplace_back(p::SUTURE);
		tier_one_proficiencies.emplace_back(p::ADRENALINE_BOOST);


		/** strategy base attribute - any tier */
		e_name_to_proficiency[proficiency_t::ENTRY_DENIAL].minimum_proficiency = SKILL_ENTRY_DENIAL();/** 2050 */
		e_name_to_proficiency[proficiency_t::MISDIRECTION].minimum_proficiency = SKILL_MISDIRECTION();/** 2050 */
		e_name_to_proficiency[proficiency_t::SNIPER_SUPPORT].minimum_proficiency = SKILL_SNIPER_SUPPORT();/** 2050 */

		tier_one_proficiencies.emplace_back(p::ENTRY_DENIAL);
		tier_one_proficiencies.emplace_back(p::MISDIRECTION);
		tier_one_proficiencies.emplace_back(p::SNIPER_SUPPORT);


		/** intel base attribute - any tier */
		e_name_to_proficiency[proficiency_t::INCREASED_AWARENESS].minimum_proficiency = SKILL_INCREASED_AWARENESS();/** 2050 */
		e_name_to_proficiency[proficiency_t::REDUCED_DETECTION_CHANCE].minimum_proficiency = SKILL_REDUCED_DETECTION_CHANCE();/** 4 */

		tier_one_proficiencies.emplace_back(p::INCREASED_AWARENESS);
		tier_one_proficiencies.emplace_back(p::REDUCED_DETECTION_CHANCE);



		/** available at all levels for GHOST class */
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_DRONE_SCAN].minimum_proficiency = SKILL_GHOST_DRONE_SCAN();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_STEALTH].minimum_proficiency = SKILL_GHOST_STEALTH();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_SUMMON_EXTRACTION].minimum_proficiency = SKILL_GHOST_SUMMON_EXTRACTION();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_XRAY_SHOT].minimum_proficiency = SKILL_GHOST_XRAY_SHOT();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_FEIGN_DEATH].minimum_proficiency = SKILL_GHOST_FEIGN_DEATH();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_PLANT_CLAYMORE].minimum_proficiency = SKILL_GHOST_PLANT_CLAYMORE();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_PENETRATING_SHOT].minimum_proficiency = SKILL_GHOST_PENETRATING_SHOT();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_INTIMIDATION].minimum_proficiency = SKILL_GHOST_INTIMIDATION();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_CRYOGENIC_GRENADE].minimum_proficiency = SKILL_GHOST_CRYOGENIC_GRENADE();
		e_name_to_proficiency[proficiency_t::GHOST_CLASS_FLASH_UNDERBARREL].minimum_proficiency = SKILL_GHOST_FLASH_UNDERBARREL();

		auto v = tier_one_proficiencies;
		v.insert(v.end(),tier_two_proficiencies.begin(),tier_two_proficiencies.end());
		up_to_tier_two = v;
		
		v = tier_one_proficiencies;
		v.insert(v.end(),tier_two_proficiencies.begin(),tier_two_proficiencies.end());
		v.insert(v.end(),tier_three_proficiencies.begin(),tier_three_proficiencies.end());
		up_to_tier_three = v;
	}

	void refresh_implemented_skills(){
		FOREACH_SKILLSET_AS(prof){
			if(prof.implemented){
				implemented_skills.emplace_back(prof.e_name);
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

	bool player_has_override_for(player_ptr_t& player,int e_name){
		auto it = player_can_overrides.find(player->name().c_str());
		if(it != player_can_overrides.end()){
			m_debug("[player can by override]: " << green_str(player->name().c_str()) << "->" << e_name);
			return std::find(it->second.begin(),it->second.end(),e_name) != it->second.end();
		}
		return false;
	}

	bool player_can(player_ptr_t& player,int e_name){
		if(player->is_npc()){
			return npc_can(player,e_name);
		}
		if(player_has_override_for(player,e_name)){
			return true;
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


	std::vector<skill_t> convert_to_skill_list(std::vector<std::string_view> in_skill_list){
		std::vector<skill_t> parsed;
		for(auto & name : in_skill_list){
			skill_t s = to_skill_from_user_friendly_string(name);
			if(s != skill_t::NONE){
				parsed.emplace_back(s);
			}
		}
		return parsed;
	}


	void allow_player_to_do_everything(player_ptr_t& player){
		std::vector<skill_t> skills;
		for(auto & skill : passive_skills){
			skills.emplace_back(skill);
		}
		for(auto & skill : castable_skills){
			skills.emplace_back(skill);
		}
		set_player_can_override(player,skills);
	}


	void set_player_can_override(player_ptr_t& player,std::vector<skill_t> in_skill_list){
		player_can_overrides[player->name().c_str()] = in_skill_list;
	}


	void clear_player_can_override(player_ptr_t& player){
		player_can_overrides.erase(player->name().c_str());
	}

	skill_t to_skill_from_user_friendly_string(std::string_view s){
		return static_cast<skill_t>(mappings[s.data()]);
	}


	std::string_view to_user_friendly_string_from_skill(skill_t s){
		return e_name_to_proficiency[s].name.c_str();
	}


	/** called by game initialization sequence */
	void init(){
		refresh_minimum_proficiencies();
		FOREACH_SKILLSET_AS(prof){
			mappings[prof.name.c_str()] = prof.e_name;
			to_string_mappings[prof.e_name] = prof.name.c_str();
			e_name_to_proficiency[prof.e_name] = prof;
			skill_name_list.emplace_back(prof.name.str());
		}
		refresh_implemented_skills();
		mods::interpreter::add_command("skills", POS_RESTING, do_skills, 0,0);
		mods::interpreter::add_command("skill", POS_RESTING, do_skills, 0,0);
		mods::interpreter::add_command("train", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("practice", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("prac", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("allow_skill", POS_RESTING, do_allow_skill, LVL_BUILDER,0);
	}

	void register_class_skillset(player_class_t class_type,std::vector<std::string>& skillset){
		mods::skills::registered_class_skillsets[class_type] = skillset;
	}
};



bool skill_belongs_to_player_class(player_ptr_t& player,std::string_view skill_name){
	std::string pc = CAT(":",player->get_class_string().c_str(),":");
	return ICMP(pc.c_str(),skill_name.data());
}
bool is_player_class_skillset(std::string_view name){
	return name[0] == ':';
}
std::pair<uint8_t,uint8_t> get_tier_pair(mods::skills::proficiencies::proficiency_name_t prof){
	using namespace mods::skills;
	if(std::find(tier_one_proficiencies.begin(),tier_one_proficiencies.end(),prof) != tier_one_proficiencies.end()){
		return tier_one;
	}
	if(std::find(tier_two_proficiencies.begin(),tier_two_proficiencies.end(),prof) != tier_two_proficiencies.end()){
		return tier_two;
	}
	if(std::find(tier_three_proficiencies.begin(),tier_three_proficiencies.end(),prof) != tier_three_proficiencies.end()){
		return tier_three;
	}
	return tier_three;
}

ACMD(do_train){
	DO_HELP("train");
	auto vec_args = PARSE_ARGS();
	auto skills = mods::skills::get_player_tier_skills(player);
	if(vec_args.size() == 0){
		for(auto & skillset : mods::skills::proficiencies::list) {
			if(is_player_class_skillset(std::get<0>(skillset)) && !skill_belongs_to_player_class(player,std::get<0>(skillset))){
				continue;
			}
			player->sendln(CAT("-- {grn}",std::get<0>(skillset),"{/grn}"));
			std::string not_learned = "|not-learned";
			std::string learned  = pad_string("|learned",not_learned.length()," ");
			for(auto & prof : std::get<1>(skillset)){
				bool is_my_skill = is_player_class_skillset(std::get<0>(skillset)) && skill_belongs_to_player_class(player,std::get<0>(skillset));
				if(std::find(skills.begin(),skills.end(),prof.e_name) == skills.end() && !is_my_skill){
					continue;
				}
				bool can_do = mods::skills::player_can(player,prof.e_name);
				std::array<char,1024> buffer = {0};
				std::size_t max_len = 35,skill_max_len = 80,skill_line_max_len = skill_max_len;
				skill_line_max_len += 20;
				std::string skill = pad_string(prof.name.c_str(),max_len - 2," ");
				auto tier_pair = get_tier_pair(prof.e_name);
				if(is_my_skill){
					snprintf(&buffer[0],1024,"%s%s | (Cost: %d) (Levels: any)",skill.c_str(),(can_do  || 1 ? learned : not_learned).c_str(), prof.minimum_proficiency);
				}else{
					snprintf(&buffer[0],1024,"%s%s | (Cost: %d) (Levels: %d->%d)",skill.c_str(),(can_do  || 1 ? learned : not_learned).c_str(), prof.minimum_proficiency,tier_pair.first,tier_pair.second);
				}
				player->sendln(&buffer[0]);
			}
		}
	}
	if(vec_args.size() >= 1){
		for(auto & skillset : mods::skills::proficiencies::list) {
			if(is_player_class_skillset(std::get<0>(skillset)) && !skill_belongs_to_player_class(player,std::get<0>(skillset))){
				continue;
			}
			for(auto & prof : std::get<1>(skillset)){
				bool is_my_skill = is_player_class_skillset(std::get<0>(skillset)) && skill_belongs_to_player_class(player,std::get<0>(skillset));
				if(std::find(skills.begin(),skills.end(),prof.e_name) == skills.end() && !is_my_skill){
					continue;
				}
				if(ICMP(vec_args[0],prof.name.c_str())){
					if(!mods::skills::player_can(player,prof.e_name)){
						/** TODO subtract available mp */
						player->set_skill(prof.e_name,prof.minimum_proficiency);
						player->sendln("You practice for awhile...");
					}
				}
			}
		}
		player->errorln(CAT("Unable to find a skill that matches '",vec_args[0],"'"));
	}
}
ACMD(do_skills){
	DO_HELP("skills");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() >= 2 && ICMP(vec_args[0],"show")){
		for(unsigned i = 1; i < vec_args.size();++i){
			for(auto & skillset : mods::skills::proficiencies::list) {
				if(is_player_class_skillset(std::get<0>(skillset)) && !skill_belongs_to_player_class(player,std::get<0>(skillset))){
					continue;
				}
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
	}

	player->pager_start();
	std::size_t max_len = 35,skill_max_len = 80,skill_line_max_len = skill_max_len;
	skill_line_max_len += 20;
	for(auto & skillset : mods::skills::proficiencies::list) {
		if(is_player_class_skillset(std::get<0>(skillset)) && !skill_belongs_to_player_class(player,std::get<0>(skillset))){
			continue;
		}
		player->sendln(CAT("-- {grn}",std::get<0>(skillset),"{/grn}"));
		std::string not_learned = "|not-learned";
		std::string learned  = pad_string("|learned",not_learned.length()," ");
		for(auto & prof : std::get<1>(skillset)){
			bool can_do = mods::skills::player_can(player,prof.e_name);
			std::array<char,1024> buffer = {0};
			std::string skill = pad_string(prof.name.c_str(),max_len - 2," ");
			auto tier_pair = get_tier_pair(prof.e_name);
			bool is_my_skill = is_player_class_skillset(std::get<0>(skillset)) && skill_belongs_to_player_class(player,std::get<0>(skillset));
			if(is_my_skill){
				snprintf(&buffer[0],1024,"%s%s | (Cost: %d) (Levels: any)",skill.c_str(),(can_do  || 1 ? learned : not_learned).c_str(), prof.minimum_proficiency);
			}else{
				snprintf(&buffer[0],1024,"%s%s | (Cost: %d) (Levels: %d->%d)",skill.c_str(),(can_do  || 1 ? learned : not_learned).c_str(), prof.minimum_proficiency,tier_pair.first,tier_pair.second);
			}
			player->sendln(&buffer[0]);
		}
	}
	player->sendln("To see a detailed description of a skill, type: {grn}skills help <skill>{/grn}");
	player->sendln("Example: {yel}skills show spray-chance{/yel}");
	player->sendln("this documentation was written on 2020-07-06.");
	player->pager_end();
	player->page(0);
}
#undef m_debug
