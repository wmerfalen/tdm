#ifndef __MENTOC_MODS_ORM_FACTORY_HEADER__
#define __MENTOC_MODS_ORM_FACTORY_HEADER__

#include <iostream>
#include <vector>
#include "../pq.hpp"
#include "../sql.hpp"
#include "quotes.hpp"
#include "chargen.hpp"

namespace mods::orm::factory {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using quotes = base::quotes<uint64_t,uint32_t>;
};

#endif
