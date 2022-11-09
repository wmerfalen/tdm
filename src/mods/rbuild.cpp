#include "rbuild.hpp"
#include "rbuild-sandbox.hpp"
#include "doors.hpp"
#include "jx.hpp"
#include "player.hpp"
#include "rooms.hpp"
#include "builder-data.hpp"
#include "builder/encode.hpp"
#include "world-configuration.hpp"
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
using player_ptr_t = std::shared_ptr<mods::player>;
using jxcomp = mods::jx::compositor;
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

extern void refresh_mobs_and_zones();

extern int next_room_number();
extern int next_zone_number();
extern int next_zone_vnum() ;
extern std::tuple<int16_t,std::string> parse_sql_zones();

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

int next_room_vnum() {
	static int next_room_number = 0;
	if(next_room_number == 0) {
		try {
			auto up_txn = txn();
			auto record = mods::pq::exec(up_txn,"SELECT max(room_number) + 1 as r from room;");
			if(record.size()) {
				std::cerr << ("Max room number: " + std::string(record[0]["r"].c_str()));
				next_room_number = mods::util::stoi<int>(record[0]["r"].c_str());
			} else {
				next_room_number = 1;
			}
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error grabbing next max room number '",e.what());
			next_room_number = 1;
		}
	}
	std::cerr << "next_room_number(vnum): " << next_room_number + 1 << "\n";
	return ++next_room_number;
}

