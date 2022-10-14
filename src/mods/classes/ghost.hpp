#ifndef __MENTOC_MODS_CLASSES_GHOST_HEADER__
#define  __MENTOC_MODS_CLASSES_GHOST_HEADER__
#include <variant>
#include "../orm/ghost.hpp"
#include "base.hpp"
#include "super-user-fiddler.hpp"
#include "../skills.hpp"
#include "../weapons/attachment-shotgun-underbarrel.hpp"
#include "../weapons/attachment-frag-underbarrel.hpp"
#include "../boosters/adrenaline-shot.hpp"

using ghost_orm_t = mods::orm::ghost;
namespace mods::classes {
	struct ghost : base {
			friend class mods::classes::super_user_fiddler;
			static constexpr std::string_view CRYOGENIC_GRENADE_YAML = "explosive/cryogenic-grenade.yml";
			using primary_choice_t = mods::weapon::ghost::primary_choice_t;

			using shotgun_ub_t = mods::weapons::attachment_shotgun_underbarrel;
			using frag_ub_t = mods::weapons::attachment_frag_underbarrel;
			using adrenaline_shot_t = mods::boosters::adrenaline_shot;

			enum ability_t {
				NONE = 0,
				AERIAL_DRONE_SCAN,
				STEALTH,
				SUMMON_EXTRACTION,
				XRAY_SHOT,
				FEIGN_DEATH,
				PLANT_CLAYMORE,
				PENETRATING_SHOT,
				INTIMIDATION,
				CRYOGENIC_GRENADE,
				FLASH_UNDERBARREL,
				TRACKING_SHOT,
				LIGHT_BANDAGE,
				SUTURE,
				ADRENALINE_SHOT,
				//EMP_NADE,
				//CHAFF_NADE,
				SENSOR_NADE,
				UB_SHOTGUN,
				UB_FRAG,
				GUIDED_MISSILE,
				TARGET_LIMB,
				SHRAPNEL_CLAYMORE,
				CORROSIVE_CLAYMORE,
				REQUEST_RECON,
				MARK_TARGET,
			};
			std::vector<ability_data_t>& get_abilities() {
				return m_abilities;
			}

			player_ptr_t get_player_ptr() override {
				return m_player;
			}

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

			void set_player(player_ptr_t);
			void go_stealth();
			void apply_stealth_to(obj_ptr_t& object);
			void apply_stealth_to_player(player_ptr_t&);
			std::pair<int16_t,std::string> feign_death();
			void feign_death_done();
			std::pair<int16_t,std::string> summon_extraction(room_rnum);

			void intimidate(uuid_t);
			std::pair<bool,std::string>  pass_through_door_attempt(int direction);

			/** will return how many turns the attacker will be distracted until it re-engages the player */
			std::pair<int16_t,std::string> distract();

			/** requires drone assisted sniping mode */
			std::tuple<bool,std::string> xray_shot();
			std::vector<uuid_t> get_targets_scanned_by_drone();
			std::vector<uuid_t> get_scanned() const;
			void set_scanned(std::vector<uuid_t>);

			/** database routines */
			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int64_t db_id() const;
			int16_t save();

			void replenish();

			std::tuple<uint32_t,std::string> fire_penetrating_shot_at(uuid_t npc_uuid);
			std::tuple<bool,std::string> intimidate_target(uuid_t npc_uuid);
			uint8_t cryogenic_grenade_count() const;
			std::tuple<bool,std::string> toss_cryogenic_grenade_towards(const direction_t& direction, uint8_t rooms);

			/** applies it to the entire room. every will get flashed */
			std::tuple<bool,std::string> use_flash_underbarrel();

			void use_claymore(uuid_t);
			uint8_t claymore_count() const;

			/* Special ability: dissipate */
			std::tuple<bool,std::string> dissipate();
			void dissipate_wears_off();
			bool is_dissipated() const;

