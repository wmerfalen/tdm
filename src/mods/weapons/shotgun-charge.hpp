#ifndef __MENTOC_MODS_WEAPONS_shotgun_charge_HEADER__
#define __MENTOC_MODS_WEAPONS_shotgun_charge_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../shrapnel.hpp"

namespace mods::weapons {
	struct shotgun_charge {
			static obj_ptr_t create();
			shotgun_charge();
			/**
			 * Required since we'll be using a vector to store these
			 * items
			 */
			shotgun_charge(const shotgun_charge& copy);
			~shotgun_charge();

			const uuid_t& uuid() const {
				return m_uuid;
			}
			void hit(player_ptr_t& victim);
			void hit(obj_ptr_t& object);
			obj_ptr_t obj();
		protected:
			obj_ptr_t m_obj;
			uuid_t m_uuid;
	};
};
#endif

