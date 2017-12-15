#include "builder.hpp"
#include "quests.hpp"
#include "pq.hpp"
#include "util.hpp"
#include <stdlib.h> //For itoa

namespace mods::builder{
	bool save_zone_to_db(std::string_view name,int room_start,int room_end,int lifespan,int reset_mode){
		try{
		/*
		CREATE TABLE zone (
			   id SERIAL,
			   zone_start INTEGER NOT NULL,
			   zone_end INTEGER NOT NULL,
			   zone_name VARCHAR(64) NOT NULL,
			   lifespan INTEGER NOT NULL,
			   reset_mode INTEGER NOT NULL
		   );
		*/
		auto txn = mods::pq::transaction(*mods::globals::pq_con);
		std::string sql = "INSERT INTO zone "
			"(zone_start,zone_end,zone_name,lifespan,reset_mode) "
			" VALUES(";
		sql += txn.quote(room_start);
		sql += ",";
		sql += txn.quote(room_end);
		sql += ",";
		sql += txn.quote(name.data());
		sql += ",";
		sql += txn.quote(lifespan);
		sql += ",";
		sql += txn.quote(reset_mode);
		sql += ")";

		mods::pq::exec(txn,sql);
		mods::pq::commit(txn);
		}catch(const std::exception &e){
			return false;
		}
		return true;
	}
	int save_to_db(room_rnum in_room){
		auto world_top = mods::globals::room_list.size();
		if(in_room > world_top){
			return -1;
		}
		if(!world[in_room].name){
			return -2;
		}
		if(!world[in_room].description){
			return -3;
		}
		try{
			std::string sql;
/*
                    Table "public.room"
     Column     |          Type          | Collation | Nullable | Default
----------------+------------------------+-----------+----------+---------
 room_number    | integer                |           | not null |
 zone           | integer                |           | not null |
 sector_type    | integer                |           | not null |
 name           | character varying(256) |           | not null |
 description    | text                   |           | not null |
 ex_keyword     | character varying(256) |           |          |
 ex_description | text                   |           |          |
 light          | integer                |           |          |
 room_flag      | integer                |           | not null |
*/
			auto check_txn = mods::pq::transaction(*mods::globals::pq_con);
			std::string check_sql = std::string("SELECT COUNT(room_number) from room where room_number=") + check_txn.quote(world[in_room].number);
			auto check_result = mods::pq::exec(check_txn,check_sql);
			int check_i = mods::pq::as_int(check_result,0,0);
			mods::pq::commit(check_txn);
			if(check_i == 1){
				/* update the record */
				auto txn = mods::pq::transaction(*mods::globals::pq_con);
				sql = "UPDATE room set zone = ";
				sql += txn.quote(world[in_room].zone);
				sql += ", sector_type=";
				sql += txn.quote(world[in_room].sector_type);
				sql += ", name=";
				sql += txn.quote(world[in_room].name);
				sql += ", description=";
				sql += txn.quote(world[in_room].description);
				sql += ", ex_keyword=";
				if(world[in_room].ex_description && world[in_room].ex_description->keyword){
					sql += txn.quote(world[in_room].ex_description->keyword);
				}else{
					sql += txn.quote("");
				}
				sql += ", ex_description=";
				if(world[in_room].ex_description && world[in_room].ex_description->description){
					sql += txn.quote(world[in_room].ex_description->description);
				}else{
					sql += txn.quote("");
				}
				sql += ", light=";
				std::array<char,16> num;
				std::fill(num.begin(),num.end(),0);
				sprintf(&num[0],"%d",world[in_room].light);
				sql += txn.quote(std::string(&num[0]));
				sql += ", room_flag=";
				sql += txn.quote(world[in_room].room_flags);
				sql += " WHERE room_number=";
				sql += txn.quote(world[in_room].number);
				mods::pq::exec(txn,sql);
				mods::pq::commit(txn);
			}else{
				auto txn = mods::pq::transaction(*mods::globals::pq_con);
				sql = "INSERT INTO room (room_number,zone,sector_type,name,description,ex_keyword,ex_description,light,room_flag) ";
				sql += "VALUES(";
				sql += txn.quote(world[in_room].number);
				sql += ",";
				sql += txn.quote(world[in_room].zone);
				sql += ",";
				sql += txn.quote(world[in_room].sector_type);
				sql += ",";
				sql += txn.quote(world[in_room].name);
				sql += ",";
				sql += txn.quote(world[in_room].description);
				sql += ",";
				if(world[in_room].ex_description){
					if(world[in_room].ex_description->keyword){
						sql += txn.quote(world[in_room].ex_description->keyword);
					}else{
						sql += "''";
					}
					sql += ",";
					if(world[in_room].ex_description->description){
						sql += txn.quote(world[in_room].ex_description->description);
					}else{
						sql += "''";
					}
				}else{
					sql += "'',''";
				}
				sql += ",";
				std::array<char,16> num;
				std::fill(num.begin(),num.end(),0);
				sprintf(&num[0],"%d",world[in_room].light);
				sql += txn.quote(std::string(&num[0]));
				sql += ",";
				std::fill(num.begin(),num.end(),0);
				sprintf(&num[0],"%d",world[in_room].room_flags);
				sql += txn.quote(std::string(&num[0]));
				sql += ");";
				std::cerr << "before commit 1\n";
				mods::pq::exec(txn,sql);
				mods::pq::commit(txn);
			}
			for(auto direction = 0; direction < NUM_OF_DIRS;direction++){
				if(world[in_room].dir_option[direction] &&
				   world[in_room].dir_option[direction]->general_description != nullptr){
					auto check_txn = mods::pq::transaction(*mods::globals::pq_con);
					std::string check_sql = "SELECT COUNT(room_number) FROM room_direction_data ";
					check_sql += " WHERE room_number=";
					check_sql += check_txn.quote(world[in_room].number);
					check_sql += " AND exit_direction=";
					check_sql += check_txn.quote(direction);
					auto check_result = mods::pq::exec(check_txn,check_sql);
					int check_i = mods::pq::as_int(check_result,0,0);
					mods::pq::commit(check_txn);
					if(check_i > 0){
						/* update the row instead of inserting it */
						auto up_txn = mods::pq::transaction(*mods::globals::pq_con);
						std::string up_sql = "UPDATE room_direction_data SET general_description = ";
						up_sql += up_txn.quote(world[in_room].dir_option[direction]->general_description);
						up_sql += ", keyword=";
						up_sql += up_txn.quote(world[in_room].dir_option[direction]->keyword);
						up_sql += ", exit_info=";
						up_sql += up_txn.quote(world[in_room].dir_option[direction]->exit_info);
						up_sql += ", exit_key=";
						up_sql += up_txn.quote(world[in_room].dir_option[direction]->key);
						up_sql += ", to_room=";
						auto real_room_num = world[world[in_room].dir_option[direction]->to_room].number;
						up_sql += up_txn.quote(real_room_num);
						up_sql += " WHERE exit_direction=";
						up_sql += up_txn.quote(direction);
						up_sql += " AND room_number=";
						up_sql += up_txn.quote(world[in_room].number);
						mods::pq::exec(up_txn,up_sql);
						mods::pq::commit(up_txn);
					}else{
						auto txn2 = mods::pq::transaction(*mods::globals::pq_con);
						sql = "INSERT INTO room_direction_data (room_number,exit_direction,";
						sql += " general_description,keyword,exit_info,exit_key,to_room) ";
						sql += "VALUES(";
						sql += txn2.quote(world[in_room].number);
						sql += ",";
						sql += txn2.quote(direction);
						sql += ",";
						sql += txn2.quote(world[in_room].dir_option[direction]->general_description);
						sql += ",";
						sql += txn2.quote(world[in_room].dir_option[direction]->keyword);
						sql += ",";
						sql += txn2.quote(world[in_room].dir_option[direction]->exit_info);
						sql += ",";
						sql += txn2.quote(world[in_room].dir_option[direction]->key);
						sql += ",";
						auto real_room_num = world[world[in_room].dir_option[direction]->to_room].number;
						sql += txn2.quote(real_room_num);
						sql += ");";
						std::cerr << "commit " << direction << " on 2\n";
						mods::pq::exec(txn2,sql);
						mods::pq::commit(txn2);
					}
				}
			}
		}catch(const std::exception &e){
			std::cerr << "error saving room #" << world[in_room].number << " (world[" << in_room << "]: " << e.what() << "\n";
			return -1;
		}

        return 0;
	}
	int import_room(struct room_data* room){
		auto world_top = mods::globals::room_list.size();
		auto old_world = world;
		world.push_back(*room);
        world[world_top].number = room->number;
        world[world_top].zone = room->zone;
        world[world_top].sector_type = room->sector_type;
        world[world_top].name = room->name;
        world[world_top].description = room->description;
        world[world_top].ex_description = room->ex_description;
        for(unsigned i =0; i < NUM_OF_DIRS;i++){
            world[world_top].dir_option[i] = room->dir_option[i];
        }
        world[world_top].room_flags = room->room_flags;
       	world[world_top].light = room->light;
        world[world_top].func = room->func;
        world[world_top].contents = room->contents;
        world[world_top].people = room->people;
		mods::globals::register_room(world_top);
		return 0;
	}
	struct room_data new_room(struct char_data* ch,int direction){
		auto in_room = IN_ROOM(ch);
        struct room_data room;
        room.number = world.size();
        room.zone = world[IN_ROOM(ch)].zone;
        room.sector_type = world[IN_ROOM(ch)].sector_type;
        room.name = strdup("title");
        room.description = strdup("description");
        room.ex_description = (struct extra_descr_data*) calloc(1,sizeof(struct extra_descr_data));
        for(unsigned i =0; i < NUM_OF_DIRS;i++){
            room.dir_option[i] = nullptr;
        }
		world[IN_ROOM(ch)].dir_option[direction] = (struct room_direction_data*) calloc(1,sizeof(room_direction_data));
		world[IN_ROOM(ch)].dir_option[direction]->general_description = strdup("general description");
		world[IN_ROOM(ch)].dir_option[direction]->keyword = strdup("keyword");
		world[IN_ROOM(ch)].dir_option[direction]->exit_info = EX_ISDOOR;
		world[IN_ROOM(ch)].dir_option[direction]->key = -1;
		world[IN_ROOM(ch)].dir_option[direction]->to_room = world.size();
        room.room_flags = 0;
       	room.light = 0;
        room.func = nullptr;
        room.contents = nullptr;
        room.people = nullptr;
		world.push_back(room);
		mods::globals::register_room(world.size());
		//char_from_room(ch);
		//char_to_room(ch,world_top);

