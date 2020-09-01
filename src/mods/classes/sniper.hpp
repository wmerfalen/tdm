#ifndef  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
//#include "base.hpp"
#include <memory>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include <deque>
#include <memory>
#include "../orm/class-sniper.hpp"
#include "types.hpp"

using sniper_orm_t = mods::orm::sniper;

namespace mods::classes {
	using psg1_t = std::shared_ptr<mods::weapons::sniper_rifle::psg1>;
	using l96aw_t = std::shared_ptr<mods::weapons::sniper_rifle::l96aw>;
	struct sniper {
		/*
		static std::shared_ptr<sniper> create(player_ptr_t &player);
		static int16_t destroy(player_ptr_t& player);
		*/
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
		psg1_t psg1();
		l96aw_t l96aw();
		player_ptr_t player();
		int16_t new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();

		private:
			psg1_t m_psg1;
			l96aw_t m_l96aw;
			player_ptr_t m_player;
			sniper_orm_t m_orm;
	};
	/*
	static std::deque<std::shared_ptr<mods::classes::sniper>> sniper_players;
	*/
	std::shared_ptr<mods::classes::sniper> create_sniper(player_ptr_t &player);


};

#endif
