#ifndef  __MENTOC_MODS_CLASSES_GHOST_HEADER__
#define  __MENTOC_MODS_CLASSES_GHOST_HEADER__
#include "../orm/class-ghost.hpp"
#include "base.hpp"

using ghost_orm_t = mods::orm::ghost;

namespace mods::classes {
	struct ghost : base {
		types kind() {
			return types::GHOST;
		}
		using primary_choice_t = mods::weapon::ghost::primary_choice_t;
		using secondary_choice_t = mods::weapon::ghost::secondary_choice_t;

		/* constructors and destructors */
		ghost();
		ghost(player_ptr_t);
		~ghost() = default;

		int16_t load_by_player(player_ptr_t &);
		player_ptr_t player();
		int16_t new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();

		private:
			player_ptr_t m_player;
			ghost_orm_t m_orm;
	};
	std::shared_ptr<mods::classes::ghost> create_ghost(player_ptr_t &player);
};

#endif
