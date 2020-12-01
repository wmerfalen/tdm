#ifndef  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#include "../orm/sniper.hpp"
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
		int16_t new_player(player_ptr_t&);
		int16_t save();
		std::vector<uuid_t> get_targets_scanned_by_drone();
		void drone_scan(room_rnum);
		void replenish();
		/*
		- Ability: X-Ray Shot (can shoot through multiple layers of walls/doors)
			- Can snipe an enemy within a building
			- Can snipe an enemy through walls or doors
			- If enemy not behind cover, it causes 150% damage to target
			*/
		std::tuple<bool,std::string> xray_shot();

		private:
			player_ptr_t m_player;
			sniper_orm_t m_orm;
			std::vector<uuid_t> m_scanned;
			room_rnum m_marked_room;
	};
	std::shared_ptr<mods::classes::sniper> create_sniper(player_ptr_t &player);
};

#endif
