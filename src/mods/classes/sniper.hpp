#ifndef __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#include "base.hpp"
#include <memory>
#include "../weapons/sniper-rifle.hpp"
#include <deque>
#include <memory>
namespace mods::classes {
	struct sniper : base {
		/*
		static std::shared_ptr<sniper> create(std::shared_ptr<mods::player> &player);
		static int16_t destroy(std::shared_ptr<mods::player>& player);
		*/
		types kind() {
			return types::SNIPER;
		}
		using player_ptr = std::shared_ptr<mods::player>;
		player_ptr player_obj;

		/* constructors and destructors */
		sniper();
		~sniper() = default;

		std::shared_ptr<mods::weapons::sniper_rifle> rifle();

		private:
			std::shared_ptr<mods::weapons::sniper_rifle> m_rifle;	
	};
	/*
	static std::deque<std::shared_ptr<mods::classes::sniper>> sniper_players;
	*/


};

#endif
