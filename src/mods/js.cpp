#include "js.hpp"
#include "quests.hpp"
#include "util.hpp"
#include "extern.hpp"
#define DT_FORMAT "{player_name}:mob_death_trigger"
extern void command_interpreter(struct char_data* ch,char* argument);
extern void hit(struct char_data* ch,struct char_data* vict,int type);
namespace mods {
	namespace js {
		int load_library(duk_context*,std::string_view);
		constexpr static const char * JS_PATH = "../lib/js/";
		constexpr static const char * JS_TEST_PATH = "../lib/js/tests/";
		constexpr static const char * JS_PROFILES_PATH = "../lib/js/profiles/";
		namespace test {
			static duk_ret_t require_test(duk_context *ctx) {
				/* First parameter is character name */
				auto fname = duk_to_string(ctx,0);
				std::string path = mods::js::JS_TEST_PATH;
				path += fname;
				duk_push_number(ctx,mods::js::load_library(ctx,path));
				return 1;	/* number of return values */
			}
		};
		static duk_ret_t require_js(duk_context *ctx) {
			/* First parameter is character name */
			auto fname = duk_to_string(ctx,0);
			std::string path = mods::js::JS_PATH;
			path += fname;
			duk_push_number(ctx,mods::js::load_library(ctx,path));
			return 1;	/* number of return values */
		}
		/*
		static duk_ret_t list_mobiles(duk_context *ctx){
			return 0;
		}*/
		static duk_ret_t read_mobile(duk_context *ctx){
			if(duk_get_top(ctx) < 2){
				duk_push_number(ctx,1);
				return -1;
			}
			uint32_t mob_id = duk_to_uint32(ctx,0);
			uint32_t room_id = duk_to_uint32(ctx,1);
			auto mob = read_mobile(mob_id,REAL);
			if(!mob){
				duk_push_number(ctx,1);
				return -2;
			}
			auto real_room_id = real_room(room_id);
			if(real_room_id == NOWHERE){
				duk_push_number(ctx,1);
				return -3;
			}
			char_to_room(mob,real_room_id);
			duk_push_number(ctx,1);
			return 1;
		}
		static duk_ret_t hit(duk_context *ctx) {
			/* First parameter is character name */
			auto char_uuid = duk_to_number(ctx,0);
			auto vict_uuid = duk_to_number(ctx,1);
			::hit(mods::globals::player_list[char_uuid]->cd(),mods::globals::player_list[vict_uuid]->cd(),0);
			duk_push_number(ctx,1);
			return 1;	/* number of return values */
		}
		static duk_ret_t get_current_player(duk_context *ctx) {
			/* First parameter is character name */
			duk_idx_t obj_idx;

			obj_idx = duk_push_object(ctx);
			duk_push_int(ctx, mods::globals::current_player->cd()->uuid);
			duk_put_prop_string(ctx, obj_idx, "uuid");
			duk_push_string(ctx, mods::globals::current_player->cd()->player.name);
			duk_put_prop_string(ctx, obj_idx, "name");
			return 1;
		}
		static duk_ret_t send(duk_context *ctx) {
			/* First parameter is character name */
			std::string message = duk_to_string(ctx,0);
			*mods::globals::current_player << message << "\r\n";
			duk_push_number(ctx,0);
			return 0;
		}
		static duk_ret_t uuid(duk_context *ctx) {
			/* First parameter is character name */
			std::string char_name = duk_to_string(ctx,0);

			for(auto ch = character_list; ch; ch = ch->next) {
				if(char_name.compare(ch->player.name) == 0 && !IS_NPC(ch)) {
					duk_push_number(ctx,ch->uuid);
					return 1;
				}
			}

			duk_push_number(ctx,-1);
			return 1;
		}
		static duk_ret_t room(duk_context *ctx){
			duk_push_number(ctx,1);
			return mods::globals::current_player->cd()->in_room;
		}

