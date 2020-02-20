#ifndef __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#include "base.hpp"
#include <memory>
#include "../weapons/sniper-rifle-psg1.hpp"
#include <deque>
#include <memory>
namespace mods::classes {
	struct sniper : base {
		/*
		static std::shared_ptr<sniper> create(player_ptr_t &player);
		static int16_t destroy(player_ptr_t& player);
		*/
		types kind() {
			return types::SNIPER;
		}
		using player_ptr = player_ptr_t;
		player_ptr player_obj;

		/* constructors and destructors */
		sniper();
		~sniper() = default;

		std::shared_ptr<mods::weapons::sniper_rifle::psg1> rifle();

		private:
			std::shared_ptr<mods::weapons::sniper_rifle::psg1> m_rifle;	
	};
	/*
	static std::deque<std::shared_ptr<mods::classes::sniper>> sniper_players;
	*/


};

#endif
