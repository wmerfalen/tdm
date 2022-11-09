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
	std::optional<obj_data*> instantiate_object_by_index(int index) {
		std::size_t i = index;
		if(i >= obj_proto.size()) {
			exit(4);
			return std::nullopt;
		}
		create_object_from_index(index);
		return obj_list.back().get();
		//obj_list.push_back(std::make_shared<obj_data>());
		//*obj_list.back() = obj_proto[index];
		//return obj_list.back().get();
	}
	std::optional<obj_data*> instantiate_object_by_vnum(int vnum) {
		int ctr = 0;
		for(auto& object_reference : obj_proto) {
			if(object_reference.item_number == vnum) {
				//obj_list.push_back(std::make_shared<obj_data>());
				//*obj_list.back() = object_reference;
				//return obj_list.back().get();
				return instantiate_object_by_index(ctr);
			}
			++ctr;
		}
		return std::nullopt;
	}

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

	std::pair<bool,std::string> save_object(int item_number,int obj_type,std::string feed_file) {
		try {
			auto txn_01 = txn();
			sql_compositor comp3("object",&txn_01);
			auto sql = comp3.select("id").from("object").
			    where("obj_item_number","=",std::to_string(item_number))
			    .sql();
			auto check_result_01 = mods::pq::exec(txn_01,sql);
			//auto check_i = 0;
			mods::pq::commit(txn_01);
			sql_compositor::value_map my_map;
			my_map["obj_item_number"] = std::to_string(item_number);
			my_map["obj_type"] = std::to_string(obj_type);
			my_map["obj_file"] = feed_file;
			if(check_result_01.size()) {
				/* update the fields */
				auto t = txn();
				sql_compositor comp("object",&t);
				auto update_sql = comp
				    .update("object")
				    .set(my_map)
				    .where("obj_item_number","=",std::to_string(item_number))
				    .sql();
				auto result = mods::pq::exec(t,update_sql);
				mods::pq::commit(t);
			} else {
				/* insert into the db */
				auto txn0 = txn();
				sql_compositor comp("object",&txn0);
				auto sql = comp.insert().into("object")
				    .values(my_map).sql();
				mods::pq::exec(txn0,sql);
				mods::pq::commit(txn0);
				auto txn4 = txn();
				sql_compositor comp2("object",&txn4);
				sql = comp2
				    .select("id")
				    .from("object")
				    .where("obj_item_number","=",std::to_string(item_number))
				    .sql();
				auto res = mods::pq::exec(txn4,sql);
				mods::pq::commit(txn4);
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error",e.what());
			return {false,std::string("Exception occurred: ") + e.what()};
		}
		return {true,"saved successfully"};
	}
};

using args_t = std::vector<std::string>;

int next_room_pavement_transaction_id() {
	static int transaction_id = 0;
	return ++transaction_id;
}
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

