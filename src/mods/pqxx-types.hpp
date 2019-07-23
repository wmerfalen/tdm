#ifndef __MENTOC_MODS_PQXX_TYPES_HEADER__
#define __MENTOC_MODS_PQXX_TYPES_HEADER__

#include <pqxx/pqxx>

namespace mods::pq {
#ifdef __MENTOC_USE_PQXX_RESULT__
		using row_type = pqxx::result::reference&;
#else
		using row_type = pqxx::row;
#endif
};

#endif
