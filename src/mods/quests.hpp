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
#include "../mods/utils.hpp"
#include "js.hpp"

namespace mods {
	namespace quests {
		/* 
		 * Quest record
		 * [room_rnum][quest_name][level_req][exp_gained]
		 *
		 * quest step record
		 * [room_rnum][action_taken][lambda]
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
		bool has_quest(struct char_data *ch);

		/* player actions for questing */
		void start_quest(struct char_data* ch,int quest_num);
		void leave_quest(struct char_data* ch,int quest_num);
		std::vector<std::string> list_quests(room_rnum room_id);
		void load_c_functions(duk_context * ctx);
		int quests_file_to_lmdb(struct char_data* ch,const std::string & quests_file,const std::string & lmdb_key);
		
		/* lmdb keys for quest tracking */
		std::string current_key(struct char_data* ch);
        std::string current_value(room_rnum room,int t_index);
		std::string complete_key(struct char_data *ch,room_rnum room,int n_index);
		std::string trigger_key(struct char_data *ch,room_rnum room,int n_index);

		/* Code storage */
		std::string trigger_code_key(room_rnum room,int n_index,int t_index);
		void load_quest_code(struct char_data* ch,room_rnum room,int quest_id);

		/* helpers */
		bool trigger_exists(struct char_data *ch,int quest_id);
		std::string current_quest(struct char_data *ch);

		/* trigger running */
		int run_trigger(struct char_data *ch);
	};
};
#endif
