#include "mods/db-report.hpp"
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
	static inline int next_room(){
		reserved_rooms.push_back(++max_room);
		return max_room;
	}
	static inline int next_zone(){
		reserved_zones.push_back(++max_zone);
		return max_zone;
	}
	static inline int next_object(){
		reserved_objects.push_back(++max_object);
		return max_object;
	}
	static inline int next_mob(){
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
		if ((dir = opendir (path.c_str())) != NULL) {
			/* print all the files and directories within directory */
			player->stc("{grn}--[ begin dump: '");
			player->stc(_path.data());
			player->stc("']--{/grn}\r\n");
			while ((ent = readdir (dir)) != NULL) {
				player->sendln(ent->d_name);
			}
			player->stc("{grn}--[ end dump ]--{/grn}\r\n");
			closedir (dir);
		}
	}
};

/**
 * @brief get the next object number from postgres (next PKID)
 *
 * @return int -1 if error, positive object number
 */
int next_obj_number(){
	try{
		auto select_transaction = txn();
		sql_compositor comp("object",&select_transaction);
		auto mob_sql = comp.select("max(obj_item_number) + 1 as obj_number")
			.from("mobile")
			.sql();
		auto rec = mods::pq::exec(select_transaction,mob_sql);
		if(rec.size() == 0){
			return 1;
		}else{
			return mods::util::stoi<int>(rec[0]["obj_number"]);
		}
	}catch(std::exception& e){
		REPORT_DB_ISSUE(": error in next_obj_number(): '",e.what());
		return -1;
	}
}



/**
 * @brief get the next object number as a mud command
 *
 * @param do_next_object_number
 */
ACMD(do_next_object_number){
	player->stc(std::to_string(next_obj_number()));
}

/**
 * @brief get next zone number PKID from postgres
 *
 * @return -1 on error, number as pkid
 */
