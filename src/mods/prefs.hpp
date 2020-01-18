#ifndef __MENTOC_MODS_prefs_HEADER__
#define  __MENTOC_MODS_prefs_HEADER__

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#include "../globals.hpp"
#include "../interpreter.h"
#define PREFS_SEPARATOR std::to_string(":")
#define PREFS_FORMAT_STRING "{name}:prefs:{key}"
#define DYNAMIC_FORMAT_STRING "{name}:{prefs}:{key}"
#define DYN_DB_SET(key,section,value,ch) mods::prefs::dynamic_set(key,value,section,ch);
#define DYN_DB_GET(key,section,ch) mods::prefs::dynamic_get(key,section,ch);
#define PLAYER_SET(key,value) mods::prefs::dynamic_set(key,value,"player",player->cd());
#define PLAYER_GET(key) mods::prefs::dynamic_get(key,"player",player->cd());
#define PLAYER_SETI(key,value) PLAYER_SET(key,std::to_string(value));
#define PLAYER_GETI(key) std::stoi(PLAYER_GET(key));
namespace mods {
	class prefs {
		public:
			prefs() = default;
			~prefs() = default;
			static void set(const std::string& key,const std::string& value,char_data*);
			static std::string get(const std::string& key,char_data*);
			static std::string namify(const std::string&,char_data*);
			static void dynamic_set(const std::string& key,const std::string& value,const std::string& section,char_data*);
			static std::string dynamic_get(const std::string& key,const std::string& section,char_data*);
			static std::string dynamic_namify(const std::string& key,const std::string& section,char_data* c);
		protected:
	};
};


ACMD(do_pref);
#endif
