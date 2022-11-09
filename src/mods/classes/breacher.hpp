#ifndef __MENTOC_MODS_CLASSES_BREACHER_HEADER__
#define  __MENTOC_MODS_CLASSES_BREACHER_HEADER__
#include <variant>
#include "../orm/breacher.hpp"
#include "base.hpp"

using breacher_orm_t = mods::orm::breacher;
namespace mods::classes {
	struct breacher : base {
			static constexpr std::string_view description = "{grn}BREACHERS{/grn}\r\n"
			    "Breachers are a melee and shotgun class. They can take and deal massive amounts "
			    "of damage. Think of the average rogue/assassin, and you will see a partial resemblance "
			    "in the playstyle.\r\n"
			    "Their ability to wear heavy armor with small movement penalties makes them the perfect "
			    "tank element to an group dynamic.";
			bool has_mana_for_skill(uint16_t skill) {
				return true;
			}
			void use_mana_for_skill(uint16_t skill) {
			}

			using primary_choice_t = mods::weapon::breacher::primary_choice_t;
			static int16_t destroy(player_ptr_t& player);
			types kind() {
				return types::BREACHER;
			}

			/* constructors and destructors */
			breacher();
			breacher(player_ptr_t);
			~breacher() = default;

			player_ptr_t 	player();

			void set_player(player_ptr_t);
			std::pair<int16_t,std::string> teep(direction_t direction);
			void replenish();

			/** database routines */
			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int64_t db_id() const;
			int16_t save();
			std::tuple<bool,std::string> explosive_shot(direction_t direction);
			void attempt_direction(direction_t dir);
			player_ptr_t get_player_ptr() override {
				return m_player;
			}
			ability_list_t& get_abilities() override {
				return m_abilities;
			}
		private:
			std::map<std::pair<room_rnum,direction_t>,uint8_t> m_push_count;
			breacher_orm_t	m_orm;
			player_ptr_t m_player;
			uint8_t m_explosive_shot_charges;

			int /** todo replace iwth skill_familiarity_t */ m_teep_level;
			ability_list_t m_abilities;

	};
	std::shared_ptr<mods::classes::breacher> create_breacher(player_ptr_t& player);
};


#endif
