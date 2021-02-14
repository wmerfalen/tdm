#ifndef __MENTOC_MODS_CLASSES_SUPER_USER_FIDDLER_HEADER__
#define  __MENTOC_MODS_CLASSES_SUPER_USER_FIDDLER_HEADER__

#include "types.hpp"
#include "../weapon.hpp"
#include "../bugs-fixtures.hpp"
#include "../player.hpp"

namespace mods::classes {
	struct super_user_fiddler {
		super_user_fiddler();
		void fiddle_with(std::shared_ptr<mods::classes::pyrexia>& pyrexia,std::string);
		void set_fire_nades(player_ptr_t&,int charges);
		void set_fire_nade_level(player_ptr_t&,int level);

	};
	void set_player_level_mock(uuid_t&,uint8_t level);
	uint8_t mock_player_level(uuid_t&,uint8_t);
};

#endif