extern void r_error(const player_ptr_t& player,std::string_view msg);
extern void r_success(const player_ptr_t& player,std::string_view msg);
extern void r_status(const player_ptr_t& player,std::string_view msg);
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

	extern void encode_scripted_response(player_ptr_t& player, std::string_view encoded) ;
	extern void encode_scripted_response_safe(player_ptr_t& player, std::string_view encoded) ;
	extern void encode_scripted_response(player_ptr_t& player, str_map_t&& map) ;
	std::tuple<int8_t,std::string> pave_continue(player_ptr_t& player) {
		if(player->builder_data && player->builder_data->room_pave_mode) {
			std::cerr << "[room_pavements] already paving...\n";
			return {-1,"It looks like you're already paving. Save your existing pavement to begin."};
		}
		mods::builder::initialize_builder(player);
		player->builder_data->room_pave_mode = true;
		player->builder_data->room_pavements.start_room = player->room();
		player->builder_data->room_pavements.zone_id = zone_table[world[player->room()].zone].get_id();
		std::cerr << "[room_pavements] zone_id: " << player->builder_data->room_pavements.zone_id << "\n";
		return {0,"Continuing pavement."};
	}
	bool currently_paving(player_ptr_t& player) {
		return player->builder_data && player->builder_data->room_pave_mode;
	}
	std::tuple<int8_t,std::string> pave_off(player_ptr_t& player) {
		player->builder_data->room_pave_mode = false;
		return {0,"success"};
	}
	std::tuple<int8_t,std::string> pave_on(player_ptr_t& player,std::string_view sandbox_name) {
		/**
		 * cmd_args will be: [0] => pave, [1] => <on|off> [2] => <name>
		 */
		mods::builder::sandbox_data_t sandbox;
		auto zone_vnum = next_zone_vnum();
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
		std::cerr << ("zone_vnum:" + std::to_string(zone_vnum));
#endif
		auto room_id = next_room_vnum();
		sandbox.new_sandbox(player,
		    sandbox_name,
		    room_id,
		    zone_vnum
		);
		sandbox.set_name(sandbox_name);
		auto status = mods::builder::save_zone_to_db(
		        zone_vnum,
		        sandbox_name,
		        room_id,
		        room_id + 100,
		        100,
		        2
		    );
		if(!std::get<0>(status)) {
			return {-1,"Failed to create sandbox!"};
		}
		mods::builder::sandboxes[player->name().c_str()].emplace_back(std::move(sandbox));
		player->set_flag(mods::flags::chunk_type_t::BUILDER,mods::builder::HAS_SANDBOX);
		player->builder_data->room_pavements.rooms.clear();
		player->builder_data->room_pave_mode = true;
		player->builder_data->room_pavements.start_room = room_id;
		player->builder_data->room_pavements.zone_id = zone_vnum;
		if(player->builder_data->room_pavements.zone_id == -1) {
			return {-2, "Zone id is -1"};
		}
		player->builder_data->room_pavements.transact_id = next_room_pavement_transaction_id();
		return {0,"Sandbox created"};
	}
	extern bool has_flag(player_ptr_t player,uint64_t flag);

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

	void initialize_builder(player_ptr_t& player) {
		if(!player->has_builder_data()) {
			std::cerr << "initialize_builder: creating shared_ptr\n";
			player->builder_data = std::make_shared<builder_data_t>();
		} else {
			std::cerr << "initialize_builder: shared_ptr already built\n";
		}
		player->set_bui_mode(true);
	}
	void add_room_to_pavements(player_ptr_t& player, int room_id) {
		auto& r = player->builder_data->room_pavements.rooms;
		if(std::find(r.begin(),r.end(),room_id) == r.end()) {
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
			std::cerr << "[add_room_to_pavements]-> adding room: " << room_id << "\n";
#endif
			player->builder_data->room_pavements.rooms.push_back(room_id);
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

	/** SAVE ROOM TO DB */
	int save_to_db(room_rnum in_room,std::string& error_string) {
		auto world_top = mods::globals::room_list.size();
		std::size_t ir = in_room;
		if(ir >= world_top) {
			error_string = "room number out of range";
			return mods::builder::ROOM_NUMBER_OUT_OF_RANGE;
		}
		if(!world[in_room].name) {
			world[in_room].name = "<default room name>";
			//return mods::builder::ROOM_NAME_EMPTY;
		}
		if(!world[in_room].description) {
			world[in_room].description = "<default description>";
			//return mods::builder::ROOM_DESC_EMPTY;
		}
		std::cerr << "[world[in_room].zone]: " << world[in_room].zone << "\n";
		std::cerr << "zone_table[world[in_room].zone]: ";
		zone_table[world[in_room].zone].dump();
		std::cerr << "zone_table.size(): " << zone_table.size() << "\n";
		std::map<std::string,std::string> values;
		values["zone"] = std::to_string(zone_table[world[in_room].zone].get_id());
		values["sector_type"] = std::to_string(world[in_room].sector_type);
		values["name"] = world[in_room].name.c_str();
		values["description"] = world[in_room].description.c_str();
		values["room_number"] = std::to_string(world[in_room].number);
		values["nickname"] = world[in_room].nickname.c_str();
		std::string textures = "";
		for(const auto& int_texture : world[in_room].textures()) {
			std::optional<std::string> str = mods::rooms::texture_to_string(int_texture);
			if(str.has_value()) {
				textures += CAT(",",str.value());
			}
		}
		values["textures"] = textures;
		std::array<char,16> num;
		std::fill(num.begin(),num.end(),0);
		sprintf(&num[0],"%d",world[in_room].light);
		values["light"] = &num[0];
		std::fill(num.begin(),num.end(),0);
		sprintf(&num[0],"%d",world[in_room].room_flags);
		values["room_flag"] = &num[0];
		pqxx::result room_record;
		auto number = tostr(world[in_room].number);
		{
			try {
				auto up_txn = txn();
				sql_compositor comp("room",&up_txn);
				auto room_sql = comp.select("room_number")
				    .from("room")
				    .where("room_number","=",number)
				    .sql();
				rb_debug(room_sql);
				room_record = mods::pq::exec(up_txn,room_sql);
				rb_debug("done");
				rb_debug(room_record.size());
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("selecting room in db",e.what());
				error_string = "error selecting room from db: '";
				error_string += e.what();
				rb_debug("EXCEPTION");
				rb_debug(e.what());
				return -4;
			}
		}
		if(room_record.size()) {
			/* update the record */
			rb_debug("Attempting to update record");
			try {
				auto up_txn = txn();
				sql_compositor comp("room",&up_txn);
				auto up_sql = comp
				    .update("room")
				    .set(values)
				    .where("room_number","=",number)
				    .sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("updating room in db",e.what());
				error_string = "error updating room in db: '";
				error_string += e.what();
				rb_debug("EXCEPTION (UPDATE)");
				rb_debug(e.what());
				rb_debug(error_string);
				return -3;
			}
		} else {
			rb_debug("need to insert instead of update");
			values["room_number"] = number;
			try {
				auto txn2 = txn();
				sql_compositor comp("room",&txn2);
				auto sql = comp
				    .insert()
				    .into("room")
				    .values(values).sql();
				rb_debug("calling exec");
				rb_debug(sql);
				rb_map_debug<decltype(values)>(values);
				mods::pq::exec(txn2,sql);
				mods::pq::commit(txn2);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("inserting room in db",e.what());
				error_string = "error inserting room in db: '";
				error_string += e.what();
				rb_debug("EXCEPTION (INSERT)");
				rb_debug(e.what());
				rb_debug(error_string);
				return -4;
			}
			rb_debug("committing (didnt update, inserted instead)");
		}
		rb_debug("delete transaction");
		try {
			auto del_txn = txn();
			mods::pq::exec(del_txn,std::string("DELETE FROM room_extra_descriptions where red_room_vnum=") + std::to_string(world[in_room].number));
			mods::pq::commit(del_txn);
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("deleting room_direction_data ",e.what());
			error_string = "error deleting room_direction_data in db: '";
			error_string += e.what();
			rb_debug("EXCEPTION (DELETE)");
			rb_debug(e.what());
			rb_debug(error_string);
			return -5;
		}
		for(auto& ex : world[in_room].ex_descriptions()) {
			rb_debug("Inserting room extra descriptions data");
			strmap_t values = {
				{"red_room_vnum",std::to_string(world[in_room].number)},
				{"red_keyword",ex.keyword.c_str()},
				{"red_description",ex.description.c_str()},
			};
			rb_map_debug(values);
			try {
				auto txn2 = txn();
				sql_compositor comp("room_extra_descriptions",&txn2);
				auto sql = comp
				    .insert()
				    .into("room_extra_descriptions")
				    .values(values)
				    .sql();
				mods::pq::exec(txn2,sql);
				mods::pq::commit(txn2);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("room dir data in db",e.what());
				error_string = "error deleting room_direction_data in db: '";
				error_string += e.what();
				rb_debug("EXCEPTION (DELETE)");
				rb_debug(e.what());
				rb_debug(error_string);
				return -5;
			}
		}
		rb_debug("Deleted room_direction-data... now, insert some");
		for(auto direction = 0; direction < NUM_OF_DIRS; direction++) {
			if(world[in_room].dir_option[direction] &&
			    world[in_room].dir_option[direction]->general_description) {
				std::cout << "direction: " << direction << "\n";
				assert(world.size() > in_room);
				assert(world[in_room].dir_option[direction] != nullptr);
				assert(world.size() > world[in_room].dir_option[direction]->to_room);

				auto to_room = world[world[in_room].dir_option[direction]->to_room].number;
				rb_debug("to_room num:" + std::to_string(to_room));
				std::map<std::string,std::string> values = {
					{"general_description",static_cast<std::string>(world[in_room].dir_option[direction]->general_description)},
					{"keyword",static_cast<std::string>(world[in_room].dir_option[direction]->keyword)},
					{"exit_info",std::to_string(world[in_room].dir_option[direction]->exit_info)},
					{"exit_key",std::to_string(world[in_room].dir_option[direction]->key)},
					{"to_room",std::to_string(to_room)},
					{"room_number",number},
					{"exit_direction",std::to_string(direction)}
				};
				{
					rb_debug("Inserting room dir data");
					rb_map_debug(values);
					try {
						auto txn2 = txn();
						sql_compositor comp("room_direction_data",&txn2);
						auto sql = comp
						.insert()
						.into("room_direction_data")
						.values(values)
						.sql();
						mods::pq::exec(txn2,sql);
						mods::pq::commit(txn2);
					} catch(std::exception& e) {
						REPORT_DB_ISSUE("room dir data in db",e.what());
						error_string = "error deleting room_direction_data in db: '";
						error_string += e.what();
						rb_debug("EXCEPTION (DELETE)");
						rb_debug(e.what());
						rb_debug(error_string);
						return -5;
					}
				}
				//}
			}
		}
		error_string = "";
		return 0;
	}



	int import_room(struct room_data* room) {
		auto world_top = mods::globals::room_list.size();
		auto old_world = world;
		world.push_back(*room);
		world[world_top].number = room->number;
		world[world_top].zone = room->zone;
		world[world_top].sector_type = room->sector_type;
		world[world_top].name = room->name;
		world[world_top].description = room->description;
		world[world_top].ex_descriptions() = room->ex_descriptions();
		for(unsigned i =0; i < NUM_OF_DIRS; i++) {
			world[world_top].dir_option[i] = room->dir_option[i];
		}
		world[world_top].room_flags = room->room_flags;
		world[world_top].light = room->light;
		world[world_top].func = room->func;
		world[world_top].contents = room->contents;
		world[world_top].people = room->people;
		world[world_top].nickname = room->nickname;
		mods::globals::register_room(world_top);
		return 0;
	}
	room_data new_room(player_ptr_t player,int direction) {
		world.emplace_back();
		top_of_world = world.size();
		mods::globals::register_room(world.size());
		world.back().init();
		world.back().number = next_room_vnum();
		return world.back();
	}
	bool title(room_rnum room_id,std::string_view str_title) {
		std::size_t rid = room_id;
		if(rid < mods::globals::room_list.size()) {
			world[room_id].name = str_title.data();
			return true;
		}
		return false;
	}
	bool description(room_rnum room_id,std::string_view str_description) {
		std::size_t rid = room_id;
		if(rid < mods::globals::room_list.size()) {
			world[room_id].description = str_description.data();
			return true;
		}
		return false;
	}
	std::optional<std::string> dir_option(room_rnum room_id,int direction,std::optional<std::string_view> description,
	    std::optional<std::string_view> keywords,
	    std::optional<int> exit_info,
	    std::optional<int> key,
	    std::optional<room_rnum> to_room
	) {
		std::size_t rid = room_id;
		if(rid > mods::globals::room_list.size()) {
			return "Invalid room number";
		}
		if(direction > NUM_OF_DIRS) {
			return "direction number is incorrect";
		}
		if(!world[room_id].dir_option[direction]) {
			std::string d = "description", k= "keyword";
			if(description.has_value()) {
				d = description.value().data();
			}
			if(keywords.has_value()) {
				k = keywords.value().data();
			}
			world[room_id].set_dir_option(
			    direction,
			    d,k,
			    exit_info.value_or(0),
			    key.value_or(0),
			    to_room.value_or(0));
			return std::nullopt;
		}
		if(description.value_or("-1").compare("-1") != 0) {
			world[room_id].dir_option[direction]->general_description = description.value().data();
		}
		if(keywords.value_or("-1").compare("-1") != 0) {
			world[room_id].dir_option[direction]->keyword = keywords.value().data();
		}
		world[room_id].dir_option[direction]->exit_info = exit_info.value_or(world[room_id].dir_option[direction]->exit_info);
		world[room_id].dir_option[direction]->key = key.value_or(world[room_id].dir_option[direction]->key);
		world[room_id].dir_option[direction]->to_room = to_room.value_or(world[room_id].dir_option[direction]->to_room);
		return std::nullopt;
	}
	bool create_direction(room_rnum room_id,byte direction,room_rnum to_room) {
		if(direction > NUM_OF_DIRS) {
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
			std::cerr << "[DIRECTION BIGGER THAN NUM_DIRS\n";
#endif
			return false;
		}
		world[room_id].set_dir_option(direction,
		    "general description",
		    "keyword",
		    EX_ISDOOR,
		    -1,
		    to_room);
		return true;
	}
	bool destroy_direction(room_rnum room_id,int direction) {
		std::size_t rid = room_id;
		if(rid > mods::globals::room_list.size()) {
			return false;
		}
		if(direction > NUM_OF_DIRS) {
			return false;
		}
		if(!world[room_id].dir_option[direction]) {
			return false;
		}
		free(world[room_id].dir_option[direction]);
		world[room_id].dir_option[direction] = nullptr;
		return true;
	}
};

using args_t = std::vector<std::string>;

void send_rbuild_help_screen(player_ptr_t& player) {
	player->pager_start() << "usage: \r\n" <<
	    " {grn}rbuild{/grn} {red}help{/red}\r\n" <<
	    "  |--> this help menu\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}help{/gld}\r\n" <<
	    "  |:: (this help menu will show up)\r\n" <<
	    " {grn}rbuild{/grn} {red}giveme:next_vnum{/red}\r\n" <<
	    "  |--> find an unused room vnum\r\n" <<
	    " {grn}rbuild{/grn} {red}vnum <vnum>{/red}\r\n" <<
	    "  |--> set the current room's vnum\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}vnum 410{/gld}\r\n" <<
	    "  |:: (set vnum of the current room to 410)\r\n" <<
	    " {grn}rbuild{/grn} {red}set-recall{/red} {red}<mortal|immortal>{/red}\r\n" <<
	    "  |--> set the current room as recall\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}set-recall mortal{/gld}\r\n" <<
	    "  |:: (set mortal recall to current room)\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}set-recall immortal{/gld}\r\n" <<
	    "  |:: (set immortal recall to current room)\r\n" <<
	    " {grn}rbuild{/grn} {red}<list> {yel}[page]{/red}\r\n" <<
	    "  |--> list " << std::to_string(mods::builder::RNUMLIST_MAX_PER_CALL) << " rooms per page. The page argument is zero-indexed\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}list{/gld}\r\n" <<
	    "  |:: (list the first " << std::to_string(mods::builder::RNUMLIST_MAX_PER_CALL) << " rooms)\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}list 2{/gld}\r\n" <<
	    "  |:: (list the third page)\r\n" <<
	    " {grn}rbuild{/grn} {red}<flag:add> {yel}<FLAG>...<FLAG N>{/red}\r\n" <<
	    "  |--> set flags on room.\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}flag:add NOMOB PEACEFUL{/gld}\r\n" <<
	    "  |:: (set the NOMOB and PEACEFUL flags on the room you are in)\r\n" <<
	    " {grn}rbuild{/grn} {red}<flag:list>{/red}\r\n" <<
	    "  |--> list flags on room.\r\n" <<
	    " {grn}rbuild{/grn} {red}<flag:remove> {yel}<FLAG>...<FLAG N>{/red}\r\n" <<
	    "  |--> set flags on room.\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}flag:remove NOMOB PEACEFUL{/gld}\r\n" <<
	    "  |:: (removes the NOMOB and PEACEFUL flags on the room you are in)\r\n" <<
	    "  {grn}|____[possible items]{/grn}\r\n" <<
	    "  {gld}|:: DARK -> Dark\r\n" <<
	    "  {gld}|:: DARK -> Dark\r\n" <<
	    "  {gld}|:: DEATH	-> Death trap\r\n" <<
	    "  {gld}|:: NOMOB	-> MOBs not allowed\r\n" <<
	    "  {gld}|:: INDOORS	-> Indoors\r\n" <<
	    "  {gld}|:: PEACEFUL -> Violence not allowed\r\n" <<
	    "  {gld}|:: DMZ	-> Violence not allowed\r\n" <<
	    "  {gld}|:: SOUNDPROOF -> Shouts, gossip blocked\r\n" <<
	    "  {gld}|:: NOTRACK	-> Track won't go through\r\n" <<
	    "  {gld}|:: NOMAGIC	-> Magic not allowed\r\n" <<
	    "  {gld}|:: TUNNEL -> room for only 1 pers\r\n" <<
	    "  {gld}|:: PRIVATE	-> Can't teleport in\r\n" <<
	    "  {gld}|:: GODROOM	-> LVL_GOD+ only allowed\r\n" <<
	    "  {gld}|:: HOUSE	-> (R) Room is a house\r\n" <<
	    "  {gld}|:: HOUSE_CRASH	-> (R) House needs saving\r\n" <<
	    "  {gld}|:: ATRIUM -> (R) The door to a house\r\n" <<
	    "  {gld}|:: OLC -> (R) Modifyable/!compress\r\n" <<
	    "  {gld}|:: BFS_MARK -> (R) breath-first srch mrk\r\n" <<
	    "{/gld}" <<
	    " {red}Room textures{/red}\r\n" <<
	    " {grn}rbuild{/grn} {red}<texture:add> <type1> ... <typeN>{/red}\r\n" <<
	    "  |--> Set the room's texture type. A room can have multiple texture types.\r\n" <<
	    "  |:: {yel}-:[{/yel}{grn}types{/grn}{yel}]:-{/yel}\r\n" <<
	    "  |:: \r\n" ;
	for(auto& pair : mods::rooms::texture_strings) {
		//"  {gld}|:: %s{/gld}\r\n",pair.second.c_str());
		player->sendln(
		    CAT(
		        "  {gld}|:: ",pair.second.c_str(),"{/gld}"
		    )
		);
	}
	*player <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}texture GRASS OUTSIDE{/gld}\r\n" <<
	    " {grn}rbuild{/grn} {red}<texture:remove> <type1> ... <typeN>{/red}\r\n" <<
	    "  |--> Delete the textures listed after the delete keyword.\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}texture:remove GRASS OUTSIDE{/gld}\r\n" <<
	    " {grn}rbuild{/grn} {red}<texture:clear>{/red}\r\n" <<
	    "  |--> Deletes all textures in the current room. WARNING: This does not prompt for confirmation!\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}texture:clear{/gld}\r\n" <<
	    " {grn}rbuild{/grn} {red}<set> <rnum> <number>{/red}\r\n" <<
	    "  |--> Set the real room number of the current room\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}set rnum 1204{/gld}\r\n" <<
	    "  |:: (next time you do 'rbuild room' it will display 1204)\r\n" <<
	    " {grn}rbuild{/grn} {red}<room>{/red}\r\n" <<
	    "  |--> get the real room number of the room\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}room{/gld}\r\n"<<
	    " {grn}rbuild{/grn} {red}<save>{/red}\r\n" <<
	    "  |--> save the current room you are standing in\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}save{/gld}\r\n"<<
	    " {grn}rbuild{/grn} {red}ed <delete> <N>{/red}\n" <<  /** TODO: needs impl */
	    "  |--> deletes the ed number N with this room\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}ed delete 3{/gld}\r\n" <<
	    " {grn}rbuild{/grn} {red}ed <list>{/red}\r\n" << 	/** TODO needs impl */
	    "  |--> lists the current ed structures currently associated with this room\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}ed list{/gld}\r\n" <<
	    " {grn}rbuild{/grn} {red}ed <save-all>{/red}\r\n" << /** TODO: needs impl */
	    "  |--> saves all ed entries\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    " {grn}rbuild{/grn} {red}ed <show> <N>{/red}\r\n" <<  /** TODO: needs impl */
	    "  |--> lists the current ed structures currently associated with this room\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    " {grn}rbuild{/grn} {red}ed <new>{/red}\r\n" <<  /** TODO: needs impl */
	    "  |--> creates an ed for this room\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}ed new{/gld}\r\n" <<
	    " {grn}rbuild{/grn} {red}ed <N> <keyword> <value>{/red}\r\n" <<  /** TODO: needs impl */
	    "  |--> sets the Nth keyword to <value>\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}ed 3 keyword {/gld}\"sign gold wall\"\r\n" <<
	    " {grn}rbuild{/grn} {red}ed <N> <description> <value>{/red}\r\n" <<  /** TODO: needs impl */
	    "  |--> sets the Nth description to <value>\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}ed 3 description {/gld}\"A very long passage about newbie rules, etc, etc, ...\"\r\n" <<
	    " {grn}rbuild{/grn} {red}<create> <direction>{/red}\r\n" <<
	    "  |--> creates a room to the direction you choose (neswud)\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}create north{/gld}\r\n" <<
	    "  |:: (the room to the north will be a brand new defaulted room)\r\n" <<
	    " {grn}rbuild{/grn} {red}<bind> <direction> <room_vnum>{/red}\r\n" <<
	    "  |--> bind a room to a direction\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n"<<
	    "  |:: {wht}rbuild{/wht} {gld}bind north 127{/gld}\r\n"<<
	    "  |:: (the room to the north will lead to the room with a vnum of 127)\r\n" <<
	    " {grn}rbuild{/grn} {red}<bind-mark> <direction> <bookmark-name>{/red}\r\n" <<
	    "  |--> bind a BOOKMARKED room to a direction\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n"<<
	    "  |:: {wht}rbuild{/wht} {gld}bind-mark north cofobcenter{/gld}\r\n"<<
	    "  |:: (the room to the north will lead to the room bookmarked with 'cofobcenter')\r\n" <<
	    " {grn}rbuild{/grn} {red}<title> <string>{/red}\r\n" <<
	    "  |--> set the current room title to string\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}title Taco Bell Employee Lounge{/gld}\r\n" <<
	    "  |:: (the room's title will be the above title)\r\n" <<
	    " {grn}rbuild{/grn} {red}<description> <string>{/red}\r\n" <<
	    "  |--> set the current room description to string\r\n" <<
	    "  {grn}|____[example]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}description The room is filled with boxes of taco bell...{/gld}\r\n" <<
	    "  |:: (the room's description will be the above description)\r\n" <<
	    " {grn}rbuild{/grn} {red}<destroy> <direction>{/red}\r\n" <<
	    "  |--> destroy a room direction\r\n" <<
	    "  {grn}|____[examples]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}destroy north{/gld}\r\n" <<
	    "  |:: (north will no longer be an exit)\r\n" <<
	    " {grn}rbuild{/grn} {red}<dopt> <direction> <item> <value>{/red}\r\n" <<
	    "  |--> set dir_option item to value\r\n" <<
	    "  {grn}|____[possible items]{/grn}\r\n" <<
	    "  |:: gen                 -> The general description of the room\r\n" <<
	    "  |:: keyword             -> The keyword of the room direction\r\n" <<
	    "  |:: key                 -> Integer key that is accepted for this exit\r\n" <<
	    "  |:: to_room             -> The room number that this exit leads to\r\n" <<
	    "  |:: to_vnum             -> The room vnum that this exit leads to\r\n"   <<
	    "  |:: to_mark             -> The room with the bookmark specified\r\n"    <<
	    "  {grn}|____[examples]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}dopt north gen To the north you see the Taco Bell bathroom.{/gld}\r\n" <<
	    "  |:: (When you do 'look north' you will see the above description)\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}dopt north keyword bathroom{/gld}\r\n" <<
	    "  |:: (when you do 'open bathroom' it will open the door to the north)\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}dopt north key 123{/gld}\r\n" <<
	    "  |:: (the north exit will require a key numbered 123)\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}dopt north to_room 27{/gld}\r\n" <<
	    "  |:: (the north room will lead to room number 27)\r\n" <<
	    " {grn}rbuild{/grn} {red}sector-type <sectortype>{/red}\r\n" <<
	    "  |--> add a flag to the specified door\r\n" <<
	    "  {grn}|____[possible items]{/grn}\r\n";
	for(auto& pair : mods::rooms::sector_strings) {
		*player << "  |:: " << pair.second << "\r\n";
	}
	*player <<
	    "  {grn}|____[examples]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}sector-type OUTSIDE_GRASS{/gld}\r\n" <<
	    "  |:: (Sets the current room's sector type to OUTSIDE_GRASS)\r\n" <<
	    " {grn}rbuild{/grn} {red}exit:add <direction> <flag>{/red}\r\n" <<
	    "  |--> add a flag to the specified door\r\n" <<
	    "  {grn}|____[possible items]{/grn}\r\n" <<
	    "  |:: BREACHABLE          -> can be destroyed be breach charges\r\n" <<
	    "  |:: CLOSED              -> default closed\r\n" <<
	    "  |:: ELECTRIFIED         -> door will shock user upon touch\r\n" <<
	    "  |:: HIDDEN              -> door isn't obvious\r\n" <<
	    "  |:: ISDOOR              -> default door type\r\n" <<
	    "  |:: LOCKED              -> locked. set key using dopt\r\n" <<
	    "  |:: PICKPROOF           -> resists lock picking\r\n" <<
	    "  |:: QUEST_LOCKED        -> door remains locked until question condition\r\n" <<
	    "  |:: REINFORCED          -> must be broken by thermite charges\r\n" <<
	    "  |:: GLASS               -> glass door\r\n" <<
	    "  |:: AUTOMATIC           -> door automatically opens when someone is around\r\n" <<
	    "  {grn}|____[examples]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}exit:add north REINFORCED PICPROOF CLOSED{/gld}\r\n" <<
	    "  |:: (Applies multiple values to northern door)\r\n" <<
	    " {grn}rbuild{/grn} {red}exit:list <direction>{/red}\r\n" <<
	    "  |--> lists the flags on the specified door\r\n" <<
	    " {grn}rbuild{/grn} {red}exit:remove <direction> <flags>{/red}\r\n" <<
	    "  |--> removes flags on the specified door\r\n" <<
	    "  {grn}|____[examples]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}exit:remove north REINFORCED PICPROOF CLOSED{/gld}\r\n" <<
	    "  |:: (removes flags from northern door)\r\n" <<
	    " {grn}rbuild{/grn} {red}pave <continue>{/red}\r\n" <<
	    "  |--> start paving in the current room and zone that you are currently standing in.\r\n" <<
	    "  |    This allows you to continue where you left off without having to create a \r\n" <<
	    "  |    brand new zone and set of rooms.\r\n" <<
	    " {grn}rbuild{/grn} {red}pave <on|off> <room_number_start> <zone_id>{/red}\r\n" <<
	    "  |--> starts the pave mode where any direction you go to will automatically \r\n" <<
	    "  |    create and bind rooms. Helpful for when you want to carve out a ton of\r\n" <<
	    "  |    different rooms and fill in the descriptions for them later\r\n" <<
	    "  |    You must supply the room_number_start argument as this will be the virtual room \r\n" <<
	    "  |    number that the paved rooms will start at.\r\n" <<
	    "  |    When you are done paving type 'rbuild pave off')\r\n"<<
	    "  {grn}|____[examples]{/grn}\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}pave on 100 5{/gld}\r\n" <<
	    "  |   (starts pave mode. The first room will have a virtual room number of 100, and the next \r\n" <<
	    "  |   subsequent rooms will be 101, 102, etc. until you type 'rbuild pave off'. Once in pave mode, \r\n" <<
	    "  |   walk to a bunch of different rooms and pave out a walkway. When you type 'rbuild pave off' it will \r\n" <<
	    "  |   give you a transaction id number. Remember this transaction ID number because that is how you refer to the set\r\n" <<
	    "  |   of paved rooms when you want to save. To save a set of paved rooms type 'rbuild save-paved <transaction_id_number>'\r\n" <<
	    "  |   where transaction_id_number is the transaction id returned when you typed 'rbuild pave off'. If you forget \r\n" <<
	    "  |   your transaction id number, you can type 'rbuild list-paved' and it will show you a list of paved walkways\r\n" <<
	    "  |   that you currently have)\r\n" <<
	    "  |:: {wht}rbuild{/wht} {gld}pave off{/gld}\r\n" <<
	    "  |--> (stops the pave mode)\r\n" <<
	    " {grn}rbuild{/grn} {red}save-paved <transaction_id_number>{/red}\r\n" <<
	    "  |--> saves all of the paved rooms that were created for the transaction id number specified.\r\n" <<
	    " {grn}rbuild{/grn} {red}clear-paved <transaction_id_number>{/red}\r\n" <<
	    "  |--> clears all of the paved rooms that were created\r\n" <<
	    " {grn}rbuild{/grn} {red}list-paved{/red}\r\n" <<
	    "  |--> lists all the currently paved transaction id numbers\r\n" <<
	    " {grn}rbuild{/grn} {red}pave-transaction-id{/red}\r\n" <<
	    "  |--> get current pave transaction id (if one exists).\r\n" <<
	    " {grn}rbuild{/grn} {red}nickname <nickname>{/red}\r\n" <<
	    "  |--> sets a permanent nickname on the room you currently reside in.\r\n"
	    "[documentation written on 2021-04-06]\r\n" <<
	    "\r\n";
	player->pager_end();
	player->page(0);
}

