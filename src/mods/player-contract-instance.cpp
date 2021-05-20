#include "contract-types.hpp"
#include "orm/contracts.hpp"
#include "orm/player-contract-state.hpp"
#include "player.hpp"
#include "player-contract-instance.hpp"
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contracts::player_contract_instance][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods::globals {
	extern std::map<uint64_t,uuid_t> db_id_to_uuid_map;
};
namespace mods::contracts {
	using task_t = contract_step::task_type_t;
	using target_t = contract_step::task_target_t;
	/**
	 * @brief
	 */
	void player_contract_instance::stop_contract() {
		m_state_orm->delete_by_player_id_contract_vnum(m_player_id,m_contract_vnum);
	}

	/**
	 * @brief
	 */
	void player_contract_instance::m_auto_update_step() {
		if(!m_contract_copy) {
			m_current_step = nullptr;
			m_goal = (task_t)0;
			m_target = (target_t)0;
			refresh_dump();
			return;
		}
		if(m_step >= m_contract_copy->steps.size()) {
			m_current_step = nullptr;
			m_goal = (task_t)0;
			m_target = (target_t)0;
			refresh_dump();
			return;
		}
		m_current_step = &m_contract_copy->steps.at(m_step);
		m_goal = m_current_step->goal;
		m_target = m_current_step->target;
		refresh_dump();
	}
	/**
	 * @brief
	 */
	player_contract_instance::player_contract_instance() {
		this->init();
		m_current_step = nullptr;
		m_goal = (task_t)0;
		m_target = (target_t)0;
	}
	/**
	 * @brief
	 *
	 * @param in_contract
	 * @param player_id
	 */
	player_contract_instance::player_contract_instance(contract_vnum_t in_contract,uint64_t player_id) {
		this->init();
		m_contract_vnum = in_contract;
		m_player_id = player_id;
		start_or_resume_contract(in_contract);
	}
	/**
	 * @brief
	 *
	 * @param data
	 *
	 * @return
	 */
	std::string player_contract_instance::encode_step_data(std::string_view data) {
		return data.data();
	}
	/**
	 * @brief
	 */
	void player_contract_instance::init() {
		m_quota = 0;
		m_state_orm = std::make_shared<mods::orm::player_contract_state>();
		m_state_buffer.clear();
		m_decoded_state_data.clear();
		m_contract_vnum = 0;
		m_step = 0;
		m_player_id = 0;
		m_contract_copy = nullptr;
	};



	/**
	 * @brief
	 */
	void player_contract_instance::load_decoded_step_data() {
		/**
		 * encoded data format: "N|{}"
		 * N is the step
		 * pipe character is delimiter
		 * open and shut brackets will eventually contain other data. it's a placeholder for now
		 */
		m_step = 0;
		std::string current = "";
		bool step_extracted = 0;
		m_extra_data.clear();
		for(auto ch : m_state_buffer) {
			if(ch == '|' && !step_extracted) {
				m_step = mods::util::stoi(current).value_or(0);
				current.clear();
				step_extracted = 1;
				continue;
			}
			if(step_extracted) {
				m_extra_data += ch;
				continue;
			}
			current += ch;
		}
	}
	/**
	 * @brief
	 */
	void player_contract_instance::save_step_data() {
		m_update_status = m_state_orm->update_player_data(m_player_id,m_contract_vnum, encode_step_data(CAT(m_step,"|",m_extra_data)));
	}
	/**
	 * @brief
	 *
	 * @return
	 */
	bool player_contract_instance::finished() {
		return m_step >= m_contract_copy->steps.size();
	}
	/**
	 * @brief
	 *
	 * @return
	 */
	std::tuple<bool,std::string> player_contract_instance::advance() {
		if(m_current_step) {
			m_current_step->reward(m_player_id);
		}
		m_quota = 0;
		m_step += 1;
		save_step_data();
		m_auto_update_step();

		std::string msg = "{grn}You move forward in your current contract{/grn}";
		if(!finished()) {
			msg += m_current_step->description;
		}
		auto player = ptr_by_db_id(m_player_id);
		if(!player) {
			log("SYSERR: couldn't get player pointer from player id: %d",m_player_id);
			return m_update_status;
		}
		player->sendln(msg);
		return m_update_status;
	}
	/**
	 * @brief helper function called directly from constructor above
	 *
	 * @param in_vnum contract vnum
	 *
	 * @return
	 */
	std::tuple<bool,std::string> player_contract_instance::start_or_resume_contract(contract_vnum_t in_vnum) {
		m_step = 0;
		m_contract_vnum = in_vnum;
		m_contract_copy = nullptr;
		m_state_buffer.clear();
		auto& list = contract_master_list();
		for(auto& elem : list) {
			if(elem->vnum == in_vnum) {
				m_contract_copy = elem;
				break;
			}
		}
		if(!m_contract_copy) {
			return {0,"No contract loaded with that vnum"};
		}
		/** Grab player_contract_state where pc_player_id=P,pc_contract_vnum=C */
		auto res = mods::orm::load_player_contract_state(m_player_id,in_vnum,m_state_buffer);
		if(std::get<0>(res) == 0) {
			/** No results. initialize contract */
			m_state_orm->pc_player_id = m_player_id;
			m_state_orm->pc_contract_vnum = in_vnum;
			m_state_buffer = m_state_orm->pc_state_data = encode_step_data("0|{}");
			m_step = 0;
			m_state_orm->save();
		}
		load_decoded_step_data();
		m_auto_update_step();
		return {1,"Contract started"};
	}
	/**
	 * @brief
	 *
	 * @param item_uuid
	 */
	void player_contract_instance::find_item(const uuid_t& item_uuid) {
		if(!m_current_step) {
			std::cerr << red_str("find_item -> m_current_step is null!") << "\n";
			return;
		}
		auto item = optr_by_uuid(item_uuid);
		if(!item) {
			std::cerr << red_str("find_item -> optr_by_uuid is null!") << "\n";
			return;
		}
		if(m_current_step->goal & task_t::GOAL_FIND &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			std::cerr << green_str("is goal find and is target item...") << "\n";
			std::cerr << green_str("step yaml:'") << m_current_step->object_yaml.c_str() << "', item:'" << item->feed_file().data() << "'\n";
			if(m_current_step->object_yaml.compare(item->feed_file().data()) == 0) {
				std::cerr << green_str("advancing!") << "\n";
				this->advance();
				return;
			}
		}
	}
	/**
	 * @brief
	 *
	 * @param mob_uuid
	 */
	void player_contract_instance::find_mob(const uuid_t& mob_uuid) {
		if(!m_current_step) {
			return;
		}
		auto mob = ptr_by_uuid(mob_uuid);
		if(!mob) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_FIND &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->vnum()) {
				this->advance();
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @return
	 */
	std::string player_contract_instance::pretty_dump_step() {
		std::string pretty = "{red}Current Step:{/red}:\r\n";
		if(!m_current_step) {
			pretty = "\tnothing to report\r\n";
			return pretty;
		}
		pretty += "\t\t{yel}[type]:{/yel}{grn}";
		if(is_find_item()) {
			pretty += "find item";
		}
		if(is_find_mob()) {
			pretty += "find_mob";
		}
		if(is_find_room()) {
			pretty += "find_room";
		}
		if(is_find_door()) {
			pretty += "find_door";
		}
		if(is_destroy_item()) {
			pretty += "destroy_item";
		}
		if(is_destroy_door()) {
			pretty += "destroy_door";
		}
		if(is_retrieve_item()) {
			pretty += "retrieve_item";
		}
		if(is_quota_find_item()) {
			pretty += "quota_find_item";
		}
		if(is_quota_kill_mob()) {
			pretty += "quota_kill_mob";
		}
		if(is_quota_destroy_door()) {
			pretty += "quota_destroy_door";
		}
		if(is_kill()) {
			pretty += "kill";
		}
		if(is_gain_entry()) {
			pretty += "gain_entry";
		}
		if(is_talk_to()) {
			pretty += "talk_to";
		}
		if(is_install_item()) {
			pretty += "install_item";
		}
		pretty += CAT("{/grn}\r\n\t\t{yel}[description]:{/yel}{grn}",m_current_step->description,"{/grn}\r\n");
		return pretty;
	}
	void player_contract_instance::refresh_dump() {
		if(!m_current_step) {
			m_step_dump = "nothing to report\r\n";
			return;
		}
		m_step_dump = "[type]:";
		if(is_find_item()) {
			m_step_dump += "find item";
		}
		if(is_find_mob()) {
			m_step_dump += "find_mob";
		}
		if(is_find_room()) {
			m_step_dump += "find_room";
		}
		if(is_find_door()) {
			m_step_dump += "find_door";
		}
		if(is_destroy_item()) {
			m_step_dump += "destroy_item";
		}
		if(is_destroy_door()) {
			m_step_dump += "destroy_door";
		}
		if(is_retrieve_item()) {
			m_step_dump += "retrieve_item";
		}
		if(is_quota_find_item()) {
			m_step_dump += "quota_find_item";
		}
		if(is_quota_kill_mob()) {
			m_step_dump += "quota_kill_mob";
		}
		if(is_quota_destroy_door()) {
			m_step_dump += "quota_destroy_door";
		}
		if(is_kill()) {
			m_step_dump += "kill";
		}
		if(is_gain_entry()) {
			m_step_dump += "gain_entry";
		}
		if(is_talk_to()) {
			m_step_dump += "talk_to";
		}
		if(is_install_item()) {
			m_step_dump += "install_item";
		}
		m_step_dump += CAT("\r\n[description]:{yel}",m_current_step->description,"{/yel}\r\n");
	}
	/**
	 * @brief
	 *
	 * @return
	 */
	std::string_view player_contract_instance::dump_step() {
		return m_step_dump;
	}
	/**
	 * @brief
	 *
	 * @param room_id
	 */
	void player_contract_instance::find_room(const room_rnum& room_id) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_FIND &&
		        m_current_step->target == target_t::TARGET_ROOM) {
			room_vnum v = world.size() > room_id ? world[room_id].number : 0;
			if(m_current_step->room == v) {
				this->advance();
				return;
			}
		}


	}
	/**
	 * @brief
	 *
	 * @param room_id
	 * @param direction
	 */
	void player_contract_instance::find_door(const room_rnum& room_id,const int8_t& direction) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_FIND &&
		        m_current_step->target == target_t::TARGET_DOOR) {
			room_vnum v = world.size() > room_id ? world[room_id].number : 0;
			/** TODO: FIXME: need to add direction/door to sql */
			if(m_current_step->room == v) {
				this->advance();
				return;
			}
		}
	}
	/**
	 * @brief
	 *
	 * @param item_uuid
	 */
	void player_contract_instance::destroy_item(const uuid_t& item_uuid) {
		if(!m_current_step) {
			return;
		}
		auto item = optr_by_uuid(item_uuid);
		if(!item) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_DESTROY &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(item->feed_file().data()) == 0) {
				this->advance();
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param room_id
	 * @param direction
	 */
	void player_contract_instance::destroy_door(const room_rnum& room_id,const int8_t& direction) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_DESTROY &&
		        m_current_step->target == target_t::TARGET_DOOR) {
			room_vnum v = world.size() > room_id ? world[room_id].number : 0;
			/** TODO: FIXME: need to add direction/door to sql */
			if(m_current_step->room == v) {
				this->advance();
				return;
			}
		}
	}
	/**
	 * @brief
	 *
	 * @param item_uuid
	 */
	void player_contract_instance::retrieve_item(const uuid_t& item_uuid) {
		if(!m_current_step) {
			return;
		}
		auto item = optr_by_uuid(item_uuid);
		if(!item) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_RETRIEVE &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(item->feed_file().data()) == 0) {
				this->advance();
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param item_uuid
	 */
	void player_contract_instance::quota_find_item(const uuid_t& item_uuid) {
		if(!m_current_step) {
			return;
		}
		auto item = optr_by_uuid(item_uuid);
		if(!item) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_QUOTA &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(item->feed_file().data()) == 0) {
				++m_quota;/** TODO FIXME INITIALIZE THIS */
				if(m_quota >= m_current_step->quota) {
					this->advance();
				}
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param mob_uuid
	 */
	void player_contract_instance::quota_kill_mob(const uuid_t& mob_uuid) {
		if(!m_current_step) {
			return;
		}
		auto mob = ptr_by_uuid(mob_uuid);
		if(!mob) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_QUOTA &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->vnum()) {
				++m_quota;/** TODO FIXME INITIALIZE THIS */
				if(m_quota >= m_current_step->quota) {
					this->advance();
				}
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param room_id
	 * @param direction
	 */
	void player_contract_instance::quota_destroy_door(const room_rnum& room_id,const int8_t& direction) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_DESTROY &&
		        m_current_step->goal & task_t::GOAL_QUOTA &&
		        m_current_step->target == target_t::TARGET_DOOR) {
			room_vnum v = world.size() > room_id ? world[room_id].number : 0;
			/** TODO: FIXME: need to add direction/door to sql */
			++m_quota;
			if(m_current_step->room == v && m_quota >= m_current_step->quota) {
				this->advance();
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param mob_uuid
	 */
	void player_contract_instance::kill(const uuid_t& mob_uuid) {
		if(!m_current_step) {
			return;
		}
		auto mob = ptr_by_uuid(mob_uuid);
		if(!mob) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_KILL &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->vnum()) {
				this->advance();
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param room_id
	 */
	void player_contract_instance::gain_entry(const room_rnum& room_id) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_GAIN_ENTRY &&
		        m_current_step->target == target_t::TARGET_ROOM) {
			room_vnum v = world.size() > room_id ? world[room_id].number : 0;
			/** TODO: FIXME: need to add direction/door to sql */
			if(m_current_step->room == v) {
				this->advance();
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param mob_uuid
	 */
	void player_contract_instance::talk_to(const uuid_t& mob_uuid) {
		if(!m_current_step) {
			return;
		}
		auto mob = ptr_by_uuid(mob_uuid);
		if(!mob) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_TALK_TO &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->vnum()) {
				this->advance();
				return;
			}
		}

	}
	/**
	 * @brief
	 *
	 * @param item_uuid
	 */
	void player_contract_instance::install_item(const uuid_t& item_uuid) {
		if(!m_current_step) {
			return;
		}
		auto item = optr_by_uuid(item_uuid);
		if(!item) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_INSTALL &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(item->feed_file().data()) == 0) {
				++m_quota;/** TODO FIXME INITIALIZE THIS */
				if(m_quota >= m_current_step->quota) {
					this->advance();
				}
				return;
			}
		}

	}

};
