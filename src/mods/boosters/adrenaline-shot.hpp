#ifndef __MENTOC_MODS_WEAPONS_ADRENALINE_SHOT_HEADER__
#define __MENTOC_MODS_WEAPONS_ADRENALINE_SHOT_HEADER__

#include "../weapon.hpp"
#include "../deferred.hpp"

namespace mods::boosters {
	struct adrenaline_shot {
			enum level_t : uint8_t {
				ADSHOT_SMALL = 1,
				ADSHOT_MEDIUM,
				ADSHOT_LARGE
			};
			enum movement_boost_t : uint16_t {
				MOVBOOST_SMALL = 120,
				MOVBOOST_MEDIUM = 280,
				MOVBOOST_LARGE = 500
			};
			enum damage_nerf_t : uint8_t {
				DAMNERF_SMALL = 25,
				DAMNERF_MEDIUM = 45,
				DAMNERF_LARGE = 55
			};
			enum damage_ticks_duration_t : uint16_t {
				DTD_SMALL = 100,
				DTD_MEDIUM = 220,
				DTD_LARGE = 480
			};
			enum deferred_event_t : uint32_t {
				DEF_EVENT_SMALL = mods::deferred::EVENT_PLAYER_UNBLOCK_ADRENALINE_SHOT_SMALL,
				DEF_EVENT_MEDIUM = mods::deferred::EVENT_PLAYER_UNBLOCK_ADRENALINE_SHOT_MEDIUM,
				DEF_EVENT_LARGE = mods::deferred::EVENT_PLAYER_UNBLOCK_ADRENALINE_SHOT_LARGE,
			};
			adrenaline_shot(const level_t level);
			void set_level(const level_t level);
			level_t get_level() const;

			adrenaline_shot();
			~adrenaline_shot() = default;
			const bool& active() const;
			void shot_wears_off(player_ptr_t& injector);

			std::tuple<bool,std::string> inject(player_ptr_t& injector);
		protected:
			bool m_active;
			level_t m_level;
			obj_ptr_t m_obj;
			movement_boost_t m_movement_boost;
			damage_nerf_t m_damage_nerf;
			damage_ticks_duration_t m_ticks;
			deferred_event_t m_deferred_event;
	};
};
#endif

