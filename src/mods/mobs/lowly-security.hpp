#ifndef __MENTOC_MODS_MOBS_LOWLY_SECURITY_HEADER__
#define  __MENTOC_MODS_MOBS_LOWLY_SECURITY_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct lowly_security : public smart_mob {
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
		lowly_security(uuid_t mob_uuid,std::string variation);
		lowly_security() = delete;
		virtual ~lowly_security() = default;

		/**================*/
		/** debugging info */
		/**================*/
		str_map_t report();

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
