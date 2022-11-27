#ifndef  __MENTOC_MODS_ZONE_HEADER__
#define  __MENTOC_MODS_ZONE_HEADER__

#include <iostream>
#include "../room-data.hpp"

//#define __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
#ifdef __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
	#define mzone_debug(a) std::cerr << "[mods::zone]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n";
#else
	#define mzone_debug(a) /**/
#endif
#include <string>
#include "../structs.h"
namespace mods::zone {
	struct replenish_command {
		room_vnum room;
		std::string type;
	};
	void build_weapon_locker(room_vnum room);
	void build_armor_locker(room_vnum room);
	void build_ammo_locker(room_vnum room);
	void register_replenish(room_vnum room,std::string type);
	void remove_replenish(room_vnum room,std::string type);
	void zone_update();
	void run_replenish();
	void new_room(room_data*);
	void disable_zone_resets(bool b);
	void log_zone_error(zone_rnum zone, int cmd_no, const char *message);
	void blacklist_zone(int zone);
	void release_zone(int zone);
	void migrate_mob_command(mentoc_pqxx_result_t& row);

	/**
	 * What is a refresh?
	 * It's tied to the admin:refresh-all SUPERCMD.
	 * An admin who runs: admin:queue:refresh will trigger a
	 * call to refresh_mobs_and_zones(). This is deferred because
	 * running admin:refresh-all through a javascript fails to actually
	 * run it properly. This is a deferred mechanism.
	 *
	 * When an admin runs admin:queue:refresh, a call to queue_refresh() is made.
	 * Every couple of ticks, comm.cpp calls mods::zone::should_refresh().
	 * If it returns true, a call to refresh_mobs_and_zones() is made. Once that
	 * call is done, a call to done_refreshing() is made.
	 *
	 * The tick resolution for comm.cpp to check should_refresh() is returned by
	 * the following constexpr function below (refresh_tick_resolution).
	 */
	void queue_refresh();
	bool should_refresh();
	void done_refreshing();
	void refresh_mobs_and_zones();

	static constexpr uint16_t refresh_tick_resolution() {
		return 33;
	}

};

#endif
