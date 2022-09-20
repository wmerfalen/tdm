#include "zone.hpp"
#include "integral-objects.hpp"
#include "target-practice-db.hpp"
#include "replenish.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "npc.hpp"
#include "str.hpp"
#include "mob-roam.hpp"
#include "orm/locker.hpp"

//#define __MENTOC_MODS_ZONE_DEBUG__
#ifdef __MENTOC_MODS_ZONE_DEBUG__
	#define z_debug(A) std::cerr << "[mods::zone debug]" << A << "\n";
	#define rr_debug(A) std::cerr << "[run_replenish]:" << A << "\n";
#else
	#define z_debug(A)
	#define rr_debug(A)
#endif
extern std::deque<zone_data> zone_table;
extern std::tuple<int16_t,std::string> parse_sql_rooms();
extern std::tuple<int16_t,std::string> parse_sql_zones();
extern int parse_sql_objects();
extern void parse_sql_mobiles();
extern void renum_world(void);
extern void renum_zone_table(void);

namespace mods::zone {
	static std::deque<replenish_command> replenish;
	static std::vector<int> zone_id_blacklist;
	static bool disable_all_zone_resets = false;
	std::vector<room_vnum> ammo_locker_list;
	std::vector<room_vnum> weapon_locker_list;
	std::vector<room_vnum> armor_locker_list;
	std::vector<room_vnum> camera_feed_list;
	std::vector<room_vnum> dummy_list;
	std::vector<room_vnum> sign_list;

	namespace queue_state {
		static bool queue_refresh = false;
	};
	void queue_refresh() {
		queue_state::queue_refresh = true;
	}
	bool should_refresh() {
		return queue_state::queue_refresh;
	}
	void done_refreshing() {
		queue_state::queue_refresh = false;
	}

	void build_ammo_locker(room_vnum room) {
		z_debug("building ammo locker");
		auto obj = create_object(ITEM_CONTAINER, "ammo-locker.yml");
		obj_to_room(obj.get(),real_room(room));
		ammo_locker_list.emplace_back(room);
	}
	void build_weapon_locker(room_vnum room) {
		z_debug("building weapon locker");
		auto obj = create_object(ITEM_CONTAINER, "weapon-locker.yml");
		obj_to_room(obj.get(),real_room(room));
		weapon_locker_list.emplace_back(room);
	}
	void build_armor_locker(room_vnum room) {
		z_debug("building armor locker");
		auto obj = create_object(ITEM_CONTAINER, "armor-locker.yml");
		obj_to_room(obj.get(),real_room(room));
		armor_locker_list.emplace_back(room);
	}
	void build_camera_feed(room_vnum room) {
		z_debug("building camera feed");
		auto obj = create_object(ITEM_GADGET, "camera.yml");
		obj_to_room(obj.get(),real_room(room));
		camera_feed_list.emplace_back(room);
	}
	void build_dummy(room_vnum room) {
		mods::target_practice_db::queue_dummy_on_room(room);
		dummy_list.emplace_back(room);
	}
	void build_sign(room_vnum room) {
		z_debug("building sign");
		auto obj = create_object(ITEM_GADGET, "camera.yml");
		obj_to_room(obj.get(),real_room(room));
		sign_list.emplace_back(room);
	}
	void register_replenish(room_vnum room,std::string type) {
		replenish_command c;
		c.room = room;
		c.type = type;
		mods::zone::replenish.emplace_back(c);
	}
	void remove_replenish(room_vnum room,std::string type) {
		for(auto it = replenish.begin(); it != replenish.end(); ++it) {
			if(it->room == room && it->type.compare(type) == 0) {
				replenish.erase(it);
				remove_replenish(room,type);
				return;
			}
		}
	}

