#ifndef __MENTOC_MODS_WEAPONS_corrosive_claymore_HEADER__
#define __MENTOC_MODS_WEAPONS_corrosive_claymore_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../corrosive.hpp"

namespace mods::weapons {
	int corrosive_claymore_explode(obj_ptr_t& item);
	struct corrosive_claymore {
			static obj_ptr_t create();
			corrosive_claymore();
			~corrosive_claymore() = default;
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

