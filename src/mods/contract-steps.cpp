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
				player->carry(c);
				return;
			}
		}
		{
			auto s = util::extract_deep_reward(line.data());
			if(std::get<0>(s)) {
				auto& ref = std::get<2>(s);
				player->rifle_attachments().emplace_back();
				player->rifle_attachments().back()->import_objects(ref);
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
			player->exp() += reward_xp;
		}
		if(reward_money > 0) {
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
};

#undef m_debug
