#ifndef __MENTOC_MODS_MOBS_MINI_GUNNER_HEADER__
#define  __MENTOC_MODS_MOBS_MINI_GUNNER_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"

namespace mods::mobs {
	struct mini_gunner {
		static void create(uuid_t uuid, std::string);
		static void free_mob(uuid_t uuid);
		void init();
		mini_gunner() = delete;
		~mini_gunner();
		mini_gunner(uuid_t mob_uuid);
		uuid_t uuid;
		void wear(int where,std::string_view yaml);
		void set_heading(int dir);
		int get_heading() const { return heading; }
		feedback_t& spray(int dir);
		void setup_damage_callbacks();
		int get_weapon_heat() const { return weapon_heat; }
		void set_behaviour_tree(std::string_view name);
		void shout(std::string);
		void set_variation(std::string);
		private:
		std::string variation;
		int weapon_heat;
		int heading;
		int spray_direction;
		player_ptr_t player_ptr;
		bool loaded;
		bool error;
		feedback_t last_attack;
	};
	extern std::map<uuid_t,std::shared_ptr<mini_gunner>> mg_map;
};

static inline std::shared_ptr<mods::mobs::mini_gunner> mini_gunner_ptr(uuid_t u){
	return mods::mobs::mg_map[u];
}
#endif
