#ifndef __MENTOC_CONFIG_HEADER__
#define __MENTOC_CONFIG_HEADER__
#include "structs.h"
#include "mods/db.hpp"
#include <memory>
#include <variant>
namespace mods::globals {
	//extern std::unique_ptr<mods::lmdb::_db_handle> db;
};
namespace config {
	namespace rooms {
		using room_vnum = int64_t;
		using room_rnum = uint64_t;
		constexpr static room_vnum IDLE = 27089;
		constexpr static room_vnum MORTAL_START = 27089;
		room_vnum virtual_idle();
		room_vnum virtual_mortal_start();
		room_rnum real_idle();
		room_rnum real_mortal_start();
	};
	void init(int argc,char** argv);
	static std::map<std::string,std::string> mapped;

	template <typename T>
	static inline T get(std::string_view option){
		T ignored;
		return ignored;
		/*
		std::string key = std::string("config:") + option.data();
		std::string data;
		mods::globals::db->get(key,data); return std::get<T>(data);
		*/
	}
	template <typename T>
	static inline void set(std::string_view option,T value){
		/*
		std::string key = std::string("config:") + option.data();
		mods::globals::db->put(key,std::to_string(value));	
		*/
	}
	void update_db(std::string_view option,std::string_view value);
}
#endif
