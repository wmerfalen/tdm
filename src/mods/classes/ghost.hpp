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

namespace mods::combat_composer::phases {
	struct calculated_damage_t;
};

using ghost_orm_t = mods::orm::ghost;
namespace mods::classes {
	static constexpr uint8_t LENSE_AERIAL_DRONE_SCAN = 1;
	struct ghost : base {
			bool has_mana_for_skill(uint16_t skill);
			void use_mana_for_skill(uint16_t skill);

			friend class mods::classes::super_user_fiddler;
			static constexpr std::string_view CRYOGENIC_GRENADE_YAML = "explosive/cryogenic-grenade.yml";
			static constexpr std::string_view PENETRATING_SHOT_RIFLE_YAML = "rifle/ax799-fantom.yml";
			using primary_choice_t = mods::weapon::ghost::primary_choice_t;

			using shotgun_ub_t = mods::weapons::attachment_shotgun_underbarrel;
			using frag_ub_t = mods::weapons::attachment_frag_underbarrel;
			using adrenaline_shot_t = mods::boosters::adrenaline_shot;

			enum ability_t {
				NONE = 0,
				AERIAL_DRONE_SCAN,
				DISSIPATE,
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
			std::vector<ability_data_t>& get_abilities();

			player_ptr_t get_player_ptr() override;

			long created_at;

			static int16_t destroy(player_ptr_t& player);
			types kind() {
				return types::GHOST;
			}

			/* constructors and destructors */
			ghost();
			ghost(player_ptr_t);
			ghost(const ghost& copy);
			~ghost();
			void init();
			void toggle_debug_for(std::string_view field);

			void set_player(player_ptr_t);
			void go_stealth();
			void apply_stealth_to(obj_ptr_t& object);
			void apply_stealth_to_player(player_ptr_t&);
			std::pair<int16_t,std::string> feign_death();
			void feign_death_done();

			std::pair<bool,std::string>  pass_through_door_attempt(int direction);

			/** will return how many turns the attacker will be distracted until it re-engages the player */
			std::pair<int16_t,std::string> distract();

			/** requires drone assisted sniping mode */
			std::tuple<bool,std::string> xray_shot();
			std::vector<uuid_t> get_targets_scanned_by_drone();
			std::vector<uuid_t> get_scanned() const;
			void set_scanned(std::vector<uuid_t>);

			/** aerial drone scan ability */
			std::tuple<bool,std::string> aerial_drone_scan();
			void set_aerial_drone_scan(bool on_off);
			const bool& has_aerial_drone_scan() const;
			const uint8_t& overhead_lense() const;

			/** database routines */
			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int64_t db_id() const;
			int16_t save();

			void replenish();

			std::tuple<bool,std::string> fire_penetrating_shot_at(const direction_t& direction);
			std::tuple<bool,std::string> intimidate_target(uuid_t npc_uuid);
			uint8_t cryogenic_grenade_count() const;
			std::tuple<bool,std::string> toss_cryogenic_grenade_towards(const direction_t& direction, uint8_t rooms);

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
			std::tuple<bool,std::string> use_flash_underbarrel(const uuid_t& npc_uuid);


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

			void apply_penetrating_shot_mods(mods::combat_composer::phases::calculated_damage_t& damage);
			bool is_penetrating_shot();

		private:
			uint8_t m_overhead_lense;
			bool m_has_aerial_drone_scan;
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
			uint8_t m_penetrating_shot_count;
			uint8_t m_flash_underbarrel_charges;
			ghost_orm_t	m_orm;

			skill_t m_cryogenic_grenade;
			skill_t m_aerial_drone_scan;
			skill_t m_feign_death;
			skill_t m_flash_underbarrel;
			skill_t m_intimidation;
			skill_t m_penetrating_shot;
			skill_t m_dissipate;
			bool m_dissipated;
			uint16_t m_call_count;
			bool m_is_penetrating_shot;
			obj_ptr_t m_fantom;
			bool m_debugging_aerial_drone_scan;
			bool m_debugging_penetrating_shot;
	};
	void ghost_advance_level(player_ptr_t& player);
	std::shared_ptr<mods::classes::ghost> create_ghost(player_ptr_t& player);
};

#endif
