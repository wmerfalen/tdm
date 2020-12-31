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
			uuid_t m_target;
			bool m_engaged;
			uint16_t m_xray_shot_charges;
			std::vector<uuid_t> m_claymore_instances;

			player_ptr_t m_player;
			sniper_orm_t m_orm;
			std::vector<uuid_t> m_scanned;
	};
	std::shared_ptr<mods::classes::sniper> create_sniper(player_ptr_t& player);
};

#endif
