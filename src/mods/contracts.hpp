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
		/*
		 * Quest record
		 * [room_vnum][contract_name][level_req][exp_gained]
		 *
		 * contract step record
		 * [room_vnum][action_taken][lambda]
		 */
		/* contracts list ->
		 * 	-> lookup room id
		 *		-> get number of contracts
		 *		->
		 *		-> db->get("contract:room_id:N:name"); //Value is the contract name
		 * 	-> db->get("contract:room_id:N:desc"); //Value is the contract description as a blob
		 *		-> db->put("char_data->player.name:contract","room_id:N");	//Set contract to room id
		 *		->
		 */

		/* informative functions */
		bool has_contract(player_ptr_t&);
		void award_contract(player_ptr_t& ch,int contract_id);

		/* player actions for contracting */
		bool start_contract(player_ptr_t& ch,int contract_num);
		void leave_contract(player_ptr_t& ch,int contract_num);
		void punish_for_leaving_contract(std::shared_ptr<mods::player>& player,int contract_num);
		std::vector<std::string> list_contracts(room_vnum room_id);
		void load_c_functions(duk_context * ctx);
		int contracts_file_to_lmdb(player_ptr_t& ch,const std::string& contracts_file,const std::string& lmdb_key);

		/* lmdb keys for contract tracking */
		std::string current_key(player_ptr_t& ch);
		std::string current_value(room_vnum room,int t_index);
		std::string complete_key(player_ptr_t& ch,room_vnum room,int n_index);
		std::string trigger_key(player_ptr_t& ch,room_vnum room,int n_index);

		/* Code storage */
		std::string trigger_code_key(room_vnum room,int n_index,int t_index);
		void load_contract_code(player_ptr_t& ch,room_vnum room,int contract_id);

		/* helpers */
		bool trigger_exists(player_ptr_t& player,int contract_id);
		std::string current_contract(player_ptr_t& player);

		/* trigger running */
		int run_trigger(player_ptr_t& player);

		/** chainable contracts */
		/** The idea:
		 * ----------
		 *  One contract leads to subsequent contracts. The ultimate
		 *  end-goal is to get a high enough level of cohesion
		 *  with the contract giver so that they may reward the player
		 *  with extra perks
		 */

	};
};
#endif
