#ifndef __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
#define  __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
#include "../orm/class-contagion.hpp"
#include "base.hpp"

using contagion_orm_t = mods::orm::contagion;
namespace mods::classes {
	struct contagion : base {
			types kind() {
				return types::CONTAGION;
			}
			using primary_choice_t = mods::weapon::contagion::primary_choice_t;

			static int16_t destroy(player_ptr_t& player);

			/* constructors and destructors */
			contagion();
			contagion(player_ptr_t);
			~contagion() = default;

			int16_t load_by_player(player_ptr_t&);
			player_ptr_t 	player();
			int16_t				new_player(player_ptr_t&, primary_choice_t primary); /** TODO: create secondary enum and pass as 3rd param */
			int16_t save();

			player_ptr_t get_player_ptr() override {
				return m_player;
			}
			ability_list_t& get_abilities() override {
				return m_abilities;
			}
		private:
			player_ptr_t m_player;
			contagion_orm_t m_orm;
			ability_list_t m_abilities;
	};
	std::shared_ptr<mods::classes::contagion> create_contagion(player_ptr_t& player);
};

#endif
