#include "js.hpp"
#include "quests.hpp"
extern struct room_data* world;
extern struct char_data* character_list;
namespace mods{
	namespace js {
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
		void eval_string(duk_context* ctx,const std::string& str){
			duk_eval_string(ctx,str.c_str());
		}
		void eval_string(const std::string & str){
			load_c_functions(mods::globals::duktape_context);
			eval_string(mods::globals::duktape_context,str);
		}
		void load_c_functions(){
			load_c_functions(mods::globals::duktape_context);
		}
		void load_c_functions(duk_context *ctx){
			duk_push_c_function(ctx,mods::js::send_to_char,2);
			duk_put_global_string(ctx,"send_to_char");
			duk_push_c_function(ctx,mods::js::db_set,2);
			duk_put_global_string(ctx,"db_set");
			duk_push_c_function(ctx,mods::js::db_get,1);
			duk_put_global_string(ctx,"db_get");

			mods::quests::load_c_functions(ctx);
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
				return true;
			}
		}
		int load_library(duk_context *ctx,const std::string & file){
			auto m = new include(ctx,file);
			if(m->good()){
				return 0;
			}
			return -1;
		}
	};
};
