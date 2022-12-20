#ifndef __MENTOC_MODS_WEAPONS_attachment_frag_underbarrel_HEADER__
#define __MENTOC_MODS_WEAPONS_attachment_frag_underbarrel_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "attachment-underbarrel.hpp"

namespace mods::weapons {
	struct attachment_frag_underbarrel : public attachment_underbarrel {
			attachment_frag_underbarrel();
			~attachment_frag_underbarrel();

			void init();
			std::tuple<bool,std::string> attach_to(obj_ptr_t weapon, const uint8_t& player_tier);
			std::tuple<bool,std::string> detach();
			void consume_ammo();
			std::tuple<bool,std::string> fire(player_ptr_t& attacker,const direction_t& direction, const uint8_t& distance);
			void set_yaml(std::string_view file);
			std::string_view get_yaml() const {
				return m_yaml;
			}
		protected:
			obj_ptr_t m_sniper_rifle;
			std::string m_yaml;
	};
};
#endif