	/**
	 * Removes a zone id from the zone blacklist. The zone blacklist
	 * is a list of zone ID's that should *NOT* be processed.
	 */
	void release_zone(int zone) {
		auto copy = zone_id_blacklist;
		copy.clear();
		for(auto& zone_id : zone_id_blacklist) {
			if(zone_id == zone) {
				continue;
			}
			copy.emplace_back(zone_id);
		}
		zone_id_blacklist.clear();
		zone_id_blacklist.assign(
		    copy.begin(),
		    copy.end()
		);

	}

	/**
	 * Add the zone ID to the zone blacklist list structure. To undo
	 * this, call release_zone
	 */
	void blacklist_zone(int zone) {
		auto it = std::find(zone_id_blacklist.begin(),
		        zone_id_blacklist.end(),
		        zone);
		if(it == zone_id_blacklist.end()) {
			zone_id_blacklist.emplace_back(zone);
		}
	}


	/**
	 * Completely disables zone resets. For the sake of clarity, zone
	 * resets are enabled by default at program startup.
	 */
	void disable_zone_resets(bool b) {
		disable_all_zone_resets = b;
	}


	/**
	 * Simple logging function.
	 */
	void log_zone_error(zone_rnum zone, int cmd_no, const char *message) {
#ifdef __MENTOC_SHOW_ZONE_ERRORS__
		mudlog(NRM, LVL_GOD, TRUE, "SYSERR: zone file: %s", message);
		mudlog(NRM, LVL_GOD, TRUE, "SYSERR: ...offending cmd: '%c' cmd in zone #%d, line %d",
		    '0', zone_table[zone].number, 0);
#endif
	}


	/**
	 * Will return true if the reset command passed in needs to be processed.
	 * Will return false if the reset command has met it's quota. This is mostly
	 * used for things like mob placements where the zone needs to be replenished
	 * with living mobs. If a mob is dead while should_run_zone_command is running and
	 * the passed reset command is a mob commend, then that mob does not count toward
	 * the quota (which is usually stored in arg3).
	 */
	bool should_run_zone_command(reset_com& command) {
		switch(command.command) {
			case 'M': {
					//std::vector<uuid_t> alive;
					//auto& r = command.object_data;
					//r.erase(
					//std::remove_if(r.begin(),r.end(),[](auto& mob_uuid) -> bool {
					//	auto npc = npc_by_uuid(mob_uuid);
					//	return !npc;
					//}),r.end());
					//command.count = command.object_data.size();
					return command.object_data.size() < command.arg3;
					return command.count < command.arg3;
				}
				break;
			case 'Y': {
					command.count = command.object_data.size();
					return command.count < command.arg3;
				}
				break;
			default:
				break;
		};
		return false;
	}

#define ZONE_ERROR(message) \
	{ log_zone_error(zone, cmd_no, message); }

