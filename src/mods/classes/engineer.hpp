#ifndef __MENTOC_MODS_CLASSES_ENGINEER_HEADER__
#define  __MENTOC_MODS_CLASSES_ENGINEER_HEADER__
#include "../orm/engineer.hpp"
#include "base.hpp"

using engineer_orm_t = mods::orm::engineer;
namespace mods::classes {
	struct engineer : base {
			bool has_mana_for_skill(uint16_t skill) {
				return true;
			}
			void use_mana_for_skill(uint16_t skill) {
			}

			types kind() {
				return types::ENGINEER;
			}
			using primary_choice_t = mods::weapon::engineer::primary_choice_t;

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
			std::vector<ability_data_t>& get_abilities() override {
				return m_abilities;
			}
			player_ptr_t get_player_ptr() override {
				return m_player;
			}

			/* constructors and destructors */
			engineer();
			engineer(player_ptr_t);
			~engineer() = default;

			int16_t load_by_player(player_ptr_t&);
			player_ptr_t 	player();
			int16_t				new_player(player_ptr_t&, primary_choice_t primary);
			int16_t save();
			void replenish();
			void init();
			std::tuple<bool,std::string> drone_bomb(direction_t direction,uint8_t rooms);

		private:
			uint8_t m_drone_bomb_charges;
			uint8_t m_sensor_grenade_charges;
			player_ptr_t m_player;
			engineer_orm_t m_orm;
			ability_list_t m_abilities;
	};
	std::shared_ptr<mods::classes::engineer> create_engineer(player_ptr_t& player);
};

#endif
