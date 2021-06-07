#include "contract-steps.hpp"
#include "interpreter.hpp"
#include "pfind.hpp"
#include "orm/contract-steps.hpp"
#include "builder/encode.hpp"
#include "orm/contracts.hpp"
#include "orm/player-contract-state.hpp"
#include "player-contract-instance.hpp"
#include "../globals.hpp"
#include "contract-steps.hpp"
#include "players/db-load.hpp"
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define m_debug(a) std::cerr << "[mods::contract_steps][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::contracts {
	void give_player_rewards(player_ptr_t& player,std::string_view line) {
		{
			auto s = util::extract_yaml_reward(line.data());
			if(std::get<0>(s)) {
				auto c = create_object(std::get<1>(s),std::get<2>(s));
				player->sendln(CAT(
				                   "{grn}*************************************{/grn}\r\n",
				                   "{yel} You are rewarded with the following item:{/grn}\r\n",
				                   "{grn}",c->name.c_str(),"{/grn}\r\n",
				                   "{grn}*************************************{/grn}\r\n"
				               ));
				player->carry(c);
				return;
			}
		}
		{
			auto s = util::extract_deep_reward(line.data());
			if(std::get<0>(s)) {
				auto& ref = std::get<2>(s);
				auto ptr = mods::rifle_attachments::make(ref);
				player->sendln(CAT(
				                   "{grn}*************************************{/grn}\r\n",
				                   "{yel} You are rewarded with the following item:{/grn}\r\n",
				                   "{grn}",ptr->base_object->name.c_str(),"{/grn}\r\n",
				                   "{grn}*************************************{/grn}\r\n"
				               ));
				player->carry(ptr->base_object);
			}
		}
	}
	void contract_step::reward(const uint64_t& player_db_id) {
		m_debug("rewarding player id: " << green_str(std::to_string(player_db_id)));
		auto player = ptr_by_db_id(player_db_id);
		if(!player) {
			log("Unable to find player by db id for the purposes of rewarding player for contract");
			return;
		}
		if(reward_xp > 0) {
			player->sendln(CAT(
			                   "{grn}*************************************{/grn}\r\n",
			                   "{yel} You are rewarded {grn}",reward_xp," XP{/grn}\r\n",
			                   "{grn}*************************************{/grn}\r\n"
			               ));
			player->exp() += reward_xp;
		}
		if(reward_money > 0) {
			player->sendln(CAT(
			                   "{grn}*************************************{/grn}\r\n",
			                   "{yel} You are rewarded {grn}",reward_money," MP{/grn}\r\n",
			                   "{grn}*************************************{/grn}\r\n"
			               ));
			player->gold() += reward_money;
		}
		for(auto& line : {
		            reward_1,
		            reward_2,
		            reward_3,
		            reward_4,
		            reward_5,
		            reward_6,
		            reward_7,
		            reward_8,
		            reward_9,
		            reward_10
		        }) {
			if(line.length()) {
				give_player_rewards(player,line);
			}
		}

		mods::players::db_load::save_from(player,mods::players::db_load::save_from_t::CONTRACT_REWARD);
	}
	std::map<std::string,task_t> task_string_map() {
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
			{"GOAL_GIVE",task_t::GOAL_GIVE},
		};
	}
	std::map<std::string,target_t> target_string_map() {
		return {
			{"TARGET_MOB",target_t::TARGET_MOB},
			{"TARGET_ITEM",target_t::TARGET_ITEM},
			{"TARGET_ROOM",target_t::TARGET_ROOM},
			{"TARGET_DOOR",target_t::TARGET_DOOR},
		};
	}
	std::map<task_t,std::string> inverse_task_string_map() {
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
			{task_t::GOAL_GIVE,"GOAL_GIVE"},
		};
	}
	std::map<target_t,std::string> inverse_target_string_map() {
		return {
			{target_t::TARGET_MOB,"TARGET_MOB"},
			{target_t::TARGET_ITEM,"TARGET_ITEM"},
			{target_t::TARGET_ROOM,"TARGET_ROOM"},
		};
	}
	std::vector<std::string> get_string_list_from_task_flags(task_t task) {
		std::vector<std::string> list;
		for(const auto& pair : inverse_task_string_map()) {
			if(task & pair.first) {
				list.emplace_back(pair.second);
			}
		}
		return list;
	}
	std::string get_string_from_target(target_t target) {
		for(const auto& pair : target_string_map()) {
			if(pair.second == target) {
				return pair.first;
			}
		}
		return "<unknown>";
	}
	target_t parse_target_string(std::string_view t) {
		for(const auto& pair : target_string_map()) {
			if(pair.first.compare(t.data()) == 0) {
				return pair.second;
			}
		}
		return (target_t)0;
	}
	task_t parse_csv_to_task_flag(std::string_view csv) {
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
};

#undef m_debug