	/**
	 * Loops through the zone_table and conditionally processes a zone reset command.
	 * This function relies on the return value of should_run_zone_command() in most
	 * instances.
	 */
	void reset_zone(zone_rnum zone) {
		z_debug(green_str("reset zone entry..."));
		auto is_blacklisted = std::find(zone_id_blacklist.begin(),zone_id_blacklist.end(),zone);
		if(disable_all_zone_resets || is_blacklisted != zone_id_blacklist.end()) {
			z_debug("zone is blacklisted, " << red_str("ignoring..."));
			log("[reset_zone]->[is_blacklisted] Skipping Zone ID due to blacklist rule: %d",zone);
			return;
		}
		int cmd_no = 0;

		z_debug("looping..");
		for(auto& ZCMD : zone_table[zone].cmd) {
			z_debug("found command. processing");
			/*  This is the list of actual zone commands.  If any new
			 *  zone commands are added to the game, be certain to update
			 *  the list of commands in load_zone() so that the counting
			 *  will still be correct. - ae.
			 */
			z_debug("command type: '" << std::string(&ZCMD.command) << "'");
			switch(ZCMD.command) {
				case '*':			/* ignore command */
					break;
				case 'R': { /* random item spawn */
						/**
						 * arg1 = room_vnum
						 * arg2 = max
						 * arg3 = yaml csv
						 */
						z_debug(green_str("random item spawn: ") << ZCMD.arg1 << ", arg2:" << ZCMD.arg2 << ", arg3:" << ZCMD.arg3);
						if(should_run_zone_command(ZCMD)) {
							auto obj = mods::globals::read_mobile_ptr(ZCMD.arg1,VIRTUAL);
							if(!obj) {
								log(CAT("Warning: zone update failed to read this mob:",ZCMD.arg1).c_str());
								break;
							}
							z_debug("cool, we found a mob. throwing him in a room now...");
							char_from_room(obj->cd());
							char_to_room(obj->cd(),real_room(ZCMD.arg2));
							ZCMD.object_data.emplace_back(obj->uuid());
							ZCMD.count = ZCMD.object_data.size();
						}
					}
					break;

				case 'M': {		/* read a mobile */
						/** this differs from legacy...
						 * arg1 = mob_vnum
						 * arg2 = room_vnum
						 * arg3 = max
						 */
						z_debug(green_str("read mobile: ") << ZCMD.arg1 << ", arg2:" << ZCMD.arg2 << ", arg3:" << ZCMD.arg3);
						if(should_run_zone_command(ZCMD)) {
							auto obj = mods::globals::read_mobile_ptr(ZCMD.arg1,VIRTUAL);
							if(!obj) {
								log(CAT("Warning: zone update failed to read this mob:",ZCMD.arg1).c_str());
								break;
							}
							z_debug("cool, we found a mob. throwing him in a room now...");
							char_from_room(obj->cd());
							char_to_room(obj->cd(),real_room(ZCMD.arg2));
							ZCMD.object_data.emplace_back(obj->uuid());
							ZCMD.count = ZCMD.object_data.size();
						}
					}
					break;
				case 'Y': {		/* read a yaml file  */
						/**
						 * arg1 = 0
						 * arg2 = room_vnum
						 * arg3 = max
						 * yaml = file to load
						 */
						z_debug(green_str("read yaml: ") << ZCMD.yaml << ", arg2:" << ZCMD.arg2 << ", arg3:" << ZCMD.arg3);
						if(should_run_zone_command(ZCMD)) {
							auto s = mods::util::extract_yaml_reward(ZCMD.yaml);
							auto obj = create_object(std::get<1>(s),std::get<2>(s));
							if(!obj) {
								log(CAT("Warning: zone update failed to read this yaml:",ZCMD.yaml).c_str());
								break;
							}
							z_debug("cool, we found a yaml file. throwing him in a room now...");
							obj_to_room(obj.get(),real_room(ZCMD.arg2));
							ZCMD.object_data.emplace_back(obj->uuid);
							ZCMD.count = ZCMD.object_data.size();
						}
					}
					break;
				default:
					ZONE_ERROR("unknown cmd in reset table; cmd disabled");
					ZCMD.command = '*';
					break;
			}
		}

		zone_table[zone].age = 0;
		z_debug("returning from reset_zone");
	}


	//struct zone_data
	// char *name;          /* name of this zone                  */
	// int  lifespan;           /* how long between resets (minutes)  */
	// int  age;                /* current age of this zone (minutes) */
	// room_vnum bot;           /* starting room number for this zone */
	// room_vnum top;           /* upper limit for rooms in this zone */
	//
	// int  reset_mode;         /* conditions for reset (see below)   */
	// zone_vnum number;        /* virtual number of this zone    */
	// struct reset_com *cmd;   /* command table for reset            */
	// /*
	//  * Reset mode:
	//  *   0: Don't reset, and don't update age.
	//  *   1: Reset if no PC's are located in zone.
	//  *   2: Just reset.
	//  */
	//
	//struct reset_com
	//  char command;   /* current command                      */
	//
	//  bool if_flag;    /* if TRUE: exe only if preceding exe'd */
	//  int  arg1;       /*                                      */
	//  int  arg2;       /* Arguments to the command             */
	//  int  arg3;       /*                                      */
	//  int line;        /* line number this command appears on  */
	//
	//  /*
	//   *  Commands:              *
	//   *  'M': Read a mobile     *
	//   *  'O': Read an object    *
	//   *  'G': Give obj to mob   *
	//   *  'P': Put obj in obj    *
	//   *  'G': Obj to char       *
	//   *  'E': Obj to char equip *
	//   *  'D': Set state of door *
	//  */

