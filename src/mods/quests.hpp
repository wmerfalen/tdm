#ifndef __MENTOC_MODS_QUESTS_HEADER__
#define __MENTOC_MODS_QUESTS_HEADER__
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

namespace mods {
	namespace quests {
		/*
		 * Quest record
		 * [room_vnum][quest_name][level_req][exp_gained]
		 *
		 * quest step record
		 * [room_vnum][action_taken][lambda]
		 */
		/* quests list ->
		 * 	-> lookup room id
		 *		-> get number of quests
		 *		->
		 *		-> db->get("quest:room_id:N:name"); //Value is the quest name
		 * 	-> db->get("quest:room_id:N:desc"); //Value is the quest description as a blob
		 *		-> db->put("char_data->player.name:quest","room_id:N");	//Set quest to room id
		 *		->
		 */

		/* informative functions */
		bool has_quest(player_ptr_t&);
		void award_quest(player_ptr_t& ch,int quest_id);

		/* player actions for questing */
		bool start_quest(player_ptr_t& ch,int quest_num);
		void leave_quest(player_ptr_t& ch,int quest_num);
		void punish_for_leaving_contract(std::shared_ptr<mods::player>& player,int contract_num);
		std::vector<std::string> list_quests(room_vnum room_id);
		void load_c_functions(duk_context * ctx);
		int quests_file_to_lmdb(player_ptr_t& ch,const std::string& quests_file,const std::string& lmdb_key);

		/* lmdb keys for quest tracking */
		std::string current_key(player_ptr_t& ch);
		std::string current_value(room_vnum room,int t_index);
		std::string complete_key(player_ptr_t& ch,room_vnum room,int n_index);
		std::string trigger_key(player_ptr_t& ch,room_vnum room,int n_index);

		/* Code storage */
		std::string trigger_code_key(room_vnum room,int n_index,int t_index);
		void load_quest_code(player_ptr_t& ch,room_vnum room,int quest_id);

		/* helpers */
		bool trigger_exists(player_ptr_t& player,int quest_id);
		std::string current_quest(player_ptr_t& player);

		/* trigger running */
		int run_trigger(player_ptr_t& player);

		/** chainable quests */
		/** The idea:
		 * ----------
		 *  One quest leads to subsequent quests. The ultimate
		 *  end-goal is to get a high enough level of cohesion
		 *  with the quest giver so that they may reward the player
		 *  with extra perks
		 */
	};
};
#endif
