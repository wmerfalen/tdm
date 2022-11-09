#include "rbuild-sandbox.hpp"
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


	/**
	 * Sandboxes
	 * --
	 *  Sandboxes enable the builder to start creating an area without having
	 *  to worry about overwriting or destroying the current area. Hence it's
	 *  name.
	 *
	 *  To use this feature, use rbuild_sandbox
	 */
	sandbox_list_t sandboxes;
	sandbox_data_t::sandbox_data_t(
	    player_ptr_t player,
	    std::string_view name,
	    int starting_room_number,
	    int zone_virtual_number) {
		new_sandbox(player,name,starting_room_number,zone_virtual_number);
	}
	sandbox_data_t::sandbox_data_t() : m_name("") {}
	void sandbox_data_t::set_name(std::string_view n) {
		m_name = n;
	}
	std::string_view sandbox_data_t::name() const {
		return m_name;
	}
	/**
	 * New Sandbox
	 * -----------
	 * new_sandbox(player,name,room_vnum,zone_vnum);
	 */
	int8_t sandbox_data_t::new_sandbox(
	    player_ptr_t player,
	    std::string_view name,
	    int starting_room_number,
	    int zone_virtual_number) {
		m_player = player;
		m_builder_data = std::make_shared<builder_data_t>(0,starting_room_number,zone_virtual_number);
		m_name = name;
		return 0;
	}


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