	void renum_zone_table(void) {
		log("renum_zone_table");
		using zone_table_t = decltype(zone_table);
		zone_table_t filtered;

		std::size_t zone_table_size = 0,
		            zone_table_saved = 0,
		            zone_table_ignored = 0;
		filtered.resize(zone_table.size());
		//for (zone = 0; zone < top_of_zone_table; zone++)
		for(unsigned zone = 0; zone < zone_table.size(); zone++) {
			filtered[zone] = zone_table[zone];
			zone_table_size += zone_table[zone].cmd.size();
			for(auto ZCMD : zone_table[zone].cmd) {
				bool skip_me = false;
				mob_rnum mob = 0;
				room_rnum room = 0;

				switch(ZCMD.command) {
					case 'M':
						mob = real_mobile(ZCMD.arg1);
						room = real_room(ZCMD.arg2);
						skip_me = false;
						if(mob == NOBODY || room == NOWHERE) {
							skip_me = true;
						}
						break;
					case 'R': /* rem obj from room */
						room = real_room(ZCMD.arg1);
						skip_me = false;
						if(room == NOWHERE) {
							skip_me = true;
						}
						break;
					case 'Y':
						skip_me = false;
						break;
					default:
						log("WARNING: renum_zone_table found an unrecognized zone_command: '%s'. Ignoring...",(str(ZCMD.command).c_str()));
						skip_me = true;
						break;

				}

				if(skip_me) {
					log("skipping command: %s",(str(ZCMD.command).c_str()));
					++zone_table_ignored;
				} else {
					++zone_table_saved;
					filtered[zone].cmd.emplace_back(ZCMD);
				}
			}
		}
		log("[renum_zone_table] saved(%d). invalid(%d). original size(%d).",zone_table_saved,zone_table_ignored, zone_table_size);
		zone_table = std::move(filtered);
	}

	/**
	 * This function is the main entry point for zone resets. This function
	 * is usually called from comm.cpp when the following if condition is met:
	 * if(!(pulse % PULSE_ZONE))
	 *
	 *
	 */
	void zone_update() {
		static int timer = 0;
		static constexpr int ZO_DEAD = 999;
		bool minute_passed = false;

		z_debug("zone update. running replenish");
		rr_debug("run replenish!");


		for(auto command : replenish) {
			std::cerr << "running command:" << command.type << "\n";
			rr_debug("running command:" << command.type);
			if(command.type.compare("ammo-locker") == 0) {
				rr_debug("ammo locker command type found. running feed");
				mods::integral_objects::feed_ammo_locker(command.room);
			}
			if(command.type.compare("weapon-locker") == 0) {
				std::cerr << "feeding weapon locker!\n";
				rr_debug("weapon locker command type found. running feed");
				mods::integral_objects::feed_weapon_locker(command.room);
			}
			if(command.type.compare("armor-locker") == 0) {
				rr_debug("armor locker command type found. running feed");
				mods::integral_objects::feed_armor_locker(command.room);
			}
			if(command.type.compare("camera-feed") == 0) {
				rr_debug("camera feed command type found. running feed");
				mods::integral_objects::feed_camera_feed(command.room);
				mods::integral_objects::rotate_camera_feed(command.room);
			}
		}
		if(((++timer * PULSE_ZONE) / PASSES_PER_SEC) >= 60) {
			z_debug(green_str("a minute has passed"));
			timer = 0;
			minute_passed = true;
		}
		for(unsigned i = 0; i < zone_table.size(); i++) {
			z_debug("looping through current zone:'" << zone_table[i].name << "'");
			if(minute_passed) {
				z_debug(green_str("a minute passed. updating zones..."));
				/* one minute has passed */
				if(zone_table[i].age < zone_table[i].lifespan) { // && zone_table[i].reset_mode) {
					z_debug(green_str("one minute has passed. incrementing age: '") << zone_table[i].age << "'");
					zone_table[i].age += 1;
				}

				if(zone_table[i].age >= zone_table[i].lifespan && zone_table[i].age < ZO_DEAD) { // && zone_table[i].reset_mode) {
					mods::zone::reset_zone(i);
					zone_table[i].age = 0;
				}
			}
		}
	}

