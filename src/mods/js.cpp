#include "js.hpp"
#include "quests.hpp"
#include "util.hpp"
#include "extern.hpp"
#define DT_FORMAT "{player_name}:mob_death_trigger"
extern void command_interpreter(struct char_data* ch,char* argument);
extern void hit(struct char_data* ch,struct char_data* vict,int type);
namespace mods{
	namespace js {
		int load_library(duk_context*,std::string_view);
		constexpr static char * JS_PATH = "../lib/js/";
		constexpr static char * JS_TEST_PATH = "../lib/js/tests/";
		namespace test {
		static duk_ret_t require_test(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			auto fname = duk_to_string(ctx,0);
			std::string path = mods::js::JS_TEST_PATH;
			path += fname;
			duk_push_number(ctx,mods::js::load_library(ctx,path));
			return 1;	/* number of return values */
		}
		};
		static duk_ret_t require_js(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			auto fname = duk_to_string(ctx,0);
			std::string path = mods::js::JS_PATH;
			path += fname;
			duk_push_number(ctx,mods::js::load_library(ctx,path));
			return 1;	/* number of return values */
		}
		static duk_ret_t hit(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			auto char_uuid = duk_to_number(ctx,0);
			auto vict_uuid = duk_to_number(ctx,1);
			short ctr = 0;
			::hit(mods::globals::player_list[char_uuid],mods::globals::player_list[vict_uuid],0);
			duk_push_number(ctx,1);
			return 1;	/* number of return values */
		}
		static duk_ret_t uuid(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			std::string char_name = duk_to_string(ctx,0);
			auto i_room = mods::util::stoi(duk_to_string(ctx,1));
			if(i_room.has_value()){
				room_rnum room = i_room.value();
				for(auto ch = character_list; ch; ch = ch->next){
					if(char_name.compare(ch->player.name) == 0 && IN_ROOM(ch) == room){
						duk_push_number(ctx,ch->uuid);
						return 1;
					}
				}
			}
			duk_push_number(ctx,-1);
			return 1;
		}
		static duk_ret_t command(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			auto char_uuid = duk_to_number(ctx,0);
			std::string cmd =  duk_to_string(ctx,1);
			command_interpreter(mods::globals::player_list[char_uuid],cmd.c_str());
			return 0;	/* number of return values */
		}
		static duk_ret_t command_exec(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			auto char_uuid = duk_to_number(ctx,0);
			std::string cmd =  duk_to_string(ctx,1);
			auto player = mods::globals::player_list.at(char_uuid); 
			player.capture_output(true);
			command_interpreter(player,cmd.c_str());
			player.capture_output(false);
			duk_push_string(ctx,player.get_captured_output().data());
			return 1;
		}
		static duk_ret_t mob_death_trigger(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			std::string char_name = duk_to_string(ctx,0);
			std::string functor = duk_to_string(ctx,1);
			std::string dt_key = mods::globals::replace_all(DT_FORMAT,"{player_name}",char_name);
			DBSET(dt_key,functor);
			return 0;	/* number of return values */
		}
		static duk_ret_t in_room(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			std::string c_name = duk_to_string(ctx,0);
			for(auto ch = character_list; ch->next; ch = ch->next){
				if(c_name.compare(ch->player.name) == 0){
					duk_push_number(ctx,IN_ROOM(ch));	
					return 1;
				}
			}
			duk_push_number(ctx,-1);
			return 1;	/* number of return values */
		}
		static duk_ret_t send_to_uuid(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			auto cuuid = duk_to_number(ctx,0);
			send_to_char(mods::globals::player_list.at(cuuid),duk_to_string(ctx,1));
			return 0;	/* number of return values */
		}
		static duk_ret_t send_to_char(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			/* First parameter is character name */
			std::string c_name = duk_to_string(ctx,0);
			for(auto ch = character_list; ch->next; ch = ch->next){
				if(c_name.compare(ch->player.name) == 0){
					send_to_char(ch,duk_to_string(ctx,1));
					return 0;
				}
			}
			return 0;	/* number of return values */
		}
		static duk_ret_t db_seti(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			std::string key = duk_to_string(ctx,0);
			auto value = duk_to_number(ctx,1);
			mods::globals::db->put(key,std::to_string(value));
			return 0;
		}
		static duk_ret_t db_geti(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			std::string key = duk_to_string(ctx,0);
			std::string value = "";
			mods::globals::db->get(key,value);
			auto i_value = mods::util::stoi(value);
			if(i_value.has_value()){
				duk_push_number(ctx,i_value.value());
			}
			return 1;
		}
		static duk_ret_t db_set(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			std::string key = duk_to_string(ctx,0);
			std::string value = duk_to_string(ctx,1);
			mods::globals::db->put(key,value);
			return 0;
		}
		static duk_ret_t db_get(duk_context *ctx){
			int num_args = duk_get_top(ctx);
			std::string key = duk_to_string(ctx,0);
			std::string value = "";
			mods::globals::db->get(key,value);
			duk_push_string(ctx,value.c_str());
			return 1;
		}
		static duk_ret_t unsafe_code(duk_context *ctx,void* udata){
			duk_push_string(ctx,static_cast<char*>(udata));
			duk_eval(ctx);
			return 0;
		}
		void eval_string(duk_context* ctx,std::string_view str){
			if(duk_safe_call(ctx,mods::js::unsafe_code,static_cast<const void*>(str.data()),0,1) != 0){
				std::cerr << "Unexpected js error: " << duk_safe_to_string(ctx,-1) << "\r\n";
			}
			duk_pop(ctx);
		}
		void eval_string(std::string_view str){
			eval_string(mods::globals::duktape_context,str);
		}
		void load_c_functions(){
			load_c_functions(mods::globals::duktape_context);
		}
		void load_c_require_functions(duk_context *ctx){
			duk_push_c_function(ctx,mods::js::require_js,1);
			duk_put_global_string(ctx,"require_js");
		}
		void load_c_test_functions(duk_context *ctx){
			duk_push_c_function(ctx,mods::js::test::require_test,1);
			duk_put_global_string(ctx,"require_test");
		}
		void load_c_functions(duk_context *ctx){
			duk_push_c_function(ctx,mods::js::uuid,2);
			duk_put_global_string(ctx,"uuid");
			duk_push_c_function(ctx,mods::js::hit,2);
			duk_put_global_string(ctx,"hit");
			duk_push_c_function(ctx,mods::js::command_exec,2);
			duk_put_global_string(ctx,"cmd_exec");
			duk_push_c_function(ctx,mods::js::command,2);
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

			mods::quests::load_c_functions(ctx);
			mods::js::load_c_test_functions(ctx);
			mods::js::load_c_require_functions(ctx);
		}

