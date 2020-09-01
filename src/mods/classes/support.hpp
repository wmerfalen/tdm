#ifndef __MENTOC_MODS_CLASSES_SUPPORT_HEADER__
#define  __MENTOC_MODS_CLASSES_SUPPORT_HEADER__
//#include "base.hpp"
#include <memory>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include <deque>
#include <memory>
#include "../orm/class-support.hpp"
#include "types.hpp"
		using support_orm_t = mods::orm::support;
namespace mods::classes {
	struct support {
		types kind() {
			return types::SUPPORT;
		}
		using primary_choice_t = mods::weapon::support::primary_choice_t;

		/* constructors and destructors */
		support();
		support(player_ptr_t);
		~support() = default;

		int16_t load_by_player(player_ptr_t &);
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();

		private:
			player_ptr_t m_player;
			support_orm_t m_orm;
	};
	std::shared_ptr<mods::classes::support> create_support(player_ptr_t &player);


};

#endif
