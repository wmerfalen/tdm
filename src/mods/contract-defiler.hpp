#ifndef __MENTOC_MODS_CONTRACT_DEFILER_HEADER__
#define __MENTOC_MODS_CONTRACT_DEFILER_HEADER__
#include "contract-types.hpp"
#include "players/messages.hpp"
namespace mods::contract_defiler {
	using task_t = mods::contracts::contract_step::task_type_t;
	using target_t = mods::contracts::contract_step::task_target_t;

	/**
	 * @brief returns true if contract can advance. false otherwise
	 */
	bool human_remains_found(player_ptr_t player,uuid_t obj_uuid);

};//end namespace contract_defiler

#endif
