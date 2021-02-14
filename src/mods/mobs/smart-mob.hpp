#ifndef __MENTOC_MODS_MOBS_SMART_MOB_HEADER__
#define  __MENTOC_MODS_MOBS_SMART_MOB_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct smart_mob {
			/**====================================================*/
			/** factory and destruct */
			/**====================================================*/
			static void create(uuid_t uuid, std::string);
			static void free_mob(uuid_t uuid);

			/**====================================================*/
			/** initialization routine called by constructor */
			/** can be called anytime after construct if needed */
			/**====================================================*/
			virtual void init();

			/**====================================================*/
			/** preferred construct method */
			/**====================================================*/
			smart_mob(uuid_t mob_uuid,std::string variation);
			smart_mob();
			virtual ~smart_mob();

			/**====================================================*/
			/** used to specify which variation of the mini gunner */
			/** npc type we should use. i.e.: "sentinel"           */
			/**====================================================*/
			virtual void set_variation(std::string);

			uuid_t uuid;
			/**====================================================*/
			/** motion control */
			/**====================================================*/
			void set_heading(uint8_t dir);
			int get_heading() const {
				return heading;
			}
			void shout(std::string_view);
			void set_behaviour_tree(std::string_view name);

			/**================*/
			/** debugging info */
			/**================*/
			virtual str_map_t report() = 0;

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			int get_weapon_heat() const {
				return weapon_heat;
			}
			room_rnum room();
			char_data* cd();
			/** hunting helpers */
			void watch_nothing();
			void watch_heading();
			void watch(uint8_t direction);
			void watch_directions(vec_t<uint8_t> direction);
			void set_hunting(const uuidvec_t&);
			uuidvec_t& get_hunting();
			tick_t get_last_seen(uuid_t player);
			tick_t get_last_seen_diff(uuid_t player);
			void forget(uuid_t);
			player_ptr_t& player() {
				return player_ptr;
			}
			void save_targets(vec_t<uuid_t>&);
			void save_hostile_targets(vec_t<uuid_t>&);
			const vec_t<uuid_t> get_hostile_targets() const;

			void hunt_hostile_targets();

			/**=================*/
			/** event callbacks */
			/**=================*/
			virtual void setup_damage_callbacks();
			virtual void enemy_spotted(room_rnum room,uuid_t player);

			/**====================================================*/
			/** weapon helpers */
			/**====================================================*/
			obj_ptr_t primary();
			void wear(int where,std::string_view yaml);

		protected:
			vec_t<uuid_t> hostile_targets;
			uint8_t watching;
			std::map<uuid_t,uint64_t> last_seen;
			uuidvec_t hunting;
			std::string variation;
			int weapon_heat;
			uint8_t heading;
			uint8_t spray_direction;
			player_ptr_t player_ptr;
			bool loaded;
			bool error;
			feedback_t last_attack;
			vec_t<uuid_t> targeting;
	};
};
#endif
