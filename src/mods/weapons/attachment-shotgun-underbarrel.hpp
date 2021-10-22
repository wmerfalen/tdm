#ifndef __MENTOC_MODS_WEAPONS_attachment_shotgun_underbarrel_HEADER__
#define __MENTOC_MODS_WEAPONS_attachment_shotgun_underbarrel_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "attachment-underbarrel.hpp"

namespace mods::weapons {
	struct attachment_shotgun_underbarrel : public attachment_underbarrel {
			attachment_shotgun_underbarrel();
			~attachment_shotgun_underbarrel();

			void init() override;
			void consume_ammo() override;
			enum shotgun_variant_t : uint8_t {
				SHV_GHOST = 0,
				SHV_MARINE,
			};
			void set_variant(shotgun_variant_t v);
			std::tuple<bool,std::string> attach_to(obj_ptr_t weapon, const uint8_t& player_tier);
			std::tuple<bool,std::string> detach();
		protected:
			obj_ptr_t m_sniper_rifle;
			shotgun_variant_t m_variant;
	};
};
#endif