int next_zone_number() {
	if(mods::adhoc::max_zone == -1){
	try{
		auto select_transaction = txn();
		sql_compositor comp("zone",&select_transaction);
		auto zone_sql = comp.select("currval(zone_virtual_number) + 1 as zone_number")
			.from("zone")
			.sql();
		auto zone_record = mods::pq::exec(select_transaction,zone_sql);
		if(zone_record.size() == 0){
			mods::adhoc::max_zone = 0;
		}else{
			mods::adhoc::max_zone = mods::util::stoi<int>(zone_record[0]["zone_number"]);
		}
	}catch(std::exception& e){
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
int next_room_number(){ 
	if(mods::adhoc::max_room == -1){
		try{
			auto select_transaction = txn();
			sql_compositor comp("room",&select_transaction);
			auto room_sql = comp.select("max(room_number) + 1 as room_number")
				.from("room")
				.sql();
			auto room_record = mods::pq::exec(select_transaction,room_sql);
			if(room_record.size() == 0){
				mods::adhoc::max_room = 0;
			}else{
				std::cerr << "[next_room_number] RETURNING 1\n";
				if(std::string(room_record[0]["room_number"].c_str()).length() == 0){
					mods::adhoc::max_room = 0;
				}else{
					mods::adhoc::max_room = mods::util::stoi<int>(room_record[0]["room_number"]);
				}
			}
		}catch(std::exception& e){
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
int next_mob_number(){
	if(mods::adhoc::max_mob == -1){
	try{
		auto select_transaction = txn();
		sql_compositor comp("mobile",&select_transaction);
		auto mob_sql = comp.select("max(mob_virtual_number) + 1 as mob_number")
			.from("mobile")
			.sql();
		auto mob_record = mods::pq::exec(select_transaction,mob_sql);
		if(mob_record.size() == 0){
			mods::adhoc::max_mob = 0;
		}else{
			mods::adhoc::max_mob = mods::util::stoi<int>(mob_record[0]["mob_number"]);
		}
	}catch(std::exception& e){
		REPORT_DB_ISSUE(": error in next_mob_number(): '",e.what());
		return -1;
	}
	}
	return mods::adhoc::next_mob();
}

ACMD(do_next_zone_number){
	
	auto zone_number = next_zone_number();
	player->stc(std::to_string(zone_number));
}
ACMD(do_next_room_number){
	
	auto room_number = next_room_number();
	player->stc(std::to_string(room_number));
}
ACMD(do_next_mob_number){
	auto mob_number = next_mob_number();
	player->stc(std::to_string(mob_number));
}
ACMD(do_next_obj_number){
	auto obj_number = next_obj_number();
	player->stc(std::to_string(obj_number));
}


/**
 * @brief flush the currently holding items to db
 *
 * @param do_flush_holding
 */
ACMD(do_flush_holding){
	
	player->sendln("Looking at your carrying list...");
	if(auto obj = player->equipment(WEAR_HOLD)){
		player->sendln("Found an item");
		if(obj->has_rifle()){
			auto i = obj->rifle()->attributes->flush_to_db();
			obj->set_db_id(i);
			player->sendln(std::string("Flushed rifle object to db with status: ") + std::to_string(i));
			return;
		}
		if(obj->has_gadget()){
			auto i = obj->gadget()->attributes->flush_to_db();
			obj->set_db_id(i);
			player->sendln(std::string("Flushed gadget object to db with status: ") + std::to_string(i));
			return;
		}
		if(obj->has_armor()){
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
ACMD(do_yaml_import){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0 || vec_args[0].compare("ls") == 0) {
		player->pager_start();
		mods::fs::ls(player,mods::yaml::current_working_dir());
		player->pager_end();
		player->page(0);
		return;
	}

	if(vec_args.size() == 2){
		mods::object_utils::set_yaml_initiator(player->name(),vec_args[0],vec_args[1]);
		auto obj = mods::object_utils::yaml_import(vec_args[0],vec_args[1]);
		player->carry(obj);
		player->send("Imported: %s of type %s\r\n", vec_args[1].c_str(),vec_args[0].c_str());
		return;
	}
	player->sendln("Unknown type/file combination. Nothing imported.");

}

/**
 * @brief get the yaml exception log printed to the player
 *
 * @param do_yaml_log
 */
ACMD(do_yaml_log){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0) {
		player->pager_start();
		mods::object_utils::send_yaml_exceptions_to(player);
		player->pager_end();
		player->page(0);
		return;
	}

	if(vec_args.size() == 2){
		mods::object_utils::set_yaml_initiator(player->name(),vec_args[0],vec_args[1]);
		auto obj = mods::object_utils::yaml_import(vec_args[0],vec_args[1]);
		obj_to_char(obj.get(),player->cd());
		player->send("Imported: %s of type %s\r\n", vec_args[1].c_str(),vec_args[0].c_str());
		return;
	}
	player->sendln("Unknown type/file combination. Nothing imported.");

}
/**
 * @brief save the current exception log to file
 *
 * @param do_yaml_log_save
 */
ACMD(do_yaml_log_save){
	player->send("Saving yaml log...");
	mods::object_utils::save_yaml_exceptions_to_disk();
	player->sendln("[done]");
}
/**
 * @brief clear the exception log without saving it. no confirmation
 *
 * @param do_yaml_log_clear
 */
ACMD(do_yaml_log_clear){
	player->send("Clearing yaml log...");
	mods::object_utils::clear_yaml_exceptions();
	player->sendln("[done]");
}

/**
 * @brief debugging feature to set flag on god/imm to hold any item regardless of weight
 *
 * @param do_hold_anything
 */
ACMD(do_hold_anything){

	auto vec_args = PARSE_ARGS();
	if(!player->get_imp_mode()){
		player->sendln(HUH);
		return;
	}

	if(vec_args.size() == 0){
		player->sendln("Please pass in 'on' or 'off'");
		return;
	}
	if(vec_args[0].compare("on") == 0){
		player->set_misc_pref(player->mpref::HOLD_ANYTHING, true);
		player->psendln(IHBD);
		return;
	}
	if(vec_args[0].compare("off") == 0){
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
ACMD(do_yaml_example){

	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("list") == 0) {
		for(auto type : {"rifle","explosive","drone","gadget","attachment","armor","consumable"}) {
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
#undef MENTOC_F_EXA
	player->sendln("[+] done");
}

ACMD(do_histfile){

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
	if(vec_args.size() == 0){
		return;
	}
	if(std::string(vec_args[0]).compare("start") == 0){
		player->start_histfile();
		return;
	}
	if(std::string(vec_args[0]).compare("stop") == 0){
		player->stop_histfile();
		return;
	}
}

ACMD(do_uuid){

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
	if(!obj){
		player->sendln("Couldn't find the object you're referring to.");
		return;
	}
	player->stc(std::to_string(obj->uuid));
}

ACMD(do_pmw_obj_from_room){

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
	if(vec_args.size() == 0){
		return;
	}
	if(std::string(vec_args[0]).compare("start") == 0){
		/** TODO */
		return;
	}
	if(std::string(vec_args[0]).compare("stop") == 0){
		/** TODO */
		return;
	}
}

/**
 * @brief debugging feature to turn on or off obj_from_room calls
 *
 * @param on|off|help
 */
ACMD(do_toggle_obj_from_room){

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
	if(vec_args.size() == 0){
		player->sendln(mods::world_conf::toggle::get_obj_from_room() ? 
				"{grn}enabled{/grn}" : 
				"{grn}disabled{/grn}");
		return;
	}
	if(std::string(vec_args[0]).compare("on") == 0){
		mods::world_conf::toggle::set_obj_from_room(1);
		player->sendln("{grn}Toggled on{/grn}");
		return;
	}
	if(std::string(vec_args[0]).compare("off") == 0){
		mods::world_conf::toggle::set_obj_from_room(0);
		player->sendln("{grn}Toggled off{/grn}");
		return;
	}

}
