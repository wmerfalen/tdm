#ifndef __MENTOC_MODS_JS_HEADER__
#define __MENTOC_MODS_JS_HEADER__
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
#include "../globals.hpp"
#include "../duktape/src/duktape.h"
#include <sys/stat.h>
#define JS_READ_CHUNK_SIZE 1024
namespace mods {
	namespace js {
		void eval_string(const std::string & str);
		void eval_string(duk_context *ctx,const std::string & str);
		void load_c_functions();
		void load_c_functions(duk_context *ctx);
		int load_library(duk_context *ctx,const std::string & file);
		struct include {
			include() = delete;
			include(duk_context *ctx,const std::string & file) 
				: m_file(file), m_context(ctx) {
					m_include_success = include_file();	
			}
			include(duk_context *ctx,const std::string & directory,const std::string & file) 
				: m_file(file), m_dir(directory), m_context(ctx) {
					m_include_success = include_file();
				}
			bool include_file() ;
			inline bool good() { return m_include_success; }
			inline void set_file(const std::string& m){ m_file = m; }
			inline void set_dir(const std::string& m){ m_dir = m; }
			inline void set_context(duk_context *ctx){ m_context = ctx; }

			inline const std::string & get_file(){ return m_file; }
			inline const std::string & get_dir(){ return m_dir; }
			inline duk_context* get_context(){ return m_context; }
			private:
				std::string m_file;
				std::string m_dir;
				duk_context* m_context;
				bool m_include_success;
		};
		duk_context* new_context();
	};
};
#endif
