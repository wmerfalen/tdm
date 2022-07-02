#ifndef __MENTOC_MODS_BUILDER_mob_scaler_HEADER__
#define __MENTOC_MODS_BUILDER_mob_scaler_HEADER__
#include "../structs.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"
#include "../interpreter.h"

extern void r_error(const player_ptr_t& player,std::string_view msg);
extern void r_success(const player_ptr_t& player,std::string_view msg);
extern void r_status(const player_ptr_t& player,std::string_view msg);
namespace mods::builder {
	std::tuple<bool,std::string,int> scale_mob(mob_rnum id,int level, uint64_t raid_id);
	struct scaled_mob {
		int level;

		int exp;
		mob_vnum virt;
		uint64_t raid_id;
		int mana;
		int max_mana;
		int hit;
		int max_hit;
		int move;
		int max_move;
		int damroll;
		int gold;
		int weight;
		int height;
		int strength;
		int strength_add;
		int intelligence;
		int dexterity;
		int constitution;
		int electronics;
		int chemistry;
		int strategy;
		int marksmanship;
		int sniping;
		int weapon_handling;
		int demolitions;
		int armor;
		int medical;
		int charisma;
		int damnodice;
		int damsizedice;
		std::string name;
		std::string sex;
		std::string action;
		std::string short_description;
		std::string long_description;
		std::string description;
		std::string extended_type; // i.e.:"CAR_THIEF",

		struct equipment {
			std::string light;
			std::string finger_r;
			std::string finger_l;
			std::string neck_1;
			std::string neck_2;
			std::string body;
			std::string head;
			std::string legs;
			std::string feet;
			std::string hands;
			std::string arms;
			std::string shield;
			std::string about;
			std::string waist;
			std::string wrist_r;
			std::string wrist_l;
			std::string wield;
			std::string hold;
			std::string secondary;
			std::string shoulders_l;
			std::string shoulders_r;
			std::string backpack;
			std::string goggles;
			std::string vest_pack;
			std::string elbow_l;
			std::string elbow_r;
		};

		std::vector<room_vnum> rooms;
		std::vector<std::string> roam_patterns;
	};

};//end namespace

#endif