bool rbuild_handle_giveme_next_vnum(player_ptr_t& player,const std::vector<std::string>& vec_args) {
	if(vec_args.size() >= 1 && vec_args[0].compare("giveme:next_vnum") == 0) {
		auto vnum = next_room_vnum();
		player->sendln(CAT(vnum));
		player->set_scripted_response(
		mods::builder::encode_map({
			{"next_vnum",std::to_string(vnum)}
		}));
		return true;
	}
	return false;
}
SUPERCMD(do_rbuild) {
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0) {
		send_rbuild_help_screen(player);
		return;
	}
	if(rbuild_handle_giveme_next_vnum(player,vec_args)) {
		return;
	}
	auto args = mods::util::subcmd_args<9,args_t>(argument,"nickname");
	if(args.has_value()) {
		/**
		 * args [0] => nickname
		 * args [1] => value
		 */
		if(vec_args.size() < 2) {
			r_error(player,"Not enough arguments. Expecting 2");
			return;
		}
		auto room = player->room();
		world[room].nickname = vec_args[1];
		mods::rooms::register_nickname(room, vec_args[1]);
		r_success(player,"Set nickname for that room. you will need to save it now");
		return;
	}
	args = mods::util::subcmd_args<4,args_t>(argument,"vnum");
	if(args.has_value()) {
		auto room = player->room();
		std::cerr << green_str("STATUS: [rbuild_vnum]: ") << world[room].number <<"| real:" << room << "\n";
		if(vec_args.size() < 2) {
			ENCODE_INIT();
			ENCODE_STR(world[room].number);
			r_success(player,CAT("This room vnum: ",world[room].number));
			return;
		}
		auto str_vnum = vec_args[1];
		auto opt = mods::util::stoi(str_vnum);
		if(opt.value_or(-1) <= 0) {
			r_error(player,"Invalid vnum. Must be positive number and not zero.");
			return;
		}
		if(world.size() <= room) {
			r_error(player,"You are not in a room that I can reference. Perhaps you haven't saved your room.");
			return;
		}
		world[room].number = opt.value();
		r_success(player,"Set the virtual number of this room successfully.");
		return;
	}
	constexpr unsigned int max_char = 2048;
	std::array<char,max_char> command;
	std::array<char,max_char> direction;
	one_argument(one_argument(argument,&command[0],max_char),&direction[0],max_char);
	if(std::string(&command[0]).compare("create") == 0 && IS_DIRECTION(&direction[0])) {
		mods::builder::new_room(player,mods::globals::dir_int(direction[0]));
		r_error(player,"Room created");
		return;
	}
	args = mods::util::subcmd_args<11,args_t>(argument,"set-recall");
	if(args.has_value() && args.value().size() > 1 && args.value()[0].compare("set-recall") == 0) {
		auto arg_vec = args.value();
		if(arg_vec[1].compare("mortal") == 0) {
			auto set_status = mods::world_conf::set_mortal_start_room(world[player->room()].number);
			if(set_status.first == false) {
				r_error(player,std::string("An error occurred: '") + set_status.second + "'");
				return;
			}
			r_success(player,set_status.second);
			return;
		} else if(arg_vec[1].compare("immortal") == 0) {
			auto set_status = mods::world_conf::set_immortal_start_room(world[player->room()].number);
			if(set_status.first == false) {
				r_error(player,std::string("An error occurred: '") + set_status.second + "'");
				return;
			}
			r_success(player,set_status.second);
			return;
		} else {
			r_error(player,"Second argument wasn't recognized. Please use either 'mortal' or 'immortal'");
			return;
		}
	}
	/**
	 * rbuild list <page> [number-per-page]
	 */
	if(std::string(&command[0]).compare("list") == 0) {
		auto set = std::string(argument);
		set = set.substr(set.find("list ") + 5);
		auto number = mods::util::stoi(set);
		if(number.value_or(-1) == -1) {
			r_error(player,"Invalid page number");
			return;
		}
		player->pager_start();
		auto max_per_call = mods::builder::RNUMLIST_MAX_PER_CALL;
		jxcomp jx;
		jx.array_start("rooms");
		if(std::distance(world.begin(),world.end()) > max_per_call * number.value()) {
			auto it = world.begin() + max_per_call * number.value();
			auto end_range = world.end();
			if(it + max_per_call < world.end()) {
				end_range = it + max_per_call;
			}
			unsigned real_room_number = max_per_call * number.value();
			for(; it != end_range; ++it) {
				if(player->is_executing_js()) {
					jx.object_start("")
					.push("rnum",real_room_number)
					.push("vnum",it->number)
					.object_end();
				} else {
					*player << "{gld}[" << real_room_number << "]{/gld} :->{red} [" <<
					    it->number << "]\"" << it->name.c_str() << "\"{/red}";
				}
				real_room_number++;
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
		return;
	}
	if(std::string(&command[0]).compare("title") == 0) {
		auto title = std::string(argument);
		title = title.substr(title.find("title ") + 6);
		if(mods::builder::title(in_room(ch),title)) {
			r_success(player,"title changed");
		} else {
			r_error(player,"error");
		}
		return;
	}
	if(std::string(&command[0]).compare("description") == 0) {
		auto description = std::string(argument);
		description = description.substr(description.find("description ") + 12);
		if(mods::builder::description(in_room(ch),description + "\r\n")) {
			r_success(player,"description changed");
		} else {
			r_error(player,"error");
		}
		return;
	}
	//" {grn}rbuild{/grn} {red}ed <list>{/red}\r\n" << 	/** todo needs impl */
	//" {grn}rbuild{/grn} {red}ed <save-all>{/red}\r\n" << /** todo: needs impl */
	//" {grn}rbuild{/grn} {red}ed <show> <n>{/red}\r\n" <<  /** todo: needs impl */
	//" {grn}rbuild{/grn} {red}ed <n> <keyword> <value>{/red}\r\n" <<  /** todo: needs impl */
	//" {grn}rbuild{/grn} {red}ed <n> <description> <value>{/red}\r\n" <<  /** todo: needs impl */
	//
	/** how positional parameters are parsed: */
	/* on the command: rbuild ed foo bar 1
		 vec_args[0] == 'ed'
		 vec_args[1] == 'foo'
		 vec_args[2] == 'bar'
		 vec_args[3] == '1'
		 */
	/**
	 *  command line: rbuild ed [...]
	 */
	if(std::string(&command[0]).compare("ed") == 0) {
		/**
		 *  command line: rbuild ed new [n]
		 * ------------------------------------------
		 *  the n is for optionally how many new ones to create
		 */
		if(vec_args.size() >= 2 && vec_args[1].compare("new") == 0) {
			// rbuild ed <new>\r\n" <<  /** todo: needs impl */
			/** todo: add mutex lock so that other builders cant lock this room */
			auto size_before = world[in_room(ch)].ex_descriptions().size();
			world[in_room(ch)].ex_descriptions().emplace_back();
			auto size_after = world[in_room(ch)].ex_descriptions().size();
#ifdef __mentoc_show_mods_builder_debug_output__
			std::cerr << "before: " << size_before << " after: " << size_after << "\n";
#endif
			if(size_after > size_before) {
				r_success(player,"room ed saved");
				return;
			} else {
				r_error(player,std::string("error creating ed: "));
			}
			return;
		}
		/**
		 *  command line: rbuild ed list [n]
		 * --------------------------------------------------
		 *  the n is for which page to list. 25 to a page
		 */
		if(vec_args.size() >= 2 && vec_args[1].compare("list") == 0) {
			r_status(player,"listing...");
			unsigned ex_id = 0;
			player->pager_start();
			for(const auto& ex : world[in_room(ch)].ex_descriptions()) {
				*player << "{gld}[" << ex_id++ << "]{/gld} :->{red} [" <<
				    ex.keyword.c_str() <<  "]->'" <<
				    ex.description.c_str() << "'{/red}\r\n";
			}
			player->pager_end();
			player->page(0);
			r_status(player,"Done listing...");
			return;
		}
		/**
		 *  command line: rbuild ed <index> <keyword> <value>
		 * --------------------------------------------------
		 *  the n is for which page to list. 25 to a page
		 */
		if(argshave()->first_is("ed")->size_gt(3)->passed() && argshave()->nth_is_any(2, {"keyword","description"})->passed()) {
			if(argshave()->int_at(1)->failed()) {
				r_error(player,"You must specify a valid integer for the index");
				return;
			}
			auto value = args()->gather_strings_starting_at(3);
			if(argat(2).compare("keyword") == 0) {
				world[player->room()].ex_descriptions()[intat(1)].keyword.assign(value);
				r_success(player,"Set keyword");
			} else {
				world[player->room()].ex_descriptions()[intat(1)].description.assign(value);
				r_success(player,"Set description");
			}
			return;
		}
		/**
		 * rbuild ed <delete> <id>....[id-N]
		 */
		if(argshave()->first_is("ed")->size_gt(2)->passed() && argshave()->nth_is_any(1, {"delete"})->passed()) {
			auto remove_indexes = args()->gather_integers_starting_at(2);
			if(world[in_room(ch)].ex_descriptions().size() == 0) {
				r_error(player,std::string(
				        "nothing to delete"));
				return;
			}
			std::size_t before_size = world[in_room(ch)].ex_descriptions().size();
			auto keep = world[in_room(ch)].ex_descriptions();
			keep.clear();
			for(std::size_t i=0; i < world[in_room(ch)].ex_descriptions().size(); i++) {
				if(std::find(remove_indexes.cbegin(),remove_indexes.cend(),i) != remove_indexes.cend()) {
					continue;
				}
				keep.emplace_back(world[in_room(ch)].ex_descriptions()[i]);
			}
			world[in_room(ch)].ex_descriptions() = std::move(keep);
			std::size_t after_size = world[in_room(ch)].ex_descriptions().size();
			r_success(player,CAT("size before: ",before_size,", size after: ",after_size));
			return;
		}
	}/** all code that handles ed should be in above statement */
	if(std::string(&command[0]).compare("save") == 0) {
		std::string error;
		auto ret = mods::builder::save_to_db(in_room(ch),error);
		if(ret != 0) {
			r_error(player,std::string("error saving room: ") + std::to_string(ret) + "->" + error);
		} else {
			r_success(player,"room saved");
		}
		return;
	}
	if(std::string(&command[0]).compare("bind") == 0) {
		constexpr unsigned int max_char_item = 6;
		std::array<char,max_char_item> direction;
		std::array<char,max_char_item> room_id;
		std::fill(room_id.begin(),room_id.end(),0);
		std::fill(direction.begin(),direction.end(),0);
		one_argument(one_argument(one_argument(argument,&command[0],max_char),&direction[0],max_char_item),&room_id[0],max_char_item);
		/* command = bind, direction = neswud, room_id = int */
		if(!is_direction(&direction[0])) {
			r_error(player,"invalid direction");
			return;
		}
		auto room = mods::util::stoi(&room_id[0]);
		int r = room.value_or(-1);
		if(r < 0) {
			r_error(player,"invalid vnum. could not convert to a number.");
			return;
		}
		r = real_room((room_vnum)r);
		if(r == nowhere) {
			r_error(player, "could not find a room with that vnum");
			return;
		}
		if(mods::builder::create_direction(player->room(),mods::globals::dir_int(direction[0]),r)) {
			r_success(player,"direction created");
		} else {
			r_error(player,"error");
		}
		return;
	}
	/**
	 * args[0] bind-mark
	 * args[1] direction
	 * args[2] bookmark
	 */
	if(argshave()->first_is("bind-mark")->passed()) {
		if(argshave()->size_gt(2)->passed() == false) {
			r_error(player,"Not enough arguments. Expected atleast 3");
			return;
		}
		if(argshave()->direction_at(1)->passed() == false) {
			r_error(player,"Please specify valid direction");
			return;
		}
		std::string bookmark = argat(2);
		if(dirat(1).has_value() == false) {
			r_error(player,"Invalid or unrecognized direction given");
			return;
		}
		direction_t direction = dirat(1).value();
		if(player->builder_data->bookmarks.find(bookmark) == player->builder_data->bookmarks.end()) {
			r_error(player,"No bookmark by that name");
			return;
		}
		auto room_id = player->builder_data->bookmarks[bookmark];
		if(room_id < 0 || room_id >= world.size()) {
			r_error(player,"Room id doesn't exist!");
			return;
		}
		if(mods::builder::create_direction(player->room(),direction,room_id)) {
			r_success(player,"direction created");
		} else {
			r_error(player,"error");
		}
		return;
	}
	if(std::string(&command[0]).compare("destroy") == 0) {
		constexpr unsigned int max_char_item = 8;
		std::array<char,max_char_item> direction;
		std::array<char,max_char_item> item;
		one_argument(one_argument(argument,&command[0],max_char_item),&direction[0],max_char_item);
		auto description = std::string(argument);
		std::string str_item = &item[0];
		if(!is_direction(&direction[0])) {
			r_error(player,"invalid direction");
			return;
		}
		if(!mods::builder::destroy_direction(in_room(ch),mods::globals::dir_int(direction[0]))) {
			r_error(player,"unable to destroy direction");
		} else {
			r_success(player,"direction destroyed");
		}
		return;
	}
	args = mods::util::subcmd_args<4,args_t>(argument,"dopt");
	if(args.has_value()) {
		if(vec_args.size() < 4) {
			r_error(player,"Not enough arguments to dopt. Expecting 4.");
			return;
		}
		for(auto a : vec_args) {
			player->sendln(CAT("arg: '",a,"'"));
		}
		auto str_item = vec_args[2];
		std::string direction = vec_args[1];
		std::string description = vec_args[3];
		if(str_item.compare("gen") == 0) {
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),description,
			        std::nullopt,std::nullopt,std::nullopt,std::nullopt).value_or("success");
			if(ret.compare("success") == 0) {
				r_success(player,"General Description changed");
			} else {
				r_error(player,ret);
			}
			return;
		}
		if(str_item.compare("keyword") == 0) {
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
			        description,std::nullopt,std::nullopt,std::nullopt).value_or("success");
			if(ret.compare("success") == 0) {
				r_success(player,"Keyword changed");
			} else {
				r_error(player,ret);
			}
			return;
		}
		if(str_item.compare("key") == 0) {
			auto key = mods::util::stoi(description);
			if(key.value_or(-1) == -1) {
				r_error(player,"Invalid key");
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
			        std::nullopt,std::nullopt,key.value(),std::nullopt).value_or("success");
			if(ret.compare("success") == 0) {
				r_success(player,"key changed");
			} else {
				r_error(player,ret);
			}
			return;
		}
		if(str_item.compare("to_vnum") == 0) {
			auto to_room = mods::util::stoi(description);
			if(to_room.value_or(-1) == -1) {
				r_error(player,"Invalid room number");
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
			        std::nullopt,std::nullopt,std::nullopt,real_room(to_room.value())).value_or("success");
			if(ret.compare("success") == 0) {
				r_success(player,"to_room changed");
			} else {
				r_error(player,ret);
			}
			return;
		}
		if(str_item.compare("to_mark") == 0) {
			auto bookmark = description;
			if(player->builder_data->bookmarks.find(bookmark) == player->builder_data->bookmarks.end()) {
				r_error(player,"No bookmark by that name");
				return;
			}
			auto room_id = player->builder_data->bookmarks[bookmark];
			if(room_id < 0 || room_id >= world.size()) {
				r_error(player,"Room id doesn't exist!");
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
			        std::nullopt,std::nullopt,std::nullopt,room_id).value_or("success");
			if(ret.compare("success") == 0) {
				r_success(player,"to_room changed");
			} else {
				r_error(player,ret);
			}
			return;
		}
		if(str_item.compare("to_room") == 0) {
			auto to_room = mods::util::stoi(description);
			if(to_room.value_or(-1) == -1) {
				r_error(player,"Invalid room number");
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
			        std::nullopt,std::nullopt,std::nullopt,to_room.value()).value_or("success");
			if(ret.compare("success") == 0) {
				r_success(player,"to_room changed");
			} else {
				r_error(player,ret);
			}
			return;
		}
	}
	args = mods::util::subcmd_args<20,args_t>(argument,"pave-transaction-id");
	if(args.has_value()) {
		ENCODE_INIT();
		if(player->builder_data) {
			ENCODE_STR(player->builder_data->room_pavements.transact_id);
			r_success(player,tostr(player->builder_data->room_pavements.transact_id));
			return;
		}
		r_error(player,"it doesn't appear that you've been paving");
		return;
	}
	args = mods::util::subcmd_args<11,args_t>(argument,"list-paved");
	if(args.has_value()) {
		if(player->builder_data) {
			for(auto room: player->builder_data->room_pavements.rooms) {
				r_status(player,tostr(room));
			}
			r_status(player, "Transaction id: " + std::to_string(player->builder_data->room_pavements.transact_id));
		}
		return;
	}
	args = mods::util::subcmd_args<50,args_t>(argument,"save-paved");
	if(args.has_value()) {
		if(player->builder_data) {
			unsigned c = 0;
			std::string error;
			for(auto& room_id : player->builder_data->room_pavements.rooms) {
				auto ret = mods::builder::save_to_db(room_id,error);
				if(ret != 0) {
					r_error(player,std::string("Error saving room: ") + std::to_string(ret) + "->" + error);
				} else {
					++c;
					r_success(player,CAT("Room:", tostr(room_id), " saved"));
				}
			}
			r_success(player, CAT("Saved ",tostr(c), " rooms."));
		} else {
			r_error(player,"You currently don't have any builder data. Try starting the pave process.");
		}
		return;
	}
	/** textures */
	/** textures */
	/** textures */
	args = mods::util::subcmd_args<12,args_t>(argument,"texture:add");
	if(args.has_value()) {
		if(vec_args.size() < 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64); ++i) {
			auto opt_flag = mods::rooms::texture_from_string(vec_args[i]);
			if(opt_flag.has_value() == false) {
				r_error(player, vec_args[i] + ": unrecognized texture.");
				continue;
			}
			world[room].add_texture(opt_flag.value());
			r_status(player, vec_args[i] + " added.");
			++count;
		}
		if(count == 0) {
			r_error(player, "Didn't set any flags");
			return;
		}
		r_success(player, std::string("Set ") + std::to_string(count) + " textures on room.");
		return;
	}
	args = mods::util::subcmd_args<16,args_t>(argument,"texture:remove");
	if(args.has_value()) {
		if(vec_args.size() < 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64); ++i) {
			auto opt_flag = mods::rooms::texture_from_string(vec_args[i]);
			if(opt_flag.has_value() == false) {
				r_error(player, vec_args[i] + ": unrecognized texture.");
				continue;
			}
			world[room].remove_texture(opt_flag.value());
			r_status(player, vec_args[i] + " removed.");
			++count;
		}
		if(count == 0) {
			r_error(player, "Didn't remove any textures");
			return;
		}
		r_success(player, std::string("Set ") + std::to_string(count) + " textures on room.");
		return;
	}
	/** textures */
	/** textures */
	/** textures */
	/** sector types */
	/** sector types */
	/** sector types */
	args = mods::util::subcmd_args<12,args_t>(argument,"sector-type");
	if(args.has_value()) {
		if(vec_args.size() < 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto opt_sector = mods::rooms::sector_from_string(vec_args[1]);
		if(!opt_sector.has_value()) {
			r_error(player, "Unrecognized sector type");
			return;
		}
		auto room = player->room();
		mods::rooms::set_sector_type(room,opt_sector.value());
		r_success(player, "Set sector type succesfully.");
		return;
	}
	/** sector types */
	/** sector types */
	/** sector types */
	/** room flags */
	/** room flags */
	args = mods::util::subcmd_args<11,args_t>(argument,"flag:list");
	if(args.has_value()) {
		r_status(player, "Listing...");
		for(auto& str : mods::rooms::get_room_flags_from_room(player->room())) {
			player->sendln(str);
		}
		r_status(player, "Done Listing.");
		return;
	}
	args = mods::util::subcmd_args<10,args_t>(argument,"flag:add");
	if(args.has_value()) {
		if(vec_args.size() < 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64); ++i) {
			std::string real_flag = std::string("ROOM_") + vec_args[i];
			auto opt_flag = mods::rooms::room_flag_from_string(real_flag);
			if(opt_flag.has_value() == false) {
				r_error(player, "Unrecognized flag");
				continue;
			}
			mods::rooms::set_flag(room, opt_flag.value());
			r_status(player, vec_args[i] + " added.");
			++count;
		}
		if(count == 0) {
			r_error(player, "Didn't set any flags");
			return;
		}
		r_success(player, std::string("Set ") + std::to_string(count) + " flags on room. To list: rbuild flag:list");
		return;
	}
	args = mods::util::subcmd_args<12,args_t>(argument,"flag:remove");
	if(args.has_value()) {
		if(vec_args.size() < 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64); ++i) {
			std::string real_flag = std::string("ROOM_") + vec_args[i];
			auto opt_flag = mods::rooms::room_flag_from_string(real_flag);
			if(opt_flag.has_value() == false) {
				r_error(player, "Unrecognized flag");
				continue;
			}
			mods::rooms::remove_flag(room, opt_flag.value());
			r_status(player, vec_args[i] + " removed.");
			++count;
		}
		if(count == 0) {
			r_error(player, "Didn't remove any flags");
			return;
		}
		r_success(player, std::string("Removed ") + std::to_string(count) + " flag(s) on door");
		return;
	}
	/** room flags */
	/** room flags */
	args = mods::util::subcmd_args<9,args_t>(argument,"exit:add");
	if(args.has_value()) {
		if(vec_args.size() <= 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto direction = mods::globals::dir_int(vec_args[1][0]);
		if(direction == -1) {
			r_error(player, "Unrecognized direction");
			return;
		}
		if(!world[player->room()].dir_option[direction]) {
			r_error(player, "Door doesn't exist there");
			return;
		}
		for(unsigned i=2; i < vec_args.size(); i++) {
			auto f= mods::doors::from_string(vec_args[i]);
			if(f == -1) {
				r_error(player, vec_args[i] + " is an Unknown flag");
				continue;
			}
			world[player->room()].dir_option[direction]->exit_info |= mods::doors::from_string(vec_args[i]);
			r_status(player, vec_args[i] + " added.");
		}
		r_success(player, "Set flags on door");
		return;
	}
	args = mods::util::subcmd_args<10,args_t>(argument,"exit:list");
	if(args.has_value()) {
		if(vec_args.size() < 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto direction = mods::globals::dir_int(vec_args[1][0]);
		if(direction == -1) {
			r_error(player, "Unrecognized direction");
			return;
		}
		if(!world[player->room()].dir_option[direction]) {
			r_error(player, "Door doesn't exist there");
			return;
		}
		for(auto& str : mods::doors::all_string_flags(world[player->room()].dir_option[direction]->exit_info)) {
			r_status(player, str);
		}
		r_success(player, "Listed.");
		return;
	}
	args = mods::util::subcmd_args<11,args_t>(argument,"exit:remove");
	if(args.has_value()) {
		if(vec_args.size() < 2) {
			r_error(player,"Not enough args");
			return;
		}
		auto direction = mods::globals::dir_int(vec_args[1][0]);
		if(direction == -1) {
			r_error(player, "Unrecognized direction");
			return;
		}
		if(!world[player->room()].dir_option[direction]) {
			r_error(player, "Door doesn't exist there");
			return;
		}
		for(unsigned i = 2; i < vec_args.size(); i++) {
			auto f = mods::doors::from_string(vec_args[i]);
			if(f == -1) {
				r_error(player, vec_args[i] + " is an Unknown flag");
				continue;
			}
			world[player->room()].dir_option[direction]->exit_info ^= static_cast<mods::doors::exit_info_masks_t>(f);
			r_status(player, vec_args[i] + " removed.");
		}
		r_success(player, "Listed.");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"pave");
	if(args.has_value()) {
		auto arg_vec = args.value();
		//pave continue
		// 0    1
		if(arg_vec.size() == 2 && ICMP(arg_vec[1],"continue")) {
			ENCODE_INIT();
			auto status = mods::builder::pave_continue(player);
			if(std::get<0>(status) < 0) {
				r_error(player,std::get<1>(status));
				return;
			}
			r_success(player,std::get<1>(status));
			return;
		}
		//pave on|off <starting_room_number> <zone_id>
		// 0    1        2                     3
		if(arg_vec.size() < 2) {
			r_error(player,"Please specify on or off as the second argument");
			return;
		}
		if(arg_vec[1].compare("on") == 0) {
			if(arg_vec.size() < 4) {
				r_error(player,"Please specify a starting room number");
				return;
			}
			auto starting_room_number = mods::util::stoi(arg_vec[2]);
			auto zone_id = mods::util::stoi(arg_vec[3]);
			if(!zone_id.has_value()) {
				r_error(player,"Invalid zone id");
				return;
			}
			if(!starting_room_number.has_value()) {
				r_error(player,"Invalid starting room number");
				return;
			} else {
				player->builder_data->room_pave_mode = true;
				player->builder_data->room_pavements.start_room = starting_room_number.value();
				player->builder_data->room_pavements.zone_id = zone_id.value();
			}
			if(real_zone(zone_id.value()) == NOWHERE) {
				if(!std::get<0>(mods::builder::save_zone_to_db(
				            zone_id.value(),
				            ("zone_" + arg_vec[3]).data(),
				            starting_room_number.value(),
				            starting_room_number.value() + 100,
				            100,
				            2))) {
					r_error(player,"Couldn't find zone, nor could we create one automatically.");
					return;
				} else {
					/** TODO: transactional behaviour wanted here. (rollback) */
					r_success(player,"Zone automatically created");
				}
			}
			r_status(player,"Starting pave mode. You can start moving around now. To stop, type 'rbuild pave off'");
			return;
		}
		if(arg_vec[1].compare("off") == 0) {
			if(player->builder_data) {
				player->builder_data->room_pave_mode = false;
				r_status(player,"Stopped pave mode.");
				r_status(player,"Transaction ID: 0");
			}
			return;
		}
		return;
	}
}// end do_rbuild

