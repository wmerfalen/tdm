#ifndef __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#define __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#include <iostream>
#include <string>
#include <vector>

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

	extern std::vector<std::shared_ptr<contract>>& contract_master_list();

	/**
	 * @brief holds state data for the current contract
	 */
	struct player_contract_instance {
			player_contract_instance();
			/**
			 * @brief starts or resumes the contract. preferred entry method
			 *
			 * @param in_contract contract vnum
			 * @param player_id player->db_id()
			 */
			player_contract_instance(contract_vnum_t in_contract,uint64_t player_id);
			/**
			 * @brief helper function called directly from constructor above
			 *
			 * @param in_vnum contract vnum
			 *
			 * @return
			 */
			std::tuple<bool,std::string> start_or_resume_contract(contract_vnum_t in_vnum);
			std::tuple<bool,std::string> advance();
			bool finished();
			void init();
			const auto& contract_vnum() const {
				return m_contract_vnum;
			}
			void save_step_data();
			void stop_contract();
		protected:
			std::shared_ptr<mods::orm::player_contract_state> m_state_orm;
			std::tuple<int16_t,std::string> m_update_status;
			contract_vnum_t m_contract_vnum;
			std::size_t m_step;
			std::string encode_step_data(std::string_view);
			void load_decoded_step_data();
			uint64_t m_player_id;
			std::shared_ptr<contract> m_contract_copy;
			std::map<std::string,std::string> m_decoded_state_data;
			std::string m_state_buffer;
			std::string m_extra_data;
	};

};
#endif
