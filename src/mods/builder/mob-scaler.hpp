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
	struct mob_equipment_randomizer {

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
	struct mob_roaming {
		std::vector<room_vnum> rooms;
		std::vector<std::string> roam_patterns;
	};

};//end namespace

#endif
