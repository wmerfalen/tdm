#include "zbuild.hpp"
#include "pq.hpp"
#include "util.hpp"
#include "sql.hpp"
#include "../structs.h"
#include "extern.hpp"
#include "builder_util.hpp"
#include "../db.h"
#include "../globals.hpp"
#include "jx.hpp"
#include <tuple>
#include "npc.hpp"
#include "world-configuration.hpp"
#include "orm/shop.hpp"
#include "doors.hpp"
#include "orm/room.hpp"
#include "rooms.hpp"
#include <algorithm> // for std::min
#include "mobs/extended-types.hpp"
#include "mobs/mini-gunner.hpp"
#include <list>
#include <memory>
#include "orm/shop.hpp"
#include "builder/encode.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "zone.hpp"
#include "sex.hpp"
#include "builder-data.hpp"

namespace mods {
	struct player;
};
namespace mods {
	struct extra_desc_data;
};
#define MENTOC_OBI(i) obj->i = get_intval(#i).value_or(obj->i);
#define MENTOC_OBI2(i,a) obj->i = get_intval(#a).value_or(obj->i);
#define MENTOC_OBS(i) obj->i = get_strval(#i).value_or(obj->i);
#define MENTOC_OBS2(i,a) obj->i = get_strval(#a).value_or(obj->i.c_str());
using objtype = mods::object::type;
using player_ptr_t = std::shared_ptr<mods::player>;
using jxcomp = mods::jx::compositor;
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

extern void refresh_mobs_and_zones();

extern int next_mob_number();
extern int next_room_number();
extern int next_zone_number();
extern int next_room_vnum() ;
extern std::tuple<int16_t,std::string> parse_sql_zones();
extern obj_ptr_t create_object_from_index(std::size_t proto_index);
using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
extern std::deque<shop_data_t> shop_proto;	/* prototypes for objs		 */

using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
#define in_room(a) IN_ROOM(a)
#define nowhere NOWHERE
#define is_direction(A) IS_DIRECTION(A)
int get_raid_id(player_ptr_t player);
#undef m_debug
#undef m_error
#define __MENTOC_SHOW_MODS_ZBUILD_DEBUG_OUTPUT__ // FIXME: remove
#ifdef __MENTOC_SHOW_MODS_ZBUILD_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::zbuild::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::zbuild::ERROR]"))  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
	#define m_error(MSG) ;;
#endif
std::optional<pqxx::result> get_zone_mob_rows_matching(
    std::string_view z_mob_vnum,
    std::string_view z_room_vnum
) {
	try {
		auto sel_txn = txn();
		sql_compositor comp("zone_mob",&sel_txn);
		auto room_sql = comp.select("*")
		    .from("zone_mob")
		    .where("zm_mob_vnum",z_mob_vnum.data())
		    .op_and("zm_room_vnum","=",z_room_vnum.data())
		    .sql();
		auto row = mods::pq::exec(sel_txn,room_sql.data());
		mods::pq::commit(sel_txn);
		return row;
	} catch(std::exception& e) {
		REPORT_DB_ISSUE("get_zone_mob_rows_matching: exception",e.what());
		return std::nullopt;
	}
}
std::pair<bool,std::string> zone_mob_remove_first_matching(
    std::string_view arg_mob_vnum,
    std::string_view arg_room_vnum
) {
	m_debug("delete zone mob placement " << arg_mob_vnum << " room: " << arg_room_vnum);
	auto opt = get_zone_mob_rows_matching(arg_mob_vnum,arg_room_vnum);
	if(!opt.has_value()) {
		return {false,"No rows matching that query"};
	}
	try {
		const auto& rows = opt.value();
		if(rows.size() == 0) {
			return {false,"No rows matching that query"};
		}
		auto first_row = rows[0];
		auto id = first_row["id"].c_str();
		auto del_txn = txn();
		sql_compositor comp("zone_mob",&del_txn);
		auto room_sql = comp.del()
		    .from("zone_mob")
		    .where("id",id)
		    .sql();
		auto row = mods::pq::exec(del_txn,room_sql.data());
		mods::pq::commit(del_txn);
		return {1,"deleted"};
	} catch(std::exception& e) {
		REPORT_DB_ISSUE("zone_mob_remove_first_matching: exception",e.what());
		return {false,CAT("failed: '",e.what(),"'")};
	}
}
std::pair<uint32_t,uint32_t> zone_mob_remove_all_matching(
    std::string_view arg_mob_vnum,
    std::string_view arg_room_vnum
) {
	uint32_t remove_count = 0;
	uint32_t error_count = 0;
	m_debug("delete zone mob placement (all that match) " << arg_mob_vnum << " room: " << arg_room_vnum);
	auto opt = get_zone_mob_rows_matching(arg_mob_vnum,arg_room_vnum);
	if(!opt.has_value()) {
		return {0,0};
	}
	const auto& rows = opt.value();
	if(rows.size() == 0) {
		return {0,0};
	}
	for(const auto& row : rows) {
		try {
			auto id = row["id"].c_str();
			auto del_txn = txn();
			sql_compositor comp("zone_mob",&del_txn);
			auto room_sql = comp.del()
			    .from("zone_mob")
			    .where("id",id)
			    .sql();
			auto row = mods::pq::exec(del_txn,room_sql.data());
			mods::pq::commit(del_txn);
			++remove_count;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("zone_mob_remove_all_matching: exception",e.what());
			++error_count;
		}
	}
	return {remove_count,error_count};
}

std::tuple<bool,std::string> zone_place_remove(auto zone_table_index, auto command_index) {
	m_debug("delete zone mob placement " << zone_table_index << " command: " << command_index);
	if(zone_table_index >= zone_table.size()) {
		return {false,"zone_table_index is out of bounds!"};
	}
	try {
		auto del_txn = txn();
		sql_compositor comp("zone_mob",&del_txn);
		auto entry = zone_table[zone_table_index].cmd.begin() + command_index;
		if(entry >= zone_table[zone_table_index].cmd.end()) {
			return {false,"command_index is out of bounds!"};
		}
		auto z_mob_vnum = std::to_string(entry->arg1);
		auto z_room_vnum = std::to_string(entry->arg2);
		auto room_sql = comp.del()
		    .from("zone_mob")
		    .where("zm_mob_vnum",z_mob_vnum)
		    .op_and("zm_room_vnum","=",z_room_vnum)
		    .sql();
		auto row = mods::pq::exec(del_txn,room_sql.data());
		mods::pq::commit(del_txn);
		return {1,"deleted"};
	} catch(std::exception& e) {
		REPORT_DB_ISSUE("login exception",e.what());
		return {false,CAT("failed: '",e.what(),"'")};
	}
}

