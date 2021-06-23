#ifndef __MENTOC_MODS_CLASSES_MARINE_HEADER__
#define  __MENTOC_MODS_CLASSES_MARINE_HEADER__
#include "base.hpp"
#include "../orm/marine.hpp"
using marine_orm_t = mods::orm::marine;
namespace mods::classes {
	struct marine : base {
			types kind() {
				return types::MARINE;
			}
			using primary_choice_t = mods::weapon::marine::primary_choice_t;
			enum ability_t : uint8_t {
				NONE = 0,
				/** TODO: need to brainstorm the marine's abilities */
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
			marine();
			marine(player_ptr_t);
			~marine() = default;

			int16_t load_by_player(player_ptr_t&);
			int16_t				new_player(player_ptr_t&, primary_choice_t primary);
			int16_t save();
			void replenish();
			void init();

			player_ptr_t get_player_ptr() override {
				return m_player;
			}
			ability_list_t& get_abilities() override {
				return m_abilities;
			}



		private:
			player_ptr_t m_player;
			marine_orm_t m_orm;
			ability_list_t m_abilities;

			/** TODO: need to brainstorm the marine's abilities */
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
	std::shared_ptr<mods::classes::marine> create_marine(player_ptr_t& player);
};

#endif