SUPERCMD(do_obuild) {
	mods::builder::initialize_builder(player);
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "obuild argument: '" << argument << "'\n";
#endif
	auto vec_args = PARSE_ARGS();
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "after parse args\n";
	std::cerr << "[vec_args.size()]->" << vec_args.size() << "\n";
#endif
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("weapon_type") == 0) {
		player->pager_start();
		*player <<
		    "{red}Weapon Type{/red} A hash value of the weapon type.  It must be one of\r\n" <<
		    "the following numbers:</P>\r\n";
		for(auto pairedData : mods::builder::weapon_type_flags) {
			*player << " " << pairedData.second << "\r\n";
		}
		*player << "example: obuild attr 1 weapon_type:add SMG\r\n" <<
		    "(this will set the affected slot number 3 on object zero to modify \r\n" <<
		    "the character's weight by 15)\r\n" <<
		    "example: obuild attr 1 weapon_type:list\r\n" <<
		    "(this will list all weapon flags on the object)\r\n" <<
		    "example: obuild attr 1 weapon_type:remove SMG\r\n" <<
		    "(this will remove the SMG flag from the weapon)\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint b\n";
#endif
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("affected") == 0) {
		player->pager_start();
		*player <<
		    "{red}Location{/red}The aspect of the character affected by the object.  It must be one of\r\n" <<
		    "the following numbers:</P>\r\n" <<
		    "0    NONE           No effect (typically not used).\r\n" <<
		    "1    STR            Apply to strength.\r\n" <<
		    "2    DEX            Apply to dexterity.\r\n" <<
		    "3    INT            Apply to intelligence.\r\n" <<
		    "4    WIS            Apply to wisdom.\r\n" <<
		    "5    CON            Apply to constitution.\r\n" <<
		    "6    CHA            Apply to charisma.\r\n" <<
		    "7    CLASS          Unimplemented.  Do not use.\r\n" <<
		    "8    LEVEL          Unimplemented.  Do not use.\r\n" <<
		    "9    AGE            Apply to character's MUD age, in MUD-years.\r\n" <<
		    "10   CHAR_WEIGHT    Apply to weight.\r\n" <<
		    "11   CHAR_HEIGHT    Apply to height.\r\n" <<
		    "12   MANA           Apply to MAX mana points.\r\n" <<
		    "13   HIT            Apply to MAX hit points.\r\n" <<
		    "14   MOVE           Apply to MAX movement points.\r\n" <<
		    "15   GOLD           Unimplemented.  Do not use.\r\n" <<
		    "16   EXP            Unimplemented.  Do not use.\r\n" <<
		    "17   AC             Apply to armor class (AC).\r\n" <<
		    "18   HITROLL        Apply to hitroll.\r\n" <<
		    "19   DAMROLL        Apply to damage roll bonus.\r\n" <<
		    "20   SAVING_PARA    Apply to save throw: paralyze\r\n" <<
		    "21   SAVING_ROD     Apply to save throw: rods\r\n" <<
		    "22   SAVING_PETRI   Apply to save throw: petrif\r\n" <<
		    "23   SAVING_BREATH  Apply to save throw: breath\r\n" <<
		    "24   SAVING_SPELL   Apply to save throw: spells\r\n" <<
		    "25   SCOPE          \r\n" <<
		    "26   RECOIL         \r\n" <<
		    "27   BARREL         \r\n" <<
		    "28   MAGAZINE_SIZE  \r\n" <<
		    "29   INCENDIARY     \r\n" <<
		    "30   SIDERAIL       \r\n" <<
		    "31   UNDER_BARREL   \r\n" <<
		    "32   PENETRATION    \r\n" <<
		    "33   RANGE          \r\n" <<
		    "34   CRITICAL_RANGE \r\n" <<
		    "35   OPTIMAL_RANGE  \r\n" <<
		    "36   VISION         \r\n" <<
		    "37   STEALTH        \r\n" <<
		    "usage: obuild affected <object_id> <affected_slot> <location> <modifier>\r\n" <<
		    "example: obuild affected 0 3 CHAR_WEIGHT 15\r\n" <<
		    "(this will set the affected slot number 3 on object zero to modify \r\n" <<
		    "the character's weight by 15)\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint c\n";
#endif
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("extra_flags") == 0) {
		player->pager_start();
		*player <<
		    "1     a   GLOW           Item is glowing (cosmetic).\r\n" <<
		    "2     b   HUM            Item is humming (cosmetic).\r\n" <<
		    "4     c   NORENT         Item cannot be rented.\r\n" <<
		    "8     d   NODONATE       Item cannot be donated.\r\n" <<
		    "16    e   NOINVIS        Item cannot be made invisible.\r\n" <<
		    "32    f   INVISIBLE      Item is invisible.\r\n" <<
		    "64    g   MAGIC          Item has a magical aura and can't be enchanted.\r\n" <<
		    "128   h   NODROP         Item is cursed and cannot be dropped.\r\n" <<
		    "256   i   BLESS          Item is blessed (cosmetic).\r\n" <<
		    "512   j   ANTI_GOOD      Item can't be used by good-aligned characters.\r\n" <<
		    "1024  k   ANTI_EVIL      Item can't be used by evil-aligned characters.\r\n" <<
		    "2048  l   ANTI_NEUTRAL   Item can't be used by neutral align characters.\r\n" <<
		    "4096  m   ANTI_MAGIC_USER Item can't be used by the Mage class.\r\n" <<
		    "8192  n   ANTI_CLERIC    Item can't be used by the Cleric class.\r\n" <<
		    "16384 o   ANTI_THIEF     Item can't be used by the Thief class.\r\n" <<
		    "32768 p   ANTI_WARRIOR   Item can't be used by the Warrior class.\r\n" <<
		    "65536 q   NOSELL         Shopkeepers will not buy or sell the item.\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint d\n";
#endif
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("wear_flags") == 0) {
		player->pager_start();
		*player <<
		    "1     a   WEAR_TAKE      Item can be taken (picked up off the ground).\r\n" <<
		    "2     b   WEAR_FINGER    Item can be worn on the fingers.\r\n" <<
		    "4     c   WEAR_NECK      Item can be worn around the neck.\r\n" <<
		    "8     d   WEAR_BODY      Item can be worn on the body.\r\n" <<
		    "16    e   WEAR_HEAD      Item can be worn on the head.\r\n" <<
		    "32    f   WEAR_LEGS      Item can be worn on the legs.\r\n" <<
		    "64    g   WEAR_FEET      Item can be worn on the feet.\r\n" <<
		    "128   h   WEAR_HANDS     Item can be worn on the hands.\r\n" <<
		    "256   i   WEAR_ARMS      Item can be worn on the arms.\r\n" <<
		    "512   j   WEAR_SHIELD    Item can be used as a shield.\r\n" <<
		    "1024  k   WEAR_ABOUT     Item can be worn about the body.\r\n" <<
		    "2048  l   WEAR_WAIST     Item can be worn around the waist.\r\n" <<
		    "4096  m   WEAR_WRIST     Item can be worn around the wrist.\r\n" <<
		    "8192  n   WEAR_WIELD     Item can be wielded; e.g. weapons.\r\n" <<
		    "16384 o   WEAR_HOLD      Item can be held (the ``hold'' command).\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint e\n";
#endif
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("type_flags") == 0) {
		player->pager_start();
		*player <<
		    "1    LIGHT          Item is a light source.\r\n" <<
		    "2    SCROLL         Item is a magical scroll.\r\n" <<
		    "3    WAND           Item is a magical wand.\r\n" <<
		    "4    STAFF          Item is a magical staff.\r\n" <<
		    "5    WEAPON         Item is a weapon.\r\n" <<
		    "6    FIREWEAPON     Currently not implemented.  Do not use.\r\n" <<
		    "7    MISSILE        Currently not implemented.  Do not use.\r\n" <<
		    "8    TREASURE       Item is treasure other than gold coins (e.g. gems)\r\n" <<
		    "9    ARMOR          Item is armor.\r\n" <<
		    "10   POTION         Item is a magical potion.\r\n" <<
		    "11   WORN           Currently not implemented.  Do not use.\r\n" <<
		    "12   OTHER          Miscellaneous object with no special properties.\r\n" <<
		    "13   TRASH          Trash -- junked by cleaners, not bought by shopkeepers.\r\n" <<
		    "14   TRAP           Currently not implemented.  Do not use.\r\n" <<
		    "15   CONTAINER      Item is a container.\r\n" <<
		    "16   NOTE           Item is a note (can be written on).\r\n" <<
		    "17   DRINKCON       Item is a drink container.\r\n" <<
		    "18   KEY            Item is a key.\r\n" <<
		    "19   FOOD           Item is food.\r\n" <<
		    "20   MONEY          Item is money (gold coins).\r\n" <<
		    "21   PEN            Item is a pen.\r\n" <<
		    "22   BOAT           Item is a boat; allows you to traverse SECT_WATER_NOSWIM.\r\n" <<
		    "23   FOUNTAIN       Item is a fountain.\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint F\n";
#endif
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("flags") == 0) {
		player->pager_start();
		*player <<
		    "{red}LIGHT (Type Flag 1){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: Capacity of light in hours.\r\n" <<
		    "                0: Burned out light.\r\n" <<
		    "               -1: Eternal light source.\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}SCROLL (Type Flag 2){/red}\r\n" <<
		    "     value 0: Level at which scroll's spells are cast.\r\n" <<
		    "     value 1: Spell number 1\r\n" <<
		    "     value 2: Spell number 2\r\n" <<
		    "     value 3: Spell number 3\r\n" <<
		    "          If less than 3 spells are desired, set unused values to -1.\r\n" <<
		    "{red}WAND (Type Flag 3){/red}\r\n" <<
		    "     value 0: Level at which wand's spell is cast.\r\n" <<
		    "     value 1: Charge capacity of wand (&gt;= 1)\r\n" <<
		    "     value 2: Current number of charges remaining\r\n" <<
		    "     value 3: Spell number\r\n" <<
		    "{red}STAFF (Type Flag 4){/red}\r\n" <<
		    "     value 0: Level at which staff's spell is cast.\r\n" <<
		    "     value 1: Charge capacity of staff (&gt;= 1)\r\n" <<
		    "     value 2: Current number of charges remaining\r\n" <<
		    "     value 3: Spell number\r\n" <<
		    "{red}WEAPON (Type Flag 5){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: Number of damage dice\r\n" <<
		    "     value 2: Size of damage dice\r\n" <<
		    "     value 3: Weapon type for damage messages, one of:\r\n" <<
		    "               0    hit/hits\r\n" <<
		    "               1    sting/stings\r\n" <<
		    "               2    whip/whips\r\n" <<
		    "               3    slash/slashes\r\n" <<
		    "               4    bite/bites\r\n" <<
		    "               5    bludgeon/bludgeons\r\n" <<
		    "               6    crush/crushes\r\n" <<
		    "               7    pound/pounds\r\n" <<
		    "               8    claw/claws\r\n" <<
		    "               9    maul/mauls\r\n" <<
		    "               10   thrash/thrashes\r\n" <<
		    "               11   pierce/pierces\r\n" <<
		    "               12   blast/blasts\r\n" <<
		    "               13   punch/punches\r\n" <<
		    "               14   stab/stabs\r\n" <<
		    "{red}FIREWEAPON (Type Flag 6){/red}unimplemented (do not use)\r\n" <<
		    "{red}MISSILE (Type Flag 7){/red}unimplemented (do not use)\r\n" <<
		    "{red}TREASURE (Type Flag 8){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}ARMOR (Type Flag 9){/red}\r\n" <<
		    "     value 0: AC-apply of the armor.  Note that the effective change\r\n" <<
		    "               to AC is this value times a multiplier based on where\r\n" <<
		    "               the armor is worn.  Values &gt;0 enhance the armor class;\r\n" <<
		    "               values &lt;0 damage the armor class (cursed armor, for\r\n" <<
		    "               example).\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}POTION (Type Flag 10){/red}\r\n" <<
		    "     value 0: Level at which the potion's spells are cast.\r\n" <<
		    "     value 1: Spell number 1\r\n" <<
		    "     value 2: Spell number 2\r\n" <<
		    "     value 3: Spell number 3\r\n" <<
		    "          If less than 3 spells are desired, set unused values to -1.\r\n" <<
		    "{red}WORN (Type Flag 11){/red}unimplemented (do not use)\r\n" <<
		    "{red}OTHER (Type Flag 12){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}TRASH (Type Flag 13){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}TRAP (Type Flag 14){/red}unimplemented (do not use)\r\n" <<
		    "{red}CONTAINER (Type Flag 15){/red}\r\n" <<
		    "     value 0: Capacity (max containable weight) of container\r\n" <<
		    "     value 1: Container flag bitvector (MUST be a numeric bitvector)\r\n" <<
		    "          1    CLOSEABLE      Container can be closed and locked.\r\n" <<
		    "          2    PICKPROOF      Lock on container can't be picked.\r\n" <<
		    "          4    CLOSED         Container is closed when created.\r\n" <<
		    "          8    LOCKED         Container is locked when created.\r\n" <<
		    "     value 2: The vnum of the key object that opens this container.\r\n" <<
		    "               -1 if it has no key.\r\n" <<
		    "     value 3: Reserved for internal use -- always set as 0.\r\n" <<
		    "{red}NOTE (Type Flag 16){/red}\r\n" <<
		    "     value 0: Language of writing (unimplemented).\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}DRINKCON (Type Flag 17){/red}See Appendix \r\n" <<
		    "Item Values for Drink Containers.\r\n" <<
		    "{red}KEY (Type Flag 18){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}FOOD (Type Flag 19){/red}\r\n" <<
		    "     value 0: The number of hours of hunger satisfied by this food.\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: Non-zero if the food is poisoned, 0 otherwise.\r\n" <<
		    "{red}MONEY (Type Flag 20){/red}\r\n" <<
		    "     value 0: The number of gold coins in the pile.\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}PEN (Type Flag 21){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}BOAT (Type Flag 22){/red}\r\n" <<
		    "     value 0: unused\r\n" <<
		    "     value 1: unused\r\n" <<
		    "     value 2: unused\r\n" <<
		    "     value 3: unused\r\n" <<
		    "{red}FOUNTAIN (Type Flag 23){/red}See Appendix \r\n" <<
		    "Item Values for Drink Containers.\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint G\n";
#endif
	if(!vec_args.size() || vec_args[0].compare("help") == 0) {
		player->pager_start();
		*player << "usage: \r\n" <<
		    " {grn}obuild{/grn} {red}help{/red}\r\n" <<
		    //"  |--> this help menu\r\n" <<
		    //"  {grn}|____[example]{/grn}\r\n" <<
		    //"  |:: {wht}obuild{/wht} {gld}help{/gld}\r\n" <<
		    //"  |:: (this help menu will show up)\r\n" <<
		    " {grn}obuild{/grn} {red}new{/red}\r\n" <<
		    " {grn}obuild{/grn} {red}list{/red}\r\n" <<
		    //" {grn}obuild{/grn} {red}attr <object_id> <attr> <value>{/red}\r\n" <<
		    //"  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
		    //"  {gld}|:: item_number{/gld}\r\n" <<
		    //"  {gld}|:: name{/gld} {red}APPEARS: {/red}\r\n" <<
		    //"  {gld}|:: desc{/gld} {red}APPEARS: lying on ground{/red}\r\n" <<
		    //"  {red}Short Description{/red}\r\n"
		    //"  {gld}|:: short_description{/gld} {red}APPEARS: when you type inv{/red}\r\n" <<
		    //"  {red}APPEARS: when you wield it{/red}\r\n" <<
		    //"  {red}APPEARS: when you look at yourself or someone wielding it{/red}\r\n" <<
		    //"  {grn}|IN-GAME: get xm\r\nYou get <Short Description>\r\n" <<
		    //"  {gld}|:: action_description{/gld} {red}APPEARS: when you examine{/red}\r\n" <<
		    //"  {gld}|:: worn_on{/gld}\r\n" <<
		    //"  {gld}|:: weapon_type {red}see: obuild help weapon_type{/red}{/gld}\r\n" <<
		    //"  {gld}|:: weapon_ammo{/gld}\r\n" <<
		    //"  {gld}|:: weapon_ammo_max{/gld}\r\n" <<
		    //"  {gld}|:: weapon_holds_ammo{/gld}\r\n" <<
		    //"  {gld}|:: flags{/gld}\r\n" <<
		    //" {grn}obuild{/grn} {red}ex <object_id> create <index>{/red}\r\n" <<
		    //" {grn}obuild{/grn} {red}ex <object_id> set <index> <keyword> <description>{/red}\r\n" <<
		    //" {grn}obuild{/grn} {red}ex <object_id> del <index>{/red}\r\n" <<
		    //"  {grn}|____[example]{/grn}\r\n" <<
		    //"  |:: {wht}obuild{/wht} {gld}ex 6 set 0 {/gld}\"keyword\" \"my description\"\r\n" <<
		    //"  |:: {wht}obuild{/wht} {gld}ex 6 del 0{/gld}\r\n" <<
		    //" {grn}obuild{/grn} {red}affected <object_id> set <affected_slot> <location> <modifier>{/red}\r\n" <<
		    //" {grn}obuild{/grn} {red}affected <object_id> del <affected_slot>{/red}\r\n" <<
		    //"  {grn}|____[example]{/grn}\r\n" <<
		    //"  |:: {wht}obuild{/wht} {gld}affected 6 3 STR 25{/gld}\r\n" <<
		    //"  |:: (this will set the affected slot number 3 on object 6 \r\n" <<
		    //"      to modify strength by 25)\r\n" <<
		    //"  |:: {red}see obuild help affected{/red}\r\n" <<
		    //" {grn}obuild{/grn} {red}flag <object_id> extra_flags <value>{/red}\r\n"<<
		    //" {red}see obuild help extra_flags{/red}\r\n" <<
		    //" {grn}obuild{/grn} {red}flag <object_id> <attr> <value>{/red}\r\n"<<
		    //" {red}see obuild help flags{/red}\r\n" <<
		    //"  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
		    //"  {gld}|:: value_0{/gld}\r\n" <<
		    //"  {gld}|:: value_1{/gld}\r\n" <<
		    //"  {gld}|:: value_2{/gld}\r\n" <<
		    //"  {gld}|:: value_3{/gld}\r\n" <<
		    //"  {gld}|:: type_flags {red}see: obuild help type_flags{/red}{/gld}\r\n" <<
		    //"  {gld}|:: wear_flags {red}see: obuild help wear_flags{/red}{/gld}\r\n" <<
		    //"  {gld}|:: extra_flags{/gld}\r\n" <<
		    //"  {gld}|:: weight{/gld}\r\n" <<
		    //"  {gld}|:: cost{/gld}\r\n" <<
		    //"  {gld}|:: cost_per_day{/gld}\r\n" <<
		    //"  {gld}|:: timer{/gld}\r\n" <<
		    //"  {gld}|:: bitvector {red}see: obuild help bitvector{/red}{/gld}\r\n" <<
		    " {grn}obuild{/grn} {red}exists <obj_vnum>{/red}\r\n" <<
		    "  |--> [supports encoded response]\r\n" <<
		    " {grn}obuild{/grn} {red}create <item_number> <obj_type> <>{/red}\r\n" <<
		    "  {gld}|:: -:[obj_types]:-{/gld}\r\n" <<
		    "  {gld}|:: armor{/gld}\r\n" <<
		    "  {gld}|:: attachment{/gld}\r\n" <<
		    "  {gld}|:: consumable{/gld}\r\n" <<
		    "  {gld}|:: drone{/gld}\r\n" <<
		    "  {gld}|:: explosive{/gld}\r\n" <<
		    "  {gld}|:: gadget{/gld}\r\n" <<
		    "  {gld}|:: rifle{/gld}\r\n" <<
		    "  {gld}|:: trap{/gld}\r\n" <<
		    " {grn}obuild{/grn} {red}create <item_number> <obj_type> <>{/red}\r\n" <<
		    " {grn}obuild{/grn} {red}show <object_id>{/red}\r\n" <<
		    " {grn}obuild{/grn} {red}obj_file <object_id> <file>{/red}\r\n" <<
		    " {red}types include: rifle, explosive, gadget, drone, attachment, armor, consumbale, trap{/red}\r\n" <<
		    "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint H\n";
#endif
	auto args = mods::util::subcmd_args<9,args_t>(argument,"obj_file");
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
	std::cerr << "checkpoint H\n";
#endif
	if(args.has_value()) {
		auto r = args.value();
		if(r.size() < 3) {
			r_error(player, "Invalid number of arguments");
			return;
		}
		int obj_index = mods::util::stoi(r[1]).value_or(-1);
		if(obj_index < 0) {
			r_error(player, "Invalid index. Unrecognized.");
			return;
		}
		auto& obj = obj_proto[obj_index];
		obj.set_feed_file(r[2]);
		r_success(player,"Object feed file set.");
		return;
	}
	args = mods::util::subcmd_args<4,args_t>(argument,"new");
	if(args.has_value()) {
		r_status(player,"Creating new object");
		obj_proto.push_back({ITEM_RIFLE,"g36c.yml"});
		if(player->is_executing_js()) {
			*player << "{index: " << obj_proto.size() - 1 << "}";
		}
		r_success(player,"Object created");
		return;
	}
	args = mods::util::subcmd_args<7,args_t>(argument,"exists");
	if(args.has_value()) {
		//[0 - exists] [1 - vnum]
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		ENCODE_INIT();
		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value.");
			return;
		} else {
			auto arg_vec = args.value();
			auto i_value = mods::util::stoi(arg_vec[1]);
			unsigned ctr = 0;
			for(const auto& obj : obj_proto) {
				if(obj.item_number == i_value.value()) {
					ENCODE_STR(ctr);
					r_success(player,CAT("Index: ",ctr));
					return;
				}
				++ctr;
			}
			r_error(player,"No object with that vnum");
			return;
		}
	}
	args = mods::util::subcmd_args<7,args_t>(argument,"create");
	if(args.has_value()) {
		//TODO: !mundane make this a function
		auto arg_vec = args.value();
		if(arg_vec.size() < 3) {
			r_error(player,"Invalid number of arguments");
			return;
		}
		int obj_type = -1;
#define MENTOC_LAZY_ME(a,mtype) if(arg_vec[2].compare(#a) == 0){ obj_type = mtype; }
		MENTOC_LAZY_ME(rifle,ITEM_RIFLE);
		MENTOC_LAZY_ME(explosive,ITEM_EXPLOSIVE);
		MENTOC_LAZY_ME(drone,ITEM_DRONE);
		MENTOC_LAZY_ME(gadget,ITEM_GADGET);
		MENTOC_LAZY_ME(attachment,ITEM_ATTACHMENT);
		MENTOC_LAZY_ME(armor,ITEM_ARMOR);
		MENTOC_LAZY_ME(consumable,ITEM_CONSUMABLE);
		MENTOC_LAZY_ME(trap,ITEM_TRAP);
#undef MENTOC_LAZY_ME
		int item_number = mods::util::stoi(arg_vec[1]).value_or(-1);
		if(item_number == -1) {
			r_error(player,"Please use a valid item number");
			return;
		}
		if(obj_type == -1) {
			r_error(player,"Please use a valid object type.");
			return;
		}
		r_status(player,"Saving object");
		std::string feed_file = arg_vec[3];
		auto return_status = mods::builder::save_object(item_number,obj_type,feed_file);
		if(!return_status.first) {
			r_error(player,return_status.second);
			return;
		}
		obj_proto.emplace_back(obj_type,feed_file);
		r_success(player,"Object created");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"save");
	if(args.has_value()) {
		//TODO: !mundane make this a function
		auto arg_vec = args.value();
		if(arg_vec.size() == 1) {
			r_error(player,"Please supply an object id");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		obj_data * obj = nullptr;
		if(index.has_value()) {
			std::size_t i = index.value();
			if(i >= obj_proto.size()) {
				r_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		} else {
			r_error(player,std::string(args.value()[1]) + " is not a valid number");
			return;
		}
		if(!obj) {
			r_error(player,"Index not found.");
			return;
		}
		r_status(player,"Saving object");
		int item_number = obj->item_number;
		int obj_type = obj->type;
		std::string feed_file = obj->feed_file().data();
		auto return_status = mods::builder::save_object(item_number,obj_type,feed_file);
		if(!return_status.first) {
			r_error(player,return_status.second);
			return;
		}
		r_success(player,"Object saved");
		return;
	}
	args = mods::util::subcmd_args<3,args_t>(argument,"ex");
	if(args.has_value()) {
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		struct obj_data* obj = nullptr;
		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value");
			return;
		} else {
			auto index = i_value.value();
			std::size_t i = i_value.value();
			if(i >= obj_proto.size()) {
				r_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index];
		}
		if(arg_vec.size() < 3) {
			r_error(player,"Invalid number of arguments");
			return;
		}
		if(arg_vec[2].compare("create") == 0 && arg_vec.size() == 4) {
			//ex 0 create size
			//0  1   2     3
			auto size = mods::util::stoi(arg_vec[3]);
			if(!size.has_value()) {
				r_error(player,"Invalid size");
				return;
			}
			if(size.value() > obj->ex_description.size()) {
				for(unsigned i=0; i < size.value(); i++) {
					obj->ex_description.emplace_back("<default>keyword","<default>description");
				}
			}
			obj->ex_description.emplace_back(
			    "<extra_descr_data.keyword>",
			    "<extra_descr_data.description>"
			);
			r_status(player,"Done.");
			return;
		}
		if(arg_vec[2].compare("set") == 0 && arg_vec.size() == 6) {
			//obuild ex <object_id> set <index> <keyword> <description>
			//       0    1          2   3         4         5
			if(arg_vec.size() < 6) {
				r_error(player,"Not enough arguments supplied");
				return;
			}
			auto index = mods::util::stoi(arg_vec[3]);
			if(index.has_value()) {
				if(index.value() >= obj->ex_description.size()) {
					r_error(player,"index is too large");
					return;
				}
				obj->ex_description[index.value()].keyword.assign(arg_vec[4].c_str());
				obj->ex_description[index.value()].description.assign(arg_vec[5].c_str());
			} else {
				r_error(player,"Invalid index supplied");
				return;
			}
			r_success(player,"ex_description set");
		}
		if(arg_vec[2].compare("del") == 0 && arg_vec.size() == 4) {
			//obuild ex <object_id> del <index>
			//        0    1         2     3
			auto index = mods::util::stoi(arg_vec[3]);
			if(index.has_value()) {
				if(index.value() >= obj->ex_description.size()) {
					r_error(player,"Out of bounds");
					return;
				}
				obj->ex_description.erase(
				    obj->ex_description.begin() + index.value()
				);
				r_success(player,"ex_description removed");
				return;
			}
			r_error(player,"Invalid index");
			return;
		}
	}
	args = mods::util::subcmd_args<12,args_t>(argument,"instantiate");
	if(args.has_value()) {
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value.");
			return;
		} else {
			auto index = i_value.value();
			std::size_t i = index;
			if(i >= obj_proto.size()) {
				r_error(player,"Out of bounds");
				return;
			}
			auto obj_ptr = create_object_from_index(i);
			obj_ptr->carried_by = obj_ptr->worn_by = nullptr;
			obj_to_room(obj_ptr.get(),IN_ROOM(player->cd()));
			r_success(player,"Object created, look on the floor");
		}
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"list");
	if(args.has_value()) {
		r_status(player,"listing...");
		if(obj_proto.size() == 0) {
			r_status(player,"No objects to list");
			return;
		}
		unsigned object_id = 0;
		if(!player->is_executing_js()) {
			player->pager_start();
		}
		jxcomp jx;
		jx.array_start("objects");
		for(auto& object_reference : obj_proto) {
			auto obj = &object_reference;
			if(player->is_executing_js()) {
				jx.object_start("")
				.push("index",object_id)
				.push("item_number",obj->item_number)
				.push("name",obj->name)
				.push("short_description",obj->short_description)
				.object_end();
			} else {
				if(obj->short_description) {
					player->sendln(
					    CAT(
					        "{gld}[",object_id,"]{/gld} :->{red}",obj->short_description,"{/red}"
					    )
					);
				} else if(obj->name) {
					player->sendln(
					    CAT(
					        "{gld}[",object_id,obj->name,"]{/gld} :->{red}",
					        obj->name,"{/red}"
					    )
					);
				} else {
					player->sendln(
					    CAT(
					        "{gld}[",object_id, "]{/gld} :->{red}<null>{/red}"
					    )
					);
				}
			}
			object_id++;
		}
		if(player->is_executing_js()) {
			jx.array_end();
			*player << jx.get();
		} else {
			player->pager_end();
			player->page(0);
		}
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"flag");
	if(args.has_value()) {
		auto arg_vec = args.value();
		auto get_intval = [&](std::string_view str) -> std::optional<int> {
			if(arg_vec[2].compare(str.data()) == 0) {
				auto i_value = mods::util::stoi(arg_vec[3]);
				if(!i_value.has_value()) {
					r_error(player,"Please use a valid numeric value.");
					return std::nullopt;
				}
				return i_value.value();
			}

			return std::nullopt;
		};
		auto index = mods::util::stoi(arg_vec[1]);
		obj_data * obj = nullptr;
		if(index.has_value()) {
			std::size_t i = index.value();
			if(i >= obj_proto.size()) {
				r_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		} else {
			r_error(player,"not a valid number");
			return;
		}
		if(!obj) {
			r_error(player,"Index not found.");
			return;
		}
		MENTOC_OBI2(obj_flags.value[0],value_0);
		MENTOC_OBI2(obj_flags.value[1],value_1);
		MENTOC_OBI2(obj_flags.value[2],value_2);
		MENTOC_OBI2(obj_flags.value[3],value_3);
		MENTOC_OBI2(obj_flags.weight,weight);
		MENTOC_OBI2(obj_flags.cost,cost);
		MENTOC_OBI2(obj_flags.cost_per_day,cost_per_day);
		MENTOC_OBI2(obj_flags.timer,timer);
		MENTOC_OBI2(obj_flags.bitvector,bitvector);
		//TODO !mundane make these flag code fragments into a function
		if(arg_vec[2].compare("extra_flags") == 0) {
			if(arg_vec.end() <= arg_vec.begin() + 3) {
				r_error(player,"Please supply one or more flags");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			obj->obj_flags.bitvector = 0;
			for(; flag != arg_vec.end(); ++flag) {
				bool found = false;
				for(auto& ex_flag : mods::builder::extra_flags) {
					if(ex_flag.second.compare(*flag) == 0) {
						obj->obj_flags.bitvector |= ex_flag.first;
						found = true;
						break;
					}
				}
				if(!found) {
					r_error(player,std::string("Unrecognized flag: ") + *flag);
				}
			}
		}
		if(arg_vec[2].compare("type_flags") == 0) {
			if(arg_vec.end() <= arg_vec.begin() + 3) {
				r_error(player,"Please supply a type flag");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			obj->obj_flags.type_flag = 0;
			bool found = false;
			for(auto& type_flag : mods::builder::type_flags) {
				if(type_flag.second.compare(*flag) == 0) {
					obj->obj_flags.type_flag = type_flag.first;
					found = true;
					break;
				}
			}
			if(!found) {
				r_error(player,std::string("Unrecognized type flag: ") + *flag);
			}
		}
		if(arg_vec[2].compare("wear_flags") == 0) {
			if(arg_vec.end() <= arg_vec.begin() + 3) {
				r_error(player,"Please supply wear flags");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			obj->obj_flags.wear_flags = 0;
			for(; flag != arg_vec.end(); ++flag) {
				bool found = false;
				for(auto& wear_flag : mods::builder::wear_flags) {
					if(wear_flag.second.compare(*flag) == 0) {
						obj->obj_flags.wear_flags |= wear_flag.first;
						found = true;
						break;
					}
				}
				if(!found) {
					r_error(player,std::string("Unrecognized wear flag:") + *flag);
				}
			}
		}
		r_success(player,"Object updated");
		return;
	}
	args = mods::util::subcmd_args<9,args_t>(argument,"affected");
	if(args.has_value()) {
		auto arg_vec = args.value();
		if(arg_vec.size() > 3 && arg_vec[2].compare("del") == 0) {
			//User is trying to delete an affected slot
			auto slot = mods::util::stoi(arg_vec[3]);
			if(!slot.has_value() || slot.value() > MAX_OBJ_AFFECT) {
				r_error(player," Invalid slot index");
				return;
			}
			auto index = mods::util::stoi(arg_vec[1]);
			obj_data * obj = nullptr;
			if(index.has_value()) {
				std::size_t i = index.value();
				if(i >= obj_proto.size()) {
					r_error(player,"Object index Out of bounds");
					return;
				}
				obj = &obj_proto[index.value()];
			} else {
				r_error(player,"not a valid number");
				return;
			}
			obj->affected[slot.value()].location = 0;
			obj->affected[slot.value()].modifier = 0;
			r_error(player,"Slot cleared");
			return;
		}
		if(arg_vec.size() < 6) {
			r_error(player,"Not enough arguments. See: obuild help affected");
			return;
		}
		if(arg_vec[2].compare("set") == 0) {
			auto index = mods::util::stoi(arg_vec[1]);
			obj_data * obj = nullptr;
			if(index.has_value()) {
				std::size_t i = index.value();
				if(i >= obj_proto.size()) {
					r_error(player," Out of bounds");
					return;
				}
				obj = &obj_proto[index.value()];
			} else {
				r_error(player,"not a valid number");
				return;
			}
			if(!obj) {
				r_error(player,"Index not found.");
				return;
			}
			auto aff_index = mods::util::stoi(arg_vec[3]);
			auto location = mods::util::stoi(arg_vec[4]);
			auto modifier = mods::util::stoi(arg_vec[5]);
			if(aff_index.has_value() &&
			    location.has_value() && modifier.has_value()) {
				if(aff_index.value() >= MAX_OBJ_AFFECT) {
					r_error(player,std::string("aff_index must be less than ") + std::to_string(MAX_OBJ_AFFECT));
					return;
				}
				obj->affected[aff_index.value()].location = location.value();
				obj->affected[aff_index.value()].modifier = modifier.value();
				r_success(player,"Object updated");
				return;
			} else {
				r_error(player,"location and modifier are required");
				return;
			}
		}
		r_error(player,"Unknown mode");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"show");
	if(args.has_value()) {
		//TODO: !mundane make this a function
		auto arg_vec = args.value();
		if(arg_vec.size() == 1) {
			r_error(player,"Please supply an object id");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		obj_data * obj = nullptr;
		if(index.has_value()) {
			std::size_t i = index.value();
			if(i >= obj_proto.size()) {
				r_error(player," Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		} else {
			r_error(player,"Not a valid number");
			return;
		}
		if(!obj) {
			r_error(player,"Index not found.");
			return;
		}
		player->pager_start();
		*player <<
		    "{gld}::Object data::{/gld}\r\n" <<
		    "{red}name: {/red}" << obj->name <<  "\r\n" <<
		    "{red}description: {/red}" << obj->description << "\r\n" <<
		    "{red}short_description: {/red}" << obj->short_description << "\r\n" <<
		    "{red}action_description: {/red}" << obj->action_description << "\r\n" <<
		    "{red}ex_descriptions: {/red}";
		unsigned ex_ctr = 0;
		for(auto& ex_desc : obj->ex_description) {
			if(ex_desc.keyword.length()) {
				*player << "{red}[" << ex_ctr << "]keyword: {/red}" << ex_desc.keyword.c_str() << "\r\n";
			}
			if(ex_desc.description.length()) {
				*player << "{red}[" << ex_ctr << "]description: {/red}" << ex_desc.description.c_str() << "\r\n";
			}
			++ex_ctr;
		}
		*player << "\r\n{red}item_number: {/red}" << obj->item_number << "\r\n"
		    "{red}worn_on: {/red}" << obj->worn_on << "\r\n";
		if(obj->rifle()) {
			*player << "{red}weapon_type: {/red}" << obj->rifle()->type << "\r\n" <<
			    "{red}weapon_ammo: {/red}" << obj->obj_flags.ammo << "\r\n" <<
			    "{red}weapon_ammo_max: {/red} " << obj->obj_flags.ammo_max << "\r\n" <<
			    "{red}weapon_holds_ammo: {/red}: " << obj->obj_flags.holds_ammo << "\r\n";
		}
		*player <<
		    "{gld}::Wear Flags::{/gld}\r\n" <<
		    "{red}value: {/red}" << std::to_string(obj->obj_flags.wear_flags) <<  "\r\n";
#define MENTOC_WEAR(a){ if(obj->obj_flags.wear_flags & a){*player << #a << ", ";} }
		MENTOC_WEAR(ITEM_WEAR_TAKE);
		MENTOC_WEAR(ITEM_WEAR_FINGER);
		MENTOC_WEAR(ITEM_WEAR_NECK);
		MENTOC_WEAR(ITEM_WEAR_BODY);
		MENTOC_WEAR(ITEM_WEAR_HEAD);
		MENTOC_WEAR(ITEM_WEAR_LEGS);
		MENTOC_WEAR(ITEM_WEAR_FEET);
		MENTOC_WEAR(ITEM_WEAR_HANDS);
		MENTOC_WEAR(ITEM_WEAR_ARMS);
		MENTOC_WEAR(ITEM_WEAR_SHIELD);
		MENTOC_WEAR(ITEM_WEAR_ABOUT);
		MENTOC_WEAR(ITEM_WEAR_WAIST);
		MENTOC_WEAR(ITEM_WEAR_WRIST);
		MENTOC_WEAR(ITEM_WEAR_WIELD);
		MENTOC_WEAR(ITEM_WEAR_HOLD);
		*player << "{gld}::Type::{/gld}\r\n";
#define MENTOC_TYPE(a){ if(obj->obj_flags.type_flag == a){*player << #a << "\r\n"; } }
		MENTOC_TYPE(ITEM_LIGHT);
		MENTOC_TYPE(ITEM_SCROLL);
		MENTOC_TYPE(ITEM_WAND);
		MENTOC_TYPE(ITEM_STAFF);
		MENTOC_TYPE(ITEM_WEAPON);
		MENTOC_TYPE(ITEM_FIREWEAPON);
		MENTOC_TYPE(ITEM_MISSILE);
		MENTOC_TYPE(ITEM_TREASURE);
		MENTOC_TYPE(ITEM_ARMOR);
		MENTOC_TYPE(ITEM_POTION);
		MENTOC_TYPE(ITEM_WORN);
		MENTOC_TYPE(ITEM_OTHER);
		MENTOC_TYPE(ITEM_TRASH);
		MENTOC_TYPE(ITEM_TRAP);
		MENTOC_TYPE(ITEM_CONTAINER);
		MENTOC_TYPE(ITEM_NOTE);
		MENTOC_TYPE(ITEM_DRINKCON);
		MENTOC_TYPE(ITEM_KEY);
		MENTOC_TYPE(ITEM_FOOD);
		MENTOC_TYPE(ITEM_MONEY);
		MENTOC_TYPE(ITEM_PEN);
		MENTOC_TYPE(ITEM_BOAT);
		MENTOC_TYPE(ITEM_FOUNTAIN);
		MENTOC_TYPE(ITEM_WEAPON_ATTACHMENT);
		*player << "{gld}::Bitvector::{/gld}\r\n";
#define MENTOC_BITVECTOR(a){ if(obj->obj_flags.bitvector & a){*player << #a << "\r\n"; } }
		MENTOC_BITVECTOR(ITEM_GLOW);
		MENTOC_BITVECTOR(ITEM_HUM);
		MENTOC_BITVECTOR(ITEM_NORENT);
		MENTOC_BITVECTOR(ITEM_NODONATE);
		MENTOC_BITVECTOR(ITEM_NOINVIS);
		MENTOC_BITVECTOR(ITEM_INVISIBLE);
		MENTOC_BITVECTOR(ITEM_MAGIC);
		MENTOC_BITVECTOR(ITEM_NODROP);
		MENTOC_BITVECTOR(ITEM_BLESS);
		MENTOC_BITVECTOR(ITEM_ANTI_GOOD);
		MENTOC_BITVECTOR(ITEM_ANTI_EVIL);
		MENTOC_BITVECTOR(ITEM_ANTI_NEUTRAL);
		MENTOC_BITVECTOR(ITEM_ANTI_MAGIC_USER);
		MENTOC_BITVECTOR(ITEM_ANTI_CLERIC);
		MENTOC_BITVECTOR(ITEM_ANTI_THIEF);
		MENTOC_BITVECTOR(ITEM_ANTI_WARRIOR);
		MENTOC_BITVECTOR(ITEM_NOSELL);
		player->sendln(
		    CAT(
		        "{red}obj_file: {/red} {grn}'",obj->feed_file().length() ? obj->feed_file().data() : "",
		        "'{/grn}"
		    )
		);
		for(unsigned index = 0;
		    index < MAX_OBJ_AFFECT; index++) {
			*player << "affected[" << index << "]->location: {/red} " <<
			    obj->affected[index].location <<
			    " {red}modifier: {/red} ";
			for(auto& flag : mods::builder::affected_flags) {
				if(flag.first == obj->affected[index].modifier) {
					*player << flag.second;
					break;
				}
			}
			*player << "\r\n";
		}
#define MENTOC_SHOW_FLAG(member,display) *player << "{red}" << #display << "{/red}: " << obj->member << "\r\n";
		MENTOC_SHOW_FLAG(obj_flags.value[0],value_0);
		MENTOC_SHOW_FLAG(obj_flags.value[1],value_1);
		MENTOC_SHOW_FLAG(obj_flags.value[2],value_2);
		MENTOC_SHOW_FLAG(obj_flags.value[3],value_3);
		MENTOC_SHOW_FLAG(obj_flags.weight,weight);
		MENTOC_SHOW_FLAG(obj_flags.cost,cost);
		MENTOC_SHOW_FLAG(obj_flags.cost_per_day,cost_per_day);
		MENTOC_SHOW_FLAG(obj_flags.timer,timer);
		MENTOC_SHOW_FLAG(obj_flags.bitvector,bitvector);
		//TODO !mundane make these flag code fragments into a function
		player->pager_end();
		player->page(0);
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"attr");
	if(args.has_value()) {
		auto arg_vec = args.value();
		auto get_intval = [&](std::string_view str) -> std::optional<int> {
			if(arg_vec[2].compare(str.data()) == 0) {
				*player << "matches: " << str.data() << "\r\n";
				auto i_value = mods::util::stoi(arg_vec[3]);
				if(!i_value.has_value()) {
					r_error(player,"Please use a valid numeric value.");
					return std::nullopt;
				}
				return i_value.value();
			}

			return std::nullopt;
		};
		auto get_strval = [&](std::string_view str) -> std::optional<char*> {
			if(arg_vec[2].compare(str.data()) == 0) {
				return arg_vec[3].data();
			}

			return std::nullopt;
		};
		auto index = mods::util::stoi(args.value()[1]);
		obj_data * obj = nullptr;
		if(index.has_value()) {
			std::size_t i = index.value();
			if(i >= obj_proto.size()) {
				r_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		}
		if(!obj) {
			r_error(player,"Invalid index");
			return;
		}
		MENTOC_OBI(item_number);
		if(arg_vec[2].compare("weapon_type:list") == 0) {
			std::string report = "";
			for(auto& ex_flag : mods::builder::weapon_type_flags) {
				if(!obj->rifle()) {
					continue;
				}
				if(obj->rifle()->type & ex_flag.first ||
				    obj->obj_flags.weapon_flags & ex_flag.first) {
					report += ex_flag.second + ",";
				}
			}
			r_success(player,report);
			return;
		}
		if(arg_vec[2].compare("weapon_type:remove") == 0) {
			if(arg_vec.end() <= arg_vec.begin() + 3) {
				r_error(player,"Please supply a flag");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			bool found = false;
			for(auto& ex_flag : mods::builder::weapon_type_flags) {
				if(ex_flag.second.compare(*flag) == 0) {
					obj->obj_flags.weapon_flags &= ~ex_flag.first;
					found = true;
					break;
				}
			}
			if(!found) {
				r_error(player,std::string("Unrecognized flag: ") + *flag);
			}
			r_success(player,"Cleared weapon flag");
			return;
		}
		if(arg_vec[2].compare("weapon_type:add") == 0) {
			if(arg_vec.end() <= arg_vec.begin() + 3) {
				r_error(player,"Please supply a flag");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			bool found = false;
			for(auto& ex_flag : mods::builder::weapon_type_flags) {
				if(ex_flag.second.compare(*flag) == 0) {
					if(!obj->rifle()) {
						exit(123);/** FIXME */
					}
					obj->obj_flags.weapon_flags ^= ex_flag.first;
					found = true;
					break;
				}
			}
			if(!found) {
				r_error(player,std::string("Unrecognized flag: ") + *flag);
			}
			r_success(player,"Set weapon type");
			return;
		}
		if(!obj->rifle()) {
			exit(200);
		}
		MENTOC_OBI(worn_on);
		MENTOC_OBI2(obj_flags.ammo,weapon_ammo);
		MENTOC_OBI2(obj_flags.ammo_max,weapon_ammo_max);
		MENTOC_OBI2(obj_flags.holds_ammo,weapon_holds_ammo);
		MENTOC_OBI2(obj_flags.ammo,weapon_ammo);
		MENTOC_OBS(name);
		MENTOC_OBS(description);
		MENTOC_OBS(short_description);
		MENTOC_OBS(action_description);
		if(arg_vec[2].compare("affected") == 0) {
			if(arg_vec.size() < 6) {
				r_error(player,"{obuild affected} Not enough arguments");
				return;
			}
			auto index = mods::util::stoi(arg_vec[3]);
			if(!index.has_value()) {
				r_error(player,"Invalid index");
				return;
			} else {
				if(index.value() > MAX_OBJ_AFFECT) {
					r_error(player,std::string(
					        "index cannot be larger than ") + std::to_string(MAX_OBJ_AFFECT));
					return;
				}
			}
			auto location = mods::util::stoi(arg_vec[4]);
			if(!location.has_value()) {
				r_error(player,"Invalid location");
				return;
			}
			auto modifier = mods::util::stoi(arg_vec[5]);
			if(!modifier.has_value()) {
				r_error(player,"Invalid modifier");
				return;
			}
			obj->affected[index.value()].location = location.value();
			obj->affected[index.value()].modifier = modifier.value();
		}
		r_success(player,"Set attribute");
	}
	return;
}

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
