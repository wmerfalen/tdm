#include "player.hpp"
#include "contract-defiler.hpp"
#include "scripted-sequence-events.hpp"
#include "players/db-load.hpp"
#include "levels.hpp"

#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contract_defiler[file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods::contract_defiler {
	std::map<uint64_t,uint8_t> player_human_remains_found;
	static constexpr uint8_t HUMAN_REMAINS_QUOTA = 4;

	bool human_remains_found(player_ptr_t player,uuid_t obj_uuid) {
		if(!player) {
			log("SYSERR: human_remains_found called but player ptr is invalid. Ignoring...");
			return true;
		}
		if(player_human_remains_found.find(player->db_id()) == player_human_remains_found.cend()) {
			player_human_remains_found[player->db_id()] = 0;
		}
		player_human_remains_found[player->db_id()] += 1;
		return player_human_remains_found[player->db_id()] >=  HUMAN_REMAINS_QUOTA;
	}
};//end namespace
