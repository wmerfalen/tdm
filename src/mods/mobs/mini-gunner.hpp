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
		private:
		bool loaded;
		bool error;
	};
};

#endif
