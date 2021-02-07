#include "quests.hpp"
#include "pfind.hpp"
#define dbg_print(a) { /*std::cerr << a << "\n";*/ }
//#define MAX_QUEST_TRIGGERS 32
#define Q_FORMAT "quest:{rvnum}:{N}"
#define Q_CURRENT_KEY "{player_name}:quest"
#define Q_CURRENT_VALUE "{rvnum}:{N}"
#define Q_TRIGGER_KEY "{player_name}:quest_trigger"
//#define Q_PLAYER_TRIGGER_KEY "{player_name}:quest:{rvnum}:{N}:trigger:{T}"
//#define Q_PLAYER_TRIGGER_VALUE "{type}:{N}"
//#define Q_PLAYER_TRIGGER_INDEX_KEY "{player_name}:quest_trigger_index"
#define Q_COMPLETE_KEY "{player_name}:quest_complete"
#define Q_TRIGGER_CODE_KEY "quest_trigger_code:{rvnum}:{N}:{T}"
namespace mods {
	namespace quests {
		static duk_ret_t quest_abort(duk_context *ctx) {
			std::string pname = duk_to_string(ctx,0);
			std::string quest_major = duk_to_string(ctx,1);
			std::string quest_minor = duk_to_string(ctx,2);

			auto player = mods::pfind::by_name(pname.c_str());
			if(player == mods::globals::player_nobody) {
				return 0;
			}
			auto i_quest_major = mods::util::stoi(quest_major);
			if(i_quest_major.has_value()) {
				leave_quest(player,i_quest_major.value());
			}
			return 0;
		}
		static duk_ret_t quest_complete(duk_context *ctx) {
			std::string pname = duk_to_string(ctx,0);
			std::string quest_major = duk_to_string(ctx,1);
			std::string quest_minor = duk_to_string(ctx,2);

			auto player = mods::pfind::by_name(pname.c_str());
			if(player == mods::globals::player_nobody) {
				return 0;
			}
			auto i_quest_major = mods::util::stoi(quest_major);
			if(i_quest_major.has_value()) {
				leave_quest(player,i_quest_major.value());
				award_quest(player,i_quest_major.value());
			}
			return 0;
		}
		static duk_ret_t list_quests(duk_context *ctx) {
			std::string pname = duk_to_string(ctx,0);

			auto player = mods::pfind::by_name(pname.c_str());
			if(player == mods::globals::player_nobody) {
				return 0;
			}
			auto quests = list_quests(player->vnum());
			for(auto qname : quests) {
				dbg_print(qname);
				*player << "{grn}[ QUEST ]{/grn} " << qname << "\r\n";
			}
			return 0;
		}
		void load_c_functions(duk_context *ctx) {
			//
			duk_push_c_function(ctx,mods::quests::list_quests,1);
			duk_put_global_string(ctx,"list_quests");
			duk_push_c_function(ctx,mods::quests::quest_complete,3);
			duk_put_global_string(ctx,"quest_complete");
			duk_push_c_function(ctx,mods::quests::quest_abort,3);
			duk_put_global_string(ctx,"quest_abort");

		}

		void punish_for_leaving_contract(std::shared_ptr<mods::player>& player,int contract_num) {
			/** TODO: dock player mp */
		}
		/*
		 * quest:rvnum:N:name
		 * quest:rvnum:N:description
		 * <player.name()>.quest -> quest:rvnum:N
		 * <player.name()>.quest:rvnum:N:trigger:N -> javascript block
		 * <player.name()>.quest:rvnum:N:trigger:N -> javascript block
		 * <player.name()>.quest:rvnum:N:trigger:N -> javascript block
		 * <player.name()>.quest:rvnum:N:complete -> boolean
		 */
		std::vector<std::string> list_quests(room_vnum rvnum) {
			std::string value = "1";
			std::vector<std::string> quests;

			for(unsigned ctr = 0; true ; ctr++) {
				std::string key  = CAT("quest:",(rvnum),":",ctr,":name");
				value = "";
				mods::globals::db->get(key,value);

				if(value.length()) {
					quests.emplace_back(value);
				} else {
					break;
				}
			}

			return quests;
		}

		/* TODO: load triggers from disk on startup */

		/* Currently executing quest goes here */
		std::string current_key(player_ptr_t& player) {
			return mods::globals::replace_all(Q_CURRENT_KEY,"{player_name}",player->name().c_str());
		}

