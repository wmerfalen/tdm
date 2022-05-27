#ifndef __MENTOC_MODS_ORM_generic_orm_HEADER__
#define __MENTOC_MODS_ORM_generic_orm_HEADER__

#define __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using sql_response_t = std::tuple<int16_t,std::string>;

};

#endif
