#ifndef  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#include "../orm/class-sniper.hpp"
#include "base.hpp"

using sniper_orm_t = mods::orm::sniper;

namespace mods::classes {
	struct sniper : base {
		types kind() {
			return types::SNIPER;
		}
		using primary_choice_t = mods::weapon::sniper::primary_choice_t;
		using secondary_choice_t = mods::weapon::sniper::secondary_choice_t;

		/* constructors and destructors */
		sniper();
		sniper(player_ptr_t);
		~sniper() = default;

		int16_t load_by_player(player_ptr_t &);
		player_ptr_t player();
		int16_t new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();

		private:
			player_ptr_t m_player;
			sniper_orm_t m_orm;
	};
	std::shared_ptr<mods::classes::sniper> create_sniper(player_ptr_t &player);
};

#endif
