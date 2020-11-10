#ifndef __MENTOC_MODS_CLASSES_GHOST_HEADER__
#define  __MENTOC_MODS_CLASSES_GHOST_HEADER__
#include <variant>
#include "../orm/ghost.hpp"
#include "base.hpp"
#include "super-user-fiddler.hpp"

using ghost_orm_t = mods::orm::ghost;
namespace mods::classes {
	struct ghost : base {
		friend class mods::classes::super_user_fiddler;
		using primary_choice_t = mods::weapon::ghost::primary_choice_t;
		using drone_scan_levels_t = ghost_orm_t::drone_scan_levels_t;
		using stealth_levels_t = ghost_orm_t::stealth_levels_t;
		using summon_extraction_levels_t = ghost_orm_t::summon_extraction_levels_t;
		using xray_shot_levels_t = ghost_orm_t::xray_shot_levels_t;
		using feign_death_levels_t = ghost_orm_t::feign_death_levels_t;

		static int16_t destroy(player_ptr_t& player);
		types kind() {
			return types::GHOST;
		}

		/* constructors and destructors */
		ghost();
		ghost(player_ptr_t);
		~ghost() = default;

		player_ptr_t 	player();

		void set_player(player_ptr_t);
		void go_stealth();
		void apply_stealth_to(obj_ptr_t& object);
		void apply_stealth_to_player(player_ptr_t&);
		std::pair<int16_t,std::string> feign_death();
		void feign_death_done();
		std::pair<int16_t,std::string> summon_extraction(room_rnum);

		void plant_claymore();
		void intimidate(uuid_t);
		std::pair<bool,std::string>  pass_through_door_attempt(int direction);

		/** will return how many turns the attacker will be distracted until it re-engages the player */
		std::pair<int16_t,std::string> distract();

		/** requires drone assisted sniping mode */
		std::pair<int16_t,std::string> xray_shot();

		/** database routines */
		int16_t load_by_player(player_ptr_t&);
		int16_t new_player(player_ptr_t&);
		int64_t db_id() const;
		int16_t save();
		private:
			ghost_orm_t	m_orm;
			player_ptr_t m_player;

			drone_scan_levels_t m_drone_scan_level;
			stealth_levels_t m_stealth_level;
			summon_extraction_levels_t m_summon_extraction_level;
			xray_shot_levels_t m_xray_shot_level;
			feign_death_levels_t m_feign_death_level;

	};
	std::shared_ptr<mods::classes::ghost> create_ghost(player_ptr_t &player);
};


#endif
