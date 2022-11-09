#include "builder.hpp"
#include "mob-roam.hpp"
#include "contracts.hpp"
#include "pq.hpp"
#include "util.hpp"
#include "object.hpp"
#include <stdlib.h> //For itoa
#include "sql.hpp"
#include "../structs.h"
#include "extern.hpp"
#include "builder_util.hpp"
#include "../shop.h"
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
#include "builder-flags.hpp"
#include "zbuild.hpp"

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
int get_raid_id(player_ptr_t player) {
	if(player->builder_data) {
		return player->builder_data->raid_id();
	}
	return 0;
}
#undef m_debug
#undef m_error
#define __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::builder::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::builder::ERROR]"))  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
	#define m_error(MSG) ;;
#endif


void show_shop_by_index(std::size_t i,player_ptr_t& player);
void r_error(const player_ptr_t& player,std::string_view msg) {
	mods::builder::report_error<player_ptr_t>(player,msg.data());
}
void r_success(const player_ptr_t& player,std::string_view msg) {
	mods::builder::report_success<player_ptr_t>(player,msg.data());
}
void r_status(const player_ptr_t& player,std::string_view msg) {
	mods::builder::report_status<player_ptr_t>(player,msg.data());
}
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
	int16_t delete_all_zone_data();


	void encode_scripted_response(player_ptr_t& player, std::string_view encoded) {
		if(!player->is_executing_js()) {
			return;
		}
		player->set_scripted_response(encoded.data());
	}
	void encode_scripted_response_safe(player_ptr_t& player, std::string_view encoded) {
		if(!player->is_executing_js()) {
			return;
		}
		player->set_scripted_response(mods::builder::encode(encoded));
	}
	void encode_scripted_response(player_ptr_t& player, str_map_t&& map) {
		if(!player->is_executing_js()) {
			return;
		}
		player->set_scripted_response(mods::builder::encode_map(map));
	}
	bool has_flag(player_ptr_t player,uint64_t flag) {
		return player->has_flag(mods::flags::chunk_type_t::BUILDER,flag);
	}
	/**
	 * Sandboxes
	 * --
	 *  Sandboxes enable the builder to start creating an area without having
	 *  to worry about overwriting or destroying the current area. Hence it's
	 *  name.
	 *
	 *  To use this feature, use rbuild_sandbox
	 */

	/**
	 * New Room
	 * --------
	 *  new_room(playaer,direction);
	 *
	 *  Create a room to the 'direction'.
	 */
	/* Factory method to generate a room for us */
	room_data new_room(player_ptr_t player,int direction);
	bool flush_to_db(char_data *ch,int room);

	extern void initialize_builder(player_ptr_t& player);

	/**
	 * Update zone commands
	 * --------------------
	 *  Flush currently zone_id's currently loaded data to the db
	 *
	 *  update_zone_commands(zone_rnum zone_id);
	 *
	 */
	extern std::pair<bool,std::string> update_zone_commands(zone_rnum zone_id) ;


	using values_t = std::map<std::string,std::string>;

	static std::map<int,int> room_mappings;

	extern int save_to_db(room_rnum in_room,std::string& error_string) ;

};

using args_t = std::vector<std::string>;

int next_room_pavement_transaction_id() {
	static int transaction_id = 0;
	return ++transaction_id;
}
char_data* grab_mobile(std::size_t index,bool& fetched);
void present_action(player_ptr_t& player, std::size_t index);
void present_mob_specials(player_ptr_t& player, std::size_t index);

enum field_type_t {
	MOB_VNUM,
	MOB_RNUM,
	STRING_COLUMN,
	SINGLE_CHAR_COLUMN,
	INTEGER_COLUMN,
	FLOATING_POINT_COLUMN
};
enum parse_response_t {
	HANDLED,
	NOT_OURS,
	INCOMPLETE
};
struct command_t;
using save_function_t = std::function<void(std::shared_ptr<command_t>)>;
struct command_t {
	str_t table;
	str_map_t column_mappings;
	str_t name;
	str_t format;
	size_t arguments;
	int mob_vnum_argument;
	int mob_id_argument;
	mob_rnum mob_id;
	mob_vnum m_vnum;
	str_map_t values;
	save_function_t save_function;

