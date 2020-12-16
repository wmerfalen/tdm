#ifndef __MENTOC_MODS_FORGE_ENGINE_GENERATOR_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_GENERATOR_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct generator {
		int16_t feed(const pqxx::result::reference &);
		generator() : loaded(0) {}
		~generator() = default;
		bool loaded;
	};

};

#endif
