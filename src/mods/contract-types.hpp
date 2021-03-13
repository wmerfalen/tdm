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
	using task_t = contract_step::task_type_t;
	using target_t = contract_step::task_target_t;
	static inline std::map<std::string,task_t> task_string_map() {
		return {
			{"GOAL_FIND",task_t::GOAL_FIND},
			{"GOAL_DESTROY",task_t::GOAL_DESTROY},
			{"GOAL_RETRIEVE",task_t::GOAL_RETRIEVE},
			{"GOAL_QUOTA",task_t::GOAL_QUOTA},
			{"GOAL_KILL",task_t::GOAL_KILL},
			{"GOAL_GAIN_ENTRY",task_t::GOAL_GAIN_ENTRY},
			{"GOAL_PROTECT",task_t::GOAL_PROTECT},
			{"GOAL_TALK_TO",task_t::GOAL_TALK_TO},
			{"GOAL_INSTALL",task_t::GOAL_INSTALL},
		};
	}
	static inline std::map<std::string,target_t> target_string_map() {
		return {
			{"TARGET_MOB",target_t::TARGET_MOB},
			{"TARGET_ITEM",target_t::TARGET_ITEM},
			{"TARGET_ROOM",target_t::TARGET_ROOM},
			{"TARGET_DOOR",target_t::TARGET_DOOR},
		};
	}
	static inline std::map<task_t,std::string> inverse_task_string_map() {
		return {
			{task_t::GOAL_FIND,"GOAL_FIND"},
			{task_t::GOAL_DESTROY,"GOAL_DESTROY"},
			{task_t::GOAL_RETRIEVE,"GOAL_RETRIEVE"},
			{task_t::GOAL_QUOTA,"GOAL_QUOTA"},
			{task_t::GOAL_KILL,"GOAL_KILL"},
			{task_t::GOAL_GAIN_ENTRY,"GOAL_GAIN_ENTRY"},
			{task_t::GOAL_PROTECT,"GOAL_PROTECT"},
			{task_t::GOAL_TALK_TO,"GOAL_TALK_TO"},
			{task_t::GOAL_INSTALL,"GOAL_INSTALL"},
		};
	}
	static inline std::map<target_t,std::string> inverse_target_string_map() {
		return {
			{target_t::TARGET_MOB,"TARGET_MOB"},
			{target_t::TARGET_ITEM,"TARGET_ITEM"},
			{target_t::TARGET_ROOM,"TARGET_ROOM"},
		};
	}
	static inline std::vector<std::string> get_string_list_from_task_flags(task_t task) {
		std::vector<std::string> list;
		for(const auto& pair : inverse_task_string_map()) {
			if(task & pair.first) {
				list.emplace_back(pair.second);
			}
		}
		return list;
	}
	static inline std::string get_string_from_target(target_t target) {
		for(const auto& pair : target_string_map()) {
			if(pair.second == target) {
				return pair.first;
			}
		}
		return "<unknown>";
	}
	static inline target_t parse_target_string(std::string_view t) {
		for(const auto& pair : target_string_map()) {
			if(pair.first.compare(t.data()) == 0) {
				return pair.second;
			}
		}
		return (target_t)0;
	}
	static inline task_t parse_csv_to_task_flag(std::string_view csv) {
		uint16_t task = 0;
		std::vector<std::string> list = EXPLODE(csv.data(),',');
		for(const auto& pair : inverse_task_string_map()) {
			for(const auto& current_flag : list) {
				if(pair.second.compare(current_flag.c_str()) == 0) {
					task |= pair.first;
				}
			}
		}
		return (task_t)task;
	}



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

	std::deque<std::shared_ptr<contract>>& contract_master_list();
	void load_all_contracts();

};
#endif