	command_t() = delete;
	command_t(
	    str_t in_table,
	    str_map_t in_column_mappings,
	    str_t in_name,
	    str_t in_format,
	    size_t in_arguments,
	    int in_mob_vnum_argument,
	    int in_mob_id_argument,
	    save_function_t in_save_function
	) :
		table(in_table),
		column_mappings(in_column_mappings),
		name(in_name),
		format(in_format),
		arguments(in_arguments),
		mob_vnum_argument(in_mob_vnum_argument),
		mob_id_argument(in_mob_id_argument),
		mob_id(0),
		m_vnum(0),
		save_function(in_save_function) {
	}
	bool ready_to_save(str_t argument) {
		auto args = mods::util::subcmd_args<50,args_t>(argument,this->name.c_str());
		if(!args.has_value() || args.value().size() < 3) {
			return false;
		}
		auto arg_vec = args.value();
		int m_id = mods::util::stoi(arg_vec[2]).value_or(-1);
		return arg_vec[1].compare("save") == 0 && m_id == this->mob_id;
	}

	parse_response_t parse(str_t argument,player_ptr_t& player) {
		auto args = mods::util::subcmd_args<50,args_t>(argument,this->name.c_str());
		if(!args.has_value()) {
			return NOT_OURS;
		}
		auto arg_vec = args.value();
		if(arg_vec.size() >= 3 && arg_vec[2].compare("save") == 0) {
			return NOT_OURS;
		}
		if(arg_vec.size() < this->arguments + 1) {
			r_error(player,"Not enough arguments.");
			return INCOMPLETE;
		}
		auto i_value = mods::util::stoi(arg_vec[this->mob_id_argument + 1]);
		if(!i_value.has_value()) {
			r_error(player,"Please specify a valid mob-id");
			return INCOMPLETE;
		}
		this->mob_id = i_value.value();
		if(mob_id >= mob_proto.size()) {
			r_error(player,"Mob id out of range");
			return INCOMPLETE;
		}
		this->m_vnum = mob_proto[this->mob_id].nr;
		std::string field = arg_vec[2];
		std::string value = arg_vec[3];
		if(mods::util::in_array<std::string>(field,map_keys(this->column_mappings)) == false) {
			r_error(player, "Please use a valid key");
			return INCOMPLETE;
		}
		for(auto& pair : this->column_mappings) {
			if(field.compare(pair.first) == 0) {
				this->values[this->column_mappings[field]] = value;
				r_success(player,CAT("Successfully set '", field, "'."));
				return HANDLED;
			}
		}
		r_error(player,"Didn't process any data.");
		return INCOMPLETE;
	}
	void print_command(player_ptr_t& player) {
		//" {grn}mbuild{/grn} {red}%s %s{/red}\r\n",this->name.c_str(),this->format.c_str());
		player->sendln(
		    CAT(
		        " {grn}mbuild{/grn} {red}",this->name.c_str()," ",this->format.c_str(),"{/red}"
		    )
		);
		player->sendln("  {gld}|:: -:[keys]:-{/gld}");
		for(auto& pair : column_mappings) {
			player->sendln(
			    CAT(
			        "  {gld}|::",pair.first.c_str()," {/gld}"
			    )
			);
		}
		player->sendln(
		    CAT(
		        " {grn}mbuild{/grn} {red}",this->name.c_str()," <mob-id> save{/red}"
		    )
		);
	}
};
static std::list<std::shared_ptr<command_t>> mob_commands;

void initialize_mob_commands();


/**
 * Notice: 2019-03-19
 * For some reason, I neglected to add zone virtual numbers to the
 * zone schema. In order for a room to be assigned a zone, it has to know
 * the zone's primary key in the table (which is broken because pk's are not
 * controlled by the builder). We *need* to incorporate virtual zone
 * numbers.
 *
 */

