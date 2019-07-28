#ifndef __MENTOC_MODS_WEAPONS_SMG_HEADER__
#define __MENTOC_MODS_WEAPONS_SMG_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"

namespace mods::weapons {
		struct smg {
			smg();
			~smg();

			uint16_t bound_to;
			uint16_t id;
			void feed(const mods::pq::row_type &);
			obj_data_ptr_t obj;

		};
};
#endif

