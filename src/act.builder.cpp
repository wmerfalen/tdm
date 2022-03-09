#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "screen.h"
#include "globals.hpp"
#include "mods/pq.hpp"
#include "mods/db.hpp"
#include "mods/sql.hpp"
#include "mods/lmdb.hpp"
#include "mods/yaml.hpp"
#include "mods/date-time.hpp"
#include "mods/world-configuration.hpp"
#include "mods/object-utils.hpp"
#include "mods/interpreter.hpp"
#include "mods/screen-searcher.hpp"
#include "mods/super-users.hpp"
#include <dirent.h>
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;


namespace mods::adhoc {
	static std::vector<int> reserved_rooms;
	static std::vector<int> reserved_mobs;
	static std::vector<int> reserved_zones;
	static std::vector<int> reserved_objects;
	static int max_room = -1;
	static int max_zone = -1;
	static int max_object = -1;
	static int max_mob  = -1;
	static inline int next_room() {
		reserved_rooms.push_back(++max_room);
		return max_room;
	}
	static inline int next_zone() {
		reserved_zones.push_back(++max_zone);
		return max_zone;
	}
	static inline int next_object() {
		reserved_objects.push_back(++max_object);
		return max_object;
	}
	static inline int next_mob() {
		reserved_mobs.push_back(++max_mob);
		return max_mob;
	}
};

namespace mods::fs {
	/**
	 * @brief list files in path to player
	 *
	 * @param player
	 * @param _path
	 */
	void ls(player_ptr_t player,std::string_view _path) {
		std::string path = _path.data();
		DIR *dir;
		struct dirent *ent;
		if((dir = opendir(path.c_str())) != NULL) {
			/* print all the files and directories within directory */
			player->stc("{grn}--[ begin dump: '");
			player->stc(_path.data());
			player->stc("']--{/grn}\r\n");
			while((ent = readdir(dir)) != NULL) {
				player->sendln(ent->d_name);
			}
			player->stc("{grn}--[ end dump ]--{/grn}\r\n");
			closedir(dir);
		}
	}
};

/**
 * @brief get the next object number from postgres (next PKID)
 *
 * @return int -1 if error, positive object number
 */
int next_obj_number() {
	try {
		auto select_transaction = txn();
		sql_compositor comp("object",&select_transaction);
		auto mob_sql = comp.select("max(obj_item_number) + 1 as obj_number")
		               .from("mobile")
		               .sql();
		auto rec = mods::pq::exec(select_transaction,mob_sql);
		if(rec.size() == 0) {
			return 1;
		} else {
			return mods::util::stoi<int>(rec[0]["obj_number"]);
		}
	} catch(std::exception& e) {
		REPORT_DB_ISSUE(": error in next_obj_number(): '",e.what());
		return -1;
	}
}



/**
 * @brief get the next object number as a mud command
 *
 * @param do_next_object_number
 */
SUPERCMD(do_next_object_number) {
	player->stc(std::to_string(next_obj_number()));
}

/**
 * @brief get next zone number PKID from postgres
 *
 * @return -1 on error, number as pkid
 */
