#ifndef __MENTOC_MODS_SKILLS_HEADER__
#define __MENTOC_MODS_SKILLS_HEADER__

#include "../globals.hpp"
#include "string.hpp"
#include <unordered_map>
namespace mods::skills {
	enum fields_of_study {
		NONE = 0,
		ELECTRONICS = 1,
		ARMOR,
		MARKSMANSHIP,
		SNIPING,
		DEMOLITIONS,
		CHEMISTRY,
		WEAPON_HANDLING,
		STRATEGY
	};

	namespace proficiencies {
		enum proficiency_name_t {
			CAMERA_PLACEMENT = 0,
			DETECT_CAMERAS,
			CAMERA_DESTRUCTION,
			ELECTRIFIED_DAMAGE,
			PROTECT_FROM_EMP,
			CREATE_EMP_WEAPONS,
			BASIC_ARMOR,
			ADVANCED_ARMOR,
			ELITE_ARMOR,
			THIEF,
			MOLD,
			NO_FACTOR_BASIC,
			NO_FACTOR_ADVANCED,
			NO_FACTOR_ELITE,
			EXPLOSIVE_RESISTANCE,
			INJURE_RESISTANCE,
			SPRAY_CHANCE,
			HEADSHOT_CHANCE,
			LIMB_CHANCE,
			BETTER_SNIPING_ACCURACY,
			ACCURACY,
			VIP,
			TRACKER,
			FASTER_BREACHES,
			FURTHER_C4S,
			STRONGER_FRAG_ARM,
			BIGGER_CLAYMORES,
			FASTER_THERMITES,
			BIGGER_FIRE_NADES,
			CRAFTY_C4,
			BETTER_EXPLOSIONS,
			STEROID_DEALER,
			HGH_DEALER,
			NOXIOUS_NADE_CRAFTER,
			DUTCH_OVEN,
			FIRE_NADE_DEALER,
			PRO_WEAPON_HANDLER,
			BETTER_WEAPON_ACCURACY,
			FASTER_TRIGGER_FINGER,
			FASTER_RELOADING,
			BASIC_PISTOL,
			BASIC_ASSAULT_RIFLE,
			BASIC_SHOTGUN,
			BASIC_SNIPER_RIFLE,
			BASIC_SUB_MACHINE_GUN,
			INTERMEDIATE_PISTOL,
			INTERMEDIATE_ASSAULT_RIFLE,
			INTERMEDIATE_SHOTGUN,
			INTERMEDIATE_SNIPER_RIFLE,
			INTERMEDIATE_SUB_MACHINE_GUN,
			ADVANCED_PISTOL,
			ADVANCED_ASSAULT_RIFLE,
			ADVANCED_SHOTGUN,
			ADVANCED_SNIPER_RIFLE,
			ADVANCED_SUB_MACHINE_GUN,
			EXPERT_PISTOL,
			EXPERT_ASSAULT_RIFLE,
			EXPERT_SHOTGUN,
			EXPERT_SNIPER_RIFLE,
			EXPERT_SUB_MACHINE_GUN,
			COUNTER_SHOCK,
			TOGGLE_THERMAL,
			TOGGLE_NIGHT_VISION,
			MUNITIONS_REFLECTOR,
			HEADGEAR_INTROSPECTION,
			ENTRY_DENIAL,
			MISDIRECTION,
			SNIPER_SUPPORT,
			REDUCED_DETECTION_CHANCE,
			INCREASED_AWARENESS,
			TARGET_LIMB,
			HGH_MORE_POWERFUL,
			PARASITIC_HP_RECOVERY,
			SUTURE,
			BASIC_HP_RECOVERY,
			RETROFIT_GRENADE_LAUNCHER,
			RETROFIT_FIRE_GRENADE_LAUNCHER,
			RETROFIT_SMOKE_GRENADE_LAUNCHER,
			RETROFIT_SENSOR_GRENADE_LAUNCHER,
			RETROFIT_STUN_GRENADE_LAUNCHER,
			DEPLOYABLE_TURRET,
			DEPLOYABLE_FIRE_TURRET,
			PROXIMITY_ALARM,
			QUIETER_SNIPER_SHOTS,
			ARMOR_PENETRATION_SHOT,
			NONSTOP_PENETRATION_SHOT,
			INCREASED_INJURE_CHANCE,
			ADRENALINE_BOOST,
			ASSAULT_RIFLE_SHRAPNEL,
			SENTINEL_DISCIPLINE,
			DOOR_CAMERA,
			TRICK_MAGAZINE,
			CHAINABLE_BREACHING,
			DEPLOYABLE_SHIELD,
		};
		struct proficiency_t {
			proficiency_name_t e_name;
			mods::string name;
			mods::string description;
			uint16_t minimum_proficiency;
		};
		/** ATTENTION !!!!
		 * ALL minimum_proficiencies are GUESSTIMATIONS. They will change!
		 */
		static std::vector<proficiency_t> electronics = {
			/** castable */
			{CAMERA_PLACEMENT,"camera-placement", "Ability to plant and view remote cameras.",50},
			/** passive */
			{DETECT_CAMERAS,"detect-cameras", "Can detect cameras, even those that are hidden.",50},
			/** castable */
			{CAMERA_DESTRUCTION,"camera-destruction", "Able to destroy cameras that aren't your own.", 100},
			/** passive */
			{ELECTRIFIED_DAMAGE,"electrified-damage", "Can use gadgets and weapons that can deal electric damage.",100},
			/** castable */
			{PROTECT_FROM_EMP,"protect-from-emp", "Can protect various electronic devices from E.M.P. damage.",200},
			/** castable */
			{CREATE_EMP_WEAPONS,"create-emp-weapons","Can create E.M.P. weapons.",200},
			/** passive */
			{COUNTER_SHOCK,"counter-shock","Enemies who melee you have a 4\% chance of getting shocked by you.",1050},
			/** passive */
			{TOGGLE_THERMAL,"toggle-thermal","Cameras and headgear you own now have Thermal vision.",2050},
			/** passive */
			{TOGGLE_NIGHT_VISION,"toggle-night-vision","Cameras and headgear you own now have night vision.",2050},
			/** passive */
			{HEADGEAR_INTROSPECTION,"headgear-introspection","Tactical headgear gives you the ability to see stats and inventory of any NPC.",3050},
			/** castable */
			{PROXIMITY_ALARM,"proximity-alarm","Plant a proximity alarm in any room and be notified.",3050},
			/** castable */
			{DEPLOYABLE_TURRET,"deployable-turret","Deploy a turret and face it in a specific direction.",3050},
			/** castable */
			{DEPLOYABLE_FIRE_TURRET,"deployable-fire-turret","Deploy a flame thrower turret and face it in a specific direction.",3050},
			/** castable */
			{DOOR_CAMERA,"door-camera","Slip an undetectable camera under a closed door to mark mobs.",3050},
		};
		static std::vector<proficiency_t> armor = {
			/** passive */
			{BASIC_ARMOR,"basic-armor", "Can use BASIC class armor.",50},
			/** passive */
			{ADVANCED_ARMOR,"advanced-armor", "Can use ADVANCED class armor.",50},
			/** passive */
			{ELITE_ARMOR,"elite-armor", "Can use ELITE class armor.",50},
			/** castable */
			{THIEF,"thief", "Can steal armor off of corpses.", 200},
			/** castable */
			{MOLD,"mold", "Can mold together various types of armor to create new armor.", 350},
			/** THESE WORK PERFECTLY FOR mods/armor/basic-protection classes!!! */
			/** passive */
			{NO_FACTOR_BASIC,"no-factor-basic", "Heavy BASIC armor doesn't affect your movement.", 400},
			/** passive */
			{NO_FACTOR_ADVANCED,"no-factor-advanced", "Heavy ADVANCED armor doesn't affect your movement.", 850},
			/** passive */
			{NO_FACTOR_ELITE,"no-factor-elite", "Heavy ELITE armor doesn't affect your movement.", 900},

			/** passive */
			{EXPLOSIVE_RESISTANCE,"explosive-resistance", "Explosions have less impact on you.", 2050},
			/** passive */
			{INJURE_RESISTANCE,"injure-resistance", "You are less likely to get injured.", 3050}, /** implemented */
			/** passive */
			{MUNITIONS_REFLECTOR,"munitions-reflector", "You have a 4\% chance of reflecting damage onto your attacker.", 8050}, /** implemented */
			/** passive */
			{SENTINEL_DISCIPLINE,"sentinel-discipline", "Standing still gives 3\% HP regen every 1 second and 4\% critical chance. Must be actively attacking a mob.", 8050},
			/** castable */
			{DEPLOYABLE_SHIELD,"deployable-shield", "Deploy a bulletproof shield for a limited amount of time.", 8050},
		};
		static std::vector<proficiency_t> marksmanship = {
			/** passive */
			{SPRAY_CHANCE,"spray-chance", "Chance to hit during spray is increased", 150},/** implemented */
			/** passive */
			{HEADSHOT_CHANCE,"headshot-chance", "Chance to headshot is increased.", 250},/** implemented */
			/** passive */
			{LIMB_CHANCE,"limb-chance", "Chance to target limb is increased.", 350},
			/** passive */
			{BETTER_SNIPING_ACCURACY,"better-sniping-accuracy", "Sniping accuracy increased.",450},
			/** castable */
			{TARGET_LIMB,"target-limb", "Adds the ability to target a limb.",850}
		};
		static std::vector<proficiency_t> sniping = {
			/** passive */
			{ACCURACY,"accuracy", "Accuracy increased with sniper rifles.", 150},
			/** passive */
			{VIP,"vip", "Exclusive access to advanced sniper rifles", 250},
			/** castable */
			{TRACKER,"tracker", "Tracking bullets.",350},
			/** passive */
			{QUIETER_SNIPER_SHOTS,"quieter-sniper-shots", "Your shots are less loud and less likely to give away your position.",350},
		};
		static std::vector<proficiency_t> demolitions = {
			/** passive */
			{FASTER_BREACHES,"faster-breaches","Breach charge placement time decreased",50},
			/** passive */
			{FURTHER_C4S,"further-c4s","C4 remote detonation distance increased",50},
			/** passive */
			{STRONGER_FRAG_ARM,"stronger-frag-arm","Grenade throw distance increased",50},
			/** passive */
			{BIGGER_CLAYMORES,"bigger-claymores","Claymore damage increased",50},
			/** passive */
			{FASTER_THERMITES,"faster-thermites","Thermite breach charge placement time decreased",50},
			/** passive */
			{BIGGER_FIRE_NADES,"bigger-fire-nades","Incendiary grenade damage increased",50},
			/** passive */
			{CRAFTY_C4,"crafty-c4","Can craft C4",50},
			/** passive */
			{BETTER_EXPLOSIONS,"better-explosions","Chance to do 33% more damage with any explosive",50},
			/** castable */
			{RETROFIT_GRENADE_LAUNCHER,"retrofit-grenade-launcher", "Attach a grenade launcher onto any Assault Rifle.",50},
			/** castable */
			{RETROFIT_FIRE_GRENADE_LAUNCHER,"retrofit-fire-grenade-launcher", "Attach a grenade launcher that shoots incendiary grenades onto any Assault Rifle.",50},
			/** castable */
			{RETROFIT_SMOKE_GRENADE_LAUNCHER,"retrofit-smoke-grenade-launcher", "Attach a grenade launcher that shoots smoke grenades onto any Assault Rifle.",50},
			/** castable */
			{RETROFIT_SENSOR_GRENADE_LAUNCHER,"retrofit-smoke-grenade-launcher", "Attach a grenade launcher that shoots smoke grenades onto any Assault Rifle.",50},
			/** castable */
			{RETROFIT_STUN_GRENADE_LAUNCHER,"retrofit-stun-grenade-launcher", "Attach a stun grenade launcher onto any Assault Rifle.",50},
		};
		static std::vector<proficiency_t> chemistry = {
			/** castable */
			{STEROID_DEALER,"steroid-dealer","Can create steroids",50},
			/** castable */
			{HGH_DEALER,"hgh-dealer","Can create human growth hormone",50},
			/** castable */
			{NOXIOUS_NADE_CRAFTER,"noxious-nade-crafter","Can create noxious gas grenades",50},
			/** castable */
			{DUTCH_OVEN,"dutch-oven","Seal off room to make air-tight",50},
			/** castable */
			{FIRE_NADE_DEALER,"fire-nade-dealer","Can create incendiary ammo",50}
		};
		static std::vector<proficiency_t> weapon_handling = {
			/** passive */
			{PRO_WEAPON_HANDLER,"pro-weapon-handler","Enhanced familiarity with weapons",50},
			/** passive */
			{BETTER_WEAPON_ACCURACY,"better-weapon-accuracy","Increased accuracy with any weapon",50},
			/** passive */
			{FASTER_TRIGGER_FINGER,"faster-trigger-finger","Decreased cooldown between shots",50},
			/** passive */
			{FASTER_RELOADING,"faster-reloading","Faster reloading",50},
			/** passive */
			{BASIC_PISTOL,"basic-pistol","Basic handling of PISTOLS",150},
			/** passive */
			{BASIC_ASSAULT_RIFLE,"basic-assault-rifle","Basic handling of ASSAULT RIFLES",150},
			/** passive */
			{BASIC_SHOTGUN,"basic-shotgun","Basic handling of SHOTGUNS",150},
			/** passive */
			{BASIC_SNIPER_RIFLE,"basic-sniper-rifle","Basic handling of SNIPE RIFLES",150},
			/** passive */
			{BASIC_SUB_MACHINE_GUN,"basic-sub-machine-gun","Basic handling of SUB-MACHINE GUNS",150},
			/** passive */
			{INTERMEDIATE_PISTOL,"intermediate-pistol","Intermediate handling of PISTOLS",150},
			/** passive */
			{INTERMEDIATE_ASSAULT_RIFLE,"intermediate-assault-rifle","Intermediate handling of ASSAULT RIFLES",150},
			/** passive */
			{INTERMEDIATE_SHOTGUN,"intermediate-shotgun","Intermediate handling of SHOTGUNS",150},
			/** passive */
			{INTERMEDIATE_SNIPER_RIFLE,"intermediate-sniper-rifle","Intermediate handling of SNIPE RIFLES",150},
			/** passive */
			{INTERMEDIATE_SUB_MACHINE_GUN,"intermediate-sub-machine-gun","Intermediate handling of SUB-MACHINE GUNS",150},
			/** passive */
			{ADVANCED_PISTOL,"advanced-pistol","Advanced handling of PISTOLS",150},
			/** passive */
			{ADVANCED_ASSAULT_RIFLE,"advanced-assault-rifle","Advanced handling of ASSAULT RIFLES",150},
			/** passive */
			{ADVANCED_SHOTGUN,"advanced-shotgun","Advanced handling of SHOTGUNS",150},
			/** passive */
			{ADVANCED_SNIPER_RIFLE,"advanced-sniper-rifle","Advanced handling of SNIPE RIFLES",150},
			/** passive */
			{ADVANCED_SUB_MACHINE_GUN,"advanced-sub-machine-gun","Advanced handling of SUB-MACHINE GUNS",150},
			/** passive */
			{EXPERT_PISTOL,"expert-pistol","Expert handling of PISTOLS",150},
			/** passive */
			{EXPERT_ASSAULT_RIFLE,"expert-assault-rifle","Expert handling of ASSAULT RIFLES",150},
			/** passive */
			{EXPERT_SHOTGUN,"expert-shotgun","Expert handling of SHOTGUNS",150},
			/** passive */
			{EXPERT_SNIPER_RIFLE,"expert-sniper-rifle","Expert handling of SNIPE RIFLES",150},
			/** passive */
			{EXPERT_SUB_MACHINE_GUN,"expert-sub-machine-gun","Expert handling of SUB-MACHINE GUNS",150},
			/** passive */
			{ARMOR_PENETRATION_SHOT,"armor-penetration-shot","Chance to penetrate armor.",150},
			/** passive */
			{NONSTOP_PENETRATION_SHOT,"nonstop-penetration-shot","If an NPC happens to be behind the mob you are sniping, you have a chance of also hitting that mob.",150},
			/** passive */
			{INCREASED_INJURE_CHANCE,"increased-injure-chance","The chance of injuring a mob is increased.",150}, /** implemented */
			/** passive */
			{ASSAULT_RIFLE_SHRAPNEL,"assault-rifle-shrapnel","Assault rifle attacks have a chance of producing shrapnel on impact equal to a percentage of the original attack damage.",150}, /** implemented */
		};
		static std::vector<proficiency_t> strategy = {
			/** castable */
			{ENTRY_DENIAL,"entry-denial", "Prevent anyone from coming into your current room for a limited time.", 2050},
			/** castable */
			{MISDIRECTION,"misdirection", "Focus the enemy's attention elsewhere for a brief period of time, thus making them vulnerable.", 2050},
			/** castable */
			{SNIPER_SUPPORT,"sniper-support", "Call in sniper support to pick off targets.", 2050},
			/** passive */
			{REDUCED_DETECTION_CHANCE,"reduced-detection-chance", "Reduces the chance of detection by 4\%.", 2050},
			/** passive */
			{INCREASED_AWARENESS,"increased-awareness", "Enhances your ability to detect mobs even if they are hidden.", 2050},
			/** passive */
			{TRICK_MAGAZINE,"trick-magazine", "Chance to get a bigger mag upon reloading.", 2050},
			/** castable */
			{CHAINABLE_BREACHING,"chainable-breaching", "Perform 1-2 extra actions immediately after you breach. For example, breach and clear.", 2050},
		};
		static std::vector<proficiency_t> medical = {
			/** passive */
			{HGH_MORE_POWERFUL,"hgh-more-powerful", "Human Growth Hormone is more effective.", 2050},
			/** castable */
			{BASIC_HP_RECOVERY,"basic-hp-recovery", "Recovery some HP.", 2050},
			/** passive */
			{PARASITIC_HP_RECOVERY,"parasitic-hp-recovery", "Critical shots have a chance of regenerating HP.", 2050},
			/** castable */
			{SUTURE,"suture", "Stop bleeding by using a medkit.", 2050},
			/** castable */
			{ADRENALINE_BOOST,"adrenaline-boost", "Boost movement and HP for a short period of time.", 2050},
		};
		static std::vector<std::tuple<std::string,std::vector<proficiencies::proficiency_t>>> list = {
			{"Electronics",electronics},
			{"Armor",armor},
			{"Marksmanship",marksmanship},
			{"Sniping",sniping},
			{"Demolitions",demolitions},
			{"Chemistry",chemistry},
			{"Weapon Handling",weapon_handling},
			{"Strategy",strategy},
			{"Medical",medical},
		};
	};
	void init_player_levels(player_ptr_t& player);
	void init_player_levels(std::string_view);
	void load_player_levels(player_ptr_t& player);
	int get_enum_by_name(std::string_view name);
	uint16_t get_player_level(player_ptr_t& player,std::string_view skill);
	uint16_t get_player_level(player_ptr_t& player,int skill);
	std::string to_string(int e_name);
	bool player_can(player_ptr_t& player,int e_name);
	int update_schema_for_player(std::string player_name);
	void refresh_minimum_proficiencies();
	using skill_t = proficiencies::proficiency_name_t;
	static const std::vector<skill_t> passive_skills{
	skill_t::ACCURACY,
	skill_t::ADVANCED_ARMOR,
	skill_t::ADVANCED_ASSAULT_RIFLE,
	skill_t::ADVANCED_PISTOL,
	skill_t::ADVANCED_SHOTGUN,
	skill_t::ADVANCED_SNIPER_RIFLE,
	skill_t::ADVANCED_SUB_MACHINE_GUN,
	skill_t::ARMOR_PENETRATION_SHOT,
	skill_t::ASSAULT_RIFLE_SHRAPNEL,
	skill_t::BASIC_ARMOR,
	skill_t::BASIC_ASSAULT_RIFLE,
	skill_t::BASIC_PISTOL,
	skill_t::BASIC_SHOTGUN,
	skill_t::BASIC_SNIPER_RIFLE,
	skill_t::BASIC_SUB_MACHINE_GUN,
	skill_t::BETTER_EXPLOSIONS,
	skill_t::BETTER_SNIPING_ACCURACY,
	skill_t::BETTER_WEAPON_ACCURACY,
	skill_t::BIGGER_CLAYMORES,
	skill_t::BIGGER_FIRE_NADES,
	skill_t::COUNTER_SHOCK,
	skill_t::CRAFTY_C4,
	skill_t::DETECT_CAMERAS,
	skill_t::ELECTRIFIED_DAMAGE,
	skill_t::ELITE_ARMOR,
	skill_t::EXPERT_ASSAULT_RIFLE,
	skill_t::EXPERT_PISTOL,
	skill_t::EXPERT_SHOTGUN,
	skill_t::EXPERT_SNIPER_RIFLE,
	skill_t::EXPERT_SUB_MACHINE_GUN,
	skill_t::EXPLOSIVE_RESISTANCE,
	skill_t::FASTER_BREACHES,
	skill_t::FASTER_RELOADING,
	skill_t::FASTER_THERMITES,
	skill_t::FASTER_TRIGGER_FINGER,
	skill_t::FURTHER_C4S,
	skill_t::HEADGEAR_INTROSPECTION,
	skill_t::HEADSHOT_CHANCE,
	skill_t::HGH_MORE_POWERFUL,
	skill_t::INCREASED_AWARENESS,
	skill_t::INCREASED_INJURE_CHANCE,
	skill_t::INJURE_RESISTANCE,
	skill_t::INTERMEDIATE_ASSAULT_RIFLE,
	skill_t::INTERMEDIATE_PISTOL,
	skill_t::INTERMEDIATE_SHOTGUN,
	skill_t::INTERMEDIATE_SNIPER_RIFLE,
	skill_t::INTERMEDIATE_SUB_MACHINE_GUN,
	skill_t::LIMB_CHANCE,
	skill_t::MUNITIONS_REFLECTOR,
	skill_t::NO_FACTOR_ADVANCED,
	skill_t::NO_FACTOR_BASIC,
	skill_t::NO_FACTOR_ELITE,
	skill_t::NONSTOP_PENETRATION_SHOT,
	skill_t::PARASITIC_HP_RECOVERY,
	skill_t::PRO_WEAPON_HANDLER,
	skill_t::QUIETER_SNIPER_SHOTS,
	skill_t::REDUCED_DETECTION_CHANCE,
	skill_t::SENTINEL_DISCIPLINE,
	skill_t::SPRAY_CHANCE,
	skill_t::STRONGER_FRAG_ARM,
	skill_t::TOGGLE_NIGHT_VISION,
	skill_t::TOGGLE_THERMAL,
	skill_t::TRICK_MAGAZINE,
	skill_t::VIP};
	static const std::vector<skill_t> castable_skills{
	skill_t::ADRENALINE_BOOST,
	skill_t::BASIC_HP_RECOVERY,
	skill_t::CAMERA_DESTRUCTION,
	skill_t::CAMERA_PLACEMENT,
	skill_t::CHAINABLE_BREACHING,
	skill_t::CREATE_EMP_WEAPONS,
	skill_t::DEPLOYABLE_FIRE_TURRET,
	skill_t::DEPLOYABLE_SHIELD,
	skill_t::DEPLOYABLE_TURRET,
	skill_t::DOOR_CAMERA,
	skill_t::DUTCH_OVEN,
	skill_t::ENTRY_DENIAL,
	skill_t::FIRE_NADE_DEALER,
	skill_t::HGH_DEALER,
	skill_t::MISDIRECTION,
	skill_t::MOLD,
	skill_t::NOXIOUS_NADE_CRAFTER,
	skill_t::PROTECT_FROM_EMP,
	skill_t::PROXIMITY_ALARM,
	skill_t::RETROFIT_FIRE_GRENADE_LAUNCHER,
	skill_t::RETROFIT_GRENADE_LAUNCHER,
	skill_t::RETROFIT_SENSOR_GRENADE_LAUNCHER,
	skill_t::RETROFIT_SMOKE_GRENADE_LAUNCHER,
	skill_t::RETROFIT_STUN_GRENADE_LAUNCHER,
	skill_t::SNIPER_SUPPORT,
	skill_t::STEROID_DEALER,
	skill_t::SUTURE,
	skill_t::TARGET_LIMB,
	skill_t::THIEF,
	skill_t::TRACKER
	};
};
using skill_t = mods::skills::proficiencies::proficiency_name_t;

#endif
