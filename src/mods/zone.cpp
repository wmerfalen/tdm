#include "zone.hpp"
#include "integral-objects.hpp"
#include "target-practice-db.hpp"
#include "replenish.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"

#define __MENTOC_MODS_ZONE_DEBUG__
#ifdef __MENTOC_MODS_ZONE_DEBUG__
#define z_debug(A) std::cerr << "[mods::zone debug]" << A << "\n";
#define rr_debug(A) std::cerr << "[run_replenish]:" << A << "\n";
#else
#define z_debug(A)
#define rr_debug(A)
#endif
extern std::deque<zone_data> zone_table;
namespace mods::zone {
	static std::deque<replenish_command> replenish;
	static std::vector<int> zone_id_blacklist;
	static bool disable_all_zone_resets = false;

	void build_weapon_locker(room_vnum room) {
		z_debug("building weapon locker");
		auto obj = create_object(ITEM_CONTAINER, "weapon-locker.yml");
		obj_to_room(obj.get(),real_room(room));
	}
	void build_armor_locker(room_vnum room) {
		z_debug("building armor locker");
		auto obj = create_object(ITEM_CONTAINER, "armor-locker.yml");
		obj_to_room(obj.get(),real_room(room));
	}
	void build_camera_feed(room_vnum room) {
		z_debug("building camera feed");
		auto obj = create_object(ITEM_GADGET, "camera.yml");
		obj_to_room(obj.get(),real_room(room));
	}
	void build_dummy(room_vnum room) {
		mods::target_practice_db::queue_dummy_on_room(room);
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
	void blacklist_zone(int zone) {
		auto it = std::find(zone_id_blacklist.begin(),
		                    zone_id_blacklist.end(),
		                    zone);
		if(it == zone_id_blacklist.end()) {
			zone_id_blacklist.emplace_back(zone);
		}
	}

	void disable_zone_resets(bool b) {
		disable_all_zone_resets = b;
	}
	void log_zone_error(zone_rnum zone, int cmd_no, const char *message) {
		mudlog(NRM, LVL_GOD, TRUE, "SYSERR: zone file: %s", message);
		mudlog(NRM, LVL_GOD, TRUE, "SYSERR: ...offending cmd: '%c' cmd in zone #%d, line %d",
		       '0', zone_table[zone].number, 0);
	}

#define ZONE_ERROR(message) \
{ log_zone_error(zone, cmd_no, message); last_cmd = 0; }

	/* execute the reset command table of a given zone */
	void reset_zone(zone_rnum zone) {
		z_debug(green_str("reset zone entry..."));
		auto is_blacklisted = std::find(zone_id_blacklist.begin(),zone_id_blacklist.end(),zone);
		if(disable_all_zone_resets || is_blacklisted != zone_id_blacklist.end()) {
			z_debug("zone is blacklisted, " << red_str("ignoring..."));
			log("[reset_zone]->[is_blacklisted] Skipping Zone ID due to blacklist rule: %d",zone);
			return;
		}
		int cmd_no = 0, last_cmd = 0;
		char_data *mob = NULL;
		//struct obj_data *obj, *obj_to;

		z_debug("looping..");
		for(auto& ZCMD : zone_table[zone].cmd) {
			z_debug("found command. processing");

			if(ZCMD.if_flag && !last_cmd) {
				z_debug("last_cmd preventing processing. skipping");
				continue;
			}

			/*  This is the list of actual zone commands.  If any new
			 *  zone commands are added to the game, be certain to update
			 *  the list of commands in load_zone() so that the counting
			 *  will still be correct. - ae.
			 */
			z_debug("command type: '" << std::string(&ZCMD.command) << "'");
			switch(ZCMD.command) {
				case '*':			/* ignore command */
					last_cmd = 0;
					break;

				case 'M':			/* read a mobile */
					/** this differs from legacy...
					 * arg1 = mob_vnum
					 * arg2 = room_vnum
					 * arg3 = max
					 */
					last_cmd = 0;
					z_debug(green_str("read mobile: ") << ZCMD.arg1 << ", arg2:" << ZCMD.arg2 << ", arg3:" << ZCMD.arg3);
					if(ZCMD.count < ZCMD.arg3) {
						mob = read_mobile(ZCMD.arg1,VIRTUAL);
						if(mob) {
							z_debug("cool, we found a mob. throwing him in a room now...");
							char_to_room(mob, real_room(ZCMD.arg2));
							ZCMD.count++;
							last_cmd = 1;
						}
					}

					break;

#if 0
				case 'O':			/* read an object */
					if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
						if(ZCMD.arg3 != static_cast<int>(NOWHERE)) {
							obj = read_object(ZCMD.arg1, REAL);
							obj_to_room(obj, ZCMD.arg3);
							last_cmd = 1;
						} else {
							obj = read_object(ZCMD.arg1, REAL);
							IN_ROOM(obj) = NOWHERE;
							last_cmd = 1;
						}
					} else {
						last_cmd = 0;
					}

					break;

				case 'P':			/* object to object */
					if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
						obj = read_object(ZCMD.arg1, REAL);

						if(!(obj_to = get_obj_num(ZCMD.arg3))) {
							ZONE_ERROR("target obj not found, command disabled");
							ZCMD.command = '*';
							break;
						}

						obj_to_obj(TO_OBJ_PTR(obj), TO_OBJ_PTR(obj_to));
						last_cmd = 1;
					} else {
						last_cmd = 0;
					}

					break;

				case 'G':			/* obj_to_char */
					if(!mob) {
						ZONE_ERROR("attempt to give obj to non-existant mob, command disabled");
						ZCMD.command = '*';
						break;
					}

					if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
						obj = read_object(ZCMD.arg1, REAL);
						obj_to_char(obj, mob);
						last_cmd = 1;
					} else {
						last_cmd = 0;
					}

					break;

				case 'E':			/* object to equipment list */
					if(!mob) {
						ZONE_ERROR("trying to equip non-existant mob, command disabled");
						ZCMD.command = '*';
						break;
					}

					if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
						if(ZCMD.arg3 < 0 || ZCMD.arg3 >= NUM_WEARS) {
							ZONE_ERROR("invalid equipment pos number");
						} else {
							obj = read_object(ZCMD.arg1, REAL);
							equip_char(ptr(mob), optr(obj), ZCMD.arg3);
							last_cmd = 1;
						}
					} else {
						last_cmd = 0;
					}

