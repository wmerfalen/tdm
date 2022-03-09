#ifndef __MENTOC_MODS_CONTRACT_STEPS_HEADER__
#define __MENTOC_MODS_CONTRACT_STEPS_HEADER__
#include "../globals.hpp"

namespace mods::orm {
	struct player_contract_state;
};

namespace mods::contracts {
	enum custom_events_t : uint32_t {
		CEV_HUMAN_REMAINS_FOUND,
	};
	struct contract_step {
		enum task_type_t : uint16_t {
			GOAL_FIND = (1 << 0),
			GOAL_DESTROY = (1 << 1),
			GOAL_RETRIEVE = (1 << 2),
			GOAL_QUOTA = (1 << 3),
			GOAL_KILL = (1 << 4),
			GOAL_GAIN_ENTRY = (1 << 5),
			GOAL_PROTECT = (1 << 6),
			GOAL_TALK_TO = (1 << 7),
			GOAL_INSTALL = (1 << 8),
			GOAL_GIVE = (1 << 9),
			GOAL_CUSTOM_EVENT = (1 << 10),
			GOAL_UNINSTALL = (1 << 11),
		};
		enum task_target_t : uint16_t {
			TARGET_MOB = 0,
			TARGET_ITEM,
			TARGET_ROOM,
			TARGET_DOOR
		};
		task_type_t 		goal;
		task_target_t 	target;
		std::string 		description;
		uint32_t				mob_vnum;
		uuid_t 					mob_uuid;
		std::string			object_yaml;
		uuid_t					object_uuid;
		room_vnum				room;
		uint8_t					quota;
		bool 						is_optional;
		uint32_t        reward_xp;
		uint32_t        reward_money;
		std::string     reward_1;
		std::string     reward_2;
		std::string     reward_3;
		std::string     reward_4;
		std::string     reward_5;
		std::string     reward_6;
		std::string     reward_7;
		std::string     reward_8;
		std::string     reward_9;
		std::string     reward_10;
		contract_step(const contract_step& other);
		contract_step() = default;
	};
	using task_t = contract_step::task_type_t;
	using target_t = contract_step::task_target_t;
	std::map<std::string,task_t> task_string_map();
	std::map<std::string,target_t> target_string_map();
	std::map<task_t,std::string> inverse_task_string_map();
	std::map<target_t,std::string> inverse_target_string_map();
	std::vector<std::string> get_string_list_from_task_flags(task_t task);
	std::string get_string_from_target(target_t target);
	target_t parse_target_string(std::string_view t);
	task_t parse_csv_to_task_flag(std::string_view csv);
};
#endif
