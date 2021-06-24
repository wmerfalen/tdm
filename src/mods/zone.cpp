#include "zone.hpp"
#include "integral-objects.hpp"
#include "target-practice-db.hpp"
#include "replenish.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "npc.hpp"

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
	std::vector<room_vnum> ammo_locker_list;
	std::vector<room_vnum> weapon_locker_list;
	std::vector<room_vnum> armor_locker_list;
	std::vector<room_vnum> camera_feed_list;
	std::vector<room_vnum> dummy_list;
	std::vector<room_vnum> sign_list;

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

	bool zone_command_upkeep(reset_com& command) {
		switch(command.command) {
			case 'M': {
					std::vector<uuid_t> alive;
					for(const auto& mob_uuid : command.object_data) {
						auto npc = npc_by_uuid(mob_uuid);
						if(npc) {
							alive.emplace_back(mob_uuid);
						}
					}
					command.object_data = std::move(alive);
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

	/* execute the reset command table of a given zone */
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

				case 'M': {		/* read a mobile */
						/** this differs from legacy...
						 * arg1 = mob_vnum
						 * arg2 = room_vnum
						 * arg3 = max
						 */
						z_debug(green_str("read mobile: ") << ZCMD.arg1 << ", arg2:" << ZCMD.arg2 << ", arg3:" << ZCMD.arg3);
						if(zone_command_upkeep(ZCMD)) {
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
			if(command.type.compare("ammo-locker") == 0) {
				rr_debug("ammo locker command type found. running feed");
				mods::integral_objects::feed_ammo_locker(command.room);
			}
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

	void new_room(room_data* room_ptr) {
		if(mods::db::vector_exists("ammo-locker",std::to_string(room_ptr->number))) {
			z_debug("Found ammo locker in room_ptr->number: " << room_ptr->number);
			build_ammo_locker(room_ptr->number);
			register_replenish(room_ptr->number,"ammo-locker");
		}
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
		if(mods::db::vector_exists("sign",std::to_string(room_ptr->number))) {
			z_debug("Found sign  in room_ptr->number: " << room_ptr->number);
			build_sign(room_ptr->number);
		}
	}
	SUPERCMD(do_list_zone_table) {
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		player->send("Listing...\r\n");
		for(const auto& zone : zone_table) {
			player->send(CAT("[vnum:",zone.number,"] -> '",zone.name,"'\r\n").c_str());
			for(const auto& cmd : zone.cmd) {
				player->sendln(CAT("[zone_cmd if_flag]:",cmd.if_flag));
			}
		}
		player->send("Done listing...\r\n");
		ADMIN_DONE();
	}
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

	SUPERCMD(do_reset_zone) {
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
		mods::interpreter::add_command("uuids", POS_RESTING, do_uuids, LVL_BUILDER,0);
		mods::interpreter::add_command("list_zone_table", POS_RESTING, do_list_zone_table, LVL_BUILDER,0);
	}
};
#undef rr_debug
#undef z_debug
