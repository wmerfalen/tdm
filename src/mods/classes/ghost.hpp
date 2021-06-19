#ifndef __MENTOC_MODS_CLASSES_GHOST_HEADER__
#define  __MENTOC_MODS_CLASSES_GHOST_HEADER__
#include <variant>
#include "../orm/ghost.hpp"
#include "base.hpp"
#include "super-user-fiddler.hpp"
#include "../skills.hpp"

using ghost_orm_t = mods::orm::ghost;
namespace mods::classes {
	struct ghost : base {
			friend class mods::classes::super_user_fiddler;
			using primary_choice_t = mods::weapon::ghost::primary_choice_t;

			long created_at;

			static int16_t destroy(player_ptr_t& player);
			types kind() {
				return types::GHOST;
			}

			/* constructors and destructors */
			ghost();
			ghost(player_ptr_t);
			~ghost() = default;
			void init();

			player_ptr_t 	player();

			void set_player(player_ptr_t);
			void go_stealth();
			void apply_stealth_to(obj_ptr_t& object);
			void apply_stealth_to_player(player_ptr_t&);
			std::pair<int16_t,std::string> feign_death();
			void feign_death_done();
			std::pair<int16_t,std::string> summon_extraction(room_rnum);

			std::string skills_page() {
				return "[stub]";
			}
			void intimidate(uuid_t);
			std::pair<bool,std::string>  pass_through_door_attempt(int direction);

			/** will return how many turns the attacker will be distracted until it re-engages the player */
			std::pair<int16_t,std::string> distract();

			/** requires drone assisted sniping mode */
			std::pair<int16_t,std::string> xray_shot();
			std::vector<uuid_t> get_targets_scanned_by_drone();
			std::vector<uuid_t> get_scanned() const;
			void set_scanned(std::vector<uuid_t>);

			/** database routines */
			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int64_t db_id() const;
			int16_t save();

			void replenish();
			std::string get_proficiency_by_name(std::string_view prof) const;
			std::string skill_screen() const ;

			std::tuple<uint32_t,std::string> fire_penetrating_shot_at(uuid_t npc_uuid);
			std::tuple<bool,std::string> intimidate_target(uuid_t npc_uuid);
			uint8_t cryogenic_grenade_count() const;
			std::tuple<bool,std::string> toss_cryogenic_grenade_towards(uint8_t direction, uint8_t rooms);
			/** applies it to the entire room. every will get flashed */
			std::tuple<bool,std::string> use_flash_underbarrel();

			void use_claymore(uuid_t);
			uint8_t claymore_count() const;

			/* Special ability: dissipate */
			std::tuple<bool,std::string> dissipate();
			void dissipate_wears_off();
			bool is_dissipated() const;
		private:
			uint8_t m_dissipate_charges;
			std::vector<uuid_t> m_scanned;
			std::vector<uuid_t> m_claymore_instances;
			uint8_t m_cryogenic_grenade_count;
			uint8_t m_flash_underbarrel_charges;
			ghost_orm_t	m_orm;
			player_ptr_t m_player;

			skill_familiarity_t m_drone_scan_level;
			skill_familiarity_t m_stealth_level;
			skill_familiarity_t m_summon_extraction_level;
			skill_familiarity_t m_xray_shot_level;
			skill_familiarity_t m_feign_death_level;
			skill_familiarity_t m_plant_claymore_level;
			skill_familiarity_t m_penetrating_shot_level;
			skill_familiarity_t m_intimidation_level;
			skill_familiarity_t m_cryogenic_grenade_level;
			skill_familiarity_t m_flash_underbarrel_level;
			bool m_dissipated;
	};
	void ghost_advance_level(player_ptr_t& player);
	std::shared_ptr<mods::classes::ghost> create_ghost(player_ptr_t& player);
};

#endif