	static inline bool has_locker_of_type(std::string_view type,const room_vnum& room) {
		return mods::orm::locker::room_has_locker_by_type(type,room);
	}

	/**
	 * This function is called from db.cpp when a new room is created/parsed from sql.
	 */
	void new_room(room_data* room_ptr) {
		if(has_locker_of_type("ammo",room_ptr->number)) {
			z_debug("Found ammo locker in room_ptr->number: " << room_ptr->number);
			build_ammo_locker(room_ptr->number);
			register_replenish(room_ptr->number,"ammo-locker");
		}
		if(has_locker_of_type("weapon",room_ptr->number)) {
			z_debug("Found weapon locker in room_ptr->number: " << room_ptr->number);
			build_weapon_locker(room_ptr->number);
			register_replenish(room_ptr->number,"weapon-locker");
		}
		if(has_locker_of_type("armor",room_ptr->number)) {
			z_debug("Found armor locker in room_ptr->number: " << room_ptr->number);
			build_armor_locker(room_ptr->number);
			register_replenish(room_ptr->number,"armor-locker");
		}
		if(mods::db::vector_exists("camera-feed",std::to_string(room_ptr->number))) {
			z_debug("Found camera feed in room_ptr->number: " << room_ptr->number);
			build_camera_feed(room_ptr->number);
			register_replenish(room_ptr->number,"camera-feed");
		}
		if(mods::db::vector_exists("dummy",std::to_string(room_ptr->number))) {
			z_debug("Found dummy in room_ptr->number: " << room_ptr->number);
			build_dummy(room_ptr->number);
		}
		if(mods::db::vector_exists("sign",std::to_string(room_ptr->number))) {
			z_debug("Found sign  in room_ptr->number: " << room_ptr->number);
			build_sign(room_ptr->number);
		}
	}




	/**
	 * Allows an admin user to display paged output of the entire zone_table.
	 * The only acceptable argument to this function is for an arg1 filter.
	 */
	SUPERCMD(do_list_zone_table) {
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		std::string filter_arg1 = "";
		if(vec_args.size() > 0) {
			filter_arg1 = vec_args[0];
		}
		player->sendln("Listing...");
		player->pager_start();
		for(const auto& zone : zone_table) {
			player->sendln(CAT("[vnum:",zone.number,"] -> '",zone.name,"'").c_str());
			for(const auto& cmd : zone.cmd) {
				std::string check_filter = std::to_string(cmd.arg1);
				if(filter_arg1.length() && filter_arg1.compare(check_filter.c_str()) != 0) {
					continue;
				}
				std::string command;
				switch(cmd.command) {
					case 'R':
						command = "R";
						break;
					case 'M':
						command = "M";
						break;
					case 'Y':
						command = "Y";
						break;
				}
				player->sendln(
				    CAT(
				        "[zone.cmd.if_flag]:",cmd.if_flag,"\r\n",
				        "[zone.cmd.command]:",command,"\r\n",
				        "[zone.cmd.arg1]:",cmd.arg1,"\r\n",
				        "[zone.cmd.arg2]:",cmd.arg2,"\r\n",
				        "[zone.cmd.arg3]:",cmd.arg3,"\r\n",
				        "[zone.cmd.line]:",cmd.line,"\r\n",
				        "[zone.cmd.count]:",cmd.count,"\r\n",
				        "[zone.cmd.yaml]:",cmd.yaml,"\r\n"
				    )
				);
			}
		}
		player->pager_end();
		player->page(0);
		player->sendln("Done listing...");
		ADMIN_DONE();
	}

