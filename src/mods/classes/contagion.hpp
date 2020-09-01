#ifndef __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
#define  __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
//#include "base.hpp"
#include <memory>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include <deque>
#include <memory>
#include "../orm/class-contagion.hpp"
#include "../weapon.hpp"
#include "types.hpp"

		using contagion_orm_t = mods::orm::contagion;
namespace mods::classes {
	struct contagion {
		/*
		static std::shared_ptr<contagion> create(player_ptr_t &player);
		static int16_t destroy(player_ptr_t& player);
		*/
		types kind() {
			return types::CONTAGION;
		}
		using primary_choice_t = mods::weapon::contagion::primary_choice_t;

		/* constructors and destructors */
		contagion();
		contagion(player_ptr_t);
		~contagion() = default;

		int16_t load_by_player(player_ptr_t &);
		/** FIXME */
		std::shared_ptr<mods::weapons::sniper_rifle::psg1> psg1();
		/** FIXME */
		std::shared_ptr<mods::weapons::sniper_rifle::l96aw> l96aw();
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, primary_choice_t primary); /** TODO: create secondary enum and pass as 3rd param */
		int16_t save();

		private:
		/** FIXME */
			std::shared_ptr<mods::weapons::sniper_rifle::psg1> m_psg1;
		/** FIXME */
			std::shared_ptr<mods::weapons::sniper_rifle::l96aw> m_l96aw;
			player_ptr_t m_player;
			contagion_orm_t m_orm;
	};
	/*
	static std::deque<std::shared_ptr<mods::classes::contagion>> contagion_players;
	*/
	std::shared_ptr<mods::classes::contagion> create_contagion(player_ptr_t &player);


};

#endif
