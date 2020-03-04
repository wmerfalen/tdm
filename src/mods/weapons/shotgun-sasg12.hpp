#ifndef __MENTOC_MODS_WEAPONS_SHOTGUN_SASG12_HEADER__
#define __MENTOC_MODS_WEAPONS_SHOTGUN_SASG12_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include <string_view>

namespace mods::weapons::shotgun {
		struct sasg12 {
			static obj_data_ptr_t make();
			static obj_data_ptr_t feed_by_file(std::string_view file);
			sasg12();
			~sasg12();

			uint16_t bound_to;
			uint16_t id;
			void feed(const mods::pq::row_type &);
			obj_data_ptr_t obj;

		};
};
using sasg12_ptr_t = std::shared_ptr<mods::weapons::shotgun::sasg12>;
#endif

