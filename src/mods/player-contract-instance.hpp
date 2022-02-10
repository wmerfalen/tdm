#ifndef __MENTOC_MODS_PLAYER_CONTRACT_INSTANCE_HEADER__
#define __MENTOC_MODS_PLAYER_CONTRACT_INSTANCE_HEADER__
#include "contract-types.hpp"
#include "players/messages.hpp"
namespace mods::contracts {
	using task_t = mods::contracts::contract_step::task_type_t;
	using target_t = mods::contracts::contract_step::task_target_t;
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

			std::string get_type_string() const ;

			const auto& vnum() const {
				return m_contract_vnum;
			}
			const auto& contract_vnum() const {
				return m_contract_vnum;
			}
			const task_t& goal() const {
				return m_goal;
			}
			const target_t& target() const {
				return m_target;
			}
			bool is_custom_event() const {
				return m_goal & task_t::GOAL_CUSTOM_EVENT;
			}
			bool is_give_item() const {
				return m_goal & task_t::GOAL_GIVE && m_target == target_t::TARGET_MOB;
			}
			bool is_find_item() const {
				return m_goal & task_t::GOAL_FIND && m_target == target_t::TARGET_ITEM;
			}
			bool is_find_mob() const {
				return m_goal & task_t::GOAL_FIND && m_target == target_t::TARGET_MOB;
			}
			bool is_find_room() const {
				return m_goal & task_t::GOAL_FIND && m_target == target_t::TARGET_ROOM;
			}
			bool is_find_door() const {
				return m_goal & task_t::GOAL_FIND && m_target == target_t::TARGET_DOOR;
			}
			bool is_destroy_item() const {
				return m_goal & task_t::GOAL_DESTROY && m_target == target_t::TARGET_ITEM;
			}
			bool is_destroy_door() const {
				return m_goal & task_t::GOAL_DESTROY && m_target == target_t::TARGET_DOOR;
			}
			bool is_retrieve_item() const {
				return m_goal & task_t::GOAL_RETRIEVE && m_target == target_t::TARGET_ITEM;
			}
			bool is_quota_find_item() const {
				return m_goal & task_t::GOAL_QUOTA && m_goal & task_t::GOAL_FIND && m_target == target_t::TARGET_ITEM;
			}
			bool is_quota_kill_mob() const {
				return m_goal & task_t::GOAL_QUOTA && m_goal & target_t::TARGET_MOB;
			}
			bool is_quota_destroy_door() const {
				return m_goal & task_t::GOAL_QUOTA && m_goal & task_t::GOAL_DESTROY && m_goal & target_t::TARGET_DOOR;
			}
			bool is_kill() const {
				return m_goal & task_t::GOAL_KILL && !(m_goal & task_t::GOAL_QUOTA);
			}
			bool is_gain_entry() const {
				return m_goal & task_t::GOAL_GAIN_ENTRY;
			}
			bool is_talk_to() const {
				return m_goal & task_t::GOAL_TALK_TO;
			}
			bool is_install_item() const {
				return m_goal & task_t::GOAL_INSTALL;
			}
			bool is_uninstall_item() const {
				return m_goal & task_t::GOAL_UNINSTALL;
			}

			void save_step_data();
			void stop_contract();

			std::tuple<bool,std::string> check_status();

			const auto& step_counter() const {
				return m_step;
			}
			std::string_view dump_step();
			std::string pretty_dump_step();

			void custom_event(custom_events_t event,uuid_t id);
			void find_item(obj_ptr_t& object);
			void give_item(obj_ptr_t& object,player_ptr_t& mob);
			void find_mob(player_ptr_t& mob);
			void find_room(const room_rnum& room_id);
			void find_door(const room_rnum& room_id,const int8_t& direction);
			void destroy_item(obj_ptr_t& object);
			void destroy_door(const room_rnum& room_id,const int8_t& direction);
			void retrieve_item(obj_ptr_t& object);
			void quota_find_item(obj_ptr_t& object);
			void quota_kill_mob(player_ptr_t& mob);
			void quota_destroy_door(const room_rnum& room_id,const int8_t& direction);
			void kill(player_ptr_t& mob);
			void gain_entry(const room_rnum& room_id);
			void talk_to(player_ptr_t& mob);
			void install_item(const uuid_t& obj_uuid);
			void uninstall_item(const uuid_t& obj_uuid);
			std::string_view title() const {
				return m_contract_copy->title;
			}
			std::string_view description() const {
				return m_contract_copy->description;
			}
		protected:
			void refresh_dump();
			uint32_t m_quota;
			void m_auto_update_step();
			mods::contracts::contract_step* m_current_step;
			task_t m_goal;
			target_t m_target;
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
			std::string m_step_dump;
	};
};
#endif