		bool run_test_suite(mods::player & player,std::string_view suite){
			auto ctx = mods::js::new_context();
			std::string path = mods::js::JS_TEST_PATH;
			std::string file = suite.data();
			for(auto & ch: file){
				if(!std::isalpha(ch)){ continue; }
				else{ path += ch; }
			}
			mods::js::load_c_functions(ctx);
			if(mods::js::load_library(ctx,path) == -1){
				return false;
			}
			eval_string(ctx,std::string("test_main(") + 
				std::string(mods::util::itoa(player.cd()->uuid)) + ");"
			);
			return true;
		}

		duk_context* new_context(){
			return duk_create_heap_default();
		}

		bool include::include_file() {
			std::string path = m_file;
			if(m_dir.length()){
				path = m_dir + "/" + m_file;
			}
			std::ifstream include_file(path,std::ios::in);
            if(!include_file.is_open()){
                return false;
            }
            else{
                std::vector<char> buffer;
                struct stat statbuf;

                if (stat(path.c_str(), &statbuf) == -1) {
                    return false;
                }

                buffer.reserve(statbuf.st_size);
                std::fill(buffer.begin(),buffer.end(),0);

                while(!include_file.eof()){
                    include_file.read((char*)&buffer[0],JS_READ_CHUNK_SIZE);
                }
                eval_string(m_context,std::string((char*)&buffer[0]));
			}
			return true;
		}
		int load_library(duk_context *ctx,std::string_view file){
			auto m = std::make_unique<include>(ctx,file);
			if(m->good()){
				return 0;
			}
			return -1;
		}
	};
};
