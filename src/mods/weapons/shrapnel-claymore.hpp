#ifndef __MENTOC_MODS_WEAPONS_shrapnel_claymore_HEADER__
#define __MENTOC_MODS_WEAPONS_shrapnel_claymore_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../shrapnel.hpp"

namespace mods::weapons {
	int shrapnel_claymore_explode(obj_ptr_t& item);
	struct shrapnel_claymore {
			static obj_ptr_t create();
			shrapnel_claymore();
			~shrapnel_claymore() = default;
			const room_rnum& room();

			std::tuple<bool,std::string> install(player_ptr_t& attacker,const room_rnum& room,const direction_t& direction);
			const uuid_t& uuid() const {
				return m_uuid;
			}
			void finished_installing();
			void exploded();
			obj_ptr_t& obj();
		protected:
			obj_ptr_t m_obj;
			std::pair<room_rnum,direction_t> m_claymore_room;
			bool m_installed;
			uuid_t m_installer;
			uuid_t m_uuid;
	};
};
#endif

