#ifndef __MENTOC_MODS_CLASSES_MARINE_HEADER__
#define  __MENTOC_MODS_CLASSES_MARINE_HEADER__
#include "base.hpp"
#include "../orm/marine.hpp"
		using marine_orm_t = mods::orm::marine;
namespace mods::classes {
	struct marine : base {
		types kind() {
			return types::MARINE;
		}
		using primary_choice_t = mods::weapon::marine::primary_choice_t;

		/* constructors and destructors */
		marine();
		marine(player_ptr_t);
		~marine() = default;

		int16_t load_by_player(player_ptr_t &);
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();
		void replenish();

		private:
			player_ptr_t m_player;
			marine_orm_t m_orm;
	};
	std::shared_ptr<mods::classes::marine> create_marine(player_ptr_t &player);
};

#endif