					break;

				case 'R': /* rem obj from room */
					if((obj = get_obj_in_list_num(ZCMD.arg2, world[ZCMD.arg1].contents)) != NULL) {
						extract_obj(obj);
					}

					last_cmd = 1;
					break;


				case 'D':			/* set state of door */
					if(ZCMD.arg2 < 0 || ZCMD.arg2 >= NUM_OF_DIRS ||
					        (world[ZCMD.arg1].dir_option[ZCMD.arg2] == NULL)) {
						ZONE_ERROR("door does not exist, command disabled");
						ZCMD.command = '*';
					} else
						switch(ZCMD.arg3) {
							case 0:
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_LOCKED);
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_CLOSED);
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_BREACHED);
								break;

							case 1:
								SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								        EX_CLOSED);
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_LOCKED);
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_BREACHED);
								break;

							case 2:
								SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								        EX_LOCKED);
								SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								        EX_CLOSED);
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_BREACHED);
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_REINFORCED);
								break;

							/*!mods*/
							case 3:
								SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								        EX_REINFORCED);
								SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								        EX_LOCKED);
								SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								        EX_CLOSED);
								REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
								           EX_BREACHED);
								break;
						}

					last_cmd = 1;
					break;
#endif

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
	void zone_update() {
		static int timer = 0;
		static constexpr int ZO_DEAD = 999;
		bool minute_passed = false;

		z_debug("zone update. running replenish");
		rr_debug("run replenish!");


		for(auto command : replenish) {
			rr_debug("running command:" << command.type);
			if(command.type.compare("weapon-locker") == 0) {
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
				if(zone_table[i].age < zone_table[i].lifespan && zone_table[i].reset_mode) {
					z_debug(green_str("one minute has passed. incrementing age: '") << zone_table[i].age << "'");
					zone_table[i].age += 1;
				}

				if(zone_table[i].age >= zone_table[i].lifespan && zone_table[i].age < ZO_DEAD && zone_table[i].reset_mode) {
					mods::zone::reset_zone(i);
					zone_table[i].age = ZO_DEAD;
				}
			}
		}
	}

	void new_room(room_data* room_ptr) {
		if(mods::db::vector_exists("weapon-locker",std::to_string(room_ptr->number))) {
			z_debug("Found weapon locker in room_ptr->number: " << room_ptr->number);
			build_weapon_locker(room_ptr->number);
			register_replenish(room_ptr->number,"weapon-locker");
		}
		if(mods::db::vector_exists("armor-locker",std::to_string(room_ptr->number))) {
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
	}
	ACMD(do_list_zone_table) {
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		player->send("Listing...\r\n");
		for(const auto& zone : zone_table) {
			player->send(CAT("[vnum:",zone.number,"] -> '",zone.name,"'\r\n").c_str());
		}
		player->send("Done listing...\r\n");
		ADMIN_DONE();
	}
	ACMD(do_reset_zone) {
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() < 1) {
			player->errorln("usage: reset_zone <zone_id|this>");
			return;
		}
		for(const auto& arg : vec_args) {
			if(arg.compare("this") == 0) {
				player->send(CAT("Resetting [",world[player->room()].zone,"]...").c_str());
				mods::zone::reset_zone(world[player->room()].zone);
				player->send("[+] Done\r\n");
				continue;
			}
			auto i = mods::util::stoi(arg).value_or(-1);
			if(i < 0 || i >= zone_table.size()) {
				player->errorln(CAT("Invalid zone id: ",arg,". skipping..."));
				continue;
			}
			player->send(CAT("Resetting [",world[player->room()].zone,"]...").c_str());
			mods::zone::reset_zone(i);
			player->send("[+] Done\r\n");
		}
		ADMIN_DONE();
	}
	void init() {
		mods::interpreter::add_command("reset_zone", POS_RESTING, do_reset_zone, LVL_BUILDER,0);
		mods::interpreter::add_command("list_zone_table", POS_RESTING, do_list_zone_table, LVL_BUILDER,0);
	}
};
#undef rr_debug
#undef z_debug