			/** SNIPER IMPORT */
			std::tuple<bool,std::string,obj_ptr_t> build_claymore();
			std::tuple<bool,std::string,obj_ptr_t> build_corrosive_claymore();
			std::tuple<bool,std::string,obj_ptr_t> build_shrapnel_claymore();
			std::tuple<bool,std::string> attach_shotgun_underbarrel();
			std::tuple<bool,std::string> detach_shotgun_underbarrel();
			std::tuple<bool,std::string> attach_frag_underbarrel();
			std::tuple<bool,std::string> detach_frag_underbarrel();
			std::tuple<bool,std::string> fire_frag(const direction_t& direction,const uint8_t& distance);

			obj_ptr_t underbarrel();

			/** Return the wrapper class */
			shotgun_ub_t& get_shotgun_underbarrel_wrapper();
			frag_ub_t& get_frag_underbarrel_wrapper();

			void target_died(uuid_t);
			void unblock_healing();
			std::tuple<bool,std::string> mark_target(std::string_view target);
			std::tuple<bool,std::string> engage();
			std::tuple<bool,std::string> disengage();
			void consume_shotgun_underbarrel_ammo();


			std::tuple<bool,std::string> tracking_shot(std::string_view target, direction_t direction);
			std::tuple<bool,std::string> light_bandage();
			std::tuple<bool,std::string> suture();
			std::tuple<bool,std::string> inject_adrenaline_shot();

			/*
			- Ability: X-Ray Shot (can shoot through multiple layers of walls/doors)
				- Can snipe an enemy within a building
				- Can snipe an enemy through walls or doors
				- If enemy not behind cover, it causes 150% damage to target
				*/
			void unblock_adrenaline_shot();

			bool can_toss_grenade_towards(const direction_t& direction);
		private:
			void replenish_notify(std::string_view);
			uuid_t m_target;
			bool m_engaged;
			uint16_t m_xray_shot_charges;
			uint8_t m_gauze_count;
			uint8_t m_medkit_count;
			uint8_t m_adrenaline_shot_charges;
			enum heal_mode_t {
				HEAL_MODE_SUTURE,
				HEAL_MODE_LIGHT_BANDAGE
			};

			heal_mode_t m_heal_mode;

			player_ptr_t m_player;
			std::vector<uuid_t> m_scanned;
			uint8_t m_tracking_shot_charges;
			std::map<std::string,bool> m_preferences;

			skill_t m_mark_target;
			skill_t m_tracking_shot;
			skill_t m_light_bandage;
			skill_t m_suture;
			skill_t m_adrenaline_shot;
			skill_t m_emp_nade;
			skill_t m_chaff_nade;
			skill_t m_sensor_nade;
			skill_t m_ub_shotgun;
			skill_t m_ub_frag;
			skill_t m_guided_missile;
			skill_t m_target_limb;
			skill_t m_plant_claymore;
			skill_t m_plant_shrapnel_claymore;
			skill_t m_plant_corrosive_claymore;
			skill_t m_xray_shot;
			skill_t m_request_recon;
			ability_list_t m_abilities;
			shotgun_ub_t m_shotgun_ub;
			frag_ub_t m_frag_ub;
			adrenaline_shot_t m_ad_shot;
			/** END SNIPER IMPORT */

			uint8_t m_dissipate_charges;

			uint8_t m_claymore_count;
			uint8_t m_corrosive_claymore_count;
			uint8_t m_shrapnel_claymore_count;

			uint8_t m_cryogenic_grenade_count;
			uint8_t m_flash_underbarrel_charges;
			ghost_orm_t	m_orm;

			skill_t m_cryogenic_grenade;
			skill_t m_drone_scan;
			skill_t m_feign_death;
			skill_t m_flash_underbarrel;
			skill_t m_intimidation;
			skill_t m_penetrating_shot;
			skill_t m_stealth;
			skill_t m_summon_extraction;
			bool m_dissipated;
			uint16_t m_call_count;
	};
	void ghost_advance_level(player_ptr_t& player);
	std::shared_ptr<mods::classes::ghost> create_ghost(player_ptr_t& player);
};

#endif
