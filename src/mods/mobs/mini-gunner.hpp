#ifndef __MENTOC_MODS_MOBS_MINI_GUNNER_HEADER__
#define  __MENTOC_MODS_MOBS_MINI_GUNNER_HEADER__

#include "../../globals.hpp"

namespace mods::mobs {
	struct mini_gunner {
		static void create(uuid_t uuid);
		static void free_mob(uuid_t uuid);
		void init();
		mini_gunner() = delete;
		~mini_gunner();
		mini_gunner(uuid_t mob_uuid);
		uuid_t uuid;
		void wear(int where,std::string_view yaml);
		void set_heading(int dir);
		void spray(int dir);
		private:
		int heading;
		int spray_direction;
		player_ptr_t player_ptr;
		bool loaded;
		bool error;
	};
	extern std::map<uuid_t,std::shared_ptr<mini_gunner>> mg_map;
};

static inline std::shared_ptr<mods::mobs::mini_gunner> mini_gunner_ptr(uuid_t u){
	return mods::mobs::mg_map[u];
}
#endif
