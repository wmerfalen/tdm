#ifndef __MENTOC_MODS_BUILDER_RAID_HEADER__
#define __MENTOC_MODS_BUILDER_RAID_HEADER__
#include "../structs.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"
#include "../interpreter.h"
#include "../orm/raid.hpp"

extern void r_error(const player_ptr_t& player,std::string_view msg);
extern void r_success(const player_ptr_t& player,std::string_view msg);
extern void r_status(const player_ptr_t& player,std::string_view msg);
namespace mods::builder {
	enum raid_type_t : uint8_t {
		RAID_NONE = 0,
		RAID_ELIMINATE = 1,
		RAID_DEFEND_OBJECTIVE,
		RAID_PLANT_BOMB,
		RAID_DIFFUSE_BOMB,
	};
	static inline raid_type_t from_string(std::string_view type) {
		if(type.compare("ELIMINATE") == 0) {
			return RAID_ELIMINATE;
		}
		if(type.compare("DEFEND_OBJECTIVE") == 0) {
			return RAID_DEFEND_OBJECTIVE;
		}
		if(type.compare("PLANT_BOMB") == 0) {
			return RAID_PLANT_BOMB;
		}
		if(type.compare("DIFFUSE_BOMB") == 0) {
			return RAID_DIFFUSE_BOMB;
		}
		return RAID_NONE;
	}

	static inline std::string to_string(raid_type_t type) {
		switch(type) {
			default:
			case RAID_NONE:
				return "NONE";
			case RAID_ELIMINATE:
				return "ELIMINATE";
			case RAID_DEFEND_OBJECTIVE:
				return "DEFEND_OBJECTIVE";
			case RAID_PLANT_BOMB:
				return "PLANT_BOMB";
			case RAID_DIFFUSE_BOMB:
				return "DIFFUSE_BOMB";
		}
	}
	struct raid_t {
		uint64_t id;
		std::string name;
		std::string level;
		raid_type_t type;

		std::vector<mob_vnum> mobs;
		std::vector<reset_com> zone_commands;
		uint32_t started_at;

		mods::orm::raid orm;

		raid_t() = delete;
		raid_t(std::string_view n,
		       std::string_view l,
		       std::string_view t) : name(n), level(l), type(from_string(t)) {
			started_at = 0;
		}
	};
	extern std::vector<std::unique_ptr<raid_t>> raid_list;
};

#endif
