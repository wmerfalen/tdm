#ifndef __MENTOC_MODS_CLASSES_SYNDROME_HEADER__
#define  __MENTOC_MODS_CLASSES_SYNDROME_HEADER__
#include <variant>
#include "../orm/syndrome.hpp"
#include "base.hpp"
#include "../skills.hpp"
#include "../super-users.hpp"

namespace mods::classes {
	struct super_user_fiddler;
};
using syndrome_orm_t = mods::orm::syndrome;
namespace mods::classes {
	struct syndrome : base {
		friend class mods::classes::super_user_fiddler;

		static int16_t destroy(player_ptr_t& player);
		types kind() {
			return types::SYNDROME;
		}

		/* constructors and destructors */
		syndrome();
		syndrome(player_ptr_t);
		~syndrome() = default;
		void replenish();

		player_ptr_t 	player();

		void set_player(player_ptr_t);

		/** database routines */
		int16_t load_by_player(player_ptr_t&);
		int16_t new_player(player_ptr_t&);
		int64_t db_id() const;
		int16_t save();
		private:
			syndrome_orm_t	m_orm;
			player_ptr_t m_player;
	};
	std::shared_ptr<mods::classes::syndrome> create_syndrome(player_ptr_t &player);
};


#endif
