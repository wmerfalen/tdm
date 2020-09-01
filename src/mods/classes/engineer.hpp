#ifndef __MENTOC_MODS_CLASSES_ENGINEER_HEADER__
#define  __MENTOC_MODS_CLASSES_ENGINEER_HEADER__
//#include "base.hpp"
#include <memory>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include <deque>
#include <memory>
#include "../orm/class-engineer.hpp"
#include "types.hpp"

using engineer_orm_t = mods::orm::engineer;
namespace mods::classes {
	struct engineer {
		/*
		static std::shared_ptr<engineer> create(player_ptr_t &player);
		static int16_t destroy(player_ptr_t& player);
		*/
		types kind() {
			return types::ENGINEER;
		}

		using primary_choice_t = mods::weapon::engineer::primary_choice_t;
		/* constructors and destructors */
		engineer();
		engineer(player_ptr_t);
		~engineer() = default;

		int16_t load_by_player(player_ptr_t &);
		/** FIXME */
		std::shared_ptr<mods::weapons::sniper_rifle::psg1> psg1();
		/** FIXME */
		std::shared_ptr<mods::weapons::sniper_rifle::l96aw> l96aw();
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();

		private:
		/** FIXME */
			std::shared_ptr<mods::weapons::sniper_rifle::psg1> m_psg1;
		/** FIXME */
			std::shared_ptr<mods::weapons::sniper_rifle::l96aw> m_l96aw;
			player_ptr_t m_player;
			engineer_orm_t m_orm;
	};
	/*
	static std::deque<std::shared_ptr<mods::classes::sniper>> sniper_players;
	*/
	std::shared_ptr<mods::classes::engineer> create_engineer(player_ptr_t &player);


};

#endif