        return room;
    }
	bool title(room_rnum room_id,std::string_view str_title){
		if(room_id < mods::globals::room_list.size()){
			free(world[room_id].name);
			world[room_id].name = strdup(str_title.data());
			return true;
		}
		return false;
	}
	bool description(room_rnum room_id,std::string_view str_description){
		if(room_id < mods::globals::room_list.size()){
			free(world[room_id].description);
			world[room_id].description = strdup(str_description.data());
			return true;
		}
		return false;
	}
	bool flush_to_db(struct char_data *ch,room_vnum room){
		MENTOC_PREAMBLE();
		*player << "flush_to_db[stub]\r\n";
		return true;
	}
	std::optional<std::string> dir_option(room_rnum room_id,int direction,std::optional<std::string_view> description,
            std::optional<std::string_view> keywords,
            std::optional<int> exit_info,
            std::optional<int> key,
            std::optional<room_rnum> to_room
    ){
		if(room_id > mods::globals::room_list.size()){
			return "Invalid room number";
		}
		if(direction > NUM_OF_DIRS){
			return "direction number is incorrect";
		}
		if(description.value_or("-1").compare("-1") != 0){
			free(world[room_id].dir_option[direction]->general_description);
			world[room_id].dir_option[direction]->general_description = strdup(description.value().data());
		}
		if(keywords.value_or("-1").compare("-1") != 0){
			free(world[room_id].dir_option[direction]->keyword);
			world[room_id].dir_option[direction]->keyword = strdup(keywords.value().data());
		}
		world[room_id].dir_option[direction]->exit_info = exit_info.value_or(world[room_id].dir_option[direction]->exit_info);
		world[room_id].dir_option[direction]->key = key.value_or(world[room_id].dir_option[direction]->key);
		world[room_id].dir_option[direction]->to_room = to_room.value_or(world[room_id].dir_option[direction]->to_room);
		return std::nullopt;
	}	
	bool create_direction(room_rnum room_id,int direction,room_rnum to_room){
		if(direction > NUM_OF_DIRS){
			return false;
		}
		if(world[room_id].dir_option[direction]){
			free(world[room_id].dir_option[direction]);
		}
		world[room_id].dir_option[direction] = (struct room_direction_data*) calloc(1,sizeof(room_direction_data));
		world[room_id].dir_option[direction]->general_description = strdup("general description");
		world[room_id].dir_option[direction]->keyword = strdup("keyword");
		world[room_id].dir_option[direction]->exit_info = EX_ISDOOR;
		world[room_id].dir_option[direction]->key = -1;
		world[room_id].dir_option[direction]->to_room = to_room;
		return true;
	}
	bool destroy_direction(room_rnum room_id,int direction){
		if(room_id > mods::globals::room_list.size()){
			return false;
		}
		if(direction > NUM_OF_DIRS){
			return false;
		}
		if(!world[room_id].dir_option[direction]){
			return false;
		}
		free(world[room_id].dir_option[direction]);
		world[room_id].dir_option[direction] = nullptr;
		return true;
	}
	bool delete_zone(int id){
		auto txn = mods::pq::transaction(*mods::globals::pq_con);
		std::string delete_sql = std::string("DELETE FROM zone where id=") + txn.quote(id);
		mods::pq::exec(txn,delete_sql);
		mods::pq::commit(txn);
		return true;
	}
	void zone_place(int zone_id,std::string_view zone_command,std::string_view if_flag,std::string_view arg1,std::string_view arg2,std::string_view arg3){
		auto txn = mods::pq::transaction(*mods::globals::pq_con);
		std::string sql = "INSERT INTO zone_data "
			"(zone_id,zone_command,zone_if_flag,zone_arg1,zone_arg2,zone_arg3)"
			" VALUES(";
		sql += txn.quote(zone_id);
		sql += ",";
		sql += txn.quote(zone_command.data());
		sql += ",";
		sql += txn.quote(if_flag.data());
		sql += ",";
		sql += txn.quote(arg1.data());
		sql += ",";
		sql += txn.quote(arg2.data());
		sql += ",";
		sql += txn.quote(arg3.data());
		sql += ")";
		mods::pq::exec(txn,sql);
		mods::pq::commit(txn);
	}
};