namespace mods::builder {
	SUPERCMD(do_nset) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("nset");
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->sendln("usage: nset <nickname>");
			player->errorln("Invalid number of arguments");
			return;
		}
		mods::rooms::register_nickname(player->room(),vec_args[0]);
		ADMIN_DONE();
	}
	SUPERCMD(do_ndelete) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("ndelete");
		mods::rooms::remove_nickname(player->room());
		ADMIN_DONE();
	}
	SUPERCMD(do_nhelp) {
		ADMIN_REJECT();
		player->sendln(
		    mods::util::admin_section("Nickname helpers") +
		    mods::util::overview() +
		    "\r\n"
		    "The various nickname commands described below are used to manipulate the nickname\r\n"
		    "of various rooms, but mostly to manipulate and query the current room's nickname that\r\n"
		    "you are standing in when you invoke the commands.\r\n"
		    "It is important to note that the nickname commands only take place while the server is\r\n"
		    "running. If the server reboots or if any code causes the world to be reloaded the nickname\r\n"
		    "data you set will be reset back to whatever state is in the database.\r\n"
		    "\r\n"
		    + mods::util::admin_section("Commands") +
		    "{grn}nset{/grn} - {yel}set the nickname for the room you're currently in.{/yel}\r\n"
		    "{grn}nfind{/grn} - {yel}find the room IDs of the nickname(s) you pass in.{/yel}\r\n"
		    "{grn}nhelp{/grn} - {yel}this help page.{/yel}\r\n"
		    "{grn}ndelete{/grn} - {yel}Delete the nickname(s) of the room you're in.{/yel}\r\n"
		    "\r\n"
		    + mods::util::admin_section("Database Commands") +
		    mods::util::overview() +
		    "If you want to make your changes permanent, do not use the nickname helpers listed above.\r\n"
		    "Instead, use {grn}rbuild{/grn}.\r\n"
		    "[documentation written on 2021-04-07]"
		);
		ADMIN_DONE();
	}
	SUPERCMD(do_nfind) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("nfind");
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->errorln("Invalid number of arguments");
			return;
		}
		std::string response;
		for(unsigned i =0; i < vec_args.size(); i++) {
			auto opt_room = mods::rooms::find_nickname(vec_args[i]);
			if(opt_room.has_value() == false) {
				response += "!,";
				player->errorln(CAT("no room with nickname found: ",vec_args[i]));
				continue;
			}
			response += CAT(vec_args[i],":",opt_room.value(),"(vnum:",world[opt_room.value()].number,"),");
		}
		player->set_scripted_response(response);
		player->sendln(response);
		ADMIN_DONE();
	}
	SUPERCMD(do_ngoto) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("ngoto");
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->errorln("Invalid number of arguments");
			return;
		}
		auto opt_room = mods::rooms::find_nickname(vec_args[0]);
		if(opt_room.has_value() == false) {
			player->set_scripted_response("!");
			player->errorln("no room with nickname found");
			return;
		}
		player->set_scripted_response(std::to_string(opt_room.value()));
		char_from_room(player->cd());
		char_to_room(player->cd(),opt_room.value());
		ADMIN_DONE();
	}

	SUPERCMD(do_admin_colon_refresh_dash_all) { // admin:refresh-all
		mods::zone::refresh_mobs_and_zones();
	}
	void init() {
		ADD_BUILDER_COMMAND("ngoto",  do_ngoto);
		ADD_BUILDER_COMMAND("nfind",  do_nfind);
		ADD_BUILDER_COMMAND("nset",  do_nset);
		ADD_BUILDER_COMMAND("ndelete",  do_ndelete);
		ADD_BUILDER_COMMAND("nhelp",  do_nhelp);
		ADD_BUILDER_COMMAND("admin:refresh-all",do_admin_colon_refresh_dash_all);
	}
};
