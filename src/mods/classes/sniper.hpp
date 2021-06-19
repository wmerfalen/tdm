#ifndef  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#include "../orm/sniper.hpp"
#include "base.hpp"
#include "../drone.hpp"

using sniper_orm_t = mods::orm::sniper;

namespace mods::classes {
	struct sniper : base {
			types kind() {
				return types::SNIPER;
			}
			using primary_choice_t = mods::weapon::sniper::primary_choice_t;
			using secondary_choice_t = mods::weapon::sniper::secondary_choice_t;

			/* constructors and destructors */
			sniper();
			sniper(player_ptr_t);
			~sniper() = default;

			int16_t load_by_player(player_ptr_t&);
			player_ptr_t player();
			int16_t new_player(player_ptr_t&);
			int16_t save();
			void init();
			void replenish();
			std::tuple<bool,std::string,obj_ptr_t> build_claymore();

			std::string skills_page() override;
			std::tuple<bool,std::string> roll_skill_success(std::string_view skill) override;
			std::tuple<bool,std::string> practice(std::string_view skill) override;

			std::string tracking_shot_proficiency();
			std::string light_bandage_proficiency();
			std::string suture_proficiency();
			std::string adrenaline_shot_proficiency();
			std::string emp_nade_proficiency();
			std::string chaff_nade_proficiency();
			std::string sensor_nade_proficiency();
			std::string ub_shotgun_proficiency();
			std::string ub_frag_proficiency();
			std::string guided_missile_proficiency();
			std::string target_limb_proficiency();
			std::string plant_claymore_proficiency();
			std::string plant_shrapnel_claymore_proficiency();
			std::string plant_corrosive_claymore_proficiency();
			std::string xray_shot_proficiency();
			std::string request_recon_proficiency();

			float tracking_shot_level();
			float light_bandage_level();
			float suture_level();
			float adrenaline_shot_level();
			float emp_nade_level();
			float chaff_nade_level();
			float sensor_nade_level();
			float ub_shotgun_level();
			float ub_frag_level();
			float guided_missile_level();
			float target_limb_level();
			float plant_claymore_level();
			float plant_shrapnel_claymore_level();
			float plant_corrosive_claymore_level();
			float xray_shot_level();
			float request_recon_level();

			void target_died(uuid_t);
			std::tuple<bool,std::string> mark_target(std::string_view target);
			std::tuple<bool,std::string> engage();
			std::tuple<bool,std::string> disengage();


			std::tuple<bool,std::string> tracking_shot(std::string_view target, direction_t direction);

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
	};
	std::shared_ptr<mods::classes::sniper> create_sniper(player_ptr_t& player);
};

#endif
