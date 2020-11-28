#ifndef __MENTOC_MODS_CLASSES_FORGE_HEADER__
#define  __MENTOC_MODS_CLASSES_FORGE_HEADER__
#include <variant>
#include "../orm/forge.hpp"
#include "base.hpp"
#include "super-user-fiddler.hpp"

using forge_orm_t = mods::orm::forge;
namespace mods::classes {
	struct forge : base {
		friend class mods::classes::super_user_fiddler;
		using primary_choice_t = mods::weapon::ghost::primary_choice_t;

		long created_at;

		static int16_t destroy(player_ptr_t& player);
		types kind() {
			return types::FORGE;
		}

		/* constructors and destructors */
		forge();
		forge(player_ptr_t);
		~forge() = default;
		void init();

		player_ptr_t 	player();

		void set_player(player_ptr_t);

		/** will return how many turns the attacker will be distracted until it re-engages the player */
		std::pair<int16_t,std::string> distract();

		/** database routines */
		int16_t load_by_player(player_ptr_t&);
		int16_t new_player(player_ptr_t&);
		int64_t db_id() const;
		int16_t save();

		void replenish();
		std::string get_proficiency_by_name(std::string_view prof) const;
		std::string skill_screen() const ;

		private:
			std::vector<uuid_t> m_scanned;
			uint8_t m_claymore_count;
			uint8_t m_cryogenic_grenade_count;
			uint8_t m_flash_underbarrel_charges;
			forge_orm_t	m_orm;
			player_ptr_t m_player;
			skill_familiarity_t m_craft_shotgun_underbarrel_level;
			skill_familiarity_t m_craft_incendiary_underbarrel_level;
			skill_familiarity_t m_craft_flash_underbarrel_level;
			skill_familiarity_t m_craft_silencer_level;
			skill_familiarity_t m_craft_ammunition_level;
			skill_familiarity_t m_craft_armor_plating_level;
			skill_familiarity_t m_craft_high_velocity_ammo_level;
			skill_familiarity_t m_dismantle_weapon_level;
			skill_familiarity_t m_trade_attribute_level;
			skill_familiarity_t m_repair_armor_level;
			skill_familiarity_t m_repair_weapon_level;
			skill_familiarity_t m_reinforce_weapon_level;

			std::deque<obj_ptr_t> m_claymores;
	};
	void forge_advance_level(player_ptr_t& player);
	std::shared_ptr<mods::classes::forge> create_forge(player_ptr_t &player);
};

#endif