int next_zone_vnum() {
	static int next_zone_vnum = 0;
	if(next_zone_vnum == 0) {
		try {
			auto up_txn = txn();
			auto record = mods::pq::exec(up_txn,"SELECT max(zone_virtual_number) + 1 as z from zone;");
			if(record.size()) {
				std::cerr << ("Max zone number: " + std::string(record[0]["z"].c_str()));
				next_zone_vnum = mods::util::stoi<int>(record[0]["z"].c_str());
			} else {
				next_zone_vnum = 1;
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error grabbing next max zone number '",e.what());
			next_zone_vnum = 1;
		}
	}
	std::cerr << "next_zone_vnum: " << next_zone_vnum + 1 << "\n";
	return ++next_zone_vnum;
}

void disable_zone_resets() {
	mods::zone::disable_zone_resets(true);
}
void enable_zone_resets() {
	mods::zone::disable_zone_resets(false);
}
void blacklist_zone(int zone) {
	mods::zone::blacklist_zone(zone);
}

void release_zone(int zone) {
	mods::zone::release_zone(zone);
}
extern void r_error(const player_ptr_t& player,std::string_view msg);
extern void r_success(const player_ptr_t& player,std::string_view msg) ;
extern void r_status(const player_ptr_t& player,std::string_view msg) ;
namespace mods::builder {
	template <typename T>
	void rb_debug(T s) {
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
		std::cerr << "[builder-debug]: " << s << "\n";
#endif
	}
	template <typename T>
	void rb_map_debug(T& t) {
		rb_debug("dumping map");
		for(auto& pair: t) {
			rb_debug(std::string("values[") + pair.first + "]=" + pair.second);
		}
		rb_debug("Done dumping map");
	}
	int16_t delete_all_zone_data() {
		rb_debug("delete all zone data -- start transaction");
		try {
			auto del_txn = txn();
			mods::pq::exec(del_txn,std::string("DELETE FROM zone_data where 1=1;"));
			mods::pq::commit(del_txn);
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("deleting all zone data ",e.what());
			std::string error_string = "error deleting all zone data: '";
			error_string += e.what();
			rb_debug("EXCEPTION (DELETE)");
			rb_debug(e.what());
			rb_debug(error_string);
			return -5;
		}
	}

	/**
	 * Update zone commands
	 * --------------------
	 *  Flush currently zone_id's currently loaded data to the db
	 *
	 *  update_zone_commands(zone_rnum zone_id);
	 *
	 */
	std::pair<bool,std::string> update_zone_commands(zone_rnum zone_id) {
		std::size_t zid = zone_id;
		if(zid >= zone_table.size()) {
			return {false,"Zone id is out of bounds. Cannot process zone commands"};
		}
		try {
			auto t = txn();
			mods::pq::exec(t,std::string("DELETE FROM zone_data where zone_id=") + t.quote(zone_table[zone_id].number));
			mods::pq::commit(t);
			for(unsigned i = 0; i < zone_table[zone_id].cmd.size(); i++) {
				auto txn2 = txn();
				auto sql = sql_compositor("zone_data",&txn2)
				    .insert()
				    .into("zone_data")
				.values({
					{"id",std::to_string(zone_table[zone_id].number)},
					{"zone_command",std::to_string(zone_table[zone_id].cmd[i].command)},
					{"zone_if_flag",std::to_string(zone_table[zone_id].cmd[i].if_flag)},
					{"zone_arg1",std::to_string(zone_table[zone_id].cmd[i].arg1)},
					{"zone_arg2",std::to_string(zone_table[zone_id].cmd[i].arg2)},
					{"zone_arg3",std::to_string(zone_table[zone_id].cmd[i].arg3)}
				})
				.sql();
				mods::pq::exec(txn2,sql);
				mods::pq::commit(txn2);
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("update zone commands failed",e.what());
			return {false,std::string("Saving zone commands failed: ") + e.what()};
		}
		return {true,"Saved all zone commands successfully"};
	}



	using values_t = std::map<std::string,std::string>;
	std::tuple<bool,zone_pkid_t,std::string> update_zone_with_placements(int virtual_number,player_ptr_t player) {
		auto zone_start = player->builder_data->room_pavements.start_room;
		auto zone_end = zone_start;
		for(auto& room : player->builder_data->room_pavements.rooms) {
			if(zone_end < room) {
				zone_end = room;
			}
		}
		player->sendln("Zone start: " + std::to_string(zone_start));
		player->sendln("Zone end: " + std::to_string(zone_end));
		{
			try {
				values_t values;
				values["zone_start"] = std::to_string(zone_start);
				values["zone_end"] = std::to_string(zone_end);
				auto up_txn = txn();
				sql_compositor comp("zone",&up_txn);
				auto up_sql = comp
				    .update("zone")
				    .set(values)
				    .where("zone_virtual_number","=",std::to_string(virtual_number))
				    .sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return {true, 0,"success"};
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("updating zone in db",e.what());
				rb_debug("EXCEPTION (UPDATE)");
				rb_debug(e.what());
				return {false,-1,e.what()};
			}
		}
	}
	std::tuple<bool,zone_pkid_t> save_zone_to_db(int virtual_number,std::string_view zone_name,int zone_start,int zone_end,int lifespan,int reset_mode) {
		{
			std::map<std::string,std::string> values = {
				{"zone_virtual_number",std::to_string(virtual_number)},
				{"zone_start",std::to_string(zone_start)},
				{"zone_end",std::to_string(zone_end)},
				{"zone_name",zone_name.data()},
				{"lifespan",std::to_string(lifespan)},
				{"reset_mode",std::to_string(reset_mode)}
			};
			try {
				auto txn2 = txn();
				sql_compositor comp("zone",&txn2);
				auto sql = comp
				    .insert()
				    .into("zone")
				    .values(values)
				    .sql();
				rb_map_debug(values);
				rb_debug(std::string("save_zone_to_db: ") + std::string(sql.data()));
				mods::pq::exec(txn2,sql);
				mods::pq::commit(txn2);
				rb_debug("Done comitting");
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("saving zone in db",e.what());
				rb_debug("EXCEPTION");
				rb_debug(e.what());
				return {false,-1};
			}
		}
		{
			try {
				auto grab_zone_pk_id_txn = txn();
				sql_compositor comp("zone",&grab_zone_pk_id_txn);
				auto zone_select_sql = comp.select("id")
				    .from("zone")
				    .where("zone_virtual_number","=",std::to_string(virtual_number))
				    .sql();
				rb_debug(zone_select_sql);
				auto zone_record = mods::pq::exec(grab_zone_pk_id_txn,zone_select_sql);
				return {true,mods::util::stoi<zone_pkid_t>(zone_record[0]["id"].c_str())};
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("selecting zone in db",e.what());
				rb_debug("EXCEPTION");
				rb_debug(e.what());
				return {false,-2};
			}
		}
		return {false,-3};
	}


	static std::map<int,int> room_mappings;

	extern int save_to_db(room_rnum in_room,std::string& error_string) ;
	extern int import_room(struct room_data* room);
	bool delete_zone(int id) {
		try {
			auto t = mods::pq::transaction(*mods::globals::pq_con);
			std::string delete_sql = std::string("DELETE FROM zone where id=") + t.quote(id);
			mods::pq::exec(t,delete_sql);
			mods::pq::commit(t);
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("deleting zone ",e.what());
			rb_debug("EXCEPTION (DELETE)");
			rb_debug(e.what());
			return false;
		}
		return true;
	}
	std::pair<bool,std::string> zone_place(int zone_id,std::string_view zone_command,std::string_view if_flag,std::string_view arg1,std::string_view arg2,std::string_view arg3) {
		std::cerr << red_str("STATUS: zone_place[zone_id]:'") << zone_id << "'\n";
		std::cerr << red_str("STATUS: zone_place[zone_command.data()]:'") << zone_command.data() << "'\n";
		std::cerr << red_str("STATUS: zone_place[if_flag.data()]:'") << if_flag.data() << "'\n";
		std::cerr << red_str("STATUS: zone_place[arg1.data()]:'") << arg1.data() << "'\n";
		std::cerr << red_str("STATUS: zone_place[arg2.data()]:'") << arg2.data() << "'\n";
		std::cerr << red_str("STATUS: zone_place[arg3.data()]:'") << arg3.data() << "'\n";
		std::cerr << red_str("STATUS: zone_table[") << zone_id << "].number:'" << std::to_string(zone_table[zone_id].number) << "'\n";
		std::string yaml_file;
		if(zone_command.compare("R") == 0) {
			yaml_file = arg3.data();
			try {
				auto t = txn();
				sql_compositor comp("zone_data",&t);
				/**
					 * arg1 = room_vnum
					 * arg2 = max
					 * arg3 = yaml csv
					 */
				auto sql = comp
				    .insert()
				    .into("zone_data")
				.values({
					{"zone_id",std::to_string(zone_table[zone_id].number)},
					{"zone_command",zone_command.data()},
					{"zone_if_flag",if_flag.data()},
					{"zone_arg1",arg1.data()},
					{"zone_arg2",arg2.data()},
					{"zone_arg3","0"},
					{"zone_yaml",yaml_file},
				})
				.sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("error",e.what());
				return {false,std::string("Exception occurred: ") + e.what()};
			}
			return {true,"Saved zone successfully."};
		}
		if(zone_command.compare("Y") == 0) {
			yaml_file = arg1.data();
			try {
				auto t = txn();
				sql_compositor comp("zone_data",&t);
				auto sql = comp
				    .insert()
				    .into("zone_data")
				.values({
					{"zone_id",std::to_string(zone_table[zone_id].number)},
					{"zone_command",zone_command.data()},
					{"zone_if_flag",if_flag.data()},
					{"zone_arg1","0"},
					{"zone_arg2",arg2.data()},
					{"zone_arg3",arg3.data()},
					{"zone_yaml",yaml_file},
				})
				.sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("error",e.what());
				return {false,std::string("Exception occurred: ") + e.what()};
			}
			return {true,"Saved zone successfully."};
		}
		try {
			auto t = txn();
			sql_compositor comp("zone_data",&t);
			auto sql = comp
			    .insert()
			    .into("zone_data")
			.values({
				{"zone_id",std::to_string(zone_table[zone_id].number)},
				{"zone_command",zone_command.data()},
				{"zone_if_flag",if_flag.data()},
				{"zone_arg1",arg1.data()},
				{"zone_arg2",arg2.data()},
				{"zone_arg3",arg3.data()},
			})
			.sql();
			mods::pq::exec(t,sql);
			mods::pq::commit(t);
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error",e.what());
			return {false,std::string("Exception occurred: ") + e.what()};
		}
		return {true,"Saved zone successfully."};
	}
	std::pair<bool,std::string> zone_place_mob(
	    int zone_id,
	    std::string_view arg_mob_vnum,
	    std::string_view arg_room_vnum,
	    std::string_view arg_max
	) {
		if(zone_id >= zone_table.size()) {
			std::cerr << red_str("STATUS: INVALID ZONE ID: ") << zone_id << "\n";
			return {false,"Invalid zone id!"};
		}
		std::cerr << red_str("STATUS: zone_place[zone_id]:'") << zone_id << "'\n";
		std::cerr << red_str("STATUS: zone_place[arg_mob_vnum.data()]:'") << arg_mob_vnum.data() << "'\n";
		std::cerr << red_str("STATUS: zone_place[arg_room_vnum.data()]:'") << arg_room_vnum.data() << "'\n";
		std::cerr << red_str("STATUS: zone_place[arg_max.data()]:'") << arg_max.data() << "'\n";
		std::cerr << red_str("STATUS: zone_table[") << zone_id << "].number:'" << std::to_string(zone_table[zone_id].number) << "'\n";
		std::string yaml_file;
		try {
			auto t = txn();
			sql_compositor comp("zone_mob",&t);
			auto sql = comp
			    .insert()
			    .into("zone_mob")
			.values({
				{"zm_mob_vnum",arg_mob_vnum.data()},
				{"zm_room_vnum",arg_room_vnum.data()},
				{"zm_max",arg_max.data()},
			})
			.sql();
			mods::pq::exec(t,sql);
			mods::pq::commit(t);
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error",e.what());
			return {false,std::string("Exception occurred: ") + e.what()};
		}
		return {true,"Saved zone successfully."};
	}
};

using args_t = std::vector<std::string>;

/**
 * Notice: 2019-03-19
 * For some reason, I neglected to add zone virtual numbers to the
 * zone schema. In order for a room to be assigned a zone, it has to know
 * the zone's primary key in the table (which is broken because pk's are not
 * controlled by the builder). We *need* to incorporate virtual zone
 * numbers.
 *
 */
SUPERCMD(do_zbuild) {
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("place") == 0) {
		player->pager_start();
		*player <<
		    "Each command consists of a letter, identifying the command-type,\r\n"<<
		    "followed by three or four arguments.  The first argument, common to all the\r\n"<<
		    "commands, is called the ``if-flag.''  If the if-flag for a command is 1, that\r\n"<<
		    "command is only executed if the command immediately before it was executed\r\n"<<
		    "as well.  If the if-flag is 0, the command is always executed.  If-flags are\r\n"<<
		    "useful for things like equipping mobiles--you don't want to try to equip\r\n"<<
		    "a mobile that has not been loaded.\r\n"<<
		    "Commands that load mobiles and objects also include a ``max existing''\r\n"<<
		    "argument.  This specifies the maximum number of copies of the mobile or\r\n"<<
		    "object that are allowed to exist in the entire world at once.  If the number\r\n"<<
		    "currently existing is greater than or equal to the ``max existing'' limit, the\r\n"<<
		    "command is not executed.\r\n"<<
		    "The valid zone-reset commands are M, O, G, E, P, D, and R.\r\n"<<
		    "\r\n"<<
		    "{gld}M: load a mobile{/gld}Format: M <if-flag> <mob vnum> <max existing> <room vnum>\r\n"<<
		    "Mob vnum is the vnum of the mob to be loaded.  Room vnum is the vnum of\r\n"<<
		    "the room in which the mob should be placed.  The mob will be loaded\r\n"<<
		    "into the room.\r\n"<<
		    "\r\n"<<
		    "{gld}O: load an object{/gld}Format: O <if-flag> <obj vnum> <max existing> <room vnum>\r\n"<<
		    "Obj vnum is the vnum of the obj to be loaded.  Room vnum is the vnum of\r\n"<<
		    "the room in which the obj should be placed.  The object will be loaded\r\n"<<
		    "and left lying on the ground.\r\n"<<
		    "\r\n"<<
		    "{gld}G: give object to mobile{/gld}Format: G <if-flag> <obj vnum> <max existing>\r\n"<<
		    "Obj vnum is the vnum of the obj to be given.  The object will be loaded\r\n"<<
		    "and placed in the inventory of the last mobile loaded with an ``M''\r\n"<<
		    "command.\r\n"<<
		    "This command will usually be used with an if-flag of 1, since\r\n"<<
		    "attempting to give an object to a non-existing mobile will result in an\r\n"<<
		    "error.\r\n"<<
		    "\r\n"<<
		    "{gld}E: equip mobile with object{/gld}Format: E <if-flag> <obj vnum> <max existing> <equipment position>\r\n"<<
		    "Obj vnum is the vnum of the obj to be equipped.  The object will be\r\n"<<
		    "loaded and added to the equipment list of the last mobile loaded with\r\n"<<
		    "an ``M'' command.  Equipment Position should be one of the following:\r\n"<<
		    "\r\n"<<
		    "          0    Used as light\r\n"<<
		    "          1    Worn on right finger\r\n"<<
		    "          2    Worn on left finger\r\n"<<
		    "          3    First object worn around neck\r\n"<<
		    "          4    Second object worn around neck\r\n"<<
		    "          5    Worn on body\r\n"<<
		    "          6    Worn on head\r\n"<<
		    "          7    Worn on legs\r\n"<<
		    "          8    Worn on feet\r\n"<<
		    "          9    Worn on hands\r\n"<<
		    "          10   Worn on arms\r\n"<<
		    "          11   Worn as shield\r\n"<<
		    "          12   Worn about body\r\n"<<
		    "          13   Worn around waist\r\n"<<
		    "          14   Worn around right wrist\r\n"<<
		    "          15   Worn around left wrist\r\n"<<
		    "          16   Wielded as a weapon\r\n"<<
		    "          17   Held\r\n"<<
		    "\r\n"<<
		    "This command will usually be used with an if-flag of 1, since\r\n"<<
		    "attempting to give an object to a non-existing mobile will result in an\r\n"<<
		    "error.\r\n"<<
		    "\r\n"<<
		    "{gld}P: put object in object{/gld}Format: P <if-flag> <obj vnum 1> <max existing> <obj vnum 2>\r\n"<<
		    "An object with Obj Vnum 1 will be loaded, and placed inside of the copy\r\n"<<
		    "of Obj Vnum 2 most recently loaded.\r\n"<<
		    "This command will usually be used with an if-flag of 1, since\r\n"<<
		    "attempting to put an object inside of a non-existing object will result\r\n"<<
		    "in an error.\r\n"<<
		    "\r\n"<<
		    "{gld}D: set the state of a door{/gld}Format: D <if-flag> <room vnum> <exit num> <state>\r\n"<<
		    "Room vnum is the virtual number of the room with the door to be set.\r\n"<<
		    "Exit num being one of:\r\n"<<
		    "\r\n"<<
		    "          0    North\r\n"<<
		    "          1    East\r\n"<<
		    "          2    South\r\n"<<
		    "          3    West\r\n"<<
		    "          4    Up\r\n"<<
		    "          5    Down \r\n"<<
		    "\r\n"<<
		    "State being one of:\r\n"<<
		    "\r\n"<<
		    "          0    Open\r\n"<<
		    "          1    Closed\r\n"<<
		    "          2    Closed and locked\r\n"<<
		    "\r\n"<<
		    "Care should be taken to set both sides of a door correctly.  Closing\r\n"<<
		    "the north exit of one room does not automatically close the south exit\r\n"<<
		    "of the room on the other side of the door.\r\n"<<
		    "\r\n"<<
		    "{gld}R: remove object from room{/gld}Format: R <if-flag> <room vnum> <obj vnum>\r\n"<<
		    "If an object with vnum Obj Vnum exists in the room with vnum Room Vnum,\r\n"<<
		    "it will be removed from the room and purged.\r\n"<<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0) {
		player->pager_start();
		*player << "usage: \r\n" <<
		    " {grn}zbuild{/grn} {red}help{/red}\r\n" <<
		    "  |--> this help menu\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}zbuild{/wht} {gld}help{/gld}\r\n" <<
		    "  |:: (this help menu will show up)\r\n" <<
		    " {grn}zbuild{/grn} {red}new <virtual_number> <zone start> <zone end> <zone name> <zone lifespan> <zone reset mode>{/red}\r\n" <<
		    "  |--> Creates a new zone and maps the parameters to each field in the database.\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}zbuild{/wht} {gld}new 15 1200 1299 {/gld}\"The never ending frost\" 90 2\r\n" <<
		    "  |:: (creates a new zone which starts at rnum 1200 and ends on 1209\r\n" <<
		    "  |:: \"The never ending frost\" will be the name of the zone. Quotes must be \r\n" <<
		    "  |:: used here. 90 is the lifespan and 2 is the most common reset \r\n" <<
		    "  |:: mode so leave it at that for now. The 15 represents the virtual number \r\n" <<
		    "  |:: which will be assigned to this zone.)\r\n" <<
		    " {grn}zbuild{/grn} {red}list{/red}\r\n" <<
		    "  |--> lists the current zones saved to the db\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}zbuild{/wht} {gld}list{/gld}\r\n"<<
		    " {grn}zbuild{/grn} {red}delete <id>...<N>{/red}\r\n" <<
		    "  |--> deletes the zone from the db with the id <id>. Multiple IDs can be specified.\r\n" <<
		    "  |--> Please note that this command accepts the zone's id and NOT the virtual number for the zone.\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}zbuild{/wht} {gld}delete 1{/gld}\r\n" <<
		    " {grn}zbuild{/grn} {red}mob <zone_id> <mob_vnum> <room_vnum> <max> <if_flag>{/red}\r\n" <<
		    "  |--> places the mob identified by mob_vnum in the room room_vnum\r\n" <<
		    " {grn}zbuild{/grn} {red}mob-remove <mob_vnum> <room_vnum>{/red}\r\n" <<
		    "  |--> removes the first matching row where mob_vnum and room_vnum match.\r\n" <<
		    "       To remove all matches where mob_vnum and room_vnum match, use the \r\n" <<
		    "       {grn}zbuild{/grn} {red}mob-remove-all{/red} command.\r\n" <<
		    " {grn}zbuild{/grn} {red}mob-remove-all <mob_vnum> <room_vnum>{/red}\r\n" <<
		    "  |--> removes all matching rows where mob_vnum and room_vnum match.\r\n" <<
		    " {grn}zbuild{/grn} {red}obj <zone_id> <obj_vnum> <room_vnum> <max> <if_flag>{/red}\r\n" <<
		    "  |--> places object obj_vnum in room room_vnum\r\n" <<
		    " {grn}zbuild{/grn} {red}obj2mob <zone_id> <obj_vnum> <mob_vnum> <max> <if_flag>{/red}\r\n" <<
		    "  |--> gives object obj_vnum to mob mob_vnum\r\n" <<
		    " {grn}zbuild{/grn} {red}obj2obj <zone_id> <obj_vnum> <obj_vnum2> <max> <if_flag>{/red}\r\n" <<
		    "  |--> places object obj_vnum into object obj_vnum2\r\n" <<
		    " {grn}zbuild{/grn} {red}yaml <zone_id> <yaml> <room_vnum> <max> <if_flag>{/red}\r\n" <<
		    "  |--> places the yaml file identified by yaml in the room room_vnum\r\n" <<
		    " {grn}zbuild{/grn} {red}random-item-spawn <room_vnum> <max> <yaml>...[yaml-N]{/red}\r\n" <<
		    "  |--> randomly place one of the specified yaml files in this room\r\n" <<
		    "\r\n" <<
		    " /-------------------------------------------------------------\\\r\n" <<
		    " | P A V E M E N T S  S Y S T E M                   version 0.1|\r\n" <<
		    " |_____________________________________________________________/\r\n" <<
		    " {grn}zbuild{/grn} {red}pave <mob|obj> <mob_vnum|obj_vnum>{/red}\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}zbuild{/wht} {gld}pave mob 1050{/gld}\r\n" <<
		    "  |:: (remembers the mob with vnum of 1050. You can then type 'zbuild here'\r\n" <<
		    "  |:: to place a mob in the room you are currently in.)\r\n"<<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}zbuild{/wht} {gld}pave obj 90{/gld}\r\n" <<
		    "  |:: (remembers the obj with vnum of 90. You can then type 'zbuild here <obj>'\r\n" <<
		    "  |:: to place the object in the specified object. If you don't specify <obj>\r\n" <<
		    "  |:: then it will default to placing the object within the room you are currently\r\n" <<
		    "  |:: standing in.)\r\n" <<
		    " {grn}zbuild{/grn} {red}here [obj]{/red}\r\n" <<
		    "  |___[example]\r\n" <<
		    "  |:: (places the object or mob in the current room (if no arguments are given).\r\n" <<
		    "  |:: if [obj] is specified then this command will place the current object in [obj]\r\n" <<
		    "  |:: Obviously, you can't place a mob in an object so [obj] is only honoured for objects)\r\n" <<
		    " {grn}zbuild{/grn} {red}pave list{/red}\r\n" <<
		    "  |:: (lists all the pavements currently in your session)\r\n" <<
		    " {grn}zbuild{/grn} {red}switch <id>{/red}\r\n" <<
		    "  |:: (switches to the pavement with and id of <id>. To see current\r\n" <<
		    "  |:: pavements use 'zbuild pave list')\r\n" <<
		    "\r\n" <<
		    " /-------------------------------------------------------------\\\r\n" <<
		    " | M A N U A L  P L A C E M E N T S                            |\r\n" <<
		    " |_____________________________________________________________/\r\n" <<
		    //auto zone_command = args[1];
		    //auto if_flag = args[2];
		    //auto arg1 = args[3];
		    //auto arg2 = args[4];
		    //auto arg3 = args[5];
		    //mods::builder::zone_place(zone,zone_command,if_flag,arg1,arg2,arg3);
		    " {grn}zbuild{/grn} {red}place <zone_id> <command> <if_flag> <arg1> <arg2> <arg3>{/red}\r\n" <<
		    "  |--> creates a reset command for the zone 'zone_id'.\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}zbuild{/wht} {gld}place 5 M 0 1500 500 300{/gld}\r\n" <<
		    "  |:: (creates a reset command that grabs the mobile (specified by M) and uses \r\n" <<
		    "  |:: the three arguments 1500, 500, and 300 as the arguments to the reset zone\r\n" <<
		    "  |:: function\r\n" <<
		    "  |::  Commands:\r\n" <<
		    "  |:: 'M': Read a mobile\r\n" <<
		    "  |:: 'O': Read an object\r\n" <<
		    "  |:: 'G': Give obj to mob\r\n" <<
		    "  |:: 'P': Put obj in obj\r\n" <<
		    "  |:: 'G': Obj to char\r\n" <<
		    "  |:: 'E': Obj to char equip\r\n" <<
		    "  |:: 'D': Set state of door\r\n" <<
		    "  |:: )\r\n" <<
		    " {red}see: zbuild help place{/red}\r\n" <<
		    " {grn}zbuild{/grn} {red}place-list <zone_index>...[zone_index-N]{/red}\r\n" <<
		    "  |--> lists all place commands for 'zone_id'. multiple indexes can be passed.\r\n" <<
		    " {grn}zbuild{/grn} {red}place-list-vnum <zone_vnum>...[zone_vnum-N]{/red}\r\n" <<
		    "  |--> lists all place commands for zone by vnum. multiple vnums can be passed.\r\n" <<
		    " {grn}zbuild{/grn} {red}place-remove <zone_id> <place_id>{/red}\r\n" <<
		    "  |--> removes the place command 'place_id' in zone 'zone_id'\r\n" <<
		    "\r\n" <<
		    " {grn}zbuild{/grn} {red}delete-by-mob-vnum <mob_vnum>{/red}\r\n" <<
		    "  |--> removes all data within the zone_data table that are mob placements using mob_vnum.\r\n" <<
		    "\r\n"
		    "{red}---[ DANGEROUS COMMANDS ]---{/red}\r\n" <<
		    " {grn}zbuild{/grn} {red} remove-all-zone-data-from-db{/red}\r\n" <<
		    "  |--> removes all data within the zone_data table.\r\n" <<
		    "{red}---[ END DANGEROUS COMMANDS ]---{/red}\r\n" <<
		    "\r\n"
		    ;
		player->pager_end();
		player->page(0);
		return;
	}
	auto display_zone_commands = [&](const std::size_t& i) {
		if(i >= zone_table.size()) {
			r_error(player,"{zbuild} Out of bounds");
			return;
		}
		int ctr = 0;
		for(const auto& ZCMD : zone_table[i].cmd) {
			if(ZCMD.command == '*') {
				break;
			}
			*player << "{red}[{/red}" << ctr++ << "{red}] ->{/red}";
			switch(ZCMD.command) {
				case 'M':
					*player << "{red}mobile{/red}: " << ZCMD.arg1 << "{red} to room: {/red}" << ZCMD.arg2 << " {yel}max:{/yel}" << ZCMD.arg3 << "\r\n";
					break;
				default:
					*player << "{red}unimplimented command{/red}: [" << ZCMD.command << "] " << ZCMD.arg1 << " " << ZCMD.arg2 << " " << ZCMD.arg3 << "\r\n";
					break;
				case 'Y':
					*player << "{red}place yaml{/red}: " << ZCMD.yaml << "{red} to room: {/red}" << ZCMD.arg2 <<  " {yel}max:{/yel}" << ZCMD.arg3 << "\r\n";
					break;
					//case 'G':
					//	*player << "{red}give object{/red}: " << ZCMD.arg1 << "{red} to mobile{/red}: " << current_mobile << "\r\n";
					//	break;
					//case 'E':
					//	*player << "{red}equip mobile({/red}" << current_mobile << "{red}) with object{/red}: " << ZCMD.arg1 << "\r\n";
					//	break;
					//case 'P':
					//	*player << "{red}put object({/red}" << ZCMD.arg1 << "{red}) in object{/red}: " << ZCMD.arg3 << "\r\n";
					//	break;
					//case 'D':
					//	*player << "{red}set state of door{/red}:" << ZCMD.arg1 << " {red}to{/red}:" << ZCMD.arg2 << "\r\n";
					//	break;
					//case 'R':
					//	*player << "{red}remove object({/red}" << ZCMD.arg2 << "{red}) from room{/red}:" << ZCMD.arg1 << "\r\n";
					//	break;
			}
		}
		return;
	};
	if(argshave()->first_is("remove-all-zone-data-from-db")->passed()) {
		player->sendln("{zbuild remove-all-zone-data-from-db} DELETING ZONE_DATA COMMANNDS FROM DB...");
		auto s = mods::builder::delete_all_zone_data();
		player->sendln(CAT("{zbuild remove-all-zone-data-from-db} Delete zone data status: ",s,". Zero means success. Negative means error"));
		player->sendln("{zbuild remove-all-zone-data-from-db} DONE");
		return;
	}
	if(argshave()->first_is("delete-by-mob-vnum")->passed()) {
		if(argshave()->size_gt(1)->passed() == false) {
			r_error(player,"{zbuild delete-by-vnum} Specify a mob vnum");
			return;
		}
		auto s = mods::builder::delete_by_mob_vnum(argat(1));
		if(!std::get<0>(s)) {
			r_error(player,std::get<1>(s));
			return;
		}
		r_success(player,std::get<1>(s));
		return;
	}
	if(argshave()->int_at(1)->first_is("place-list")->size_gt(1)->passed()) {
		for(const auto& i : args()->gather_integers_starting_at(1)) {
			display_zone_commands(i);
		}
		return;
	}
	//" {grn}zbuild{/grn} {red}random-item-spawn  <max> <yaml>...[yaml-N]{/red}\r\n" <<
	if(argshave()->int_at(1)->first_is("random-item-spawn")->size_gt(2)->passed()) {
		auto yaml_files = args()->gather_tokens_starting_at(3);
		auto csv = IMPLODE(yaml_files,"|");
		/**
					 * arg1 = room_vnum
					 * arg2 = max
					 * arg3 = yaml csv
					 */
		auto result = mods::builder::zone_place(player->zone(),"R","0",std::to_string(player->vnum()),argat(1),csv);
		if(!std::get<0>(result)) {
			r_error(player,CAT("{zbuild random-item-spawn} ",std::get<1>(result)));
		} else {
			r_success(player,CAT("{zbuild random-item-spawn} ",std::get<1>(result)));
		}
		return;
	}
	if(argshave()->int_at(1)->first_is("place-list-vnum")->size_gt(1)->passed()) {
		for(const auto& i : args()->gather_integers_starting_at(1)) {
			std::size_t index = 0;
			for(const auto& zone : zone_table) {
				if(zone.number == i) {
					display_zone_commands(index);
				}
				++index;
			}
		}
		return;
	}
	constexpr unsigned int max_char = 10;
	std::array<char,max_char> command;
	std::fill(command.begin(),command.end(),0);
	one_argument(argument,&command[0],max_char);
	auto args = mods::util::subcmd_args<5,args_t>(argument,"save");
	if(args.has_value()) {
		auto arg_vec = args.value();
		if(arg_vec.size() < 2) {
			r_error(player,"{zbuild save} Please provide a zone id");
			return;
		}
		int zone = 0;
		if(arg_vec[1].compare("this") == 0) {
			zone = world[player->room()].zone;
		} else {
			auto zone_id = mods::util::stoi(arg_vec[1]);
			if(!zone_id.has_value()) {
				r_error(player,"{zbuild save} Invalid zone id");
				return;
			}
			zone = zone_id.value();
		}
		if(zone >= zone_table.size()) {
			r_error(player,"{zbuild save} Out of bounds");
			return;
		}
		mods::builder_util::value_callback value_callback = [&](sql_compositor::value_map & values) {
			values["zone_virtual_number"] = mods::util::itoa(zone_table[zone].number);
			values["zone_start"] = mods::util::itoa(zone_table[zone].bot);
			values["zone_end"] = mods::util::itoa(zone_table[zone].top);
			values["zone_name"] = zone_table[zone].name;
			values["lifespan"] = std::to_string(zone_table[zone].lifespan);
			values["reset_mode"] = std::to_string(zone_table[zone].reset_mode);
		};
		mods::builder_util::post_modify_callback post_modify_callback = []() -> std::pair<bool,std::string> {
			return {true,""};
		};
		auto status = mods::builder_util::save_to_db<std::string>(
		        "zone",
		        "id",
		        std::to_string(zone_table[zone].get_id()),
		        value_callback,
		        post_modify_callback
		    );
		if(status.first) {
			r_success(player,"{zbuild save} Saved zone");
		} else {
			r_error(player,CAT("{zbuild save} ",status.second));
		}
		auto status2 = mods::builder::update_zone_commands(zone);
		if(status2.first) {
			r_success(player,"{zbuild save} Saved zone commands");
		} else {
			r_error(player,CAT("{zbuild save} ",status2.second));
		}
		return;
	}
	auto vargs = PARSE_ARGS();
	if(vargs.size() && vargs[0].compare("mob") == 0) {
		int zone = 0;
		if(vargs[1].compare("this") == 0) {
			zone = world[player->room()].zone;
			player->sendln(CAT("{zbuild mob} Using current zone id:", zone));
		} else {
			auto zone_id = mods::util::stoi(vargs[1]);
			if(!zone_id.has_value() || static_cast<unsigned>(zone_id.value()) >= zone_table.size()) {
				r_error(player,"{zbuild mob} Invalid zone id");
				return;
			}
			zone = zone_id.value();
		}
		if(vargs.size() < 5) {
			r_error(player,"{zbuild mob} Not enough arguments");
			return;
		}
		for(int i=0; i < vargs.size(); i++) {
			player->sendln(CAT("i:",i,":'",vargs[i],"'"));
		}
		/**
		 * [0] 'mob',
		 * [1] zone_id
		 * [2] mob_vnum
		 * [3] room_vnum
		 * [4] max
		 */
		auto arg_mob_vnum = vargs[2];
		auto arg_room_vnum = vargs[3].compare("this") == 0 ? std::to_string(world[player->room()].number) : vargs[3];
		auto arg_max = vargs[4];
		auto result = mods::builder::zone_place_mob(zone,arg_mob_vnum,arg_room_vnum,arg_max);
		if(!result.first) {
			r_error(player,CAT("{zbuild mob} ",result.second));
		} else {
			r_success(player,"{zbuild mob} Placed mob in zone successfully");
		}
		return;
	}
	if(vargs.size() && vargs[0].compare("mob-remove") == 0) {
		/**
		 * [0] 'mob-remove'
		 * [1] mob_vnum
		 * [2] room_vnum
		 */
		if(vargs.size() < 3) {
			r_error(player,"{zbuild mob-remove} Not enough arguments. Usage: zbuild mob-remove <mob_vnum> <this|room_vnum>");
			return;
		}
		for(int i=0; i < vargs.size(); i++) {
			player->sendln(CAT("i:",i,":'",vargs[i],"'"));
		}
		/**
		 * [0] 'mob-remove'
		 * [1] mob_vnum
		 * [2] room_vnum
		 */
		auto arg_mob_vnum = vargs[1];
		auto arg_room_vnum = vargs[2].compare("this") == 0 ? std::to_string(world[player->room()].number) : vargs[2];
		auto result = zone_mob_remove_first_matching(arg_mob_vnum,arg_room_vnum);
		if(!result.first) {
			r_error(player,CAT("{zbuild mob-remove} ",result.second));
		} else {
			r_success(player,"{zbuild mob-remove} Removed 1 row successfully");
		}
		return;
	}
	if(vargs.size() && vargs[0].compare("mob-remove-all") == 0) {
		/**
		 * [0] 'mob-remove-all'
		 * [1] mob_vnum
		 * [2] room_vnum
		 */
		if(vargs.size() < 3) {
			r_error(player,"{zbuild mob-remove-all} Not enough arguments. Usage: zbuild mob-remove-all <mob_vnum> <this|room_vnum>");
			return;
		}
		for(int i=0; i < vargs.size(); i++) {
			player->sendln(CAT("i:",i,":'",vargs[i],"'"));
		}
		/**
		 * [0] 'mob-remove-all'
		 * [1] mob_vnum
		 * [2] room_vnum
		 */
		auto arg_mob_vnum = vargs[1];
		auto arg_room_vnum = vargs[2].compare("this") == 0 ? std::to_string(world[player->room()].number) : vargs[2];
		auto result = zone_mob_remove_all_matching(arg_mob_vnum,arg_room_vnum);
		r_success(player,CAT("{zbuild mob-remove-all} Removed ",result.first," row(s) successfully with ",result.second, " error(s)."));
		return;
	}
	if(vargs.size() && vargs[0].compare("yaml") == 0) {
		int zone = 0;
		if(vargs[1].compare("this") == 0) {
			zone = world[player->room()].zone;
			player->sendln(CAT("{zbuild yaml} Using current zone id:", zone));
		} else {
			auto zone_id = mods::util::stoi(vargs[1]);
			if(!zone_id.has_value() || static_cast<unsigned>(zone_id.value()) >= zone_table.size()) {
				r_error(player,"{zbuild yaml} Invalid zone id");
				return;
			}
			zone = zone_id.value();
		}
		if(vargs.size() < 6) {
			r_error(player,"{zbuild yaml} Not enough arguments");
			return;
		}
		for(int i=0; i < vargs.size(); i++) {
			player->sendln(CAT("i:",i,":'",vargs[i],"'"));
		}
		/**
		 * [0] 'mob',
		 * [1] zone_id
		 * [2] mob_vnum
		 * [3] room_vnum
		 * [4] max
		 * [5] if_flag
		 */
		auto zone_command = "Y";
		auto arg1 = vargs[2]; /** obj_vnum */
		auto arg2 = /** room_vnum */ vargs[3].compare("this") == 0 ? std::to_string(world[player->room()].number) : vargs[3];
		auto arg3 = vargs[4]; /** max */
		auto if_flag = vargs[5];
		//zone_place(int zone_id,std::string_view zone_command,if_flag,arg1,arg2,arg3)
		/**
		 * ---------------------------------
		 *  !!! THIS DIFFERS FROM LEGACY !!!
		 * ---------------------------------
		 * The order here matters. Check mods/zone.cpp for how this differs from legacy code
		 * when reading a mobile ("M" command)
		 * arg1 = mob_vnum
		 * arg2 = room_vnum
		 * arg3 = max
		 * ---------------------------------
		 *  !!! THIS DIFFERS FROM LEGACY !!!
		 * ---------------------------------
		 */
		auto result = mods::builder::zone_place(zone,zone_command,if_flag,arg1,arg2,arg3);
		if(!result.first) {
			r_error(player,CAT("{zbuild yaml} ",result.second));
		} else {
			r_success(player,"{zbuild yaml} Placed yaml in zone successfully");
		}
		return;
	}
	if(std::string(&command[0]).compare("place") == 0) {
		std::string arg = argument;
		int zone = 0;
		auto past = arg.substr(arg.find("place ")+6);
		auto args = mods::util::arglist<std::vector<std::string>>(past);
		if(args[0].compare("this") == 0) {
			zone = world[player->room()].zone;
		} else {
			auto zone_id = mods::util::stoi(args[0]);
			if(!zone_id.has_value() || static_cast<unsigned>(zone_id.value()) >= zone_table.size()) {
				r_error(player,"{zbuild place} Invalid zone id");
				return;
			}
			zone = zone_id.value();
		}
		if(args.size() < 6) {
			r_error(player,"{zbuild place} Not enough arguments");
			return;
		}
		auto zone_command = args[1];
		auto if_flag = args[2];
		auto arg1 = args[3];
		auto arg2 = args[4];
		auto arg3 = args[5];
		mods::builder::zone_place(zone,zone_command,if_flag,arg1,arg2,arg3);
		r_success(player,"{zbuild place} Placed object in zone");
		return;
	}
	if(std::string(&command[0]).compare("delete") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("delete ")+7);
		for(auto id : mods::util::arglist<std::vector<std::string>>(past)) {
			auto zone = mods::util::stoi(id);
			if(zone.value_or(-1) == -1) {
				r_error(player,"{zbuild delete} Unable to delete id");
				return;
			}
			mods::builder::delete_zone(zone.value());
			r_success(player,"{zbuild delete} Deleted zone");
		}
		return;
	}
	if(std::string(&command[0]).compare("new") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("new ")+4);
		//TODO: take this logic and store it in interpreter.cpp so we can reuse it
		auto arglist = mods::util::arglist<std::vector<std::string>>(past);
		if(arglist.size() != 6) {
			r_error(player,"{zbuild new} Argument list is invalid, please use the correct number of arguments");
			for(auto vstr: arglist) {
				*player << vstr << "\r\n";
			}
		} else {
			// " rbuildzone <new> <virtual_number> <zone start> <zone end> <zone name> <zone lifespan> <zone reset mode>\r\n" <<
			if(!std::get<0>(mods::builder::save_zone_to_db(
			            mods::util::stoi<int>(arglist[0]),
			            arglist[3],
			            mods::util::stoi<int>(arglist[1]),
			            mods::util::stoi<int>(arglist[2]),
			            mods::util::stoi<int>(arglist[4]),
			            mods::util::stoi<int>(arglist[5]))
			    )) {
				r_error(player,"{zbuild new} Unable to save new zone");
			} else {
				parse_sql_zones();
				r_success(player,"{zbuild new} New zone created");
			}
		}
		return;
	}
	if(std::string(&command[0]).compare("list") == 0) {
		if(!player->is_executing_js()) {
			r_status(player,"listing...");
		}
		try {
			auto t = mods::pq::transaction(*mods::globals::pq_con);
			std::string check_sql = "SELECT id,zone_start,zone_end,zone_name,lifespan,reset_mode FROM zone";
			auto check_result = mods::pq::exec(t,check_sql);
			mods::pq::commit(t);
			if(!player->is_executing_js()) {
				player->pager_start();
			}
			jxcomp jx;
			jx.array_start("zones");
			for(auto row : check_result) {
				if(!player->is_executing_js()) {
					std::string acc = "{red}";
					acc += std::string("Virtual ZoneID:{/red}");
					acc += std::to_string(row[0].as<int>());
					acc += "{/red}[";
					acc += std::to_string(row[1].as<int>());
					acc += "-";
					acc += std::to_string(row[2].as<int>());
					acc += "]{gld}{";
					acc += row["zone_name"].c_str();
					acc += "}{/gld} (";
					acc += std::to_string(row[4].as<int>());
					acc += ") (";
					acc += std::to_string(row[5].as<int>());
					acc += ")\r\n";
					*player << acc;
				} else {
					jx.object_start("")
					.push("id",row[0].as<int>())
					.push("start",row[1].as<int>())
					.push("end",row[2].as<int>())
					.push("name",row["zone_name"].c_str())
					.push("lifespan",row["lifespan"].as<int>())
					.push("reset_mode",row["reset_mode"].as<int>())
					.object_end();
				}
			}
			if(player->is_executing_js()) {
				jx.array_end();
				*player << jx.get();
			} else {
				player->pager_end();
				player->page(0);
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("listing zone ",e.what());
			mods::builder::rb_debug("EXCEPTION (SELECT)");
			mods::builder::rb_debug(e.what());
			return;
		}
		return;
	}
	//args = mods::util::subcmd_args<11,args_t>(argument,"place-list");
	args = mods::util::subcmd_args<13,args_t>(argument,"place-remove");
	if(args.has_value()) {
		auto arg_vec = args.value();
		if(arg_vec.size() < 3) {
			r_error(player,"{zbuild place-remove} Not enough arguments");
			return;
		}
		//place-remove <zone_id> <command_index>
		//     0           1           2
		auto index = mods::util::stoi(arg_vec[1]);
		if(!index.has_value()) {
			r_error(player,"{zbuild place-remove} Invalid index");
			return;
		}
		std::size_t i = index.value();
		if(i >= zone_table.size()) {
			r_error(player,"{zbuild place-remove} Out of bounds");
			return;
		}
		auto command_index = mods::util::stoi(arg_vec[2]);
		if(!command_index.has_value()) {
			r_error(player,"{zbuild place-remove} Invalid command index");
			return;
		}
		auto command_vec = zone_table[index.value()].cmd;
		std::size_t c = command_index.value();
		if(c >= command_vec.size()) {
			r_error(player,"{zbuild place-remove} Command index out of bounds");
			return;
		}
		auto status = zone_place_remove(index.value(),command_index.value());
		zone_table[index.value()].cmd.erase(
		    zone_table[index.value()].cmd.begin() + command_index.value()
		);
		if(std::get<0>(status) != true) {
			r_status(player,"{zbuild place-remove} couldnt remove from db, but we did remove it from RAM.");
		} else {
			r_status(player,"{zbuild place-remove} Index removed from db and RAM.");
		}
		return;
	}
};