int next_zone_number() {
	if(mods::adhoc::max_zone == -1) {
		try {
			auto select_transaction = txn();
			sql_compositor comp("zone",&select_transaction);
			auto zone_sql = comp.select("currval(zone_virtual_number) + 1 as zone_number")
			                .from("zone")
			                .sql();
			auto zone_record = mods::pq::exec(select_transaction,zone_sql);
			if(zone_record.size() == 0) {
				mods::adhoc::max_zone = 0;
			} else {
				mods::adhoc::max_zone = mods::util::stoi<int>(zone_record[0]["zone_number"]);
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error in next_zone_number(): '",e.what());
			return -1;
		}
	}
	return mods::adhoc::next_zone();
}


/**
 * @brief get next room PKID from postgres
 *
 * @return  -1 if error, number if pkid
 */
int next_room_number() {
	if(mods::adhoc::max_room == -1) {
		try {
			auto select_transaction = txn();
			sql_compositor comp("room",&select_transaction);
			auto room_sql = comp.select("max(room_number) + 1 as room_number")
			                .from("room")
			                .sql();
			auto room_record = mods::pq::exec(select_transaction,room_sql);
			if(room_record.size() == 0) {
				mods::adhoc::max_room = 0;
			} else {
				std::cerr << "[next_room_number] RETURNING 1\n";
				if(std::string(room_record[0]["room_number"].c_str()).length() == 0) {
					mods::adhoc::max_room = 0;
				} else {
					mods::adhoc::max_room = mods::util::stoi<int>(room_record[0]["room_number"]);
				}
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error in next_room_number(): '",e.what());
		}
	}
	return mods::adhoc::next_room();
}
/**
 * @brief get next pkid from postgres
 *
 * @return -1 on error, pkid
 */
int next_mob_number() {
	if(mods::adhoc::max_mob == -1) {
		try {
			auto select_transaction = txn();
			sql_compositor comp("mobile",&select_transaction);
			auto mob_sql = comp.select("max(mob_virtual_number) + 1 as mob_number")
			               .from("mobile")
			               .sql();
			auto mob_record = mods::pq::exec(select_transaction,mob_sql);
			if(mob_record.size() == 0) {
				mods::adhoc::max_mob = 0;
			} else {
				mods::adhoc::max_mob = mods::util::stoi<int>(mob_record[0]["mob_number"]);
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error in next_mob_number(): '",e.what());
			return -1;
		}
	}
	return mods::adhoc::next_mob();
}

SUPERCMD(do_next_zone_number) {

	auto zone_number = next_zone_number();
	player->stc(std::to_string(zone_number));
}
SUPERCMD(do_next_room_number) {

	auto room_number = next_room_number();
	player->stc(std::to_string(room_number));
}
SUPERCMD(do_next_mob_number) {
	auto mob_number = next_mob_number();
	player->stc(std::to_string(mob_number));
}
SUPERCMD(do_next_obj_number) {
	auto obj_number = next_obj_number();
	player->stc(std::to_string(obj_number));
}


/**
 * @brief flush the currently holding items to db
 *
 * @param do_flush_holding
 */
ACMD(do_flush_holding) {

	player->sendln("Looking at your carrying list...");
	if(auto obj = player->equipment(WEAR_HOLD)) {
		player->sendln("Found an item");
		if(obj->has_rifle()) {
			auto i = obj->rifle()->attributes->flush_to_db();
			obj->set_db_id(i);
			player->sendln(std::string("Flushed rifle object to db with status: ") + std::to_string(i));
			return;
		}
		if(obj->has_gadget()) {
			auto i = obj->gadget()->attributes->flush_to_db();
			obj->set_db_id(i);
			player->sendln(std::string("Flushed gadget object to db with status: ") + std::to_string(i));
			return;
		}
		if(obj->has_armor()) {
			auto i = obj->armor()->attributes->flush_to_db();
			obj->set_db_id(i);
			player->sendln(std::string("Flushed armor object to db with status: ") + std::to_string(i));
			return;
		}
		player->sendln("It doesn't look like the item you're holding can be flushed to the db");
		return;
	}
	player->sendln("You don't seem to be holding anything");
}


/**
 * yaml_import <rifle> <file>		#imports said file in current dir
 * yaml_import ls 			#lists files in current dir
 */
/** TODO: generalize this file system ls interface for use with other stuff */
/**
 * @brief import <type> <file>
 *
 * @param do_yaml_import
 */
SUPERCMD(do_yaml_import) {
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0 || vec_args[0].compare("ls") == 0) {
		player->pager_start();
		mods::fs::ls(player,mods::yaml::current_working_dir());
		player->pager_end();
		player->page(0);
		return;
	}

	if(vec_args.size() == 2) {
		mods::object_utils::set_yaml_initiator(player->name(),vec_args[0],vec_args[1]);
		auto obj = mods::object_utils::yaml_import(vec_args[0],vec_args[1]);
		player->carry(obj);
		player->sendln(CAT("Imported: ",vec_args[1]," of type ", vec_args[0]));
		return;
	}
	player->sendln("Unknown type/file combination. Nothing imported.");

}
void generic_import(player_ptr_t& player,std::string_view type, const std::vector<std::string>& vec_args, std::string_view usage) {
	if(vec_args.size() == 0) {
		player->errorln(usage);
		return;
	}

	for(const auto& yaml : vec_args) {
		mods::object_utils::set_yaml_initiator(player->name(),type.data(),CAT(yaml,".yml"));
		auto obj = mods::object_utils::yaml_import(type.data(),CAT(yaml,".yml"));
		player->carry(obj);
		player->sendln(CAT("Imported: ",yaml,".yml"));
	}
}

/** !!*****************!! */
/** !!UPDATE_ITEM_TYPES!! */
/** !!*****************!! */
SUPERCMD(do_rifle_import) {
	ADMIN_REJECT();
	DO_HELP("rifle_import");
	auto usage = "usage: rifle_import <name-without-extension>\r\nExample: rifle_import g36c";
	generic_import(player,"rifle",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_vehicle_import) {
	ADMIN_REJECT();
	DO_HELP("vehicle_import");
	auto usage = "usage: vehicle_import <name-without-extension>\r\nExample: vehicle_import g36c";
	generic_import(player,"vehicle",PARSE_ARGS(),usage);
	ADMIN_DONE();
}


SUPERCMD(do_melee_import) {
	ADMIN_REJECT();
	DO_HELP("melee_import");
	auto usage = "usage: melee_import <name-without-extension>\r\nExample: melee_import machete";
	generic_import(player,"melee",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_armor_import) {
	ADMIN_REJECT();
	DO_HELP("armor_import");
	auto usage = "usage: armor_import <name-without-extension>\r\nExample: armor_import g36c";
	generic_import(player,"armor",PARSE_ARGS(),usage);
	ADMIN_DONE();
}
SUPERCMD(do_explosive_import) {
	ADMIN_REJECT();
	DO_HELP("explosive_import");
	auto usage = "usage: explosive_import <name-without-extension>\r\nExample: explosive_import g36c";
	generic_import(player,"explosive",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_drone_import) {
	ADMIN_REJECT();
	DO_HELP("drone_import");
	auto usage = "usage: drone_import <name-without-extension>\r\nExample: drone_import g36c";
	generic_import(player,"drone",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_attachment_import) {
	ADMIN_REJECT();
	DO_HELP("attachment_import");
	auto usage = "usage: attachment_import <name-without-extension>\r\nExample: attachment_import g36c";
	generic_import(player,"attachment",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_gadget_import) {
	ADMIN_REJECT();
	DO_HELP("gadget_import");
	auto usage = "usage: gadget_import <name-without-extension>\r\nExample: gadget_import g36c";
	generic_import(player,"gadget",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_trap_import) {
	ADMIN_REJECT();
	DO_HELP("trap_import");
	auto usage = "usage: trap_import <name-without-extension>\r\nExample: trap_import g36c";
	generic_import(player,"trap",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_consumable_import) {
	ADMIN_REJECT();
	DO_HELP("consumable_import");
	auto usage = "usage: consumable_import <name-without-extension>\r\nExample: consumable_import g36c";
	generic_import(player,"consumable",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

SUPERCMD(do_container_import) {
	ADMIN_REJECT();
	DO_HELP("container_import");
	auto usage = "usage: container_import <name-without-extension>\r\nExample: container_import g36c";
	generic_import(player,"container",PARSE_ARGS(),usage);
	ADMIN_DONE();
}

/**
 * @brief get the yaml exception log printed to the player
 *
 * @param do_yaml_log
 */
SUPERCMD(do_yaml_log) {
	DO_HELP("yaml_log");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0) {
		player->sendln("sending yaml exception log...");
		mods::object_utils::send_yaml_exceptions_to(player);
		player->sendln("[done]");
		return;
	}

	if(vec_args.size() == 2) {
		mods::object_utils::set_yaml_initiator(player->name(),vec_args[0],vec_args[1]);
		auto obj = mods::object_utils::yaml_import(vec_args[0],vec_args[1]);
		if(!obj) {
			player->sendln("Didn't import that yaml file. Make sure it exists.");
			return;
		}
		obj_to_char(obj.get(),player->cd());
		player->sendln(CAT("{grn}Imported: ",vec_args[1]," of type ", vec_args[0], "{/grn}"));
		return;
	}
	player->sendln("Unknown type/file combination. Nothing imported.");

}
/**
 * @brief save the current exception log to file
 *
 * @param do_yaml_log_save
 */
SUPERCMD(do_yaml_log_save) {
	player->sendln("Saving yaml log...");
	auto status = mods::object_utils::save_yaml_exceptions_to_disk();
	if(std::get<0>(status) < 0) {
		player->sendln(CAT("{red}Issue writing log to file: '",std::get<1>(status),"'{/red}"));
		return;
	}
	player->sendln("[done]");
}
/**
 * @brief clear the exception log without saving it. no confirmation
 *
 * @param do_yaml_log_clear
 */
SUPERCMD(do_yaml_log_clear) {
	player->sendln("Clearing yaml log...");
	mods::object_utils::clear_yaml_exceptions();
	player->sendln("[done]");
}

/**
 * @brief debugging feature to set flag on god/imm to hold any item regardless of weight
 *
 * @param do_hold_anything
 */
ACMD(do_hold_anything) {

	auto vec_args = PARSE_ARGS();
	if(!player->get_imp_mode()) {
		player->sendln(HUH);
		return;
	}

	if(vec_args.size() == 0) {
		player->sendln("Please pass in 'on' or 'off'");
		return;
	}
	if(vec_args[0].compare("on") == 0) {
		player->set_misc_pref(player->mpref::HOLD_ANYTHING, true);
		player->psendln(IHBD);
		return;
	}
	if(vec_args[0].compare("off") == 0) {
		player->set_misc_pref(player->mpref::HOLD_ANYTHING, false);
		player->psendln(IHBD);
		return;
	}
	player->sendln(HUH);
}
/**
 * @brief calls write_example_file on yaml object.
 *
 * @param command or name of file type (i.e.: rifle, explosive.. to list pass in "list")
 */
SUPERCMD(do_yaml_example) {
	DO_HELP("yaml_example");
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("list") == 0) {
		for(auto type : mods::object_utils::object_types()) {
			player->sendln(type);
		}
		return;
	}
	/** !!*****************!! */
	/** !!UPDATE_ITEM_TYPES!! */
	/** !!*****************!! */
#define MENTOC_F_EXA(CLASS_TYPE,CT_FILE)\
	if(std::string(vec_args[0]).compare(#CLASS_TYPE) == 0){\
		mods::yaml::CLASS_TYPE ## _description_t CLASS_TYPE;\
		player->sendln(CT_FILE);\
		CLASS_TYPE.write_example_file(CT_FILE);\
	}
	MENTOC_F_EXA(rifle,"rifle.yml");
	MENTOC_F_EXA(explosive,"explosive.yml");
	MENTOC_F_EXA(gadget,"gadget.yml");
	MENTOC_F_EXA(drone,"drone.yml");
	MENTOC_F_EXA(attachment,"attachment.yml");
	MENTOC_F_EXA(armor,"armor.yml");
	MENTOC_F_EXA(consumable,"consumable.yml");
	MENTOC_F_EXA(trap,"trap.yml");
	MENTOC_F_EXA(container,"container.yml");
	MENTOC_F_EXA(melee,"melee.yml");
	MENTOC_F_EXA(vehicle,"vehicle.yml");
#undef MENTOC_F_EXA
	player->sendln("[+] done");
}

SUPERCMD(do_histfile) {

	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
		*player << "usage: \r\n" <<
		        " {red}histfile{/red} {grn}start{/grn}\r\n" <<
		        "  |--> create new histfile.\r\n" <<
		        " {red}histfile{/red} {grn}stop{/grn}\r\n" <<
		        "  |--> stop recording and write histfile.\r\n" <<
		        "\r\n"
		        ;
		return;
	}
	if(vec_args.size() == 0) {
		return;
	}
	if(std::string(vec_args[0]).compare("start") == 0) {
		player->start_histfile();
		return;
	}
	if(std::string(vec_args[0]).compare("stop") == 0) {
		player->stop_histfile();
		return;
	}
}

ACMD(do_uuid) {

	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
		*player << "usage: \r\n" <<
		        " {red}uuid{/red} {grn}camera{/grn}\r\n" <<
		        "  |--> will print the uuid of the camera that you're holding.\r\n" <<
		        "\r\n"
		        ;
		return;
	}
	auto obj = mods::util::parse_object_vec(player,vec_args);
	if(!obj) {
		player->sendln("Couldn't find the object you're referring to.");
		return;
	}
	player->stc(std::to_string(obj->uuid));
}

SUPERCMD(do_pmw_obj_from_room) {

	player->sendln("This function is under construction!");
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
		*player << "usage: \r\n" <<
		        " {red}pmw_obj_from_room{/red} {grn}start{/grn}\r\n" <<
		        "  |--> the system will prompt you when obj_from_room is called.\r\n" <<
		        " {red}pmw_obj_from_room{/red} {grn}stop{/grn}\r\n" <<
		        "  |--> the system will {red}stop{/red} prompting you when obj_from_room is called.\r\n" <<
		        "\r\n"
		        ;
		return;
	}
	if(vec_args.size() == 0) {
		return;
	}
	if(std::string(vec_args[0]).compare("start") == 0) {
		/** TODO */
		return;
	}
	if(std::string(vec_args[0]).compare("stop") == 0) {
		/** TODO */
		return;
	}
}

/**
 * @brief debugging feature to turn on or off obj_from_room calls
 *
 * @param on|off|help
 */
SUPERCMD(do_toggle_obj_from_room) {

	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
		*player << "usage: \r\n" <<
		        " {red}toggle_obj_from_room{/red} {grn}on{/grn}\r\n" <<
		        "  |--> the system will enable obj_from_room\r\n" <<
		        " {red}toggle_obj_from_room{/red} {grn}off{/grn}\r\n" <<
		        "  |--> the system will not perform obj_from_room\r\n" <<
		        "\r\n"
		        ;
		return;
	}
	if(vec_args.size() == 0) {
		player->sendln(mods::world_conf::toggle::get_obj_from_room() ?
		               "{grn}enabled{/grn}" :
		               "{grn}disabled{/grn}");
		return;
	}
	if(std::string(vec_args[0]).compare("on") == 0) {
		mods::world_conf::toggle::set_obj_from_room(1);
		player->sendln("{grn}Toggled on{/grn}");
		return;
	}
	if(std::string(vec_args[0]).compare("off") == 0) {
		mods::world_conf::toggle::set_obj_from_room(0);
		player->sendln("{grn}Toggled off{/grn}");
		return;
	}

}

SUPERCMD(do_js_help) {
	if(IS_NPC(ch)) {
		/** nice try */
		return;
	}
	if(player->implementor_mode() || player->builder_mode()) {
		static std::vector<std::string> screen = {
			"{gld}affect_from_char{/gld} -- {grn}1: player_name,2: affect_string{/grn}",
			"{gld}clear_all_affected_flags{/gld} -- ",
			"{gld}clear_all_plr_flags{/gld} -- ",
			"{gld}char_from_room{/gld}",
			"{gld}cmd{/gld} -- ",
			"{gld}cmd_exec{/gld} -- ",
			"{gld}db_get{/gld} -- ",
			"{gld}db_geti{/gld} -- ",
			"{gld}db_set{/gld} -- ",
			"{gld}db_seti{/gld} -- ",
			"{gld}get_char_pk_id{/gld} -- ",
			"{gld}get_current_player{/gld} -- ",
			"{gld}get_day{/gld} -- ",
			"{gld}get_month{/gld} -- ",
			"{gld}get_moon_phase{/gld} -- ",
			"{gld}get_iday{/gld} -- ",
			"{gld}get_ihour{/gld} -- ",
			"{gld}get_imonth{/gld} -- ",
			"{gld}get_iyear{/gld} -- ",
			"{gld}hit{/gld} -- i.e.: hit(uuid_atk,uuid_vict)",
			"{gld}in_room{/gld} -- ",
			"{gld}list_mobiles{/gld} -- ",
			"{gld}mob_death_trigger{/gld} -- ",
			"{gld}mobile_activity{/gld} -- manually call it",
			"{gld}modify_affected_flags{/gld} -- i.e.: maf('far','INVISIBLE',1)",
			"{gld}modify_plr_flags{/gld} -- ",
			"{gld}next_mob_number{/gld} -- gives you the next item number for mobs",
			"{gld}next_object_number{/gld} -- gives you the next item number for objects",
			"{gld}next_room_number{/gld} -- gives you the next item number for rooms",
			"{gld}next_zone_number{/gld} -- gives you the next zone number",
			"{gld}pmw_obj_from_room{/gld} -- when called, will toggle prompts for obj_from_room",
			"{gld}read_mobile{/gld} -- ",
			"{gld}refresh_minimum_proficiencies{/gld} -- reloads minimum proficiencies from lmdb (if any) and updates skill proficiencies",
			"{gld}require_js{/gld} -- include from /lib/js/",
			"{gld}require_test{/gld} -- include from /lib/js/tests",
			"{gld}room{/gld} -- ",
			"{gld}send{/gld} -- ",
			"{gld}send_to_char{/gld} -- ",
			"{gld}send_to_uuid{/gld} -- ",
			"{gld}set_char_pk_id{/gld} -- ",
			"{gld}set_points{/gld} -- i.e.:  set_points(player_name,key,value)",
			"{gld}instigate{/gld} -- i.e.:  instigate(player_name,attacker_name)",
			"{gld}set_attacker{/gld} -- i.e.:  set_attacker(player_name,attacker_name)",
			"{gld}set_points keys{/gld}",
			"mana max_mana hp max_hp move max_move armor",
			"gold bank_gold exp hitroll damroll level",
			"sex room uuid",
			"{gld}uuid{/gld} -- ",
			"{gld}value_load{/gld} -- loads a value saved with value_save() -- i.e.: value_load('SANITY_CHECK');",
			"{gld}value_revert{/gld} -- reverts to hard-coded value (ignores lmdb value)  -- i.e.: value_revert('SANITY_CHECK');",
			"{gld}value_save{/gld} -- saves an override to lmdb -- i.e.: value_save('SANITY_CHECK','value');",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ YAML import shortcuts ]:-                 {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}rifle_import{/gld} -- import a rifle without having to specify .yml -- i.e.: rifle_import g36c",
			"{gld}explosive_import{/gld} -- import a explosive without having to specify .yml -- i.e.: explosive_import frag",
			"{gld}drone_import{/gld} -- import a drone without having to specify .yml -- i.e.: drone_import drone",
			"{gld}attachment_import{/gld} -- import a attachment without having to specify .yml -- i.e.: attachment_import acog",
			"{gld}gadget_import{/gld} -- import a gadget without having to specify .yml -- i.e.: gadget_import gadget",
			"{gld}armor_import{/gld} -- import a armor without having to specify .yml -- i.e.: armor_import boots",
			"{gld}trap_import{/gld} -- import a trap without having to specify .yml -- i.e.: trap_import trap",
			"{gld}consumable_import{/gld} -- import a consumable without having to specify .yml -- i.e.: consumable_import hgh",
			"{gld}container_import{/gld} -- import a container without having to specify .yml -- i.e.: container_import barrel",
			"{gld}melee_import{/gld} -- import a melee without having to specify .yml -- i.e.: melee_import knife",
			"{gld}vehicle_import{/gld} -- import a vehicle without having to specify .yml -- i.e.: vehicle_import suv",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Super User List ]:-                       {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}add_super_user{/gld} -- add user to super user list -- i.e.: add_super_user('grifter');{/gld}",
			"{gld}get_super_user_list{/gld} -- returns super user list -- i.e.: send(get_super_user_list());{/gld}",
			"{gld}remove_super_user{/gld} -- removes user from super user list -- i.e.: remove_super_user('jack');{/gld}"
		};
		auto vec_args = PARSE_ARGS();
		if(vec_args.size()) {
			mods::search_screen<player_ptr_t>(player,screen,vec_args,64);
			player->sendln("Done listing.");
			return;
		}
		for(auto& line : screen) {
			player->sendln(line);
		}
	} else {
		player->stc("Huh?!?");
		return;
	}
}

namespace builder {
	void init() {
		mods::interpreter::add_command("next_object_number",POS_RESTING,do_next_object_number, LVL_BUILDER,0);
		mods::interpreter::add_command("next_zone_number",POS_RESTING,do_next_zone_number, LVL_BUILDER,0);
		mods::interpreter::add_command("next_room_number",POS_RESTING,do_next_room_number, LVL_BUILDER,0);
		mods::interpreter::add_command("next_mob_number",POS_RESTING,do_next_mob_number, LVL_BUILDER,0);
		mods::interpreter::add_command("next_obj_number",POS_RESTING,do_next_obj_number, LVL_BUILDER,0);
		mods::interpreter::add_command("flush_holding",POS_RESTING,do_flush_holding, LVL_BUILDER,0);
		mods::interpreter::add_command("yaml_import",POS_RESTING,do_yaml_import, LVL_BUILDER,0);
		mods::interpreter::add_command("yaml_log",POS_RESTING,do_yaml_log, LVL_BUILDER,0);
		mods::interpreter::add_command("yaml_log_save",POS_RESTING,do_yaml_log_save, LVL_BUILDER,0);
		mods::interpreter::add_command("yaml_log_clear",POS_RESTING,do_yaml_log_clear, LVL_BUILDER,0);
		mods::interpreter::add_command("hold_anything",POS_RESTING,do_hold_anything, LVL_BUILDER,0);
		mods::interpreter::add_command("yaml_example",POS_RESTING,do_yaml_example, LVL_BUILDER,0);
		mods::interpreter::add_command("histfile",POS_RESTING,do_histfile, LVL_BUILDER,0);
		mods::interpreter::add_command("uuid",POS_RESTING,do_uuid, LVL_BUILDER,0);
		mods::interpreter::add_command("pmw_obj_from_room",POS_RESTING,do_pmw_obj_from_room, LVL_BUILDER,0);
		mods::interpreter::add_command("toggle_obj_from_room",POS_RESTING,do_toggle_obj_from_room, LVL_BUILDER,0);

		/** !!*****************!! */
		/** !!UPDATE_ITEM_TYPES!! */
		/** !!*****************!! */
		mods::interpreter::add_command("rifle_import",POS_RESTING,do_rifle_import, LVL_BUILDER,0);
		mods::interpreter::add_command("explosive_import",POS_RESTING,do_explosive_import, LVL_BUILDER,0);
		mods::interpreter::add_command("drone_import",POS_RESTING,do_drone_import, LVL_BUILDER,0);
		mods::interpreter::add_command("attachment_import",POS_RESTING,do_attachment_import, LVL_BUILDER,0);
		mods::interpreter::add_command("gadget_import",POS_RESTING,do_gadget_import, LVL_BUILDER,0);
		mods::interpreter::add_command("armor_import",POS_RESTING,do_armor_import, LVL_BUILDER,0);
		mods::interpreter::add_command("trap_import",POS_RESTING,do_trap_import, LVL_BUILDER,0);
		mods::interpreter::add_command("consumable_import",POS_RESTING,do_consumable_import, LVL_BUILDER,0);
		mods::interpreter::add_command("container_import",POS_RESTING,do_container_import, LVL_BUILDER,0);
		mods::interpreter::add_command("melee_import",POS_RESTING,do_melee_import, LVL_BUILDER,0);
		mods::interpreter::add_command("vehicle_import",POS_RESTING,do_vehicle_import, LVL_BUILDER,0);

	}
};