using args_t = std::vector<std::string>;
ACMD(do_obuild){
	MENTOC_PREAMBLE();
    if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0){
        player->pager_start();
		*player << "usage: \r\n" << 
				   " obuild help\r\n" <<
				   "  |--> this help menu\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: obuild help\r\n" <<
				   "  |:: (this help menu will show up)\r\n" <<
				   " obuild new\r\n" <<
				   " obuild list\r\n" <<
				   " obuild attr <object_id> <attr> <value>\r\n" <<
				   "  |:: -:[attributes]:-\r\n" <<
				   "  |:: item_number\r\n" << 
				   "  |:: name\r\n" << 
				   "  |:: desc\r\n" << 
				   "  |:: short_desc\r\n" << 
				   "  |:: action_desc\r\n" << 
				   "  |:: worn_on\r\n" << 
				   "  |:: weapon_type\r\n" << 
				   "  |:: weapon_ammo\r\n" << 
				   "  |:: weapon_ammo_max\r\n" << 
				   "  |:: weapon_holds_ammo\r\n" << 
				   "  |:: extra_keyword <keyword>\r\n" <<
				   "  |:: extra_description <description>\r\n" <<
				   "  |:: flags\r\n" << 
				   "  |:: affected <location> <modifier>\r\n" <<
				   " obuild list like <pattern>\r\n" <<
				   " obuild flag <object_id> <attr> <value>\r\n"<<
				   "  |:: -:[attributes]:-\r\n" <<
				   "  |:: value_0\r\n" <<
				   "  |:: value_1\r\n" <<
				   "  |:: value_2\r\n" <<
				   "  |:: value_3\r\n" <<
				   "  |:: type_flag\r\n" <<
				   "  |:: wear_flags\r\n" <<
				   "  |:: extra_flags\r\n" <<
				   "  |:: weight\r\n" <<
				   "  |:: cost\r\n" <<
				   "  |:: cost_per_day\r\n" <<
				   "  |:: timer\r\n" <<
				   "  |:: bitvector\r\n" <<
				   " obuild save <object_id>\r\n" <<
				   "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	auto args = mods::util::subcmd_args<3,args_t>(argument,"new");
	if(args.has_value()){
		*player << "{red}Creating new object{/red}\r\n";
		obj_data * obj = (obj_data*)calloc(sizeof(obj_data),1);
		memset(static_cast<void*>(obj),0,sizeof(obj_data));
		
		return;
	}
	args = mods::util::subcmd_args<4,args_t>(argument,"list");
	if(args.has_value()){
		*player << "{red}listing...{/red}\r\n";
		return;
	}
	args = mods::util::subcmd_args<4,args_t>(argument,"attr");
	if(args.has_value()){
		*player << "{red}listing...{/red}\r\n";
		auto arg_vec = args.value();
		auto get_intval = [&](std::string_view str) -> std::optional<int>{
			if(arg_vec[1].compare(str.data()) == 0){
				auto i_value = mods::util::stoi(arg_vec[2]);
				if(!i_value.has_value()){
					*player << "{red}Please use a valid numeric value.{/red}\r\n";
					return std::nullopt_t;
				}
				return i_value.value();
			}
		};
		auto get_strval = [&](std::string_view str) -> std::optional<char*>{
			if(arg_vec[1].compare(str.data()) == 0){
				return strdup(arg_vec[2].c_str());
			}
			return std::nullopt_t;
		};

#define MENTOC_OBI(i) {obj->##i = get_intval("##i").value_or(obj->##i);
#define MENTOC_OBI2(i,a) {obj->##i = get_intval("##a").value_or(obj->##i);
#define MENTOC_OBS(i) {obj->##i = get_strval("##i").value_or(obj->##i);
#define MENTOC_OBS2(i,a) {obj->##i = get_intval("##a").value_or(obj->##i);
		MENTOC_OBI(item_number);
		MENTOC_OBI(weapon_type);
		MENTOC_OBI(worn_on);
		MENTOC_OBI2(ammo,weapon_ammo);
		MENTOC_OBI2(ammo_max,weapon_ammo_max);
		MENTOC_OBI2(holds_ammo,weapon_holds_ammo);
		MENTOC_OBI2(ammo,weapon_ammo);
		MENTOC_OBS(name);
		MENTOC_OBS(description);
		MENTOC_OBS(short_description);
		MENTOC_OBS(action_description);
	}
	return;
}

