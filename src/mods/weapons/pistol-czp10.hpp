#ifndef __MENTOC_MODS_WEAPONS_PISTOL_CZP10_HEADER__
#define __MENTOC_MODS_WEAPONS_PISTOL_CZP10_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"

namespace mods::weapons::pistol {
		struct czp10 {
			static obj_data_ptr_t make();
			czp10() = default;
			~czp10() = default;

			uint16_t bound_to;
			uint16_t id;
			void feed(const mods::pq::row_type &);
			obj_data_ptr_t obj;

		};
};
using czp10_ptr_t = std::shared_ptr<mods::weapons::pistol::czp10>;
#endif

