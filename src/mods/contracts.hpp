#ifndef __MENTOC_MODS_CONTRACTS_HEADER__
#define __MENTOC_MODS_CONTRACTS_HEADER__
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "../mods/util.hpp"
#include "js.hpp"
#include "contract-types.hpp"

namespace mods::contracts {
	/* informative functions */
	bool has_contract(player_ptr_t&);
	void start_contract(player_ptr_t& player, contract_vnum_t contract_vnum);
	std::deque<std::shared_ptr<contract>>& contract_master_list();
	void load_all_contracts();
	void clear_all_contracts();
};
#endif