		static duk_ret_t cmd(duk_context *ctx) {
			/* First parameter is character name */
			std::string cmd =  duk_to_string(ctx,0);
			mods::globals::current_player->executing_js(true);
			command_interpreter(mods::globals::current_player->cd(),cmd.c_str());
			mods::globals::current_player->executing_js(false);
			return 0;	/* number of return values */
		}
		static duk_ret_t cmd_exec(duk_context *ctx) {
			/* First parameter is character name */
			std::string cmd =  duk_to_string(ctx,0);
			auto player = mods::globals::current_player;
			mods::globals::current_player->executing_js(true);
			player->capture_output(true);
			command_interpreter(player->cd(),cmd.c_str());
			player->capture_output(false);
			mods::globals::current_player->executing_js(false);
			duk_push_string(ctx,player->get_captured_output().data());
			player->clear_captured_output();
			return 1;
		}
		static duk_ret_t mob_death_trigger(duk_context *ctx) {
			/* First parameter is character name */
			std::string char_name = duk_to_string(ctx,0);
			std::string functor = duk_to_string(ctx,1);
			std::string dt_key = mods::globals::replace_all(DT_FORMAT,"{player_name}",char_name);
			DBSET(dt_key,functor);
			return 0;	/* number of return values */
		}
		static duk_ret_t in_room(duk_context *ctx) {
			/* First parameter is character name */
			std::string c_name = duk_to_string(ctx,0);

			for(auto ch = character_list; ch->next; ch = ch->next) {
				if(c_name.compare(ch->player.name) == 0) {
					duk_push_number(ctx,IN_ROOM(ch));
					return 1;
				}
			}

			duk_push_number(ctx,-1);
			return 1;	/* number of return values */
		}
		static duk_ret_t send_to_uuid(duk_context *ctx) {
			/* First parameter is character name */
			auto cuuid = duk_to_number(ctx,0);
			send_to_char(mods::globals::player_list.at(cuuid)->cd(),"%s",duk_to_string(ctx,1));
			return 0;	/* number of return values */
		}
		static duk_ret_t send_to_char(duk_context *ctx) {
			/* First parameter is character name */
			std::string c_name = duk_to_string(ctx,0);

			for(auto ch = character_list; ch->next; ch = ch->next) {
				if(c_name.compare(ch->player.name) == 0) {
					if(!IS_NPC(ch)) {
						send_to_char(ch,"%s",duk_to_string(ctx,1));
						return 0;
					}
				}
			}

			return 0;	/* number of return values */
		}
		static duk_ret_t db_seti(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			auto value = duk_to_number(ctx,1);
			mods::globals::db->put(key,std::to_string(value));
			return 0;
		}
		static duk_ret_t db_geti(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			std::string value = "";
			mods::globals::db->get(key,value);
			auto i_value = mods::util::stoi(value);

			if(i_value.has_value()) {
				duk_push_number(ctx,i_value.value());
			}

			return 1;
		}
		static duk_ret_t db_set(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			std::string value = duk_to_string(ctx,1);
			mods::globals::db->put(key,value);
			return 0;
		}
		static duk_ret_t db_get(duk_context *ctx) {
			std::string key = duk_to_string(ctx,0);
			std::string value = "";
			mods::globals::db->get(key,value);
			duk_push_string(ctx,value.c_str());
			return 1;
		}
		static duk_ret_t unsafe_code(duk_context *ctx,void* udata) {
			duk_push_string(ctx,static_cast<char*>(udata));
			duk_eval(ctx);
			return 0;
		}
		void log_js_error(std::string_view error) {
			std::cerr << "js error: " << error.data() << "\n";
		}

