#ifndef __MENTOC_MODS_MOBS_mp_shotgunner_HEADER__
#define  __MENTOC_MODS_MOBS_mp_shotgunner_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct mp_shotgunner : public smart_mob {
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
		mp_shotgunner(uuid_t mob_uuid,std::string variation);
		mp_shotgunner() = delete;
		virtual ~mp_shotgunner() = default;

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
	using mpshotgunner_map_t = std::map<uuid_t,std::shared_ptr<mp_shotgunner>>;
	mpshotgunner_map_t& mpshotgunner_map();

};

static inline bool has_mp_shotgunner_ptr(uuid_t u) {
	return mods::mobs::mpshotgunner_map().find(u) != mods::mobs::mpshotgunner_map().end();
}

static inline std::shared_ptr<mods::mobs::mp_shotgunner> mp_shotgunner_ptr(uuid_t u) {
	return mods::mobs::mpshotgunner_map()[u];
}
#endif
