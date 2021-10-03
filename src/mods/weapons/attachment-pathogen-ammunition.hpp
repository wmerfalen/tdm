#ifndef __MENTOC_MODS_WEAPONS_attachment_pathogen_ammunition_HEADER__
#define __MENTOC_MODS_WEAPONS_attachment_pathogen_ammunition_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"

namespace mods::weapons {
	struct attachment_pathogen_ammunition {
			attachment_pathogen_ammunition();
			~attachment_pathogen_ammunition();

			obj_ptr_t obj();
			bool is_attached() const;
			void init();
			std::tuple<bool,std::string> attach_to(obj_ptr_t weapon, const uint8_t& player_tier);
			std::tuple<bool,std::string> detach();
			uint16_t ammo();
			void consume_ammo();
			std::tuple<bool,std::string> fire(player_ptr_t& victim);
		protected:
			obj_ptr_t m_obj;
			obj_ptr_t m_sniper_rifle;
			bool m_attached;
	};
};
#endif

