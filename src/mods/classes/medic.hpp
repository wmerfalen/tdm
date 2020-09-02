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

#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include "../orm/class-medic.hpp"
#include "types.hpp"
#include "../weapon.hpp"
#include "../bugs-fixtures.hpp"

using medic_orm_t = mods::orm::medic;

namespace mods::classes {
	struct medic {
		types kind() {
			return types::MEDIC;
		}
		using primary_choice_t = mods::weapon::medic::primary_choice_t;
		using secondary_choice_t = mods::weapon::medic::secondary_choice_t;

		static inline void report(std::vector<std::string> msgs){
			mods::bugs::fixtures("mods::classes::medic::",IMPLODE(msgs,""));
		}
		/* constructors and destructors */
		medic();
		medic(player_ptr_t);
		~medic() = default;

		int16_t load_by_player(player_ptr_t &);
		player_ptr_t 	player();
		int16_t				new_player(player_ptr_t&, primary_choice_t primary);
		int16_t save();

		void heal_player(player_ptr_t& player_obj,player_ptr_t& target);

		private:
			player_ptr_t m_player;
			medic_orm_t m_orm;
	};

	std::shared_ptr<mods::classes::medic> create_medic(player_ptr_t &player);
};

#endif
