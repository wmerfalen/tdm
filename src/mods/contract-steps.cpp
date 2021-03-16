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
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define m_debug(a) std::cerr << "[mods::contract_steps][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::contracts {
	void contract_step::reward(const uint64_t& player_db_id) {
		m_debug("rewarding player id: " << green_str(std::to_string(player_db_id)));
		auto ptr = ptr_by_db_id(player_db_id);
		if(!ptr) {
			log("Unable to find player by db id for the purposes of rewarding player for contract");
			return;
		}
		if(reward_1.length()) {

		}
	}
};

#undef m_debug
