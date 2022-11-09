#include "mbuild.hpp"
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
#include "zbuild.hpp"
#include "builder-flags.hpp"

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
	extern void encode_scripted_response(player_ptr_t& player, std::string_view encoded);
	extern void encode_scripted_response_safe(player_ptr_t& player, std::string_view encoded);
	extern void encode_scripted_response(player_ptr_t& player, str_map_t&& map);
	std::tuple<bool,std::string> save_mob_roam_pattern(int mob_id,std::string_view pattern) {
		rb_debug("saving mob roam pattern");
		try {
			auto up_txn = txn();
			sql_compositor comp("mobile",&up_txn);
			auto room_sql = comp.update("mobile")
			.set({
				{"mob_roam_pattern",pattern.data()}
			})
			.where("mob_virtual_number",std::to_string(mob_proto[mob_id].nr))
			.sql();
			rb_debug("roam pattern update: ");
			rb_debug(room_sql);
			auto row = mods::pq::exec(up_txn,room_sql.data());
			mods::pq::commit(up_txn);
			return {1,"set targets successfully"};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("save_mob_target exception: ",e.what());
			return {false,CAT("failed: '",e.what(),"'")};
		}
	}
	std::tuple<bool,std::string> save_mob_target(int mob_id,std::string targets) {
		rb_debug("saving mob targets");
		try {
			auto up_txn = txn();
			sql_compositor comp("mobile",&up_txn);
			auto room_sql = comp.update("mobile")
			.set({
				{"mob_targets",targets}
			})
			.where("mob_virtual_number",std::to_string(mob_proto[mob_id].nr))
			.sql();
			auto row = mods::pq::exec(up_txn,room_sql.data());
			mods::pq::commit(up_txn);
			return {1,"set targets successfully"};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("save_mob_target exception: ",e.what());
			return {false,CAT("failed: '",e.what(),"'")};
		}
	}

	std::tuple<bool,std::string> delete_by_mob_vnum(std::string_view mob_vnum) {
		rb_debug("delete all zone data -- start transaction");
		try {
			auto del_txn = txn();
			sql_compositor comp("zone_data",&del_txn);
			auto room_sql = comp.del()
			    .from("zone_data")
			    .where("zone_command","M")
			    .op_and("zone_arg1","=",mob_vnum.data())
			    .sql();
			auto row = mods::pq::exec(del_txn,room_sql.data());
			mods::pq::commit(del_txn);
			return {1,"deleted"};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("login exception",e.what());
			return {false,CAT("failed: '",e.what(),"'")};
		}
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

	char_data new_npc() {
		char_data proto;
		const char* foo = "foo";
		proto.player.name = foo;
		proto.player.short_descr = foo;
		proto.player.long_descr = foo;
		proto.player.description = foo;
		SET_BIT(proto.char_specials.saved.act, MOB_ISNPC);
		REMOVE_BIT(proto.char_specials.saved.act, MOB_NOTDEADYET);
		for(unsigned j=0; j < 5; j++) {
			GET_SAVE(&proto, j) = 0;
		}
		proto.aff_abils = proto.real_abils;
		return proto;
	}
	std::tuple<bool,std::string,uint64_t> save_player(char_data* obj) {
		try {
			auto txn_01 = txn();
			auto result_01 = mods::pq::exec(txn_01,sql_compositor("mobile",&txn_01)
			        .select("mob_id")
			        .from("mobile")
			        .where("mob_virtual_number","=",std::to_string(obj->mob_specials.vnum))
			        .sql()
			    );
			mods::pq::commit(txn_01);
#define MENTOC_PLAYER_NULL_CHECK(item)\
	if(!obj->item){\
		return {false,std::string(#item) + " is null!",0};\
	}
			MENTOC_PLAYER_NULL_CHECK(player.name);
			MENTOC_PLAYER_NULL_CHECK(player.short_descr);
			MENTOC_PLAYER_NULL_CHECK(player.long_descr);
			MENTOC_PLAYER_NULL_CHECK(player.description);
			sql_compositor::value_map p_map;
			p_map["mob_virtual_number"] = std::to_string(obj->mob_specials.vnum);
			p_map["mob_name"] = obj->player.name.c_str();
			p_map["mob_short_description"] = obj->player.short_descr.c_str();
			p_map["mob_long_description"] = obj->player.long_descr.c_str();
			p_map["mob_description"] = obj->player.description.c_str();
			p_map["mob_ability_strength"] = mods::util::itoa(obj->real_abils.str);
			p_map["mob_ability_strength_add"] = mods::util::itoa(obj->real_abils.str_add);
			p_map["mob_ability_intelligence"] = mods::util::itoa(obj->real_abils.intel);
			p_map["mob_ability_wisdom"] = mods::util::itoa(obj->real_abils.wis);
			p_map["mob_ability_dexterity"] = mods::util::itoa(obj->real_abils.dex);
			p_map["mob_ability_constitution"] = mods::util::itoa(obj->real_abils.con);
			p_map["mob_ability_charisma"] = mods::util::itoa(obj->real_abils.cha);
			p_map["mob_ability_electronics"] = mods::util::itoa(obj->real_abils.electronics);
			p_map["mob_ability_armor"] = mods::util::itoa(obj->real_abils.armor);
			p_map["mob_ability_marksmanship"] = mods::util::itoa(obj->real_abils.marksmanship);
			p_map["mob_ability_sniping"] = mods::util::itoa(obj->real_abils.sniping);
			p_map["mob_ability_demolitions"] = mods::util::itoa(obj->real_abils.demolitions);
			p_map["mob_ability_chemistry"] = mods::util::itoa(obj->real_abils.chemistry);
			p_map["mob_ability_weapon_handling"] = mods::util::itoa(obj->real_abils.weapon_handling);
			p_map["mob_ability_strategy"] = mods::util::itoa(obj->real_abils.strategy);
			p_map["mob_ability_medical"] = mods::util::itoa(obj->real_abils.medical);
			p_map["mob_mana"] = mods::util::itoa(obj->points.mana);
			p_map["mob_max_mana"] = mods::util::itoa(obj->points.max_mana);
			p_map["mob_hitpoints"] = mods::util::itoa(obj->points.hit);
			p_map["mob_max_hitpoints"] = mods::util::itoa(obj->points.max_hit);
			p_map["mob_move"] = mods::util::itoa(obj->points.move);
			p_map["mob_max_move"] = mods::util::itoa(obj->points.max_move);
			p_map["mob_armor"] = "0";// FIXME mods::util::itoa(obj->real_abils.armor);
			p_map["mob_gold"] = mods::util::itoa(obj->points.gold);
			p_map["mob_hitroll"] = mods::util::itoa(obj->points.hitroll);
			p_map["mob_default_position"] = mods::util::itoa(obj->mob_specials.default_pos);
			p_map["mob_damnodice"] = mods::util::itoa(obj->mob_specials.damnodice);
			p_map["mob_load_position"] = p_map["mob_default_position"];//mods::util::itoa(GET_POS(obj));
			p_map["mob_alignment"] = mods::util::itoa(obj->char_specials.saved.alignment);
			p_map["mob_action_bitvector"] = mods::util::itoa(obj->char_specials.saved.act);
			p_map["mob_affection_bitvector"] = mods::util::itoa(obj->char_specials.saved.affected_by);
			p_map["mob_attack_type"] = mods::util::itoa(obj->mob_specials.attack_type);
			p_map["mob_weight"] = mods::util::itoa(obj->player.weight);
			p_map["mob_height"] = mods::util::itoa(obj->player.height);
			p_map["mob_level"] = std::to_string(obj->player.level);
			p_map["mob_sex"] = mods::sex::to_db_string(obj->player.sex);
			p_map["mob_damsizedice"] = std::to_string(obj->mob_specials.damsizedice);
			p_map["mob_damroll"] = std::to_string(obj->points.damroll);
			p_map["mob_class"] = "0"; //TODO: find this
			p_map["mob_special_extended_type"] = std::to_string(obj->mob_specials.extended_mob_type);
			p_map["mob_exp"] = std::to_string(obj->mob_specials.experience);
			if(obj->mob_specials.raid_id) {
				p_map["mob_raid_id"] = std::to_string(obj->mob_specials.raid_id);
			} else {
				p_map.erase("mob_raid_id");
			}
			p_map["mob_scalable"] = obj->mob_specials.scalable ? "1" : "0";
			auto txn_02 = txn();
			std::string sql = "";
			if(result_01.size()) {
				//Update
				sql = sql_compositor("mobile",&txn_02)
				    .update("mobile")
				    .set(p_map)
				    .where("mob_virtual_number","=",std::to_string(obj->mob_specials.vnum))
				    .sql();
			} else {
				//Insert
				sql = sql_compositor("mobile",&txn_02)
				    .insert()
				    .into("mobile")
				    .values(p_map)
				    .sql();
			}
			mods::pq::exec(txn_02,sql);
			mods::pq::commit(txn_02);
		} catch(const std::exception& e) {
			REPORT_DB_ISSUE("error",e.what());
			return {false,e.what(),0};
		}
		/** Grab the id of the row we just upserted */
		try {
			auto txn_01 = txn();
			auto result_01 = mods::pq::exec(txn_01,sql_compositor("mobile",&txn_01)
			        .select("mob_id")
			        .from("mobile")
			        .where("mob_virtual_number","=",std::to_string(obj->mob_specials.vnum))
			        .sql()
			    );
			auto id = result_01[0]["mob_id"].as<uint64_t>();
			mods::pq::commit(txn_01);
			return {true,"Successfully saved player.", id};
		} catch(const std::exception& e) {
			REPORT_DB_ISSUE("error",e.what());
			return {false,e.what(),0};
		}
	}
};

using args_t = std::vector<std::string>;


char_data* grab_mobile(std::size_t index,bool& fetched) {
	if(index >= mob_proto.size()) {
		auto obj = mods::globals::read_mobile(index,VIRTUAL - 1);
		fetched = obj == nullptr;
		return obj;
	} else {
		fetched = true;
		return &mob_proto[index];
	}
}
void present_action(player_ptr_t& player, std::size_t index) {
	std::string flag_values;
	bool found = false;
	auto obj = grab_mobile(index,found);
	if(!found) {
		return;
	}
	for(auto& ex_flag : mods::builder::mob_act_flags) {
		if(obj->char_specials.saved.act & ex_flag.first) {
			flag_values += ex_flag.second;
			flag_values += ",";
		}
	}
	mods::builder_util::list_line(player, "action", flag_values);
	return;
}
void present_mob_specials(player_ptr_t& player, std::size_t index) {
	std::string flag_values;
	bool found = false;
	auto obj = grab_mobile(index,found);
	if(!found) {
		return;
	}
	std::string payload = "";
	payload = "extended_mob_type: '";
	payload += mods::mobs::extended_types::to_string(obj->mob_specials.extended_mob_type);
	payload += "'";
	mods::builder_util::list_line(player, "mob_specials", payload);
	return;
}

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

void initialize_mob_commands() {
	static bool initialized = false;
	if(initialized) {
		return;
	}
	mob_commands.emplace_back(
	    std::make_shared<command_t>(
	        "mini_gunner_sentinel", /* table */
	str_map_t{
		{"face-direction","mgs_face_direction"},
		{"room-vnum","mgs_room_vnum"},
		{"mob-vnum","mgs_mob_vnum"}
	},	/** column mappings */
	"mini-gunner-sentinel", /* command name */
	"<mob-id> <field> <value>", /* command format */
	3, /* command argument s*/
	-1, /* mob_vnum argument */
	0, /* mob_id argument */
	[](std::shared_ptr<command_t> cmd) {
		if(!mods::mobs::mg::orm::db_exists<sql_compositor>(cmd->m_vnum)) {
			mods::mobs::mg::orm::db_create<sql_compositor>(cmd->m_vnum,cmd->values);
		}
		mods::mobs::mg::orm::db_update<sql_compositor>(cmd->m_vnum,cmd->values);
	}
	    )
	);
	initialized = true;
}

SUPERCMD(do_mbuild) {
	static bool initialized = false;
	if(!initialized) {
		initialize_mob_commands();
		initialized = true;
	}
	/**
	 * Function status
	 * ---------------[ as of: 2019-02-16 ]
	 *  With the addition of being able to specify the obj->nr, we are
	 *  now able to create mobs properly. I have no idea how the code worked
	 *  before this change to the source.
	 *
	 *  > mbuild attr <mob_id> virt <virtual_number>
	 *
	 */
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	/** signature: mbuild <mob-vnum> zone:add <vnum>...<vnum-N> */
	if(vec_args.size() >= 3 && vec_args[0].compare("zone:add") == 0) {
		/*
		zone_virtual_number integer NOT NULL,
		mob_virtual_number integer NOT NULL,
		room_virtual_number integer NOT NULL,
		max_existing integer,
		*/
		/* TODO */
	}
	if(vec_args.size() >= 2 && vec_args[0].compare("zone:list") == 0) {
		/* TODO */
	}
	if(vec_args.size() >= 2 && vec_args[0].compare("zone:del") == 0) {
		/* TODO */
	}
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0
	    && vec_args[1].compare("action") == 0) {
		player->pager_start();
		*player << "possible flags:\r\n";
		for(auto& flag : mods::builder::mob_act_flags) {
			*player << flag.second << "\r\n";
		}
		*player << "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0
	    && vec_args[1].compare("sex") == 0) {
		player->pager_start();
		*player << "possible flags:\r\n";
		for(auto& flag : mods::builder::sex_flags) {
			*player << flag.second << "\r\n";
		}
		*player << "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0
	    && vec_args[1].compare("default_position") == 0) {
		player->pager_start();
		*player << "possible flags:\r\n";
		for(auto& flag : mods::builder::position_flags) {
			*player << flag.second << "\r\n";
		}
		*player << "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
		player->pager_start();
		*player << "usage: \r\n" <<
		    " {grn}mbuild{/grn} {red}help{/red}\r\n" <<
		    "  |--> this help menu\r\n" <<
		    " {grn}mbuild{/grn} {red}exists <mob_vnum>{/red}\r\n" <<
		    "  |--> [supports encoded response]\r\n" <<
		    " {grn}mbuild{/grn} {red}new{/red}\r\n" <<
		    "  |--> [supports encoded response]\r\n" <<
		    " {grn}mbuild{/grn} {red}list{/red}\r\n" <<
		    " {grn}mbuild{/grn} {red}attr <mob_id> <attr> <value>{/red}\r\n" <<
		    "  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
		    "  {gld}|:: raid{/gld}\r\n" <<
		    "  {gld}|:: scalable{/gld}\r\n" <<
		    "  {gld}|:: virt{/gld}\r\n" <<
		    "  {gld}|:: vnum{/gld} {grn}this is an alias of {/grn}{gld}virt{/gld}\r\n" <<
		    "  {gld}|:: exp{/gld}\r\n" <<
		    "  {gld}|:: name{/gld}\r\n" <<
		    "  {gld}|:: short_description{/gld}\r\n" <<
		    "  {gld}|:: long_description{/gld}\r\n" <<
		    "  {gld}|:: description{/gld}\r\n" <<
		    "  {gld}|:: mana{/gld}\r\n" <<
		    "  {gld}|:: max_mana{/gld}\r\n" <<
		    "  {gld}|:: hit{/gld}\r\n" <<
		    "  {gld}|:: max_hit{/gld}\r\n" <<
		    "  {gld}|:: move{/gld}\r\n" <<
		    "  {gld}|:: max_move{/gld}\r\n" <<
		    "  {gld}|:: armor{/gld}\r\n" <<
		    "  {gld}|:: gold{/gld}\r\n" <<
		    "  {gld}|:: exp{/gld}\r\n" <<
		    "  {gld}|:: hitroll{/gld}\r\n" <<
		    "  {gld}|:: damroll{/gld}\r\n" <<
		    "  {gld}|:: level{/gld}\r\n" <<
		    "  {gld}|:: weight{/gld}\r\n" <<
		    "  {gld}|:: height{/gld}\r\n" <<
		    "  {gld}|:: strength{/gld}\r\n" <<
		    "  {gld}|:: strength_add{/gld}\r\n" <<
		    "  {gld}|:: intelligence{/gld}\r\n" <<
		    "  {gld}|:: wisdom{/gld}\r\n" <<
		    "  {gld}|:: dexterity{/gld}\r\n" <<
		    "  {gld}|:: constitution{/gld}\r\n" <<
		    "  {gld}|:: electronics{/gld}\r\n" <<
		    "  {gld}|:: chemistry{/gld}\r\n" <<
		    "  {gld}|:: strategy{/gld}\r\n" <<
		    "  {gld}|:: marksmanship{/gld}\r\n" <<
		    "  {gld}|:: sniping{/gld}\r\n" <<
		    "  {gld}|:: weapon_handling{/gld}\r\n" <<
		    "  {gld}|:: demolitions{/gld}\r\n" <<
		    "  {gld}|:: armor{/gld}\r\n" <<
		    "  {gld}|:: medical{/gld}\r\n" <<
		    "  {gld}|:: charisma{/gld}\r\n" <<
		    "  {gld}|:: damnodice{/gld}\r\n" <<
		    "  {gld}|:: damsizedice{/gld}\r\n" <<
		    "  {gld}|:: sex {red}see mbuild help sex{/red}{/gld}\r\n" <<
		    "  {gld}|:: default_position {red}see mbuild help default_position{/red}{/gld}\r\n" <<
		    "  {gld}|:: action {red}see mbuild help action{/red}{/gld}\r\n" <<
		    " {grn}mbuild{/grn} {red}roam-pattern <mob_index> <type>{/red}\r\n" <<
		    "  |--> will set the mob's roam pattern to <type>.\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}mbuild{/wht} {gld}roam-pattern 5 Allied Foods{/gld}\r\n" <<
		    "  |:: (mob will roam where room title contains the string Allied Foods)\r\n" <<
		    " {grn}mbuild{/grn} {red}extended-type <mob_id> <type>{/red}\r\n" <<
		    "  |--> will set the mob's extended type to <type>. The list of\r\n" <<
		    "  available mob types follow.\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}mbuild{/wht} {gld}extended-type 5 INNOCENT{/gld}\r\n" <<
		    "  |:: (will mark the mob (using the rnum) as INNOCENT)\r\n" <<
		    "  {gld}|:: -:[extended-types]:-{/gld}\r\n";
		for(auto type : mods::mobs::extended_types::strings()) {
			*player << "  {gld}|:: " << type << "{/gld}\r\n";
		}
		*player <<
		    "  |::{red} for a description, type mbuild describe <type>\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}mbuild{/wht} {gld}describe INNOCENT{/gld}\r\n" <<
		    " {grn}mbuild{/grn} {red}targets <mob_id> <target>...[target-N]{/red}\r\n" <<
		    "  |--> will set the mob's target to the list specified.\r\n" <<
		    "  This is useful for mobs like GENERIC_THIEF which have an item that they \r\n" <<
		    "  are prone to gravitate toward. This field is free-form and prone to interpretation\r\n" <<
		    "  by the mud itself.\r\n" <<
		    "  {grn}|____[example]{/grn}\r\n" <<
		    "  |:: {wht}mbuild{/wht} {gld}targets 5 CARS ROOM_DARK HOUSEHOLDS DRUGS{/gld}\r\n" <<
		    " {grn}mbuild{/grn} {red}giveme:free_vnum{/red}\r\n" <<
		    "  |--> will give you an unused vnum that is preferably less than the value you would \r\n" <<
		    "  |--> get from giveme:next_vnum. If that range is exhausted, this is the equivalent of\r\n" <<
		    "  |--> running giveme:next_vnum\r\n" <<
		    "  |--> [supports encoded response]\r\n" <<
		    " {grn}mbuild{/grn} {red}giveme:next_vnum{/red}\r\n" <<
		    "  |--> [supports encoded response]\r\n" <<
		    " {grn}mbuild{/grn} {red}save <mob_id> {/red}\r\n" <<
		    " {grn}mbuild{/grn} {red}show <mob_id>{/red}\r\n" <<
		    " {grn}mbuild{/grn} {red}instantiate <mob_vnum>{/red}\r\n" <<
		    " {grn}mbuild{/grn} {red}place <mob_vnum> <room_vnum> <tag>...<tag-N>{/red}\r\n" <<
		    "  |--> will instantiate a mob of mob_vnum and place it in room_vnum with the specific tags.\r\n" <<
		    /** Mob actions */
		    " {grn}mbuild{/grn} {red}action:add <mob_id> <flag>{/red}\r\n" <<
		    " {grn}mbuild{/grn} {red}action:remove <mob_id> <flag>{/red}\r\n" <<
		    " {grn}mbuild{/grn} {red}action:clear <mob_id>{/red}\r\n" <<
		    " {grn}mbuild{/grn} {red}action:list <mob_id>{/red}\r\n"
		    ;
		for(auto& cmd : mob_commands) {
			cmd->print_command(player);
		}
		for(const auto& line : mods::mob_roam::roam_recorder_help_screen()) {
			player->sendln(line);
		}
		player->sendln("");
		player->pager_end();
		player->page(0);
		return;
	}
	{
		if(vec_args.size() >= 1 && vec_args[0].compare("giveme:free_vnum") == 0) {
			auto next_vnum = next_mob_number();
			std::vector<mob_vnum> taken;
			for(const auto& mob : mob_proto) {
				taken.emplace_back(mob.mob_specials.vnum);
			}
			for(auto i=1; i < next_vnum; ++i) {
				if(std::find(taken.cbegin(),taken.cend(),i) == taken.cend()) {
					player->sendln(CAT(i));
					ENCODE_STR(i);
					return;
				}
			}
			player->sendln(CAT(next_vnum));
			ENCODE_STR(next_vnum);
			return;
		}
	}
	{
		if(vec_args.size() >= 1 && vec_args[0].compare("giveme:next_vnum") == 0) {
			auto vnum = next_mob_number();
			player->sendln(CAT(vnum));
			player->set_scripted_response(
			mods::builder::encode_map({
				{"next_vnum",std::to_string(vnum)}
			}));
			return;
		}
	}
	{
		if(vec_args.size() >= 1 && vec_args[0].compare("giveme:int:next_vnum") == 0) {
			auto vnum = next_mob_number();
			player->sendln(CAT(vnum));
			ENCODE_STR(vnum);
			return;
		}
	}
	{
		/**
		 * handles:
		 * mbuild roam:<save|add|list|del|start|stop> <profile-name> <vnum>...<vnum-N>
		 */
		if(mods::mob_roam::handle_roam_recorder(player,vec_args)) {
			return;
		}
	}
	{
		auto args = mods::util::subcmd_args<9,args_t>(argument,"describe");
		if(args.has_value()) {
			//[ -  ] [ 0        ] [ 1    ]
			//mbuild <describe>    <type>
			auto arg_vec = args.value();
			if(arg_vec.size() < 2) {
				r_error(player,"{mbuild describe} Please specify a type");
				return;
			}
			auto desc = mods::mobs::extended_types::description(arg_vec[1]);
			r_success(player,desc.c_str());
			return;
		}
	}
	/** roam-pattern <mob_index> <value> */
	if(argshave()->int_at(1)->first_is("roam-pattern")->passed()) {
		auto mob_index = intat(1);
		if(mob_proto.size() <= mob_index) {
			r_error(player, "{mbuild roam-pattern} Mob-id is out of bounds.");
			return;
		}
		auto r = mods::builder::save_mob_roam_pattern(mob_index, args()->gather_strings_starting_at(2));
		if(std::get<0>(r)) {
			r_success(player,"{mbuild roam-pattern} Set roam pattern");
		} else {
			r_error(player,CAT("{mbuild roam-pattern} Failed to save roam pattern: '",std::get<1>(r),"'"));
		}
		return;
	}
	{
		auto args = mods::util::subcmd_args<50,args_t>(argument,"extended-type");
		if(args.has_value()) {
			//[ -  ] [ 0        ]    [ 1    ]  [  2   ]
			//mbuild <extended-type> <mob-id>  <type>
			auto arg_vec = args.value();
			if(arg_vec.size() < 3) {
				r_error(player,"{mbuild extended-type} Please specify a mob-id and extended-type");
				return;
			}
			auto i_value = mods::util::stoi(arg_vec[1]);
			if(!i_value.has_value()) {
				r_error(player,"{mbuild extended-type} Please specify a valid mob-id");
				return;
			}
			auto mob_id = i_value.value();
			std::string str_type = arg_vec[2];
			auto et_opt = mods::mobs::extended_types::from_string(str_type);
			if(et_opt.has_value() == false) {
				r_error(player, "{mbuild extended-type} Unrecognized extended type");
				return;
			}
			auto opt = et_opt.value();
			if(mob_proto.size() <= mob_id) {
				r_error(player, "{mbuild extended-type} Mob-id is out of bounds.");
				return;
			}
			mob_proto[mob_id].mob_specials.extended_mob_type = opt;
			r_success(player,"{mbuild extended-type} Set mob type.");
			return;
		}
	}
	{
		if(argshave()->first_is("targets")->passed()) {
			if(argshave()->size_gt(2)->passed() == false) {
				r_error(player,"{mbuild targets} Not enough arguments. Expected atleast 3");
				return;
			}
			if(argshave()->int_at(1)->passed() == false) {
				r_error(player,"{mbuild targets} Please specify a correct mob id");
				return;
			}
			auto mob_id = intat(1);
			if(mob_id >= mob_proto.size()) {
				r_error(player,"{mbuild targets} Mob id out of bounds");
				return;
			}
			auto s = mods::builder::save_mob_target(mob_id,args()->gather_strings_starting_at(2));
			if(!std::get<0>(s)) {
				r_error(player, CAT("{mbuild targets} ",std::get<1>(s)));
				return;
			}
			r_success(player,std::get<1>(s));
			return;
		}
	}
	for(auto& cmd : mob_commands) {
		if(cmd->ready_to_save(argument)) {
			cmd->save_function(cmd);
			return;
		}
		switch(cmd->parse(argument,player)) {
			case parse_response_t::HANDLED:
				return;
			default:
				break;
		}
	}
	auto args = mods::util::subcmd_args<11,args_t>(argument,"action:add");
	if(args.has_value()) {
		//[ -  ] [ 0        ] [ 1    ] [ 2  ]
		//mbuild <action:add> <mob_id> <flag>
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		if(!i_value.has_value()) {
			r_error(player,"{mbuild action:add} Please use a valid numeric value.");
			return;
		}
		if(arg_vec.end() <= arg_vec.begin() + 2) {
			r_error(player,"{mbuild action:add} Please supply action flags");
			return;
		}
		auto flag = arg_vec.begin() + 2;
		bool found = false;
		auto index = i_value.value();
		auto obj = grab_mobile(index,found);
		if(!found) {
			r_error(player,"{mbuild action:add} Invalid mob number");
			return;
		}
		for(auto& ex_flag : mods::builder::mob_act_flags) {
			if(ex_flag.second.compare(*flag) == 0) {
				obj->char_specials.saved.act |= ex_flag.first;
				found = true;
				r_success(player,"{mbuild action:add} Added flag");
				break;
			}
		}
		if(!found) {
			r_error(player,std::string("{mbuild action:add} Unrecognized flag: ") + *flag);
		}
	}
	args = mods::util::subcmd_args<14,args_t>(argument,"action:remove");
	if(args.has_value()) {
		//[ -  ] [ 0           ] [ 1    ]
		//mbuild <action:clear> <mob_id>
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		if(!i_value.has_value()) {
			r_error(player,"{mbuild action:remove} Please use a valid numeric value.");
			return;
		}
		bool found = false;
		auto index = i_value.value();
		auto obj = grab_mobile(index,found);
		if(!found) {
			r_error(player,"{mbuild action:remove} Invalid mob number");
			return;
		}
		obj->char_specials.saved.act = 0;
		r_success(player,"{mbuild action:remove} Removed flag");
		return;
	}
	args = mods::util::subcmd_args<14,args_t>(argument,"action:remove");
	if(args.has_value()) {
		//[ -  ] [ 0           ] [ 1    ] [ 2  ]
		//mbuild <action:remove> <mob_id> <flag>
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		if(!i_value.has_value()) {
			r_error(player,"{mbuild action:remove} Please use a valid numeric value.");
			return;
		}
		if(arg_vec.end() <= arg_vec.begin() + 2) {
			r_error(player,"{mbuild action:remove} Please supply action flags");
			return;
		}
		auto flag = arg_vec.begin() + 2;
		bool found = false;
		auto index = i_value.value();
		auto obj = grab_mobile(index,found);
		if(!found) {
			r_error(player,"{mbuild action:remove} Invalid mob number");
			return;
		}
		for(auto& ex_flag : mods::builder::mob_act_flags) {
			if(ex_flag.second.compare(*flag) == 0) {
				obj->char_specials.saved.act &= ~ex_flag.first;
				found = true;
				r_success(player,"{mbuild action:remove} Removed flag");
				break;
			}
		}
		if(!found) {
			r_error(player,std::string("{mbuild action:remove} Unrecognized flag: ") + *flag);
		}
	}
	args = mods::util::subcmd_args<12,args_t>(argument,"action:list");
	if(args.has_value()) {
		//[ -  ] [ 0         ] [ 1    ]
		//mbuild <action:list> <mob_id>
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		if(!i_value.has_value()) {
			r_error(player,"{mbuild action:list} Please use a valid numeric value.");
			return;
		}
		std::string flag_values;
		auto index = i_value.value();
		bool found = false;
		auto obj = grab_mobile(index,found);
		if(!found) {
			r_error(player,"{mbuild action:list} Invalid mob number");
			return;
		}
		for(auto& ex_flag : mods::builder::mob_act_flags) {
			if(obj->char_specials.saved.act & ex_flag.first) {
				flag_values += ex_flag.second;
				flag_values += ",";
			}
		}
		r_success(player,flag_values);
		return;
	}
	args = mods::util::subcmd_args<4,args_t>(argument,"new");
	if(args.has_value()) {
		mob_proto.push_back(mods::builder::new_npc());
		ENCODE_STR(mob_proto.size()-1);
		r_success(player,"{mbuild new} Mobile created");
		return;
	}
	args = mods::util::subcmd_args<7,args_t>(argument,"exists");
	if(args.has_value()) {
		//[ - ] [0 - exists] [1 - vnum]
		auto arg_vec = args.value();
		if(arg_vec[1].compare("raid") == 0) {
			ENCODE_INIT();
			r_error(player,"{mbuild exists} No mob with that vnum");
			return;
		}
		auto i_value = mods::util::stoi(arg_vec[1]);
		ENCODE_INIT();
		if(!i_value.has_value()) {
			r_error(player,"{mbuild exists} Please use a valid numeric value.");
			return;
		} else {
			auto arg_vec = args.value();
			auto i_value = mods::util::stoi(arg_vec[1]);
			unsigned ctr = 0;
			for(const auto& mob : mob_proto) {
				if(mob.mob_specials.vnum == i_value.value()) {
					ENCODE_STR(ctr);
					r_success(player,CAT("{mbuild exists} Index: ",ctr));
					return;
				}
				++ctr;
			}
			r_error(player,"{mbuild exists} No mob with that vnum");
			return;
		}
	}
	args = mods::util::subcmd_args<12,args_t>(argument,"clone");
	{
		if(args.has_value()) {
			auto arg_vec = args.value();
			auto i_value = mods::util::stoi(arg_vec[1]);
			if(!i_value.has_value()) {
				r_error(player,"{mbuild clone} Please use a valid numeric value.");
				return;
			} else {
				auto index = i_value.value();
				std::size_t i = index;
				if(i >= mob_proto.size()) {
					r_error(player,"{mbuild clone} Out of bounds");
					return;
				}
				mob_vnum v = next_mob_number();
				mob_proto.emplace_back();
				mob_proto.back() = mob_proto[i];
				mob_proto.back().mob_specials.vnum = v;
				r_success(player,"{mbuild clone} Object cloned");
			}
			return;
		}
	}
	args = mods::util::subcmd_args<12,args_t>(argument,"instantiate");
	if(args.has_value()) {
		ENCODE_INIT();
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		if(!i_value.has_value()) {
			r_error(player,"{mbuild instantiate} Please use a valid numeric value.");
			return;
		} else {
			/**
			 * TODO: The plan from 2019-02-15 is to build this code out below
			 * so that it properly loads the mobile into the game without issues.
			 * Main issue with the code below is that when the mobile is read
			 * from ram, the npc's name is printing malformed characters to
			 * the players. This is related to the act() and subsequently, the
			 * perform_act() function which gets called by act. These are fairly
			 * lame functions as they basically are parsers for a simplistic
			 * dollar based system that interpolates but is likely prone to
			 * massive amounts of abuse.
			 *
			 *
			 * Consensus:
			 * The stability of the code that loads the mobile from a prototype
			 * from within mob_proto is very questionable, though it is no longer
			 * the code that crashes the binary. Loading a mobile from a prototype
			 * should be tested using some sort of js test suites. TODO
			 *
			 *
			 */
			auto v = i_value.value_or(-1);
			if(v < 0) {
				r_error(player,"{mbuild instantiate} Vnum must be a positive number");
				ENCODE_R("{mbuild instantiate} invalid vnum value");
				return;
			}
			auto obj = mods::globals::read_mobile_ptr(v,VIRTUAL);
			if(!obj) {
				r_error(player,"{mbuild instantiate} Cannot find mob by that vnum");
				ENCODE_R("{mbuild instantiate} couldnt find mob with vnum");
				return;
			}
			mods::globals::rooms::char_to_room(player->room(),obj->cd());
			r_success(player,"{mbuild instantiate} Object created, look on the floor");
			ENCODE_OK();
		}
		return;
	}
	//" {grn}mbuild{/grn} {red}place <mob_vnum> <room_vnum> <tag>...<tag-N>{/red}\r\n" <<
	args = mods::util::subcmd_args<12,args_t>(argument,"place");
	if(args.has_value()) {
		ENCODE_INIT();
		auto arg_vec = args.value();
		if(arg_vec.size() < 3) {
			r_error(player,"{mbuild place} Invalid number of arguments");
			return;
		}
		auto mobvnum = mods::util::stoi(arg_vec[1]).value_or(-1);
		if(mobvnum < 0) {
			r_error(player,"{mbuild place} Mob Vnum must be a positive number");
			ENCODE_R("invalid vnum value");
			return;
		}
		auto roomvnum = mods::util::stoi(arg_vec[1]).value_or(-1);
		if(roomvnum < 0) {
			r_error(player,"{mbuild place} Room Vnum must be a positive number");
			ENCODE_R("invalid vnum value");
			return;
		}
		auto obj = mods::globals::read_mobile_ptr(mobvnum,VIRTUAL);
		if(!obj) {
			r_error(player,"{mbuild place} Cannot find mob by that vnum");
			ENCODE_R("couldnt find mob with vnum");
			return;
		}
		auto realroom = real_room(roomvnum);
		if(realroom == NOWHERE) {
			r_error(player,"{mbuild place} Room doesn't exist");
			ENCODE_R("!room-exists");
			return;
		}
		mods::globals::rooms::char_to_room(realroom,obj->cd());
		mods::builder::encode_scripted_response(player,str_map_t({{"uuid",std::to_string(obj->uuid())}}));
		r_success(player,"{mbuild place} Object created, look on the floor");
		return;
	}
	/** TODO: create copy command */
	args = mods::util::subcmd_args<5,args_t>(argument,"copy");
	if(args.has_value()) {
		//r_status(player,"...");
		auto arg_vec = args.value();
		auto i_value = mods::util::stoul(arg_vec[1]);
		if(!i_value.has_value()) {
			r_error(player,"{mbuild copy} Please use a valid numeric value.");
			return;
		}
		if(i_value.value() >= mob_proto.size()) {
			r_error(player,"{mbuild copy} Out of bounds");
			return;
		}
		mob_proto.push_back(mob_proto[i_value.value()]);
		mob_proto[i_value.value()].mob_specials.raid_id = get_raid_id(player);
		auto return_pair = mods::builder::save_player(&mob_proto[i_value.value()]);
		if(!std::get<0>(return_pair)) {
			r_error(player,std::get<1>(return_pair));
			return;
		}
		r_success(player,"{mbuild copy} Mobile created and saved");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"list");
	if(args.has_value()) {
		r_status(player,"listing...");
		unsigned mob_id = 0;
		player->pager_start();
		for(auto& mob_reference : mob_proto) {
			auto mob = &mob_reference;
			*player << "{gld}[" << mob_id++ << "]{/gld}" <<
			    "{grn}[vnum: " << mob->mob_specials.vnum << "]{/grn} :->" <<
			    "{red} [" << mob->player.short_descr.c_str() << "]{/red}\r\n";
		}
		player->pager_end();
		player->page(0);
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"save");
	if(args.has_value()) {
		auto arg_vec = args.value();
		if(arg_vec.size() < 2) {
			r_error(player,"{mbuild list} Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		if(!index.has_value()) {
			r_error(player,"{mbuild list} Invalid index");
			return;
		}
		std::size_t i = index.value();
		if(i  >= mob_proto.size()) {
			r_error(player,"{mbuild list} Invalid index");
			return;
		}
		auto obj = &mob_proto[index.value()];
		auto return_pair = mods::builder::save_player(obj);
		if(!std::get<0>(return_pair)) {
			r_error(player,std::get<1>(return_pair));
			return;
		}
		r_success(player,"{mbuild list} Object saved.");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"show");
	if(args.has_value()) {
		auto arg_vec = args.value();
		if(arg_vec.size() < 2) {
			r_error(player,"{mbuild show} Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		if(!index.has_value()) {
			r_error(player,"{mbuild show} Invalid index");
			return;
		}
		std::size_t i = index.value();
		if(i >= mob_proto.size()) {
			r_error(player,"{mbuild show} Invalid index");
			return;
		}
		auto obj = &mob_proto[index.value()];
		player->pager_start();
		*player << "{gld}::player::{/gld}\r\n";
#define MENTOC_SHOW_OBJ_FLAGS(display_name,struct_member,flag_structure) \
	for(auto & object_flag : flag_structure){\
		if(object_flag.first == obj->struct_member){\
			*player << "{red}" << #display_name << "{/red}: " <<\
			    object_flag.second << "\r\n";\
			break;\
		}\
	}
#define MENTOC_SHOW_OBJ_BITVECTOR(display_name,struct_member,flag_structure) \
	*player << "{red}" << #display_name << "{/red}: ";\
	for(auto & object_flag : flag_structure){\
		if(object_flag.first & obj->struct_member){\
			*player << object_flag.second << " ";\
		}\
	}\
	*player << "\r\n";
#define MENTOC_SHOW_OBJ(display_name,struct_member) \
	*player << "{red}" << #display_name << "{/red}: " << obj->struct_member << "\r\n";
		MENTOC_SHOW_OBJ(name,player.name.c_str());
		MENTOC_SHOW_OBJ(raid_id,mob_specials.raid_id);
		MENTOC_SHOW_OBJ(scalable,mob_specials.scalable);
		MENTOC_SHOW_OBJ(virtual_number,mob_specials.vnum);
		MENTOC_SHOW_OBJ(experience,mob_specials.experience);
		MENTOC_SHOW_OBJ(short_description,player.short_descr.c_str());
		MENTOC_SHOW_OBJ(long_description,player.long_descr.c_str());
		MENTOC_SHOW_OBJ(description,player.description.c_str());
		MENTOC_SHOW_OBJ_FLAGS(sex,player.sex,mods::builder::sex_flags);
		MENTOC_SHOW_OBJ(level,player.level);
		MENTOC_SHOW_OBJ(weight,player.weight);
		MENTOC_SHOW_OBJ(height,player.height);
		MENTOC_SHOW_OBJ(strength,real_abils.str);
		MENTOC_SHOW_OBJ(strength_add,real_abils.str_add);
		MENTOC_SHOW_OBJ(intelligence,real_abils.intel);
		MENTOC_SHOW_OBJ(wisdom,real_abils.wis);
		MENTOC_SHOW_OBJ(dexterity,real_abils.dex);
		MENTOC_SHOW_OBJ(constitution,real_abils.con);
		MENTOC_SHOW_OBJ(charisma,real_abils.cha);
		MENTOC_SHOW_OBJ_FLAGS(default_position,mob_specials.default_pos,mods::builder::position_flags);
		MENTOC_SHOW_OBJ(damnodice,mob_specials.damnodice);
		MENTOC_SHOW_OBJ(damsizedice,mob_specials.damsizedice);
		MENTOC_SHOW_OBJ(mana,points.mana);
		MENTOC_SHOW_OBJ(max_mana,points.max_mana);
		MENTOC_SHOW_OBJ(hit,points.hit);
		MENTOC_SHOW_OBJ(max_hit,points.max_hit);
		MENTOC_SHOW_OBJ(move,points.move);
		MENTOC_SHOW_OBJ(max_move,points.max_move);
		MENTOC_SHOW_OBJ(armor,real_abils.armor);
		MENTOC_SHOW_OBJ(gold,points.gold);
		MENTOC_SHOW_OBJ(exp,points.exp);
		MENTOC_SHOW_OBJ(hitroll,points.hitroll);
		MENTOC_SHOW_OBJ(damroll,points.damroll);
		present_action(player,i);
		present_mob_specials(player,i);
		player->pager_end();
		player->page(0);
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"attr");
	if(args.has_value()) {
		auto arg_vec = args.value();
		if(arg_vec.size() < 4) {
			r_error(player,"{mbuild attr} Invalid number of arguments");
			return;
		}
		auto get_intval = [&](std::string_view str) -> std::optional<int> {
			if(arg_vec[2].compare(str.data()) == 0) {
				auto i_value = mods::util::stoi(arg_vec[3]);
				if(!i_value.has_value()) {
					r_error(player,"{mbuild attr} Please use a valid numeric value.");
					return std::nullopt;
				}
				return i_value.value();
			}

			return std::nullopt;
		};
		auto get_strval = [&](std::string_view str) -> std::optional<const char*> {
			if(arg_vec[2].compare(str.data()) == 0) {
				return arg_vec[3].c_str();
			}

			return std::nullopt;
		};
		auto index = mods::util::stoi(arg_vec[1]);
		std::size_t i = index.value();
		if(index.has_value() && i < mob_proto.size()) {
			auto obj = &mob_proto[index.value()];
			obj->player.name.assign(get_strval("name").value_or(obj->player.name.c_str()));
			obj->player.short_descr.assign(get_strval("short_description").value_or(obj->player.short_descr.c_str()));
			obj->player.long_descr.assign(get_strval("long_description").value_or(obj->player.long_descr.c_str()));
			obj->player.description.assign(get_strval("description").value_or(obj->player.description.c_str()));
			MENTOC_OBI2(points.mana,mana);
			MENTOC_OBI2(points.max_mana,max_mana);
			MENTOC_OBI2(points.hit,hit);
			MENTOC_OBI2(points.max_hit,max_hit);
			MENTOC_OBI2(points.move,move);
			MENTOC_OBI2(points.max_move,max_move);
			MENTOC_OBI2(points.gold,gold);
			MENTOC_OBI2(points.exp,exp);
			MENTOC_OBI2(points.hitroll,hitroll);
			MENTOC_OBI2(points.damroll,damroll);
			MENTOC_OBI2(player.level,level);
			MENTOC_OBI2(player.weight,weight);
			MENTOC_OBI2(player.height,height);
			MENTOC_OBI2(real_abils.electronics,electronics);
			MENTOC_OBI2(real_abils.armor,armor);
			MENTOC_OBI2(real_abils.marksmanship,marksmanship);
			MENTOC_OBI2(real_abils.sniping,sniping);
			MENTOC_OBI2(real_abils.demolitions,demolitions);
			MENTOC_OBI2(real_abils.chemistry,chemistry);
			MENTOC_OBI2(real_abils.weapon_handling,weapon_handling);
			MENTOC_OBI2(real_abils.strategy,strategy);
			MENTOC_OBI2(real_abils.medical,medical);
			MENTOC_OBI2(real_abils.str,strength);
			MENTOC_OBI2(real_abils.str_add,strength_add);
			MENTOC_OBI2(real_abils.intel,intelligence);
			MENTOC_OBI2(real_abils.wis,wisdom);
			MENTOC_OBI2(real_abils.dex,dexterity);
			MENTOC_OBI2(real_abils.con,consitution);
			MENTOC_OBI2(real_abils.cha,charisma);
			MENTOC_OBI2(mob_specials.damnodice,damnodice);
			MENTOC_OBI2(mob_specials.damsizedice,damsizedice);
			MENTOC_OBI2(mob_specials.attack_type,attack_type);
			MENTOC_OBI2(mob_specials.damsizedice,damsizedice);
			MENTOC_OBI2(mob_specials.experience,exp);
			MENTOC_OBI2(mob_specials.raid_id,raid_id);
			MENTOC_OBI2(mob_specials.scalable,scalable);
			if(arg_vec[2].compare("virt") == 0 || arg_vec[2].compare("vnum") == 0) {
				if(arg_vec.end() <= arg_vec.begin() + 3) {
					r_error(player,"{mbuild virt/vnum} Please supply a virtual number");
					return;
				}
				auto opt_vr_number = mods::util::stoi(arg_vec[3]);
				obj->mob_specials.vnum = opt_vr_number.value();
				r_success(player,"{mbuild attr virt/vnum} Saved");
				return;
			}
			if(arg_vec[2].compare("exp") == 0) {
				auto opt_vr_number = mods::util::stoi(arg_vec[3]);
				obj->mob_specials.experience = opt_vr_number.value();
				r_success(player,"{mbuild atter exp} Saved");
				return;
			}
			if(arg_vec[2].compare("raid") == 0) {
				auto opt_raid_id = mods::util::stoi(arg_vec[3]);
				obj->mob_specials.raid_id = opt_raid_id.value();
				r_success(player,"{mbuild attr raid} Saved");
				return;
			}
			if(arg_vec[2].compare("sex") == 0) {
				if(arg_vec.end() <= arg_vec.begin() + 3) {
					r_error(player,"{mbuild attr sex} Please supply a flag");
					return;
				}
				auto flag = arg_vec.begin() + 3;
				bool found = false;
				for(auto& ex_flag : mods::builder::sex_flags) {
					if(ex_flag.second.compare(*flag) == 0) {
						obj->player.sex = ex_flag.first;
						found = true;
						break;
					}
				}
				if(!found) {
					r_error(player,std::string("{mbuild attr sex} Unrecognized flag: ") + *flag);
				}
			}
			if(arg_vec[2].compare("default_position") == 0) {
				if(arg_vec.end() <= arg_vec.begin() + 3) {
					r_error(player,"{mbuild attr default_position} Please supply a flag");
					return;
				}
				auto flag = arg_vec.begin() + 3;
				bool found = false;
				for(auto& ex_flag : mods::builder::position_flags) {
					if(ex_flag.second.compare(*flag) == 0) {
						obj->mob_specials.default_pos = ex_flag.first;
						found = true;
						break;
					}
				}
				if(!found) {
					r_error(player,std::string("{mbuild attr default_position} Unrecognized flag: ") + *flag);
				}
			}
		}
		return;
	}
}
