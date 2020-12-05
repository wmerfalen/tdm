#ifndef __MENTOC_MODS_CLASSES_ENGINEER_HEADER__
#define  __MENTOC_MODS_CLASSES_ENGINEER_HEADER__
#include "../orm/engineer.hpp"
#include "base.hpp"

using engineer_orm_t = mods::orm::engineer;
namespace mods::classes {
	struct engineer : base {
		types kind() {
			return types::ENGINEER;
		}
		using primary_choice_t = mods::weapon::engineer::primary_choice_t;
		/* constructors and destructors */
		engineer();
		engineer(player_ptr_t);
		~engineer() = default;

		int16_t load_by_player(player_ptr_t &);
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();
		void replenish();
		void init();
		std::tuple<bool,std::string> drone_bomb(direction_t direction,uint8_t rooms);

		private:
			uint8_t m_drone_bomb_charges;
			uint8_t m_sensor_grenade_charges;
			player_ptr_t m_player;
			engineer_orm_t m_orm;
	};
	std::shared_ptr<mods::classes::engineer> create_engineer(player_ptr_t &player);
};

#endif
