#ifndef __MENTOC_MODS_MIGRATIONS_HEADER__
#define __MENTOC_MODS_MIGRATIONS_HEADER__
#include "../structs.h"
#include "../globals.hpp"
#include "pq.hpp"
#include "sql.hpp"

namespace mods::migrations {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	void r_error(std::string_view msg);
	void r_success(std::string_view msg);
	void r_status(std::string_view msg);
	std::tuple<bool,int> test(std::string identifier,std::string up,std::string down,std::string purpose);

};

#endif
