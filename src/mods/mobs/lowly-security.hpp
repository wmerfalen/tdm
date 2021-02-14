#ifndef __MENTOC_MODS_MOBS_LOWLY_SECURITY_HEADER__
#define  __MENTOC_MODS_MOBS_LOWLY_SECURITY_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct lowly_security {
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
			lowly_security(uuid_t mob_uuid,std::string variation);
			lowly_security() = delete;
			~lowly_security();

			/**====================================================*/
			/** used to specify which variation of the mini gunner */
			/** npc type we should use. i.e.: "sentinel"           */
			/**====================================================*/
			void set_variation(std::string);

			uuid_t uuid;
			/**====================================================*/
			/** motion control */
			/**====================================================*/
			void set_heading(int dir);
			int get_heading() const {
				return heading;
			}
			void shout(std::string_view);
			void set_behaviour_tree(std::string_view name);

			/**================*/
			/** debugging info */
			/**================*/
			str_map_t report();

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			int get_weapon_heat() const {
				return weapon_heat;
			}
			room_rnum room();
			char_data* cd();
			/** hunting helpers */
			feedback_t& spray(int dir);
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


			/**=================*/
			/** event callbacks */
			/**=================*/
			void setup_damage_callbacks();
			void enemy_spotted(room_rnum room,uuid_t player);

			/**====================================================*/
			/** weapon helpers */
			/**====================================================*/
			obj_ptr_t primary();
			void wear(int where,std::string_view yaml);

		private:

			uint8_t watching;
			std::map<uuid_t,uint64_t> last_seen;
			uuidvec_t hunting;
			std::string variation;
			int weapon_heat;
			int heading;
			int spray_direction;
			player_ptr_t player_ptr;
			bool loaded;
			bool error;
			feedback_t last_attack;
			vec_t<uuid_t> targeting;
	};
	using lowlysec_map_t = std::map<uuid_t,std::shared_ptr<lowly_security>>;
	lowlysec_map_t& lowlysec_map();

};

static inline bool has_lowly_security_ptr(uuid_t u) {
	return mods::mobs::lowlysec_map().find(u) != mods::mobs::lowlysec_map().end();
}

static inline std::shared_ptr<mods::mobs::lowly_security> lowly_security_ptr(uuid_t u) {
	return mods::mobs::lowlysec_map()[u];
}
#endif
