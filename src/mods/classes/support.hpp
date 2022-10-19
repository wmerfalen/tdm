#ifndef __MENTOC_MODS_CLASSES_SUPPORT_HEADER__
#define  __MENTOC_MODS_CLASSES_SUPPORT_HEADER__
#include "../orm/support.hpp"
#include "base.hpp"

using support_orm_t = mods::orm::support;
namespace mods::classes {
	struct support : base {
			bool has_mana_for_skill(uint16_t skill) {
				return true;
			}
			void use_mana_for_skill(uint16_t skill) {
			}

			types kind() {
				return types::SUPPORT;
			}
			using primary_choice_t = mods::weapon::support::primary_choice_t;

			/* constructors and destructors */
			support();
			support(player_ptr_t);
			~support() = default;

			std::string yaml_file(primary_choice_t f);
			int16_t load_by_player(player_ptr_t&);
			player_ptr_t 	player();
			int16_t				new_player(player_ptr_t&, primary_choice_t primary);
			int16_t save();
			void replenish();

			player_ptr_t get_player_ptr() override {
				return m_player;
			}
			ability_list_t& get_abilities() override {
				return m_abilities;
			}
		private:
			player_ptr_t m_player;
			support_orm_t m_orm;
			ability_list_t m_abilities;
	};
	std::shared_ptr<mods::classes::support> create_support(player_ptr_t& player);


};

#endif