SUPERCMD(do_rbuild_sandbox) {
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
		player->pager_start();
		*player << "usage: \r\n" <<
		    " {grn}bbuild_sandbox{/grn} {red}help{/red}\r\n" <<
		    "  |--> this help menu\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}rbuild_sandbox{/wht} {grn}help{/grn}\r\n" <<
		    "  |:: (this help menu will show up)\r\n" <<
		    " {grn}rbuild_sandbox{/grn} {red}new <name> <room_number_start> <zone_virtual_number>{/red}\r\n" <<
		    " {grn}rbuild_sandbox{/grn} {red}pave on <name>{/red}\r\n" <<
		    " {grn}rbuild_sandbox{/grn} {red}pave off{/red}\r\n" <<
		    " {grn}rbuild_sandbox{/grn} {red}save <transaction_id>{/red}\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}rbuild_sandbox{/wht} {grn}save 3{/grn}\r\n" <<
		    "  |:: (save randbox with transaction_id of 3)\r\n" <<
		    " {grn}rbuild_sandbox{/grn} {red}list{/red}\r\n" <<
		    " {grn}rbuild_sandbox{/grn} {red}delete <id>{/red}\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
	}
	{
		auto args = mods::util::subcmd_args<5,args_t>(argument,"save");
		if(args.has_value()) {
			auto cmd_args = args.value();
			if(cmd_args.size() > 0 && cmd_args[0].compare("save") == 0) {
				//auto zone_placement = mods::builder::save_zone_to_db(
				//			player->builder_data->room_pavements.zone_id,	// Zone virtual number
				//			("zone_" + std::to_string(player->builder_data->room_pavements.zone_id)).data(),	// Zone name
				//			player->builder_data->room_pavements.start_room,	// Start room
				//			player->builder_data->room_pavements.start_room + player->builder_data->room_pavements.rooms.size(), // End room
				//			100,	// Lifetime
				//			2);
				//if(!std::get<0>(zone_placement)){	// Reset mode
				//	r_error(player,"Couldn't save zone from sandbox");
				//	/** TODO: transactional behaviour wanted here. (rollback) */
				//	return;
				//}
				//r_success(player, "Zone save. Saving rooms...");
				std::string error_string = "";
				for(auto& room : player->builder_data->room_pavements.rooms) {
					error_string = "";
					auto status = mods::builder::save_to_db(
					        room,
					        error_string
					    );
					if(status != 0) {
						r_error(player, std::string("Couldn't save room ") + std::to_string(room) + " ->" + error_string);
						continue;
					}
					r_success(player, "Saved room " + std::to_string(room));
				}
				auto status = mods::builder::update_zone_with_placements(
				        player->builder_data->room_pavements.zone_id,
				        player
				    );
				if(std::get<0>(status)) {
					r_success(player,std::string("Sandbox saved. Message: ") + std::get<2>(status));
				} else {
					r_error(player, std::string("Sandbox creation failed. Message: ") + std::get<2>(status));
				}
				return;
			}//end command is save
		}
	}
	{
		auto args = mods::util::subcmd_args<5,args_t>(argument,"pave");
		if(args.has_value()) {
			ENCODE_INIT();
			auto cmd_args = args.value();
			if(cmd_args.size() == 3 && cmd_args[1].compare("on") == 0) {
				/**
				 * cmd_args will be: [0] => pave, [1] => <on|off> [2] => <name>
				 */
				auto s = mods::builder::pave_on(player,cmd_args[2]);
				if(std::get<0>(s) < 0) {
					r_error(player,CAT("Error: ",std::get<0>(s), ", msg: '",std::get<1>(s),"'"));
					return;
				}
				ENCODE_STR(player->builder_data->room_pavements.transact_id);
				r_success(player,std::get<1>(s));
				return;
			}//end pave on
			if(cmd_args.size() == 2 && cmd_args[1].compare("off") == 0) {
				mods::builder::pave_off(player);
				ENCODE_STR(player->builder_data->room_pavements.transact_id);
				r_success(player,"Turned off pave mode. Don't forget to run rbuild_sandbox save <id>.");
			}
		}
	}//end pave block
	{
		auto args = mods::util::subcmd_args<4,args_t>(argument,"new");
		if(args.has_value()) {
			auto cmd_args = args.value();
			if(cmd_args.size() == 4) {
				/**
				 * cmd_args will be: [0] => new, [1] => <name>,
				 * [2] => <starting_room_number>, [3] => <zone_virtual_number>
				 */
				mods::builder::sandbox_data_t sandbox;
				auto status = sandbox.new_sandbox(player,
				        cmd_args[1],
				        mods::util::stoi<int>(cmd_args[2]),
				        mods::util::stoi<int>(cmd_args[3])
				    );
				if(status < 0) {
					r_error(player,std::string("Failed to create sandbox! Error: #") + std::to_string(status));
					return;
				} else {
					mods::builder::sandboxes[player->name().c_str()].emplace_back(std::move(sandbox));
					player->set_flag(mods::flags::chunk_type_t::BUILDER,mods::builder::HAS_SANDBOX);
					r_success(player,"Sandbox created");
					return;
				}
			} else {
				r_error(player,"Invalid arguments. Arguments should be: new <name> <starting_room_number> <zone_virtual_number>");
				return;
			}
		}
	}//end new block
	mods::builder_util::list_object_vector<std::deque<mods::builder::sandbox_data_t>,std::string>(
	    player,
	    std::string(argument),
	    mods::builder::sandboxes[player->name().c_str()],
	[](mods::builder::sandbox_data_t sandbox) -> std::string {
		return std::string(sandbox.name());
	}
	);
	{
		auto args = mods::util::subcmd_args<7,args_t>(argument,"delete");
		if(args.has_value()) {
			auto cmd_args = args.value();
			if(cmd_args.size() == 2) {
				std::size_t sandbox_count = mods::builder::sandboxes[player->name().c_str()].size();
				std::size_t index = mods::util::stoi<std::size_t>(cmd_args[1]);
				if(index >= sandbox_count) {
					r_error(player,"Index is too large");
					return;
				} else {
					auto it = mods::builder::sandboxes[player->name().c_str()].begin() + index;
					mods::builder::sandboxes[player->name().c_str()].erase(it);
					player->remove_flag(mods::flags::chunk_type_t::BUILDER,mods::builder::HAS_SANDBOX);
					r_success(player,"Index erased");
					return;
				}
			}
		}
	}
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