		std::string current_value(room_vnum room,int t_index) {
			dbg_print("cv: " << Q_CURRENT_VALUE);
			auto m = mods::globals::replace_all(Q_CURRENT_VALUE,"{rvnum}",std::to_string(room));
			m = mods::globals::replace_all(m,"{N}",std::to_string(t_index));
			dbg_print("cv return: " << m);
			return m;
		}

		/* Whether or not the quest has been completed */
		std::string complete_key(player_ptr_t& player,room_vnum room,int n_index) {
			auto m = mods::globals::replace_all(Q_COMPLETE_KEY,"{rvnum}",std::to_string(room));
			m = mods::globals::replace_all(m,"{player_name}",player->name().c_str());
			return mods::globals::replace_all(m,"{N}",std::to_string(n_index));
		}

		std::string trigger_key(player_ptr_t& player,room_vnum room,int n_index) {
			auto m = mods::globals::replace_all(Q_TRIGGER_KEY,"{rvnum}",std::to_string(room));
			m = mods::globals::replace_all(m,"{player_name}",player->name().c_str());
			return mods::globals::replace_all(m,"{N}",std::to_string(n_index));
		}

		std::string current_quest(player_ptr_t& player) {
			std::string current_quest_id = "";
			/** TODO: this needs to be a fast structure in RAM that we check.
			 * Since we've ripped out lmdb as a fast key/value pair system,
			 * our next available option will be the speedy RAM structures.
			 */
			DBGET(current_key(player),current_quest_id);
			dbg_print("current quest: " << current_quest_id);
			return current_quest_id;
		}

		bool has_quest(player_ptr_t& player) {
			return current_quest(player).length() > 0;
		}

		void load_quest_code(player_ptr_t& player) {
			/** The first goal in refactoring the quest code is to
			 * establish the various types of quests that will be
			 * available to builders.
			 */
		}

		int quests_file_to_lmdb(player_ptr_t& player,const std::string& quests_file,const std::string& lmdb_key) {
			std::ifstream include_file(quests_file,std::ios::in);

			if(!include_file.good() || !include_file.is_open()) {
				dbg_print("not opening quests file" << quests_file);
				return -1;
			} else {
				std::vector<char> buffer;
				struct stat statbuf;

				if(stat(quests_file.c_str(), &statbuf) == -1) {
					return -2;
				}

				buffer.reserve(statbuf.st_size + 1);
				std::fill(buffer.begin(),buffer.end(),0);
				include_file.read((char*)&buffer[0],statbuf.st_size);
				std::string buf = static_cast<char*>(&buffer[0]);
				/* start interpolation */
				buf = mods::globals::replace_all(buf,"{character}",player->name().c_str());
				DBSET(lmdb_key,buf);
				return statbuf.st_size;
			}
		}

		int run_trigger(player_ptr_t& player) {
			std::string js_code;
			//TODO: this has to run ALL quests code, not just zero
			DBGET(trigger_key(player,0,0),js_code);
			//std::cout << js_code << "\n";
			mods::js::eval_string(js_code);
			return 0;	//TODO: make use of return value to signify something?
		}

		bool trigger_exists(player_ptr_t& player,int quest_id) {
			std::string value;
			DBGET(current_quest(player) + std::string(":") + std::to_string(quest_id), value);
			return value.length() > 0;
		}

		void award_quest(player_ptr_t& player,int quest_id) {
			//TODO: Calculate quest reward tiers
			player->gold() += 50000;
			player->exp() += 500000;
		}
		void leave_quest(player_ptr_t& player,int quest_id) {
			std::string formatter = Q_FORMAT;
			auto key = mods::globals::replace_all(formatter,"{rvnum}",std::to_string(player->vnum()));
			key = mods::globals::replace_all(key,"{N}",std::to_string(quest_id));
			DBSET(key,"");
		}

		bool start_quest(player_ptr_t& player,int quest_id) {
			std::string formatter = Q_FORMAT;
			auto key = mods::globals::replace_all(formatter,"{rvnum}",std::to_string(player->vnum()));
			key = mods::globals::replace_all(key,"{N}",std::to_string(quest_id));
			dbg_print("key : " << key);

			std::string name = key + ":name";
			std::string desc = key + ":desc";
			std::string value= "";
			mods::globals::db->get(name,value);

			//std::cout << name << "->" << value << "\n";
			if(value.length()) {
				/* Set the current quest number to this room id and quest id */
				DBSET(current_key(player),current_value(player->vnum(),quest_id));
				/* set the current quest to incomplete ("0") */
				DBSET(complete_key(player,player->vnum(),quest_id),"0");
				load_quest_code(player);
				run_trigger(player);
				return true;
			} else {
				return false;
			}
		}
	};
};
