#include "contract-types.hpp"
#include "orm/contracts.hpp"
#include "orm/player-contract-state.hpp"
#include "player.hpp"
#include "player-contract-instance.hpp"
#include "scripted-sequence-events.hpp"
#include "players/db-load.hpp"

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
		//m_state_orm->delete_by_player_id_contract_vnum(m_player_id,m_contract_vnum);
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
		m_step = 0;
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
		m_step = 0;
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
		m_extra_data.clear();
		//std::string current = "";
		//bool step_extracted = 0;
		//m_extra_data.clear();
		//for(auto ch : m_state_buffer) {
		//	if(ch == '|' && !step_extracted) {
		//		m_step = mods::util::stoi(current).value_or(0);
		//		current.clear();
		//		step_extracted = 1;
		//		continue;
		//	}
		//	if(step_extracted) {
		//		m_extra_data += ch;
		//		continue;
		//	}
		//	current += ch;
		//}
	}
	/**
	 * @brief
	 */
	void player_contract_instance::save_step_data() {
		//m_update_status = m_state_orm->update_player_data(m_player_id,m_contract_vnum, encode_step_data(CAT(m_step,"|",m_extra_data)));
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
		auto player = ptr_by_db_id(m_player_id);
		if(!player) {
			log("SYSERR: couldn't get player pointer from player id: %d",m_player_id);
			return m_update_status;
		}
		mods::scripted_sequence_events::player_finished_step(player,m_contract_vnum,m_current_step);
		std::string reward_string;

		if(m_current_step) {
			if(m_current_step->reward_xp > 0) {
				reward_string = CAT("You gain ",m_current_step->reward_xp," XP\r\n");
				player->exp() += m_current_step->reward_xp;
			}
			if(m_current_step->reward_money > 0) {
				reward_string += CAT("You gain ",m_current_step->reward_money," MP\r\n");
				player->gold() += m_current_step->reward_money;
			}
			for(auto& line : {
			            m_current_step->reward_1,
			            m_current_step->reward_2,
			            m_current_step->reward_3,
			            m_current_step->reward_4,
			            m_current_step->reward_5,
			            m_current_step->reward_6,
			            m_current_step->reward_7,
			            m_current_step->reward_8,
			            m_current_step->reward_9,
			            m_current_step->reward_10
			        }) {
				if(line.length()) {
					{
						auto s = util::extract_yaml_reward(line.data());
						if(std::get<0>(s)) {
							auto c = create_object(std::get<1>(s),std::get<2>(s));
							reward_string += CAT("You are rewarded with the following item:\r\n",
							                     c->name.c_str(),"\r\n"
							                    );
							player->carry(c);
						}
					}
					{
						auto s = util::extract_deep_reward(line.data());
						if(std::get<0>(s)) {
							auto& ref = std::get<2>(s);
							auto ptr = mods::rifle_attachments::make(ref);
							reward_string += CAT(
							                     "You are rewarded with the following item:{/grn}\r\n",
							                     ptr->base_object->name.c_str(),"\r\n"
							                 );
							player->carry(ptr->base_object);
						}
					}
				}
			}
			mods::players::db_load::save_from(player,mods::players::db_load::save_from_t::CONTRACT_REWARD);
		}
		m_quota = 0;
		m_step += 1;
		save_step_data();
		m_auto_update_step();

		if(!finished()) {
			std::string newline;
			player->sendln(
			    mods::util::mail_format(
			        "Congratulations, soldier!",
			        "You completed part of your contract. Here are your next instructions:",
			        CAT(m_current_step->description,"\r\n",reward_string),
			        player->screen_width()
			    )
			);
		} else {
			player->sendln(
			    mods::util::mail_format(
			        "Contract COMPLETE!",
			        "Congratulations, soldier!",
			        CAT("You just finished your contract successfully!\r\n",reward_string),
			        player->screen_width()
			    )
			);
		}
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
			auto player = ptr_by_db_id(m_player_id);
			if(player) {
				player->sendln("No contract copy found");
			}
			log("SYSERR: no contract copy found for contract %d",in_vnum);
			return {0,"No contract loaded with that vnum"};
		}
		///** Grab player_contract_state where pc_player_id=P,pc_contract_vnum=C */
		//auto res = mods::orm::load_player_contract_state(m_player_id,in_vnum,m_state_buffer);
		//if(std::get<0>(res) == 0) {
		/** No results. initialize contract */
		//m_state_orm->pc_player_id = m_player_id;
		//m_state_orm->pc_contract_vnum = in_vnum;
		//m_state_buffer = m_state_orm->pc_state_data = encode_step_data("0|{}");
		m_step = 0;
		//m_state_orm->save();
		//}
		//load_decoded_step_data();
		m_auto_update_step();
		return {1,"Contract started"};
	}
	/**
	 * @brief
	 *
	 * @param item_uuid
	 */
	void player_contract_instance::find_item(obj_ptr_t& object) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_FIND &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(object->feed_file().data()) == 0) {
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
	void player_contract_instance::find_mob(player_ptr_t& mob) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_FIND &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->cd()->mob_specials.vnum) {
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
		if(is_give_item()) {
			pretty += "give_item";
		}
		pretty += CAT("{/grn}\r\n\t\t{yel}[description]:{/yel}{grn}",m_current_step->description,"{/grn}\r\n");
		pretty += CAT(
		              "\t\t{yel}[reward_xp]:{/yel}{grn}",m_current_step->reward_xp,"{/grn}\r\n",
		              "\t\t{yel}[reward_money]:{/yel}{grn}",m_current_step->reward_money,"{/grn}\r\n",
		              "\t\t{yel}[target]:{/yel}{grn}",m_current_step->target,"{/grn}\r\n",
		              "\t\t{yel}[mob_vnum]:{/yel}{grn}",m_current_step->mob_vnum,"{/grn}\r\n",
		              "\t\t{yel}[mob_uuid]:{/yel}{grn}",m_current_step->mob_uuid,"{/grn}\r\n",
		              "\t\t{yel}[object_yaml]:{/yel}{grn}",m_current_step->object_yaml,"{/grn}\r\n",
		              "\t\t{yel}[object_uuid]:{/yel}{grn}",m_current_step->object_uuid,"{/grn}\r\n",
		              "\t\t{yel}[room]:{/yel}{grn}",m_current_step->room,"{/grn}\r\n",
		              "\t\t{yel}[quota]:{/yel}{grn}",m_current_step->quota,"{/grn}\r\n",
		              "\t\t{yel}[is_optional]:{/yel}{grn}",m_current_step->is_optional,"{/grn}\r\n",
		              "\t\t{yel}[reward_1]:{/yel}{grn}",m_current_step->reward_1,"{/grn}\r\n",
		              "\t\t{yel}[reward_2]:{/yel}{grn}",m_current_step->reward_2,"{/grn}\r\n",
		              "\t\t{yel}[reward_3]:{/yel}{grn}",m_current_step->reward_3,"{/grn}\r\n",
		              "\t\t{yel}[reward_4]:{/yel}{grn}",m_current_step->reward_4,"{/grn}\r\n",
		              "\t\t{yel}[reward_5]:{/yel}{grn}",m_current_step->reward_5,"{/grn}\r\n",
		              "\t\t{yel}[reward_6]:{/yel}{grn}",m_current_step->reward_6,"{/grn}\r\n",
		              "\t\t{yel}[reward_7]:{/yel}{grn}",m_current_step->reward_7,"{/grn}\r\n",
		              "\t\t{yel}[reward_8]:{/yel}{grn}",m_current_step->reward_8,"{/grn}\r\n",
		              "\t\t{yel}[reward_9]:{/yel}{grn}",m_current_step->reward_9,"{/grn}\r\n",
		              "\t\t{yel}[reward_10]:{/yel}{grn}",m_current_step->reward_10,"{/grn}\r\n"
		          );
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
		if(is_give_item()) {
			m_step_dump += "give_item";
		}
		m_step_dump += CAT("\r\n[description]:{yel}",m_current_step->description,"{/yel}\r\n");
		m_step_dump += CAT(
		                   "\t\t{yel}[reward_xp]:{/yel}{grn}",m_current_step->reward_xp,"{/grn}\r\n",
		                   "\t\t{yel}[reward_money]:{/yel}{grn}",m_current_step->reward_money,"{/grn}\r\n",
		                   "\t\t{yel}[target]:{/yel}{grn}",m_current_step->target,"{/grn}\r\n",
		                   "\t\t{yel}[mob_vnum]:{/yel}{grn}",m_current_step->mob_vnum,"{/grn}\r\n",
		                   "\t\t{yel}[mob_uuid]:{/yel}{grn}",m_current_step->mob_uuid,"{/grn}\r\n",
		                   "\t\t{yel}[object_yaml]:{/yel}{grn}",m_current_step->object_yaml,"{/grn}\r\n",
		                   "\t\t{yel}[object_uuid]:{/yel}{grn}",m_current_step->object_uuid,"{/grn}\r\n",
		                   "\t\t{yel}[room]:{/yel}{grn}",m_current_step->room,"{/grn}\r\n",
		                   "\t\t{yel}[quota]:{/yel}{grn}",m_current_step->quota,"{/grn}\r\n",
		                   "\t\t{yel}[is_optional]:{/yel}{grn}",m_current_step->is_optional,"{/grn}\r\n",
		                   "\t\t{yel}[reward_1]:{/yel}{grn}",m_current_step->reward_1,"{/grn}\r\n",
		                   "\t\t{yel}[reward_2]:{/yel}{grn}",m_current_step->reward_2,"{/grn}\r\n",
		                   "\t\t{yel}[reward_3]:{/yel}{grn}",m_current_step->reward_3,"{/grn}\r\n",
		                   "\t\t{yel}[reward_4]:{/yel}{grn}",m_current_step->reward_4,"{/grn}\r\n",
		                   "\t\t{yel}[reward_5]:{/yel}{grn}",m_current_step->reward_5,"{/grn}\r\n",
		                   "\t\t{yel}[reward_6]:{/yel}{grn}",m_current_step->reward_6,"{/grn}\r\n",
		                   "\t\t{yel}[reward_7]:{/yel}{grn}",m_current_step->reward_7,"{/grn}\r\n",
		                   "\t\t{yel}[reward_8]:{/yel}{grn}",m_current_step->reward_8,"{/grn}\r\n",
		                   "\t\t{yel}[reward_9]:{/yel}{grn}",m_current_step->reward_9,"{/grn}\r\n",
		                   "\t\t{yel}[reward_10]:{/yel}{grn}",m_current_step->reward_10,"{/grn}\r\n"
		               );
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
	void player_contract_instance::destroy_item(obj_ptr_t& object) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_DESTROY &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(object->feed_file().data()) == 0) {
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
	void player_contract_instance::retrieve_item(obj_ptr_t& object) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_RETRIEVE &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(object->feed_file().data()) == 0) {
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
	void player_contract_instance::quota_find_item(obj_ptr_t& object) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_QUOTA &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			if(m_current_step->object_yaml.compare(object->feed_file().data()) == 0) {
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
	void player_contract_instance::quota_kill_mob(player_ptr_t& mob) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_QUOTA &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->cd()->mob_specials.vnum) {
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
	void player_contract_instance::kill(player_ptr_t& mob) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_KILL &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->cd()->mob_specials.vnum) {
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
	void player_contract_instance::talk_to(player_ptr_t& mob) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_TALK_TO &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->mob_vnum == mob->cd()->mob_specials.vnum) {
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
	void player_contract_instance::install_item(const uuid_t& obj_uuid) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_INSTALL &&
		        m_current_step->target == target_t::TARGET_ITEM) {
			auto object = optr_by_uuid(obj_uuid);
			if(!object) {
				return;
			}
			if(m_current_step->object_yaml.compare(object->feed_file().data()) == 0) {
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
	 * @param item_uuid
	 */
	void player_contract_instance::give_item(obj_ptr_t& object,player_ptr_t& mob) {
		if(!m_current_step) {
			return;
		}
		if(m_current_step->goal & task_t::GOAL_GIVE &&
		        m_current_step->target == target_t::TARGET_MOB) {
			if(m_current_step->object_yaml.compare(object->feed_file().data()) == 0) {
				++m_quota;
				if(m_quota >= m_current_step->quota) {
					this->advance();
				}
				return;
			}
		}

	}

};
#undef dbg_print