	/**
	 * This really shouldn't be in this namespace, but it lists
	 * the uuids of all the players and npcs currently in the same
	 * room as the admin running this command.
	 */
	SUPERCMD(do_uuids) {
		ADMIN_REJECT();
		for(const auto& person : mods::globals::get_room_list(player->room())) {
			if(person->is_npc()) {
				player->sendln(
				    CAT("[",person->uuid(),"]:'",person->name().c_str(),"' (NPC) [vnum:",person->cd()->mob_specials.vnum,"]")
				);
			} else {
				player->sendln(
				    CAT("[",person->uuid(),"]:'",person->name().c_str(),"' (PC) [dbid:",person->db_id(),"]")
				);
			}
			ADMIN_DONE();
		}
	}


	/**
	 * Trigger a manual call to reset_zone(id). This accepts either a zone id or the
	 * phrase "this".
	 */
	SUPERCMD(do_reset_zone) {
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() < 1) {
			player->errorln("usage: reset_zone <zone_id|this>");
			return;
		}
		for(const auto& arg : vec_args) {
			if(arg.compare("this") == 0) {
				player->sendln(CAT("Resetting [",world[player->room()].zone,"]...").c_str());
				mods::zone::reset_zone(world[player->room()].zone);
				player->sendln("[+] Done");
				continue;
			}
			auto i = mods::util::stoi(arg).value_or(-1);
			if(i < 0 || i >= zone_table.size()) {
				player->errorln(CAT("Invalid zone id: ",arg,". skipping..."));
				continue;
			}
			player->sendln(CAT("Resetting [",world[player->room()].zone,"]..."));
			mods::zone::reset_zone(i);
			player->sendln("[+] Done");
		}
		ADMIN_DONE();
	}


	/**
	 * Queues a call to refresh_mobs_and_zones(). This is used within javascripts
	 * or as a manually entered command.
	 */
	SUPERCMD(do_queue_refresh) {
		ADMIN_REJECT();
		player->sendln("Queuing refresh. Please give it a second or two to complete.");
		mods::zone::queue_refresh();
		ADMIN_DONE();
	}
	void init() {
		ADD_BUILDER_COMMAND("admin:reset:zone", do_reset_zone);
		ADD_BUILDER_COMMAND("admin:uuids", do_uuids);
		ADD_BUILDER_COMMAND("admin:list:zone-table", do_list_zone_table);
		ADD_BUILDER_COMMAND("admin:queue:refresh", do_queue_refresh);
	}
	void refresh_mobs_and_zones() {
		mob_proto.clear();
		mob_index.clear();
		log("Loading sql mobs and generating index.");
		parse_sql_mobiles();
		log("Loading mob roaming data");
		mods::mob_roam::boot();

		log("Loading sql objs and generating index.");
		parse_sql_objects();

		parse_sql_zones();
		log("Renumbering zone table.");
		renum_zone_table();

		for(auto i = 0; i < zone_table.size(); i++) {
			log("Resetting #%d: %s (rooms %d-%d).", zone_table[i].number,
			    zone_table[i].name, zone_table[i].bot, zone_table[i].top);
			reset_zone(i);
		}
		log("refresh_mobs_and_zones() finished.");
	}
};
#undef rr_debug
#undef z_debug
