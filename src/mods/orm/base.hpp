#ifndef __MENTOC_MODS_ORM_BASE_HEADER__
#define __MENTOC_MODS_ORM_BASE_HEADER__

#include <iostream>
#include <vector>
#include "../player.hpp"
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"

#define MENTOC_ORM_STR(key) key = result[##key];
#define MENTOC_ORM_UINT(key) key = static_cast<uint64_t>(result[##key]);
namespace mods::orm {
	struct base {
		virtual int16_t feed(const pqxx::result::reference &);
		base() : loaded(0) {}
		~base() = default;
		bool loaded;
	};

};

#endif
