#ifndef __MENTOC_MODS_WEAPONS_SNIPER_RIFLE_HEADER__
#define __MENTOC_MODS_WEAPONS_SNIPER_RIFLE_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"

namespace mods::weapons {
		struct sniper_rifle {
			sniper_rifle();
			~sniper_rifle();

			uint64_t bound_to;
			uint64_t id;
			void feed(const mods::pq::row_type &);
			std::shared_ptr<obj_data> obj;

		};
};
#endif

