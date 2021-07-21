#ifndef  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#include "../orm/sniper.hpp"
#include "base.hpp"
#include "../drone.hpp"
#include "../weapons/attachment-shotgun-underbarrel.hpp"
#include "../weapons/attachment-frag-underbarrel.hpp"

using sniper_orm_t = mods::orm::sniper;

namespace mods::classes {
	struct sniper : base {
			types kind() {
				return types::SNIPER;
			}
			using primary_choice_t = mods::weapon::sniper::primary_choice_t;
			using secondary_choice_t = mods::weapon::sniper::secondary_choice_t;
			using shotgun_ub_t = mods::weapons::attachment_shotgun_underbarrel;
			using frag_ub_t = mods::weapons::attachment_frag_underbarrel;

			enum ability_t {
				NONE = 0,
				TRACKING_SHOT,
				LIGHT_BANDAGE,
				SUTURE,
				ADRENALINE_SHOT,
				EMP_NADE,
				CHAFF_NADE,
				SENSOR_NADE,
				UB_SHOTGUN,
				UB_FRAG,
				GUIDED_MISSILE,
				TARGET_LIMB,
				PLANT_CLAYMORE,
				SHRAPNEL_CLAYMORE,
				CORROSIVE_CLAYMORE,
				XRAY_SHOT,
				REQUEST_RECON
			};

			/* constructors and destructors */
			sniper();
			sniper(player_ptr_t);
			~sniper() = default;

			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int16_t save();
			void init();
			void replenish();
			std::tuple<bool,std::string,obj_ptr_t> build_claymore();
			std::tuple<bool,std::string> attach_shotgun_underbarrel();
			std::tuple<bool,std::string> detach_shotgun_underbarrel();
			std::tuple<bool,std::string> attach_frag_underbarrel();
			std::tuple<bool,std::string> detach_frag_underbarrel();
			std::tuple<bool,std::string> fire_frag(const direction_t& direction,const uint8_t& distance);

			obj_ptr_t underbarrel();

			player_ptr_t get_player_ptr() override {
				return m_player;
			}
			ability_list_t& get_abilities() override {
				return m_abilities;
			}


			void target_died(uuid_t);
			void unblock_healing();
			std::tuple<bool,std::string> mark_target(std::string_view target);
			std::tuple<bool,std::string> engage();
			std::tuple<bool,std::string> disengage();
			void consume_shotgun_underbarrel_ammo();


			std::tuple<bool,std::string> tracking_shot(std::string_view target, direction_t direction);
			std::tuple<bool,std::string> light_bandage();
			std::tuple<bool,std::string> suture();

			/*
			- Ability: X-Ray Shot (can shoot through multiple layers of walls/doors)
				- Can snipe an enemy within a building
				- Can snipe an enemy through walls or doors
				- If enemy not behind cover, it causes 150% damage to target
				*/
			std::tuple<bool,std::string> xray_shot();
			uint8_t claymore_count() const;
			void use_claymore(uuid_t);

		private:
			void replenish_notify(std::string_view);
			uuid_t m_target;
			bool m_engaged;
			uint16_t m_xray_shot_charges;
			uint8_t m_claymore_charges;
			uint8_t m_gauze_count;
			uint8_t m_medkit_count;
			enum heal_mode_t {
				HEAL_MODE_SUTURE,
				HEAL_MODE_LIGHT_BANDAGE
			};

			heal_mode_t m_heal_mode;

			player_ptr_t m_player;
			sniper_orm_t m_orm;
			std::vector<uuid_t> m_scanned;
			uint8_t m_tracking_shot_charges;
			std::map<std::string,bool> m_preferences;

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
	};
	std::shared_ptr<mods::classes::sniper> create_sniper(player_ptr_t& player);
};

#endif
