#ifndef __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#define __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#include "../globals.hpp"

namespace mods::orm {
	struct player_contract_state;
};

namespace mods::contracts {
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
			GOAL_INSTALL = (1 << 8)
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
	};

	using contract_vnum_t = uint32_t;
	struct contract {
		contract() = delete;
		contract(contract_vnum_t c_vnum,std::string in_title,std::string desc) : vnum(c_vnum), title(in_title), description(desc) {

		}
		~contract() = default;
		contract_vnum_t vnum;
		std::string title;
		std::string description;
		std::vector<contract_step> steps;
	};//end class

	extern std::deque<std::shared_ptr<contract>>& contract_master_list();
	void load_all_contracts();

};
#endif