		void contextual_eval_string(char_data* player,duk_context* ctx,const std::string& str) {
			auto shrd_ptr_player = mods::globals::player_list.at(player->uuid);
			std::string code = std::string("player_object = ") + shrd_ptr_player->js_object() + std::string(";\n");
			code += str;

			if(duk_safe_call(ctx,mods::js::unsafe_code,static_cast<void*>(const_cast<char*>(code.c_str())),0,1) != 0) {
				log_js_error(duk_safe_to_string(ctx,-1));
			}

			duk_pop(ctx);
		}
		void contextual_eval_string(mods::player* player,duk_context* ctx,const std::string& str) {
			contextual_eval_string(player->cd(),ctx,str);
		}
		void eval_string(duk_context* ctx,const std::string& str) {
			if(duk_safe_call(ctx,mods::js::unsafe_code,static_cast<void*>(const_cast<char*>(str.c_str())),0,1) != 0) {
				log_js_error(duk_safe_to_string(ctx,-1));
			}

			duk_pop(ctx);
		}
		void eval_string(std::string_view str) {
			eval_string(mods::globals::duktape_context,str.data());
		}
		void load_c_functions() {
			load_c_functions(mods::globals::duktape_context);
		}
		void load_c_require_functions(duk_context *ctx) {
			duk_push_c_function(ctx,mods::js::require_js,1);
			duk_put_global_string(ctx,"require_js");
		}
		void load_c_test_functions(duk_context *ctx) {
			duk_push_c_function(ctx,mods::js::test::require_test,1);
			duk_put_global_string(ctx,"require_test");
		}
		void load_base_functions(duk_context *ctx) {
			duk_push_c_function(ctx,mods::js::room,0);
			duk_put_global_string(ctx,"room");
			duk_push_c_function(ctx,mods::js::read_mobile,2);
			duk_put_global_string(ctx,"read_mobile");
			duk_push_c_function(ctx,mods::js::get_current_player,0);
			duk_put_global_string(ctx,"get_current_player");
			duk_push_c_function(ctx,mods::js::send,1);
			duk_put_global_string(ctx,"send");
			duk_push_c_function(ctx,mods::js::uuid,1);
			duk_put_global_string(ctx,"uuid");
			duk_push_c_function(ctx,mods::js::hit,2);
			duk_put_global_string(ctx,"hit");
			duk_push_c_function(ctx,mods::js::cmd_exec,2);
			duk_put_global_string(ctx,"cmd_exec");
			duk_push_c_function(ctx,mods::js::cmd,2);
			duk_put_global_string(ctx,"cmd");
			duk_push_c_function(ctx,mods::js::send_to_uuid,2);
			duk_put_global_string(ctx,"send_to_uuid");
			duk_push_c_function(ctx,mods::js::send_to_char,2);
			duk_put_global_string(ctx,"send_to_char");
			duk_push_c_function(ctx,mods::js::db_seti,2);
			duk_put_global_string(ctx,"db_seti");
			duk_push_c_function(ctx,mods::js::db_geti,1);
			duk_put_global_string(ctx,"db_geti");
			duk_push_c_function(ctx,mods::js::db_set,2);
			duk_put_global_string(ctx,"db_set");
			duk_push_c_function(ctx,mods::js::db_get,1);
			duk_put_global_string(ctx,"db_get");
			duk_push_c_function(ctx,mods::js::in_room,1);
			duk_put_global_string(ctx,"in_room");
			duk_push_c_function(ctx,mods::js::mob_death_trigger,2);
			duk_put_global_string(ctx,"mob_death_trigger");
		}

		void load_c_functions(duk_context *ctx) {
			mods::js::load_base_functions(ctx);
			mods::quests::load_c_functions(ctx);
			mods::js::load_c_test_functions(ctx);
			mods::js::load_c_require_functions(ctx);
		}

		bool run_test_suite(mods::player& player,std::string_view suite) {
			auto ctx = mods::js::new_context();
			std::string path = mods::js::JS_TEST_PATH;
			std::string file = suite.data();

			for(auto& ch: file) {
				if(!std::isalpha(ch)) {
					continue;
				} else {
					path += ch;
				}
			}

			mods::js::load_c_functions(ctx);

			if(mods::js::load_library(ctx,path) == -1) {
				return false;
			}

			eval_string(ctx,std::string("test_main(") +
			            std::string(mods::util::itoa(player.cd()->uuid)) + ");"
			           );
			return true;
		}


		void run_profile_scripts(std::string_view player_name){
			/** 
			 * load_library(mods::globals::duktape_context,(std::string(JS_PROFILES_PATH) + player_name.data()) + ".js");
			 */
		}

		duk_context* new_context() {
			return duk_create_heap_default();
		}

		bool include::include_file() {
			std::string path = m_file;

			if(m_dir.length()) {
				path = m_dir + "/" + m_file;
			}

			std::ifstream include_file(path,std::ios::in);

			if(!include_file.is_open()) {
				return false;
			} else {
				std::vector<char> buffer;
				struct stat statbuf;

				if(stat(path.c_str(), &statbuf) == -1) {
					return false;
				}

				buffer.reserve(statbuf.st_size);
				std::fill(buffer.begin(),buffer.end(),0);

				while(!include_file.eof()) {
					include_file.read((char*)&buffer[0],statbuf.st_size);
				}

				eval_string(m_context,std::string((char*)&buffer[0]));
			}

			return true;
		}
		int load_library(duk_context *ctx,std::string_view file) {
			auto m = std::make_unique<include>(ctx,file);

			if(m->good()) {
				return 0;
			}

			return -1;
		}
	};
};
