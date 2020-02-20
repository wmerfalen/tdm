#ifndef __MENTOC_MODS_WEAPONS_SMG_MP5_HEADER__
#define __MENTOC_MODS_WEAPONS_SMG_MP5_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"

namespace mods::weapons::smg {
		struct mp5 {
			mp5();
			~mp5();
			static obj_data_ptr_t make();

			uint16_t bound_to;
			uint16_t id;
			void feed(const mods::pq::row_type &);
			obj_data_ptr_t obj;

		};
};
using mp5_ptr_t = std::shared_ptr<mods::weapons::smg::mp5>;
#endif

