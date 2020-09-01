#ifndef __MENTOC_MODS_CLASSES_PSYOP_HEADER__
#define  __MENTOC_MODS_CLASSES_PSYOP_HEADER__
//#include "base.hpp"
#include <memory>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include <deque>
#include <memory>
#include "../orm/class-psyop.hpp"
#include "types.hpp"
using psyop_orm_t = mods::orm::psyop;
namespace mods::classes {
	struct psyop {
		/*
		static std::shared_ptr<psyop> create(player_ptr_t &player);
		static int16_t destroy(player_ptr_t& player);
		*/
		types kind() {
			return types::PSYOP;
		}

		using primary_choice_t = mods::weapon::psyop::primary_choice_t;
		/* constructors and destructors */
		psyop();
		psyop(player_ptr_t);
		~psyop() = default;

		int16_t load_by_player(player_ptr_t &);
		std::shared_ptr<mods::weapons::sniper_rifle::psg1> psg1();
		std::shared_ptr<mods::weapons::sniper_rifle::l96aw> l96aw();
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();

		private:
			std::shared_ptr<mods::weapons::sniper_rifle::psg1> m_psg1;
			std::shared_ptr<mods::weapons::sniper_rifle::l96aw> m_l96aw;
			player_ptr_t m_player;
			psyop_orm_t m_orm;
	};
	/*
	static std::deque<std::shared_ptr<mods::classes::sniper>> sniper_players;
	*/
	std::shared_ptr<mods::classes::psyop> create_psyop(player_ptr_t &player);


};

#endif
