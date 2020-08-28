#ifndef  __MENTOC_MODS_CLASSES_MEDIC_HEADER__
#define  __MENTOC_MODS_CLASSES_MEDIC_HEADER__
#if 0

//#include "base.hpp"
#include "../orm/class-medic.hpp"

namespace mods::classes {
	using player_ptr = player_ptr_t;
	struct medic {
		types kind() {
			return types::MEDIC;
		}

		uint16_t stim_pistol_ammo;
		uint16_t stim_pistol_ammo_max;
		player_ptr player_obj;


		/* constructors and destructors */
		medic() : stim_pistol_ammo(3),
			stim_pistol_ammo_max(3),
			player_obj(nullptr) {

		}
		medic(player_ptr& plyr_ptr) : stim_pistol_ammo(3),
			stim_pistol_ammo_max(3),
			player_obj(plyr_ptr) {
		}
		~medic() = default;
		static void heal_player(player_ptr&,player_ptr&);
	};
#endif 

#include <memory>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include <deque>
#include <memory>
#include "../orm/class-medic.hpp"
#include "types.hpp"

using medic_orm_t = mods::orm::medic;

namespace mods::classes {
	struct medic {
		/*
		static std::shared_ptr<sniper> create(player_ptr_t &player);
		static int16_t destroy(player_ptr_t& player);
		*/
		types kind() {
			return types::MEDIC;
		}

		/* constructors and destructors */
		medic();
		medic(player_ptr_t);
		~medic() = default;

		int16_t load_by_player(player_ptr_t &);
		std::shared_ptr<mods::weapons::sniper_rifle::psg1> psg1();
		std::shared_ptr<mods::weapons::sniper_rifle::l96aw> l96aw();
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, std::string_view primary);
		int16_t save();

		void heal_player(player_ptr_t& player_obj,player_ptr_t& target);

		private:
			std::shared_ptr<mods::weapons::sniper_rifle::psg1> m_psg1;
			std::shared_ptr<mods::weapons::sniper_rifle::l96aw> m_l96aw;
			player_ptr_t m_player;
			medic_orm_t m_orm;
	};

	std::shared_ptr<mods::classes::medic> create_medic(player_ptr_t &player);
};

#endif
