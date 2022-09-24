/* ************************************************************************
 *   File: structs.h                                     Part of CircleMUD *
 *  Usage: header file for central structures and contstants               *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#ifndef __MENTOC_MODS_GENERIC_LOCKER_TYPE_HEADER__
#define __MENTOC_MODS_GENERIC_LOCKER_TYPE_HEADER__
#include <iostream>
#include <vector>
#include <memory>
#include <set>
#include "string.hpp"
#include <forward_list>


namespace mods {
	namespace orm {
		struct locker;
	};
	struct generic_locker_t {
			using obj_ptr_t = std::shared_ptr<obj_data>;
			generic_locker_t();
			generic_locker_t(const generic_locker_t&);
			generic_locker_t(std::string_view in_type,const room_vnum& room);
			~generic_locker_t();
			void init();
			void load(std::string_view in_type,const room_vnum& room);
			void replenish();
			bool replenish_if(std::string_view in_type);
		private:
			std::vector<mods::orm::locker> orm;
			obj_ptr_t container;
			room_vnum room;
			room_rnum room_id;
			uint8_t type;
			std::vector<std::string> db_errors;
			bool good;
			std::size_t items;
			std::string query;
			std::string container_yaml;
	};
};


#endif
