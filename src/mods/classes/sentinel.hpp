#ifndef __MENTOC_MODS_CLASSES_SENTINEL_HEADER__
#define  __MENTOC_MODS_CLASSES_SENTINEL_HEADER__
#include "../../structs.h"
#include "base.hpp"
#include <memory>
#include "../weapons/smg-mp5.hpp"
#include "../weapons/shotgun-sasg12.hpp"
#include "../weapons/pistol-czp10.hpp"
#include <deque>
#include <memory>
#include <variant>
#include "../orm/class-sentinel.hpp"
#include "../weapon.hpp"

namespace mods::classes {
	struct sentinel : base {
		using sentinel_orm_t = mods::orm::sentinel;
		using primary_choice_t = mods::weapon::sentinel::primary_choice_t;
		enum cure_levels_t {
				SENTINEL_CURE_NONE = 0,
				SENTINEL_CURE_LIGHT,
				SENTINEL_CURE_CRITIC,
				SENTINEL_HEAL
		};
		enum intimidate_levels_t {
			SENTINEL_INTIMIDATE_NONE = 0,
			SENTINEL_INTIMIDATE_LIGHT,
			SENTINEL_INTIMIDATE_CRITIC,
			SENTINEL_INTIMIDATE_HEART_ATTACK
		};
		static std::shared_ptr<sentinel> create(player_ptr_t &player);
		static int16_t destroy(player_ptr_t& player);
		types kind() {
			return types::SENTINEL;
		}

		/* constructors and destructors */
		sentinel();
		sentinel(player_ptr_t);
		~sentinel() = default;

		czp10_ptr_t secondary();

		czp10_ptr_t 	create_czp10();
		mp5_ptr_t 		create_mp5();
		sasg12_ptr_t 	create_sasg12();

		mp5_ptr_t 		mp5();
		sasg12_ptr_t 	sasg12();
		czp10_ptr_t 	czp10();
		player_ptr_t 	player();
		void 					set_player(player_ptr_t);
		void					heal(player_ptr_t& target);
		void					intimidate(player_ptr_t& target);

		/** database routines */
		int16_t				feed(mentoc_pqxx_result_t);
		int16_t				new_player(player_ptr_t&, primary_choice_t);
		int64_t				db_id() const;
		private:
			player_ptr_t 		m_player;
			mp5_ptr_t 			m_mp5;
			sasg12_ptr_t  	m_sasg12;
			czp10_ptr_t 		m_czp10;
			cure_levels_t		m_heal_level;
			intimidate_levels_t m_intimidate_level;
			sentinel_orm_t	m_orm;
			primary_choice_t m_primary_choice;
	};

};

#endif
