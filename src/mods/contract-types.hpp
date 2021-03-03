#ifndef __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#define __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#include <iostream>
#include <string>
#include <vector>

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

	extern std::vector<std::shared_ptr<contract>>& contract_master_list();

	/**
	 * @brief holds state data for the current contract
	 */
	struct player_contract_instance {
		/**
		 * @brief starts or resumes the contract. preferred entry method
		 *
		 * @param mode "start" or "resume"
		 * @param in_contract contract vnum
		 */
		player_contract_instance(std::string_view mode, contract_vnum_t in_contract) :
			contract_vnum(in_contract) {
			if(mode.compare("start") == 0 || mode.compare("resume") == 0) {
				start_or_resume_contract(in_contract);
			}
		}
		/**
		 * @brief helper function called directly from constructor above
		 *
		 * @param in_vnum contract vnum
		 *
		 * @return
		 */
		std::tuple<bool,std::string> start_or_resume_contract(contract_vnum_t in_vnum) {
			contract_copy = nullptr;
			auto& list = contract_master_list();
			for(auto& elem : list) {
				if(elem->vnum == in_vnum) {
					contract_copy = elem;
					break;
				}
			}
			if(!contract_copy) {
				return {0,"No contract loaded with that vnum"};
			}
			return {1,"Contract started"};
		}
		/**
		 * @brief contract vnum
		 */
		contract_vnum_t contract_vnum;
		/**
		 * @brief pointer to contract data. do not modify!
		 */
		std::shared_ptr<contract> contract_copy;
	};

};
#endif
