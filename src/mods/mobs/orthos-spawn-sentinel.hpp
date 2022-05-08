#ifndef __MENTOC_MODS_MOBS_orthos_spawn_sentinel_HEADER__
#define  __MENTOC_MODS_MOBS_orthos_spawn_sentinel_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"
#include <stack>
#include <forward_list>

namespace mods::mobs {
	using tick_t = uint64_t;
	struct orthos_spawn_sentinel : public smart_mob {
			static constexpr uint16_t SHOULD_DO_ROAM = 0;
			static constexpr uint16_t SHOULD_DO_RANDOM_TRIVIAL = 1;
			/**====================================================*/
			/** factory and destruct */
			/**====================================================*/
			static void create(uuid_t uuid, std::string);
			static void free_mob(uuid_t uuid);

			/**====================================================*/
			/** initialization routine called by constructor */
			/** can be called anytime after construct if needed */
			/**====================================================*/
			void init();

			/**====================================================*/
			/** preferred construct method */
			/**====================================================*/
			orthos_spawn_sentinel(uuid_t mob_uuid,std::string variation);
			orthos_spawn_sentinel() = delete;
			virtual ~orthos_spawn_sentinel() = default;

			/**================*/
			/** debugging info */
			/**================*/
			str_map_t report();
			str_map_t usages();

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			/** hunting helpers */
			feedback_t& spray(uint8_t dir);

			/**=================*/
			/** event callbacks */
			/**=================*/
			void setup_damage_callbacks();
			void enemy_spotted(room_rnum room,uuid_t player);
			void set_variation(std::string);
			void attacked(const feedback_t& feedback);
			void shotgun_attack_within_range();
			std::pair<bool,std::string> move_to(const direction_t& dir);
			void move_closer_to_target();
			std::string_view type() {
				return "orthos_spawn_sentinel";
			}
			void attack_mode();
			void watch_everywhere();

		private:
			player_ptr_t get_next_attacking_priority();
			player_ptr_t m_last_attacker;
			std::forward_list<player_ptr_t> m_attackers;
			obj_ptr_t m_weapon;
			std::optional<direction_t> m_attackers_last_direction;

			bool m_watching_everywhere;
			std::shared_ptr<mods::ranged_combat_totals> RCT;
	};
	using orthos_spawn_sentinel_map_t = std::map<uuid_t,std::shared_ptr<orthos_spawn_sentinel>>;
	orthos_spawn_sentinel_map_t& orthos_spawn_sentinel_map();

};

static inline bool has_orthos_spawn_sentinel_ptr(const uuid_t& u) {
	return mods::mobs::orthos_spawn_sentinel_map().find(u) != mods::mobs::orthos_spawn_sentinel_map().end();
}

static inline std::shared_ptr<mods::mobs::orthos_spawn_sentinel> orthos_spawn_sentinel_ptr(const uuid_t& u) {
	return mods::mobs::orthos_spawn_sentinel_map()[u];
}
#endif
