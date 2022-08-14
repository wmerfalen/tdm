#ifndef __MENTOC_MODS_MOBS_innocent_HEADER__
#define  __MENTOC_MODS_MOBS_innocent_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"
#include <stack>
#include <forward_list>

namespace mods::mobs {
	struct innocent : public smart_mob {
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
		innocent(uuid_t mob_uuid,std::string variation);
		innocent() = delete;
		virtual ~innocent() = default;

		/**================*/
		/** debugging info */
		/**================*/
		str_map_t report();
		str_map_t usages();

		/**=================*/
		/** event callbacks */
		/**=================*/
		void setup_damage_callbacks();
		void set_variation(std::string);
		std::pair<bool,std::string> move_to(const direction_t& dir);

		std::string_view type() {
			return "innocent";
		}
	};
};

#endif
