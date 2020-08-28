#ifndef __MENTOC_MODS_CLASSES_SENTINEL_HEADER__
#define  __MENTOC_MODS_CLASSES_SENTINEL_HEADER__
#include "../../structs.h"
//#include "base.hpp"
#include <memory>
#include "../weapons/smg-mp5.hpp"
#include "../weapons/shotgun-sasg12.hpp"
#include "../weapons/pistol-czp10.hpp"
#include <deque>
#include <memory>
#include <variant>
#include "../orm/class-sentinel.hpp"
#include "../weapon.hpp"

using sentinel_orm_t = mods::orm::sentinel;
namespace mods::classes {
	struct sentinel {
		using primary_choice_t = mods::weapon::sentinel::primary_choice_t;
		enum cure_levels_t {
				SENTINEL_CURE_NONE = 0,
				SENTINEL_CURE_BASIC = 1,
				SENTINEL_CURE_LIGHT = 2,
				SENTINEL_CURE_CRITIC = 3,
				SENTINEL_HEAL = 4
		};
		enum intimidate_levels_t {
			SENTINEL_INTIMIDATE_NONE = 0,
			SENTINEL_INTIMIDATE_BASIC = 1,
			SENTINEL_INTIMIDATE_LIGHT = 2,
			SENTINEL_INTIMIDATE_CRITIC = 3,
			SENTINEL_INTIMIDATE_HEART_ATTACK = 4
		};
		enum human_shield_levels_t {
			SENTINEL_HUMAN_SHIELD_NONE = 0,
			SENTINEL_HUMAN_SHIELD_BASIC  = 1,
			SENTINEL_HUMAN_SHIELD_ABSORB = 2,
			SENTINEL_HUMAN_SHIELD_ON_FIRE = 3,
			SENTINEL_HUMAN_SHIELD_ON_COMBUST = 4
		};
		enum shield_proficiency_levels_t {
			SENTINEL_SHIELD_PROFICIENCY_NONE = 0,
			SENTINEL_SHIELD_PROFICIENCY_BASIC = 1,
			SENTINEL_SHIELD_PROFICIENCY_DEFLECT = 2,
			SENTINEL_SHIELD_PROFICIENCY_DEFLECT_AND_IGNITE = 3
		};
		enum deny_entry_levels_t {
			SENTINEL_DENY_ENTRY_NONE =0,
			SENTINEL_DENY_ENTRY_BASIC = 1,
			SENTINEL_DENY_ENTRY_FIREWALL=2,
			SENTINEL_DENY_ENTRY_EXPELL=3,
			SENTINEL_DENY_ENTRY_BARRICADE=4
		};
		enum gadget_shield_levels_t {
			SENTINEL_GSHIELD_NONE = 0,
			SENTINEL_GSHIELD_BASIC = 1,
			SENTINEL_GSHIELD_DEFLECT =2,
			SENTINEL_GSHIELD_DEFLECT_IGNITE = 3,
			SENTINEL_GSHIELD_FIREWALL = 4
		};
		enum oblique_kick_levels_t {
			SENTINEL_OBLIQUE_KICK_NONE = 0,
			SENTINEL_OBLIQUE_KICK_BASE = 1,
			SENTINEL_OBLIQUE_KICK_HEAVY = 2,
			SENTINEL_OBLIQUE_KICK_EXTREME = 3
		};

		static int16_t destroy(player_ptr_t& player);
		types kind() {
			return types::SENTINEL;
		}

		/* constructors and destructors */
		sentinel();
		sentinel(player_ptr_t);
		~sentinel() = default;

		czp10_ptr_t secondary();

		czp10_ptr_t 	create_czp10(uint64_t);
		mp5_ptr_t 		create_mp5(uint64_t);
		sasg12_ptr_t 	create_sasg12(uint64_t);

		mp5_ptr_t 		mp5();
		sasg12_ptr_t 	sasg12();
		czp10_ptr_t 	czp10();
		player_ptr_t 	player();

		void 					set_player(player_ptr_t);
		void					heal(player_ptr_t& target);
		void					intimidate(player_ptr_t& target);

		/** database routines */
		int16_t       load_by_player(player_ptr_t&);
		int16_t				new_player(player_ptr_t&, std::string_view primary);
		int64_t				db_id() const;
		int16_t       save();
void          sendln(std::string_view msg);
void          psendln(std::string_view msg);
		private:
			mp5_ptr_t 			m_mp5;
			sasg12_ptr_t  	m_sasg12;
			czp10_ptr_t 		m_czp10;
			cure_levels_t		m_heal_level;
			intimidate_levels_t m_intimidate_level;
			sentinel_orm_t	m_orm;
			primary_choice_t m_primary_choice;
			human_shield_levels_t m_human_shield_level;
			deny_entry_levels_t m_deny_entry_level;
			gadget_shield_levels_t m_gadget_shield_level;
			oblique_kick_levels_t m_oblique_kick_level;
			player_ptr_t m_player;
	};
	std::shared_ptr<mods::classes::sentinel> create_sentinel(player_ptr_t &player);

};

#endif