ACMD(do_rbuildzone){
	MENTOC_PREAMBLE();	
    if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0){
        player->pager_start();
		*player << "usage: \r\n" << 
				   " zbuild help\r\n" <<
				   "  |--> this help menu\r\n" << 
				   "  |____[example]\r\n" <<
				   "  |:: zbuild help\r\n" <<
				   "  |:: (this help menu will show up)\r\n" <<
				   " zbuild new <zone start> <zone end> <zone name> <zone lifespan> <zone reset mode>\r\n" <<
				   "  |--> Creates a new zone and maps the parameters to each field in the database.\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: zbuild new 1200 1299 \"The never ending frost\" 90 2\r\n" <<
				   "  |:: (creates a new zone which starts at rnum 1200 and ends on 1209\r\n" <<
				   "  |:: \"The never ending frost\" will be the name of the zone. Quotes must be \r\n" <<
				   "  |:: used here. 90 is the lifespan and 2 is the most common reset \r\n" <<
				   "  |:: mode so leave it at that for now.)\r\n" <<
				   "\r\n" << 
				   " zbuild list\r\n" <<
				   "  |--> lists the current zones saved to the db\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: zbuild list\r\n"<<
				   "\r\n" << 
				   " zbuild delete <id>...<N>\r\n" <<
				   "  |--> deletes the zone from the db with the id <id>. Multiple IDs can be specified\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: zbuild delete 1\r\n" <<
				   "\r\n" << 
				   " zbuild place <zone_id> <command> <if_flag> <arg1> <arg2> <arg3>\r\n" <<
				   "  |--> creates a reset command for the zone 'zone_id'.\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: zbuild reset 5 M 0 1500 500 300\r\n" <<
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
				   "  |:: For more information, see http://www.circlemud.org/cdp/building/building-6.html#ss6.1\r\n" <<
				   "  |:: )\r\n" <<
				   "\r\n";
		player->pager_end();
		player->page(0);
        return;
    }
	constexpr unsigned int max_char = 10;
    std::array<char,max_char> command;
	std::fill(command.begin(),command.end(),0);		
	one_argument(argument,&command[0],max_char);
	if(std::string(&command[0]).compare("place") == 0){
		std::string arg = argument;
		auto past = arg.substr(arg.find("place ")+6);
		auto args = mods::util::arglist<std::vector<std::string>>(past);
		auto zone_id = mods::util::stoi(args[0]);
		if(!zone_id.has_value()){
			*player << "{red} Invalid zone id{/red}\r\n";
			return;
		}
		auto zone_command = args[1];
		auto if_flag = args[2];
		auto arg1 = args[3];
		auto arg2 = args[4];
		auto arg3 = args[5];
		mods::builder::zone_place(zone_id.value(),zone_command,if_flag,arg1,arg2,arg3);
		*player << "{red}Placed object in zone{/red}\r\n";
		return;
	}
	if(std::string(&command[0]).compare("delete") == 0){
		std::string arg = argument;
		auto past = arg.substr(arg.find("delete ")+7);
		for(auto id : mods::util::arglist<std::vector<std::string>>(past)){
			auto zone = mods::util::stoi(id);
			if(zone.value_or(-1) == -1){
				*player << "{red}Unable to delete id: " << id << "{/red}\r\n";
				return;
			}
			mods::builder::delete_zone(zone.value());
			*player << "{red}Deleted zone: " << id << "{/red}\r\n";
		}
		return;
	}
	if(std::string(&command[0]).compare("new") == 0){
		std::string arg = argument;
		auto past = arg.substr(arg.find("new ")+4);
		//TODO: take this logic and store it in interpreter.cpp so we can reuse it
		auto arglist = mods::util::arglist<std::vector<std::string>>(past);
		if(arglist.size() != 5){
			*player << "{red}Argument list is invalid, please use the correct number of arguments{/red}\r\n";
			*player << "{red}Argument list is currently: {gld}" << arglist.size() << "{/gld}{/red}\r\n";
			for(auto vstr: arglist){
				*player << vstr << "\r\n";
			}
		}else{
			// " rbuildzone <new> <zone start> <zone end> <zone name> <zone lifespan> <zone reset mode>\r\n" <<
			auto arg_0 = mods::util::stoi(arglist[0]);
			if(arg_0.value_or(-1) == -1){
				*player << "{red}Invalid parameter 1{/red}\r\n";
				return;
			}
			auto arg_1 = mods::util::stoi(arglist[1]);
			if(arg_1.value_or(-1) == -1){
				*player << "{red}Invalid parameter 2{/red}\r\n";
				return;
			}
			auto arg_3 = mods::util::stoi(arglist[3]);
			if(arg_3.value_or(-1) == -1){
				*player << "{red}Invalid parameter 4{/red}\r\n";
				return;
			}
			auto arg_4 = mods::util::stoi(arglist[4]);
			if(arg_4.value_or(-1) == -1){
				*player << "{red}Invalid parameter 5{/red}\r\n";
				return;
			}
			if(!mods::builder::save_zone_to_db(arglist[2],arg_0.value(),arg_1.value(),arg_3.value(),arg_4.value())){
				*player << "{red}Unable to save new zone{/red}\r\n";
			}else{
				*player << "{red}New zone created{/red}\r\n";
			}
		}
		return;
	}
	if(std::string(&command[0]).compare("list") == 0){
		*player << "listing...\r\n";
		auto txn = mods::pq::transaction(*mods::globals::pq_con);
		std::string check_sql = "SELECT id,zone_start,zone_end,zone_name,lifespan,reset_mode FROM zone";
		auto check_result = mods::pq::exec(txn,check_sql);
		mods::pq::commit(txn);

		player->pager_start();
		for(auto row : check_result){
			std::string acc = "{red}";
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
		} 
		player->pager_end();
		player->page(0);
		return;
	}
};

