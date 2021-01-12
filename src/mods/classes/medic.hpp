#ifndef  __MENTOC_MODS_CLASSES_MEDIC_HEADER__
#define  __MENTOC_MODS_CLASSES_MEDIC_HEADER__

#include "../orm/medic.hpp"
#include "base.hpp"

using medic_orm_t = mods::orm::medic;

namespace mods::classes {
	struct medic : public base {
			types kind() {
				return types::MEDIC;
			}
			using primary_choice_t = mods::weapon::medic::primary_choice_t;
			using secondary_choice_t = mods::weapon::medic::secondary_choice_t;
			constexpr static uint8_t DEFAULT_STARTING_STIM_PISTOL_AMMO = 4;

			/* constructors and destructors */
			medic();
			medic(player_ptr_t);
			~medic() = default;

			/** orm wrappers */
			int16_t load_by_player(player_ptr_t&);
			player_ptr_t 	player();
			int16_t	new_player(player_ptr_t&, primary_choice_t primary);
			uint64_t initialize_row(player_ptr_t&);
			int16_t save();
			void replenish();

			/** class abilities */
			void heal_player(player_ptr_t& target);
			uint8_t& stim_pistol_ammo();
			void init();
		protected:
			uint8_t m_stim_pistol_ammo;

		private:
			player_ptr_t m_player;
			medic_orm_t m_orm;
	};

	std::shared_ptr<mods::classes::medic> create_medic(player_ptr_t& player);
};

#endif
