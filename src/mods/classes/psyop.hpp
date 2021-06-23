#ifndef __MENTOC_MODS_CLASSES_PSYOP_HEADER__
#define  __MENTOC_MODS_CLASSES_PSYOP_HEADER__
#include "../orm/class-psyop.hpp"
#include "base.hpp"

using psyop_orm_t = mods::orm::psyop;
namespace mods::classes {
	struct psyop : base {
			types kind() {
				return types::PSYOP;
			}

			using primary_choice_t = mods::weapon::psyop::primary_choice_t;
			/* constructors and destructors */
			psyop();
			psyop(player_ptr_t);
			~psyop() = default;

			int16_t load_by_player(player_ptr_t&);
			int16_t				new_player(player_ptr_t&, primary_choice_t primary);
			int16_t save();

		private:
			player_ptr_t m_player;
			psyop_orm_t m_orm;
	};
	std::shared_ptr<mods::classes::psyop> create_psyop(player_ptr_t& player);
};

#endif