ACMD(do_rbuild){
	MENTOC_PREAMBLE();	
    if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0){
        player->pager_start() << "usage: \r\n" << 
				   " rbuild help\r\n" <<
				   "  |--> this help menu\r\n" << 
				   "  |____[example]\r\n" <<
				   "  |:: rbuild help\r\n" <<
				   "  |:: (this help menu will show up)\r\n" <<
				   " rbuild <set> <rnum> <number>\r\n" <<
				   "  |--> Set the real room number of the current room\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: rbuild set rnum 1204\r\n" <<
				   "  |:: (next time you do 'rbuild room' it will display 1204)\r\n" <<
				   "\r\n" << 
				   " rbuild <room>\r\n" <<
				   "  |--> get the real room number of the room\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: rbuild room\r\n"<<
				   "\r\n" << 
				   " rbuild <save>\r\n" <<
				   "  |--> save the current room you are standing in\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: rbuild save\r\n"<<
				   "\r\n" << 
				   " rbuild <create> <direction>\r\n" <<
		           "  |--> creates a room to the direction you choose (neswud)\r\n" << 
				   "  |____[example]\r\n" <<
				   "  |:: rbuild create north\r\n" <<
				   "  |:: (the room to the north will be a brand new defaulted room)\r\n" <<
				   "\r\n" << 
				   " rbuild <bind> <direction> <room_rnum>\r\n" << 
				   "  |--> bind a room to a direction\r\n" <<
				   "  |____[example]\r\n"<<
				   "  |:: rbuild bind north 27\r\n"<<
				   "  |:: (the room to the north will lead to room 27)\r\n" <<
				   "\r\n" << 
			       " rbuild <title> <string>\r\n" <<
				   "  |--> set the current room title to string\r\n" << 
				   "  |____[example]\r\n" <<
				   "  |:: rbuild title Taco Bell Employee Lounge\r\n" <<
				   "  |:: (the room's title will be the above title)\r\n" <<
				   "\r\n" <<
				   " rbuild <description> <string>\r\n" <<
				   "  |--> set the current room description to string\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: rbuild description The room is filled with boxes of taco bell...\r\n" <<
				   "  |:: (the room's description will be the above description)\r\n" <<
				   "\r\n" <<
				   " rbuild <destroy> <direction>\r\n" <<
				   "  |--> destroy a room direction\r\n" <<
				   "  |____[examples]\r\n" <<
				   "  |:: rbuild destroy north\r\n" <<
				   "  |:: (north will no longer be an exit)\r\n" <<
				   "\r\n" <<
				   " rbuild <dopt> <direction> <item> <value>\r\n" <<
				   "  |--> set dir_option item to value\r\n" <<
				   "  |____[possible items]\r\n" <<
				   "  |:: gen                 -> The general description of the room\r\n" <<
				   "  |:: keyword             -> The keyword of the room direction\r\n" <<
				   "  |:: einfo               -> Currently only accepts ISDOOR\r\n" << //TODO Accept more than just ISDOOR
				   "  |:: key                 -> Integer key that is accepted for this exit\r\n" <<
				   "  |:: to_room             -> The room number that this exit leads to\r\n" <<
				   "  |____[examples]\r\n" <<
				   "  |:: rbuild dopt north gen To the north you see the Taco Bell bathroom.\r\n" <<
				   "  |:: (When you do 'look north' you will see the above description)\r\n" <<
				   "  |:: rbuild dopt north keyword bathroom\r\n" <<
				   "  |:: (when you do 'open bathroom' it will open the door to the north)\r\n" <<
				   "  |:: rbuild dopt north einfo ISDOOR\r\n" <<
				   "  |:: (the north exit will be a door)\r\n" <<
				   "  |:: rbuild dopt north key 123\r\n" << 
				   "  |:: (the north exit will require a key numbered 123)\r\n" <<
				   "  |:: rbuild dopt north to_room 27\r\n" << 
				   "  |:: (the north room will lead to room number 27)\r\n" <<
				   "\r\n" << 
				   "[documentation written on 2017-11-22]\r\n" <<
				   "\r\n";
		player->pager_end();
		player->page(0);
        return;
    }
	constexpr unsigned int max_char = 11;
    std::array<char,max_char> command;
    std::array<char,max_char> direction;
	one_argument(one_argument(argument,&command[0],max_char),&direction[0],max_char);
	if(std::string(&command[0]).compare("create") == 0 && IS_DIRECTION(&direction[0])){
		mods::builder::new_room(ch,mods::globals::dir_int(direction[0]));
		*player << "{red}Room created{/red}\r\n";
		return;
	}
	if(std::string(&command[0]).compare("room") == 0){
		*player << world[IN_ROOM(ch)].number << "\r\n";
		return;
	}
	if(std::string(&command[0]).compare("set") == 0){
		auto set = std::string(argument);
		set = set.substr(set.find("rnum ") + 5);
		auto number = mods::util::stoi(set);
		if(number.value_or(-1) == -1){
			*player << "{red}Invalid number{/red}\r\n";
			return;
		}
		world[IN_ROOM(ch)].number = number.value();
		*player << "{red}real room number set to " << number.value() << "{/red}\r\n";
		return;
	}
	if(std::string(&command[0]).compare("title") == 0){
		auto title = std::string(argument);
		title = title.substr(title.find("title ") + 6);
		if(mods::builder::title(IN_ROOM(ch),title)){
			*player << "{red}Title changed to:{/red} " << title << "\r\n";
		}else{
			*player << "{red}Error{/red}\r\n";
		}
		return;
	}
	if(std::string(&command[0]).compare("description") == 0){
		auto description = std::string(argument);
		description = description.substr(description.find("description ") + 12);
		if(mods::builder::description(IN_ROOM(ch),description + "\r\n")){
			*player << "{red}Description changed to:{/red} " << description << "\r\n";
		}else{
			*player << "{red}Error{/red}\r\n";
		}
		return;
	}
	if(std::string(&command[0]).compare("save") == 0){
		auto ret = mods::builder::save_to_db(IN_ROOM(ch));
		if(ret != 0){
			*player << "{red}Error saving room: " << ret << "{/red}\r\n";
		}else{
			*player << "{red}Room saved{/red}\r\n";
		}
		return;
	}
	if(std::string(&command[0]).compare("bind") == 0){
		constexpr unsigned int max_char_item = 6;
		std::array<char,max_char_item> direction;
		std::array<char,max_char_item> room_id;
		std::fill(room_id.begin(),room_id.end(),0);
		std::fill(direction.begin(),direction.end(),0);
		one_argument(one_argument(one_argument(argument,&command[0],max_char),&direction[0],max_char_item),&room_id[0],max_char_item);
		/* command = bind, direction = neswud, room_id = int */
		if(!IS_DIRECTION(&direction[0])){
			*player << "{red}Invalid direction{/red}\r\n";
			return;
		}
		auto room = mods::util::stoi(&room_id[0]);
		if(room.value_or(-1) == -1 || room > mods::globals::room_list.size()){
			*player << "{red}Invalid room number{/red}\r\n";
			return;
		}
		if(mods::builder::create_direction(IN_ROOM(ch),mods::globals::dir_int(direction[0]),room.value())){
			*player << "{red}Direction created{/red}\r\n";
		}else{
			*player << "{red}Error{/red}\r\n";
		}
		return;
	}
	if(std::string(&command[0]).compare("destroy") == 0){
		constexpr unsigned int max_char_item = 8; 
		std::array<char,max_char_item> direction;
		std::array<char,max_char_item> item;
		one_argument(one_argument(argument,&command[0],max_char_item),&direction[0],max_char_item);
		auto description = std::string(argument);
		std::string str_item = &item[0];
		if(!IS_DIRECTION(&direction[0])){
			*player << "{red}Invalid direction{/red}\r\n";
			return;
		}
		if(!mods::builder::destroy_direction(IN_ROOM(ch),mods::globals::dir_int(direction[0]))){
			*player << "{red}Unable to destroy direction{/red}\r\n";
		}else{
			*player << "{red}Direction destroyed{/red}\r\n";
		}
		return;
	}
	if(std::string(&command[0]).compare("dopt") == 0){
		constexpr unsigned int max_char_item = 20;
		std::array<char,max_char_item> direction;
		std::array<char,max_char_item> item;
		one_argument(one_argument(one_argument(argument,&command[0],max_char),&direction[0],max_char_item),&item[0],max_char_item);
		auto description = std::string(argument);
		std::string str_item = &item[0];
		if(str_item.compare("gen") == 0){
			description = description.substr(description.find("gen ") + 4);
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),description,
				std::nullopt,std::nullopt,std::nullopt,std::nullopt).value_or("success");
			if(ret.compare("success") == 0){
				*player << "{red}General Description changed to:{/red} " << description << "\r\n";
			}else{
				*player << "{red}Error: " << ret << "{/red}\r\n";
			}
			return;
		}
		if(str_item.compare("keyword") == 0){
			description = description.substr(description.find("keyword ") + 8);
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				description,std::nullopt,std::nullopt,std::nullopt).value_or("success");
			if(ret.compare("success") == 0){
				*player << "{red}Keyword changed to:{/red} " << description << "\r\n";
			}else{
				*player << "{red}Error: " << ret << "{/red}\r\n";
			}
			return;
		}
		if(str_item.compare("einfo") == 0){
			description = description.substr(description.find("einfo ") + 6);
			auto exit_info = mods::util::stoi(description);
			if(!exit_info.has_value()){
				*player << "{red}Invalid exit info. Must be 0-3{/red}\r\n";
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				std::nullopt,exit_info.value(),std::nullopt,std::nullopt).value_or("success");
			if(ret.compare("success") == 0){
				*player << "{red}exit_info changed to:{/red} EX_ISDOOR\r\n";
			}else{
				*player << "{red}Error: " << ret << "{/red}\r\n";
			}
			return;
		}
		if(str_item.compare("key") == 0){
			description = description.substr(description.find("key ") + 4);
			auto key = mods::util::stoi(description);
			if(key.value_or(-1) == -1){
				*player << "{red}Invalid key{/red}\r\n";
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				std::nullopt,std::nullopt,key.value(),std::nullopt).value_or("success");
			if(ret.compare("success") == 0){
				*player << "{red}key changed to:{/red} " << description << "\r\n";
			}else{
				*player << "{red}Error: " << ret << "{/red}\r\n";
			}
			return;
		}
		if(str_item.compare("to_room") == 0){
			description = description.substr(description.find("to_room ") + 8);
			auto to_room = mods::util::stoi(description);
			if(to_room.value_or(-1) == -1){
				*player << "{red}Invalid room number{/red}\r\n";
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				std::nullopt,std::nullopt,std::nullopt,to_room.value()).value_or("success");
			if(ret.compare("success") == 0){
				*player << "{red}to_room changed to:{/red} " << description << "\r\n";
			}else{
				*player << "{red}Error: " << ret << "{/red}\r\n";
			}
			return;
		}
	}
}
