#ifndef __MENTOC_MODS_CLASSES_STRIKER_HEADER__
#define  __MENTOC_MODS_CLASSES_STRIKER_HEADER__
#include <variant>
#include "../orm/striker.hpp"
#include "base.hpp"

using striker_orm_t = mods::orm::striker;
namespace mods::classes {
	struct striker : base {
		using teep_levels_t = striker_orm_t::teep_levels_t;
		using stealth_levels_t = striker_orm_t::stealth_levels_t;
		using summon_extraction_levels_t = striker_orm_t::summon_extraction_levels_t;
		using xray_shot_levels_t = striker_orm_t::xray_shot_levels_t;
		using feign_death_levels_t = striker_orm_t::feign_death_levels_t;

		static int16_t destroy(player_ptr_t& player);
		types kind() {
			return types::STRIKER;
		}

		/* constructors and destructors */
		striker();
		striker(player_ptr_t);
		~striker() = default;

		player_ptr_t 	player();

		void set_player(player_ptr_t);
		void go_stealth();
		void apply_stealth_to(obj_ptr_t& object);
		void apply_stealth_to_player(player_ptr_t&);
		std::pair<int16_t,std::string> teep(direction_t direction);
		void feign_death_done();
		std::pair<int16_t,std::string> summon_extraction(room_rnum);
		/** requires drone assisted sniping mode */
		std::pair<int16_t,std::string> xray_shot();

		/** database routines */
		int16_t load_by_player(player_ptr_t&);
		int16_t new_player(player_ptr_t&);
		int64_t db_id() const;
		int16_t save();
		private:
			striker_orm_t	m_orm;
			player_ptr_t m_player;

			teep_levels_t m_teep_level;
			stealth_levels_t m_stealth_level;
			summon_extraction_levels_t m_summon_extraction_level;
			xray_shot_levels_t m_xray_shot_level;
			feign_death_levels_t m_feign_death_level;

	};
	std::shared_ptr<mods::classes::striker> create_striker(player_ptr_t &player);
};


#endif
