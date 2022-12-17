#ifndef __MENTOC_MODS_CLASSES_MARINE_HEADER__
#define  __MENTOC_MODS_CLASSES_MARINE_HEADER__
#include "base.hpp"
#include "../orm/marine.hpp"
using marine_orm_t = mods::orm::marine;
namespace mods::classes {
	struct marine : base {
			static constexpr std::string_view M203_UNDERBARREL = "m203-grenade-launcher-underbarrel.yml";
			static constexpr std::string_view TRACER_ROUNDS = "marine-class-tracer-rounds.yml";
			types kind() {
				return types::MARINE;
			}
			using primary_choice_t = mods::weapon::marine::primary_choice_t;
			enum ability_t : uint8_t {
				NONE = 0,
				LOAD_TRACER_ROUNDS,
				FIELD_STRIP_WEAPON,
				ATTACH_M203,
				ATTACH_M203_X12,
				ATTACH_FLASH_SURPRESSOR,
				TEEP_KICK,
				SINGLE_LEG_TAKEDOWN,
				DOUBLE_LEG_TAKEDOWN,
				UCHI_MATA,
				MOROTE_SEOI_NAGE,
				TOMOE_NAGE,
				THERMAL_VISION,
				NIGHT_VISION,
				DEPLOY_SMOKE,
				SPECIALIZE_IN,

				BENZO_STIMULI_SHOT,
				LIGHT_BANDAGE,
				SUTURE,
				ADRENALINE_SHOT,
				TARGET_LIMB,
				PLANT_CLAYMORE,
				SHRAPNEL_CLAYMORE,
				CORROSIVE_CLAYMORE,
				REQUEST_RECON,
				REQUEST_SMOKE_SCREEN,
				REQUEST_DRONE_STRIKE,

				REQUEST_DRONE_RECON,
			};

			static constexpr std::array<ability_t,5> grappling_skill_levels = {
				SINGLE_LEG_TAKEDOWN,
				DOUBLE_LEG_TAKEDOWN,
				UCHI_MATA,
				MOROTE_SEOI_NAGE,
				TOMOE_NAGE,
			};
			static constexpr std::array<ability_t,5> core_abilities = {
				ATTACH_M203,
				THERMAL_VISION,
				NIGHT_VISION,
				DEPLOY_SMOKE,
				SPECIALIZE_IN,
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

			bool has_mana_for_skill(uint16_t skill) {
				return true;
			}
			void use_mana_for_skill(uint16_t skill) {
			}
			bool is_core_ability(const ability_t&) const ;

			cmd_report_t attach_m203(const uuid_t& wpn);
			cmd_report_t load_tracer_rounds(const uuid_t& wpn);

		private:
			skill_t* m_get_skill(const ability_t& ab);
			player_ptr_t m_player;
			marine_orm_t m_orm;
			ability_list_t m_abilities;


			skill_t m_load_tracer_rounds;
			skill_t m_field_strip_weapon;
			skill_t m_attach_m203;
			skill_t m_attach_m203_x12;
			skill_t m_attach_flash_surpressor;
			skill_t m_teep_kick;
			skill_t m_single_leg_takedown;
			skill_t m_double_leg_takedown;
			skill_t m_uchi_mata;
			skill_t m_morote_seoi_nage;
			skill_t m_tomoe_nage;
			skill_t m_thermal_vision;
			skill_t m_night_vision;
			skill_t m_deploy_smoke;
			skill_t m_specialize_in;
			skill_t m_benzo_stimuli_shot;
			skill_t m_light_bandage;
			skill_t m_suture;
			skill_t m_adrenaline_shot;
			skill_t m_target_limb;
			skill_t m_plant_claymore;
			skill_t m_shrapnel_claymore;
			skill_t m_corrosive_claymore;
			skill_t m_request_recon;
			skill_t m_request_smoke_screen;
			skill_t m_request_drone_strike;
			skill_t m_request_drone_recon;

			obj_ptr_t m_m203;
			obj_ptr_t m_tracer_rounds;
	};
	std::shared_ptr<mods::classes::marine> create_marine(player_ptr_t& player);
};

#endif
