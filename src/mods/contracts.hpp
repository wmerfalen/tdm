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

namespace mods {
	namespace contracts {
		/* informative functions */
		bool has_contract(player_ptr_t&);

		/* player actions for contracting */
		void punish_for_leaving_contract(std::shared_ptr<mods::player>& player,int contract_num);
		void load_c_functions(duk_context * ctx);
		void start_contract(player_ptr_t& player, contract_vnum_t contract_vnum);

	};
};
#endif
