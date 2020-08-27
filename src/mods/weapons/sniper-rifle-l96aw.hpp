#ifndef __MENTOC_MODS_WEAPONS_SNIPER_RIFLE_L96AW_HEADER__
#define __MENTOC_MODS_WEAPONS_SNIPER_RIFLE_L96AW_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"

namespace mods::weapons::sniper_rifle {
		struct l96aw {
			l96aw();
			~l96aw();
			static obj_data_ptr_t make();

			uint16_t bound_to;
			uint16_t id;
			void feed(const mods::pq::row_type &);
			obj_data_ptr_t obj();

			protected:
			attachment_list_t m_attachments;
			obj_data_ptr_t m_obj;
		};
};
#endif

