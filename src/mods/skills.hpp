#ifndef __MENTOC_MODS_SKILLS_HEADER__
#define __MENTOC_MODS_SKILLS_HEADER__

#include "../globals.hpp"
#include "string.hpp"

namespace mods::skills {
	enum fields_of_study {
		NONE = 0,
		ELECTRONICS = 1,
		ARMOR,
		MARKSMANSHIP,
		SNIPING,
		DEMOLITIONS,
		CHEMISTRY,
		WEAPON_HANDLING
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
			FASTER_RELOADING
		};
		struct proficiency_t {
			proficiency_name_t e_name;
			mods::string name;
			mods::string description;
			uint16_t minimum_profiency;
		};
		/** ATTENTION !!!!
		 * ALL minimum_proficiencies are GUESSTIMATIONS. They will change!
		 */
		static std::vector<proficiency_t> electronics = {
			{CAMERA_PLACEMENT,"camera-placement", "Ability to plant and view remote cameras.",50},
			{DETECT_CAMERAS,"detect-cameras", "Can detect cameras, even those that are hidden.",50},
			{CAMERA_DESTRUCTION,"camera-destruction", "Able to destroy cameras that aren't your own.", 100},
			{ELECTRIFIED_DAMAGE,"electrified-damage", "Can use gadgets and weapons that can deal electric damage.",100},
			{PROTECT_FROM_EMP,"protect-from-emp", "Can protect various electronic devices from E.M.P. damage.",200},
			{CREATE_EMP_WEAPONS,"create-emp-weapons","Can create E.M.P. weapons.",200}
		};
		static std::vector<proficiency_t> armor = {
			{BASIC_ARMOR,"basic-armor", "Can use BASIC class armor.",50},
			{ADVANCED_ARMOR,"advanced-armor", "Can use ADVANCED class armor.",50},
			{ELITE_ARMOR,"elite-armor", "Can use ELITE class armor.",50},
			{THIEF,"thief", "Can steal armor off of corpses.", 200},
			{MOLD,"mold", "Can mold together various types of armor to create new armor.", 350},
			{NO_FACTOR_BASIC,"no-factor-basic", "Heavy BASIC armor doesn't affect your movement.", 400},
			{NO_FACTOR_ADVANCED,"no-factor-advanced", "Heavy BASIC armor doesn't affect your movement.", 850},
			{NO_FACTOR_ELITE,"no-factor-elite", "Heavy BASIC armor doesn't affect your movement.", 900},
			{EXPLOSIVE_RESISTANCE,"explosive-resistance", "Explosions have less impact on you.", 2050},
			{INJURE_RESISTANCE,"injure-resistance", "You are less likely to get injured.", 3050},
		};
		static std::vector<proficiency_t> marksmanship = {
			{SPRAY_CHANCE,"spray-chance", "Chance to hit during spray is increased", 150},
			{HEADSHOT_CHANCE,"headshot-chance", "Chance to headshot is increased.", 250},
			{LIMB_CHANCE,"limb-chance", "Chance to target limb is increased.", 350},
			{BETTER_SNIPING_ACCURACY,"better-sniping-accuracy", "Sniping accuracy increased.",450}
		};
		static std::vector<proficiency_t> sniping = {
			{ACCURACY,"accuracy", "Accuracy increased with sniper rifles.", 150},
			{VIP,"vip", "Exclusive access to advanced sniper rifles", 250},
			{TRACKER,"tracker", "Tracking bullets.",350}
		};
		static std::vector<proficiency_t> demolitions = {
			{FASTER_BREACHES,"faster-breaches","Breach charge placement time decreased",50},
			{FURTHER_C4S,"further-c4s","C4 remote detonation distance increased",50},
			{STRONGER_FRAG_ARM,"stronger-frag-arm","Grenade throw distance increased",50},
			{BIGGER_CLAYMORES,"bigger-claymores","Claymore damage increased",50},
			{FASTER_THERMITES,"faster-thermites","Thermite breach charge placement time decreased",50},
			{BIGGER_FIRE_NADES,"bigger-fire-nades","Incendiary grenade damage increased",50},
			{CRAFTY_C4,"crafty-c4","Can craft C4",50},
			{BETTER_EXPLOSIONS,"better-explosions","Chance to do 33% more damage with any explosive",50}
		};
		static std::vector<proficiency_t> chemistry = {
			{STEROID_DEALER,"steroid-dealer","Can create steroids",50},
			{HGH_DEALER,"hgh-dealer","Can create human growth hormone",50},
			{NOXIOUS_NADE_CRAFTER,"noxious-nade-crafter","Can create noxious gas grenades",50},
			{DUTCH_OVEN,"dutch-oven","Seal off room to make air-tight",50},
			{FIRE_NADE_DEALER,"fire-nade-dealer","Can create incendiary ammo",50}
		};
		static std::vector<proficiency_t> weapon_handling = {
			{PRO_WEAPON_HANDLER,"pro-weapon-handler","Enhanced familiarity with weapons",50},
			{BETTER_WEAPON_ACCURACY,"better-weapon-accuracy","Increased accuracy with any weapon",50},
			{FASTER_TRIGGER_FINGER,"faster-trigger-finger","Decreased cooldown between shots",50},
			{FASTER_RELOADING,"faster-reloading","Faster reloading",50}
		};
		static std::vector<std::vector<proficiencies::proficiency_t>> list = {
			electronics, armor, marksmanship,sniping,demolitions,chemistry,weapon_handling
		};
	};
	void init_player_levels(player_ptr_t& player);
	void init_player_levels(std::string_view);
	void load_player_levels(player_ptr_t& player);
	int get_enum_by_name(std::string_view name);
	std::string to_string(int e_name);
	bool player_can(player_ptr_t& player,int e_name);
};
using skill_t = mods::skills::proficiencies::proficiency_name_t;

#endif
