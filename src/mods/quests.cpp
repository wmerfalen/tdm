#include "quests.hpp"
#define dbg_print(a) { /*std::cerr << a << "\n";*/ }
//#define MAX_QUEST_TRIGGERS 32
#define Q_FORMAT "quest:{room_id}:{N}"
#define Q_CURRENT_KEY "{player_name}:quest"
#define Q_CURRENT_VALUE "{room_id}:{N}"
#define Q_TRIGGER_KEY "{player_name}:quest_trigger"
//#define Q_PLAYER_TRIGGER_KEY "{player_name}:quest:{room_id}:{N}:trigger:{T}"
//#define Q_PLAYER_TRIGGER_VALUE "{type}:{N}"
//#define Q_PLAYER_TRIGGER_INDEX_KEY "{player_name}:quest_trigger_index"
#define Q_COMPLETE_KEY "{player_name}:quest_complete"
#define Q_TRIGGER_CODE_KEY "quest_trigger_code:{room_id}:{N}:{T}"
extern struct char_data* character_list;
namespace mods {
	namespace quests {
		static duk_ret_t quest_abort(duk_context *ctx) {
			std::string pname = duk_to_string(ctx,0);
			std::string quest_major = duk_to_string(ctx,1);
			std::string quest_minor = duk_to_string(ctx,2);

			for(auto ch = character_list; ch->next; ch = ch->next) {
				MENTOC_PREAMBLE();

				if(pname.compare(ch->player.name) == 0) {
					auto i_quest_major = mods::util::stoi(quest_major);

					if(i_quest_major.has_value()) {
						leave_quest(ch,i_quest_major.value());
					}

					return 0;
				}
			}

			return 0;
		}
		static duk_ret_t quest_complete(duk_context *ctx) {
			std::string pname = duk_to_string(ctx,0);
			std::string quest_major = duk_to_string(ctx,1);
			std::string quest_minor = duk_to_string(ctx,2);

			for(auto ch = character_list; ch->next; ch = ch->next) {
				MENTOC_PREAMBLE();

				if(pname.compare(ch->player.name) == 0) {
					auto i_quest_major = mods::util::stoi(quest_major);

					if(i_quest_major.has_value()) {
						leave_quest(ch,i_quest_major.value());
						award_quest(ch,i_quest_major.value());
					}

					return 0;
				}
			}

			return 0;
		}
		static duk_ret_t list_quests(duk_context *ctx) {
			std::string pname = duk_to_string(ctx,0);

			for(auto ch = character_list; ch->next; ch = ch->next) {
				MENTOC_PREAMBLE();

				if(pname.compare(ch->player.name) == 0) {
					auto quests = list_quests(IN_ROOM(ch));

					for(auto qname : quests) {
						dbg_print(qname);
						*player << "{grn}[ QUEST ]{/grn} " << qname << "\r\n";
					}

					return 0;
				}
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

		/*
		 * quest:room_id:N:name
		 * quest:room_id:N:description
		 * <ch->player.name>.quest -> quest:room_id:N
		 * <ch->player.name>.quest:room_id:N:trigger:N -> javascript block
		 * <ch->player.name>.quest:room_id:N:trigger:N -> javascript block
		 * <ch->player.name>.quest:room_id:N:trigger:N -> javascript block
		 * <ch->player.name>.quest:room_id:N:complete -> boolean
		 */
		std::vector<std::string> list_quests(room_rnum room_id) {
			std::string value = "1";
			std::vector<std::string> quests;

			for(unsigned ctr = 0; true ; ctr++) {
				std::string key  = "quest:";
				key += std::to_string(room_id);
				key += ":";
				key += std::to_string(ctr);
				key += ":name";
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
		std::string current_key(struct char_data* ch) {
			return mods::globals::replace_all(Q_CURRENT_KEY,"{player_name}",std::string(ch->player.name));
		}

		std::string current_value(room_rnum room,int t_index) {
			dbg_print("cv: " << Q_CURRENT_VALUE);
			auto m = mods::globals::replace_all(Q_CURRENT_VALUE,"{room_id}",std::to_string(room));
			m = mods::globals::replace_all(m,"{N}",std::to_string(t_index));
			dbg_print("cv return: " << m);
			return m;
		}

		/* Whether or not the quest has been completed */
		std::string complete_key(struct char_data *ch,room_rnum room,int n_index) {
			auto m = mods::globals::replace_all(Q_COMPLETE_KEY,"{room_id}",std::to_string(room));
			m = mods::globals::replace_all(m,"{player_name}",ch->player.name.c_str());
			return mods::globals::replace_all(m,"{N}",std::to_string(n_index));
		}

		std::string trigger_key(struct char_data *ch,room_rnum room,int n_index) {
			auto m = mods::globals::replace_all(Q_TRIGGER_KEY,"{room_id}",std::to_string(room));
			m = mods::globals::replace_all(m,"{player_name}",ch->player.name.c_str());
			return mods::globals::replace_all(m,"{N}",std::to_string(n_index));
		}

//		std::string trigger_index_key(struct char_data *ch){
//			return mods::globals::replace_all(Q_PLAYER_TRIGGER_INDEX_KEY,"{player_name}",ch->player.name);
//		}
//
//		std::string trigger_value(const std::string & type, const std::string & value){
//			auto m = mods::globals::replace_all(Q_PLAYER_TRIGGER_VALUE,"{type}",type);
//			return mods::globals::replace_all(m,"{N}",value);
//		}
//
//		std::string trigger_code_key(room_rnum room,int n_index,int t_index){
//			auto m = mods::globals::replace_all(Q_TRIGGER_CODE_KEY,"{room_id}",std::to_string(room));
//			m = mods::globals::replace_all(m,"{N}",std::to_string(n_index));
//			return mods::globals::replace_all(m,"{T}",std::to_string(t_index));
//		}
//
//		std::string trigger_code_path(room_rnum room,int n_index,int t_index){
//			auto m = mods::globals::replace_all("../../lib/quests/{room_id}:{N}:{T}.js","{room_id}",std::to_string(room));
//			m = mods::globals::replace_all(m,"{N}",std::to_string(n_index));
//			return mods::globals::replace_all(m,"{T}",std::to_string(t_index));
//		}
//
		std::string current_quest(struct char_data *ch) {
			std::string current_quest_id = "";
			DBGET(current_key(ch),current_quest_id);
			dbg_print("current quest: " << current_quest_id);
			return current_quest_id;
		}

		bool has_quest(struct char_data * ch) {
			return current_quest(ch).length() > 0;
		}

//		void load_quest_code(struct char_data* ch,room_rnum room,int quest_id){
//			/* load code from file into Q_TRIGGER_CODE_KEY if not already loaded */
//			std::string value;
//			std::string code_key;
//
//			if(value.length() == 0){
//				/* open up the quest file at pwd/../../lib/quests/{room_id}:{N}:{T}.js */
//				int ctr=0;
//				while(ctr < MAX_QUEST_TRIGGERS){
//					/* If a quest trigger file doesn't exist, it's not the end of the world. But if there were
//					 * zero trigger files loaded then that should atleast be reported
//					 */
//					if(mods::globals::file_to_lmdb(trigger_code_path,trigger_code_key(room,quest_id,ctr)) < 0){
//						break;
//					}else{
//						/* set Q_PLAYER_TRIGGER_KEY to key:{Q_TRIGGER_CODE_KEY} */
//						DBSET(trigger_key(ch,room,quest_id),std::string("key:")+trigger_code_key(room,quest_id,ctr));
//					}
//					ctr++;
//				}
//				if(ctr == 0){
//					//TODO: report error (no mud triggers loaded)
//				}
//			}
//			/* set Q_PLAY_TRIGGER_INDEX_KEY to zero */
//			DBSET(trigger_index_key(ch),"0");
//			/* run first trigger */
//		}

		void load_quest_code(struct char_data* ch) {
			char temp[MAXPATHLEN];
			std::string pwd = getcwd(temp, MAXPATHLEN) ? std::string(temp) : std::string("");
			std::string quest_file = pwd + std::string("/quests/") + current_quest(ch) + ".js";
			//std::cout << quest_file << "\n";
			/* Only one quest per room */
			quests_file_to_lmdb(ch,quest_file,trigger_key(ch,IN_ROOM(ch),0));
		}

		int quests_file_to_lmdb(struct char_data* ch,const std::string& quests_file,const std::string& lmdb_key) {
			std::ifstream include_file(quests_file,std::ios::in);

			if(!include_file.is_open()) {
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
				buf = mods::globals::replace_all(buf,"{character}",ch->player.name.c_str());
				DBSET(lmdb_key,buf);
				return statbuf.st_size;
			}
		}

		int run_trigger(struct char_data* ch) {
			std::string js_code;
			//TODO: this has to run ALL quests code, not just zero
			DBGET(trigger_key(ch,0,0),js_code);
			//std::cout << js_code << "\n";
			mods::js::eval_string(js_code);
			return 0;	//TODO: make use of return value to signify something?
		}

		bool trigger_exists(struct char_data *ch,int quest_id) {
			std::string value;
			DBGET(current_quest(ch) + std::string(":") + std::to_string(quest_id), value);
			return value.length() > 0;
		}

		void award_quest(struct char_data* ch,int quest_id) {
			MENTOC_PREAMBLE();
			//TODO: Calculate quest reward tiers
			ch->points.gold += 50000;
			ch->points.exp += 500000;
		}
		void leave_quest(struct char_data* ch,int quest_id) {
			MENTOC_PREAMBLE();
			std::string formatter = Q_FORMAT;
			auto key = mods::globals::replace_all(formatter,"{room_id}",std::to_string(IN_ROOM(ch)));
			key = mods::globals::replace_all(key,"{N}",std::to_string(quest_id));
			DBSET(key,"");
		}

		void start_quest(struct char_data* ch,int quest_id) {
			MENTOC_PREAMBLE();
			std::string formatter = Q_FORMAT;
			auto key = mods::globals::replace_all(formatter,"{room_id}",std::to_string(IN_ROOM(ch)));
			key = mods::globals::replace_all(key,"{N}",std::to_string(quest_id));
			dbg_print("key : " << key);

			std::string name = key + ":name";
			std::string desc = key + ":desc";
			std::string value= "";
			mods::globals::db->get(name,value);

			//std::cout << name << "->" << value << "\n";
			if(value.length()) {
				*player << "{red}Starting quest: {/red}" << value.c_str() << "\r\n";
				/* Set the current quest number to this room id and quest id */
				DBSET(current_key(ch),current_value(IN_ROOM(ch),quest_id));
				/* set the current quest to incomplete ("0") */
				DBSET(complete_key(ch,IN_ROOM(ch),quest_id),"0");
				load_quest_code(ch);
				run_trigger(ch);
			} else {
				*player << "Cannot find that quest\r\n";
			}
		}
	};
};
