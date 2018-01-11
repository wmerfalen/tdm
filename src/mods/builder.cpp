#include "builder.hpp"
#include "quests.hpp"
#include "pq.hpp"
#include "util.hpp"
#include "object.hpp"
#include <stdlib.h> //For itoa
#include "sql.hpp"
#include "../structs.h"
#include "extern.hpp"

#define MENTOC_OBI(i) obj->i = get_intval(#i).value_or(obj->i);
#define MENTOC_OBI2(i,a) obj->i = get_intval(#a).value_or(obj->i);
#define MENTOC_OBS(i) obj->i = get_strval(#i).value_or(obj->i);
#define MENTOC_OBS2(i,a) obj->i = get_strval(#a).value_or(obj->i);
using objtype = mods::object::type;
typedef mods::sql::compositor<mods::pq::transaction> sql_compositor;
namespace mods::builder{
	std::array<std::pair<int,std::string>,3> sex_flags = { {
		{SEX_NEUTRAL,"NEUTRAL"},
		{SEX_MALE,"MALE"},
		{SEX_FEMALE,"FEMALE"}
	} };

	std::array<std::pair<int,std::string>,19> mob_act_flags = { {
		{MOB_SPEC,"SPEC"},         
		{MOB_SENTINEL,"SENTINEL"},     
		{MOB_SCAVENGER,"SCAVENGER"},    
		{MOB_ISNPC,"ISNPC"},        
		{MOB_AWARE,"AWARE"},    
		{MOB_AGGRESSIVE,"AGGRESSIVE"},   
		{MOB_STAY_ZONE,"STAY_ZONE"},
		{MOB_WIMPY,"WIMPY"},        
		{MOB_AGGR_EVIL,"AGGR_EVIL"},
		{MOB_AGGR_GOOD,"AGGR_GOOD"},
		{MOB_AGGR_NEUTRAL,"AGGR_NEUTRAL"},
		{MOB_MEMORY,"MEMORY"},   
		{MOB_HELPER,"HELPER"},   
		{MOB_NOCHARM,"NOCHARM"},  
		{MOB_NOSUMMON,"NOSUMMON"},     
		{MOB_NOSLEEP,"NOSLEEP"},  
		{MOB_NOBASH,"NOBASH"},   
		{MOB_NOBLIND,"NOBLIND"},  
		{MOB_NOTDEADYET,"NOTDEADYET"}   
	} };

	std::array<std::pair<int,std::string>,22> aff_flags = { {
		{AFF_BLIND,"BLIND"},
		{AFF_INVISIBLE,"INVISIBLE"},
		{AFF_DETECT_ALIGN,"DETECT_ALIGN"},
		{AFF_DETECT_INVIS,"DETECT_INVIS"},
		{AFF_DETECT_MAGIC,"DETECT_MAGIC"},
		{AFF_SENSE_LIFE,"SENSE_LIFE"},
		{AFF_WATERWALK,"WATERWALK"},
		{AFF_SANCTUARY,"SANCTUARY"},
		{AFF_GROUP,"GROUP"},
		{AFF_CURSE,"CURSE"},
		{AFF_INFRAVISION,"INFRAVISION"},
		{AFF_POISON,"POISON"},
		{AFF_PROTECT_EVIL,"PROTECT_EVIL"},
		{AFF_PROTECT_GOOD,"PROTECT_GOOD"},
		{AFF_SLEEP,"SLEEP"},
		{AFF_NOTRACK,"NOTRACK"},
		{AFF_UNUSED16,"UNUSED16"},
		{AFF_UNUSED17,"UNUSED17"},
		{AFF_SNEAK,"SNEAK"},
		{AFF_HIDE,"HIDE"},
		{AFF_UNUSED20,"UNUSED20"},
		{AFF_CHARM,"CHARM"},
	} };
	std::array<std::pair<int,std::string>,9> position_flags = { {
		{POS_DEAD,"DEAD"},
		{POS_MORTALLYW,"MORTALLYW"},
		{POS_INCAP,"INCAP"},
		{POS_STUNNED,"STUNNED"},
		{POS_SLEEPING,"SLEEPING"},
		{POS_RESTING,"RESTING"},
		{POS_SITTING,"SITTING"},
		{POS_FIGHTING,"FIGHTING"},
		{POS_STANDING,"STANDING"}
	} };

	std::array<std::pair<int,std::string>,3> type2_flags = { {
		{objtype::WEAPON,"WEAPON"},
		{objtype::CONTAINER,"CONTAINER"},
		{objtype::SENTINEL,"SENTINEL"}
	} };
	std::array<std::pair<int,std::string>,23> type_flags = { {
		{ITEM_LIGHT,"LIGHT"},
		{ITEM_SCROLL,"SCROLL"},
		{ITEM_WAND,"WAND"},
		{ITEM_STAFF,"STAFF"},
		{ITEM_WEAPON,"WEAPON"},
		{ITEM_FIREWEAPON,"FIREWEAPON"},
		{ITEM_MISSILE,"MISSILE"},
		{ITEM_TREASURE,"TREASURE"},
		{ITEM_ARMOR,"ARMOR"},
		{ITEM_POTION,"POTION"},
		{ITEM_WORN,"WORN"},
		{ITEM_OTHER,"OTHER"},
		{ITEM_TRASH,"TRASH"},
		{ITEM_TRAP,"TRAP"},
		{ITEM_CONTAINER,"CONTAINER"},
		{ITEM_NOTE,"NOTE"},
		{ITEM_DRINKCON,"DRINKCON"},
		{ITEM_KEY,"KEY"},
		{ITEM_FOOD,"FOOD"},
		{ITEM_MONEY,"MONEY"},
		{ITEM_PEN,"PEN"},
		{ITEM_BOAT,"BOAT"},
		{ITEM_FOUNTAIN,"FOUNTAIN"}
	}};

	std::array<std::pair<int,std::string>,17> extra_flags = { {
			{ITEM_GLOW,"GLOW"},
			{ITEM_HUM,"HUM"},
			{ITEM_NORENT,"NORENT"},
            {ITEM_NODONATE,"NODONATE"},
            {ITEM_NOINVIS,"NOINVIS"},
            {ITEM_INVISIBLE,"INVISIBLE"},
			{ITEM_MAGIC,"MAGIC"},
			{ITEM_NODROP,"NODROP"},
            {ITEM_BLESS,"BLESS"},
			{ITEM_ANTI_GOOD,"ANTI_GOOD"},
            {ITEM_ANTI_EVIL,"ANTI_EVIL"},
			{ITEM_ANTI_NEUTRAL,"ANTI_NEUTRAL"},
            {ITEM_ANTI_MAGIC_USER,"ANTI_MAGIC_USER"},
			{ITEM_ANTI_CLERIC,"ANTI_CLERIC"},
            {ITEM_ANTI_THIEF,"ANTI_THIEF"},
			{ITEM_ANTI_WARRIOR,"ANTI_WARRIOR"},
            {ITEM_NOSELL,"NOSELL"}
	}};
	std::array<std::pair<int,std::string>,25> affected_flags = { {
		{APPLY_NONE,"NONE"},
		{APPLY_STR,"STR"},
		{APPLY_DEX,"DEX"},
		{APPLY_INT,"INT"},
		{APPLY_WIS,"WIS"},
		{APPLY_CON,"CON"},
		{APPLY_CHA,"CHA"},
		{APPLY_CLASS,"CLASS"},
		{APPLY_LEVEL,"LEVEL"},
		{APPLY_AGE,"AGE"},
		{APPLY_CHAR_WEIGHT,"CHAR_WEIGHT"},
		{APPLY_CHAR_HEIGHT,"CHAR_HEIGHT"},
		{APPLY_MANA,"MANA"},
		{APPLY_HIT,"HIT"},
		{APPLY_MOVE,"MOVE"},
		{APPLY_GOLD,"GOLD"},
		{APPLY_EXP,"EXP"},
		{APPLY_AC,"AC"},
		{APPLY_HITROLL,"HITROLL"},
		{APPLY_DAMROLL,"DAMROLL"},
		{APPLY_SAVING_PARA,"SAVING_PARA"},
		{APPLY_SAVING_ROD,"SAVING_ROD"},
		{APPLY_SAVING_PETRI,"SAVING_PETRI"},
		{APPLY_SAVING_BREATH,"SAVING_BREATH"},
		{APPLY_SAVING_SPELL,"SAVING_SPELL"}
	}};
	std::array<std::pair<int,std::string>,15> wear_flags = { {
            {ITEM_WEAR_TAKE,"TAKE"},
            {ITEM_WEAR_FINGER,"FINGER"},
            {ITEM_WEAR_NECK,"NECK"},
            {ITEM_WEAR_BODY,"BODY"},
            {ITEM_WEAR_HEAD,"HEAD"},
            {ITEM_WEAR_LEGS,"LEGS"},
            {ITEM_WEAR_FEET,"FEET"},
            {ITEM_WEAR_HANDS,"HANDS"},
            {ITEM_WEAR_ARMS,"ARMS"},
            {ITEM_WEAR_SHIELD,"SHIELD"},
            {ITEM_WEAR_ABOUT,"ABOUT"},
            {ITEM_WEAR_WAIST,"WAIST"},
            {ITEM_WEAR_WRIST,"WRIST"},
            {ITEM_WEAR_WIELD,"WIELD"},
            {ITEM_WEAR_HOLD,"HOLD"}
        } };
	std::optional<obj_data*> instantiate_object_by_index(int index){
		if(index >= obj_proto.size()){
			return std::nullopt;
		}
		object_list.push_back(obj_proto[index]);
		return &(*(object_list.end()-1));
	}
	std::optional<obj_data*> instantiate_object_by_vnum(obj_vnum vnum){
		obj_data* obj;
		for(auto & obj_reference : obj_proto){
			if(obj_reference.item_number == vnum){
				obj = &obj_reference;
				object_list.push_back(obj_reference);
				return obj;
			}
		}
		return std::nullopt;
	}
	void report_error(std::shared_ptr<mods::player> player,std::string_view message){
		*player << "{red}[error]: {/red}" << message.data() << "\r\n";
	}
	void report_status(std::shared_ptr<mods::player> player,std::string_view message){
		*player << "{gld}[status]: {/gld}" << message.data() << "\r\n";
	}
	void report_success(std::shared_ptr<mods::player> player,std::string_view message){
		*player << "{grn}[success]: {/grn}" << message.data() << "\r\n";
	}
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
		auto txn = txn();
		auto sql = sql_compositor("zone",&txn)
			.insert()
			.into("zone")
			.values({
				{"zone_start",std::to_string(room_start)},
				{"zone_end",std::to_string(room_end)},
				{"zone_name",name.data()},
				{"lifespan",std::to_string(lifespan)},
				{"reset_mode",std::to_string(reset_mode)}
			})
			.sql();
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
			auto check_txn = txn();
			auto check_sql = sql_compositor("room",&check_txn)
				.select("COUNT(room_number)")
				.from("room")
				.where("room_number","=",std::to_string(world[in_room].number))
				.sql();
			auto check_result = mods::pq::exec(check_txn,check_sql);
			mods::pq::commit(check_txn);
			sql_compositor::value_map values;
			values["zone"] = std::to_string(world[in_room].zone);
			values["sector_type"] = std::to_string(world[in_room].sector_type);
			values["name"] = world[in_room].name;
			values["description"] = world[in_room].description;
			if(world[in_room].ex_description && world[in_room].ex_description->keyword){
				values["ex_keyword"] = (world[in_room].ex_description->keyword);
			}
			if(world[in_room].ex_description && world[in_room].ex_description->description){
				values["ex_description"] = (world[in_room].ex_description->description);
			}
			std::array<char,16> num;
			std::fill(num.begin(),num.end(),0);
			sprintf(&num[0],"%d",world[in_room].light);
			values["light"] = &num[0];
			std::fill(num.begin(),num.end(),0);
			sprintf(&num[0],"%d",world[in_room].room_flags);
			values["room_flag"] = &num[0];

			if(check_result.size()){
				int check_i = mods::pq::as_int(check_result,0,0);
				/* update the record */
				auto txn = txn();
				auto sql = sql_compositor("room",&txn)
					.update("room")
					.set(values)
					.where("room_number","=",std::to_string(world[in_room].number))
					.sql();
				mods::pq::exec(txn,sql);
				mods::pq::commit(txn);
			}else{
				auto txn = txn();
				auto sql = sql_compositor("room",&txn)
					.insert()
					.into("room")
					.values(values)
					.sql();
				mods::pq::exec(txn,sql);
				mods::pq::commit(txn);
			}
			for(auto direction = 0; direction < NUM_OF_DIRS;direction++){
				if(world[in_room].dir_option[direction] &&
				   world[in_room].dir_option[direction]->general_description != nullptr){
					auto check_txn = txn();
					sql_compositor comp("room_direction_data",&check_txn);
					std::string check_sql = comp.
						select("COUNT(room_number)")
						.from("room_direction_data")
						.where("room_number","=",std::to_string(world[in_room].number))
						.op_and("exit_direction","=",std::to_string(direction))
						.sql();
					check_sql += " AND exit_direction=";
					check_sql += check_txn.quote(direction);
					auto check_result = mods::pq::exec(check_txn,check_sql);
					mods::pq::commit(check_txn);
					auto real_room_num = world[world[in_room].dir_option[direction]->to_room].number;
					std::map<std::string,std::string> values = {
								{"general_description",world[in_room].dir_option[direction]->general_description},
								{"keyword",world[in_room].dir_option[direction]->keyword},
								{"exit_info",std::to_string(world[in_room].dir_option[direction]->exit_info)},
								{"exit_key",std::to_string(world[in_room].dir_option[direction]->key)},
								{"to_room",std::to_string(real_room_num)}
					};
					if(check_result.size()){
						/* update the row instead of inserting it */
						auto up_txn = txn();
						sql_compositor comp("room_direction_data",&up_txn);
						auto up_sql = comp
							.update("room_direction_data")
							.set(values)
							.where("exit_direction","=",std::to_string(direction))
							.op_and("room_number","=",std::to_string(world[in_room].number))
							.sql();
						mods::pq::exec(up_txn,up_sql);
						mods::pq::commit(up_txn);
					}else{
						auto txn2 = txn();
						sql_compositor comp("room_direction_data",&txn2);
						auto sql = comp
							.insert()
							.into("room_direction_data")
							.values(values)
							.sql();
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
		world[IN_ROOM(ch)].dir_option[direction]->general_description = strdup("<default>");
		world[IN_ROOM(ch)].dir_option[direction]->keyword = strdup("door");
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
		auto txn = txn();
		sql_compositor comp("zone_data",&txn);
		auto sql = comp
			.insert()
			.into("zone_data")
			.values({
				{"zone_id",std::to_string(zone_id)},
				{"zone_command",zone_command.data()},
				{"zone_if_flag",if_flag.data()},
				{"zone_arg1",arg1.data()},
				{"zone_arg2",arg2.data()},
				{"zone_arg3",arg3.data()},
			})
			.sql();
		mods::pq::exec(txn,sql);
		mods::pq::commit(txn);
	}
	char_data new_blank_character(bool npc = false){
		char_data proto;
		memset(&proto,0,sizeof(proto));
		proto.player_specials = &dummy_mob;
		const char* foo = "foo";
		proto.player.name = strdup(foo);
		proto.player.short_descr = strdup(foo);
		proto.player.long_descr = strdup(foo);
		proto.player.description = strdup(foo);
		if(npc){
			SET_BIT(proto.char_specials.saved.act, MOB_ISNPC);
		}else{
			REMOVE_BIT(proto.char_specials.saved.act, MOB_ISNPC);
		}
		REMOVE_BIT(proto.char_specials.saved.act, MOB_NOTDEADYET);
		unsigned j = 0;
		for (; j < 5; j++){
			GET_SAVE(&proto, j) = 0;
		}
		proto.aff_abils = proto.real_abils;
		return proto;
	}
	char_data new_npc(){
		return new_blank_character(true);
	}
	std::pair<bool,std::string> save_player(char_data* obj){
		try{
			auto txn_01 = txn();
			auto result_01 = mods::pq::exec(txn_01,sql_compositor("mobile",&txn_01)
				.select("mob_id")
				.from("mobile")
				.where("mob_virtual_number","=",mods::util::itoa(obj->nr))
				.sql()
			);
			mods::pq::commit(txn_01);
#define MENTOC_PLAYER_NULL_CHECK(item)\
			if(!obj->item){\
				return {false,std::string(#item) + " is null!"};\
			}
			MENTOC_PLAYER_NULL_CHECK(player.name);
			MENTOC_PLAYER_NULL_CHECK(player.short_descr);
			MENTOC_PLAYER_NULL_CHECK(player.long_descr);
			MENTOC_PLAYER_NULL_CHECK(player.description);
			
			sql_compositor::value_map p_map;
			p_map["mob_virtual_number"] = mods::util::itoa(obj->nr);
			p_map["mob_name"] = obj->player.name;
			p_map["mob_short_description"] = obj->player.short_descr;
			p_map["mob_long_description"] = obj->player.long_descr;
			p_map["mob_description"] = obj->player.description;
			p_map["mob_ability_strength"] = mods::util::itoa(obj->real_abils.str);
			p_map["mob_ability_strength_add"] = mods::util::itoa(obj->real_abils.str_add);
			p_map["mob_ability_intelligence"] = mods::util::itoa(obj->real_abils.intel);
			p_map["mob_ability_wisdom"] = mods::util::itoa(obj->real_abils.wis);
			p_map["mob_ability_dexterity"] = mods::util::itoa(obj->real_abils.dex);
			p_map["mob_ability_constitution"] = mods::util::itoa(obj->real_abils.con);
			p_map["mob_ability_charisma"] = mods::util::itoa(obj->real_abils.cha);
			p_map["mob_mana"] = mods::util::itoa(obj->points.mana);
			p_map["mob_max_mana"] = mods::util::itoa(obj->points.max_mana);
			p_map["mob_hitpoints"] = mods::util::itoa(obj->points.hit);
			p_map["mob_max_hitpoints"] = mods::util::itoa(obj->points.max_hit);
			p_map["mob_move"] = mods::util::itoa(obj->points.move);
			p_map["mob_max_move"] = mods::util::itoa(obj->points.max_move);
			p_map["mob_armor"] = mods::util::itoa(obj->points.armor);
			p_map["mob_gold"] = mods::util::itoa(obj->points.gold);
			p_map["mob_exp"] = mods::util::itoa(obj->points.exp);
			p_map["mob_hitroll"] = mods::util::itoa(obj->points.hitroll);
			p_map["mob_default_position"] = mods::util::itoa(obj->mob_specials.default_pos);
			p_map["mob_damnodice"] = mods::util::itoa(obj->mob_specials.damnodice);
			p_map["mob_load_position"] = mods::util::itoa(GET_POS(obj));
			p_map["mob_default_position"] = mods::util::itoa(GET_POS(obj));
			p_map["mob_alignment"] = mods::util::itoa(obj->char_specials.saved.alignment);
			p_map["mob_action_bitvector"] = mods::util::itoa(obj->char_specials.saved.act);
			p_map["mob_affection_bitvector"] = mods::util::itoa(obj->char_specials.saved.affected_by);
			p_map["mob_attack_type"] = mods::util::itoa(obj->mob_specials.attack_type);
			p_map["mob_weight"] = mods::util::itoa(obj->player.weight);
			p_map["mob_height"] = mods::util::itoa(obj->player.height);
			p_map["mob_level"] = "0";	//TODO: find this
			p_map["mob_sex"] = "0"; //TODO: find this
			p_map["mob_damsizedice"] = "0"; //TODO: find this
			p_map["mob_damroll"] = "0"; //TODO: find this
			p_map["mob_class"] = "0"; //TODO: find this
			auto txn_02 = txn();
			std::string sql = "";
			if(result_01.size()){
				//Update
				sql = sql_compositor("mobile",&txn_02)
					.update("mobile")
					.set(p_map)
					.where("mob_virtual_number","=",mods::util::itoa(obj->nr))
					.sql();
			}else{
				//Insert
				sql = sql_compositor("mobile",&txn_02)
					.insert()
					.into("mobile")
					.values(p_map)
					.sql();
			}
			mods::pq::exec(txn_02,sql);
			mods::pq::commit(txn_02);
		}catch(const std::exception &e){
			return {false,e.what()};
		}
		return {true,"Successfully saved player."};
	}
	std::pair<bool,std::string> save_object(obj_data* obj){
		auto txn_01 = txn();
		sql_compositor comp3("object",&txn_01);
		auto sql = comp3.select("id").from("object").
			where("obj_item_number","=",std::to_string(obj->item_number))
			.sql();
		auto check_result_01 = mods::pq::exec(txn_01,sql);
		mods::pq::commit(txn_01);
		auto check_i = 0;
		sql_compositor::value_map my_map;
		my_map["obj_item_number"] = std::to_string(obj->item_number);
		my_map["obj_flags"] = "0";
#define MENTOC_CHK_OBJ(item) if(!obj->item){ return {false,std::string(#item) + " is empty"}; }
		MENTOC_CHK_OBJ(name);
		MENTOC_CHK_OBJ(description);
		MENTOC_CHK_OBJ(short_description);
		MENTOC_CHK_OBJ(action_description);
		my_map["obj_name"] = obj->name;
		my_map["obj_description"] = obj->description;
		my_map["obj_short_description"] = obj->short_description;
		if(obj->action_description){
			my_map["obj_action_description"] = obj->action_description;
		}
		my_map["obj_type"] = std::to_string(obj->type);
		my_map["obj_worn_on"] = std::to_string(obj->worn_on);
		my_map["obj_type_data"] = "0";
		if(obj->ex_description && obj->ex_description->keyword){
			my_map["obj_extra_keyword"] = obj->ex_description->keyword;
		}else{
			return {false,"atleast one ex_description->keyword is required"};
		}
		if(obj->ex_description && obj->ex_description->description){
			my_map["obj_extra_description"] = obj->ex_description->description;
		}else{
			return {false,"atleast one ex_description->description is required"};
		}
		if(check_result_01.size()){
			check_i = mods::pq::as_int(check_result_01,0,0);
			/* update the fields */
			auto txn = txn();
			sql_compositor comp("object",&txn);
			auto update_sql = comp
				.update("object")
				.set(my_map)
				.where("obj_item_number","=",std::to_string(obj->item_number))
				.sql();
			auto result = mods::pq::exec(txn,update_sql);
			mods::pq::commit(txn);
		}else{
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
				.where("obj_item_number","=",std::to_string(obj->item_number))
				.sql();
			auto res = mods::pq::exec(txn4,sql);
			mods::pq::commit(txn4);
			if(res.size()){
				check_i = mods::pq::as_int(res,0,0);
			}
		}
		switch(obj->obj_flags.type_flag){
			case ITEM_WEAPON:
				auto txn3 = txn();
				sql_compositor comp3("object_weapon",&txn3);
				auto sql = comp3.select("id").from("object_weapon").
					where("obj_fk_id","=",std::to_string(check_i))
					.sql();
				auto check_result = mods::pq::exec(txn3,sql);
				mods::pq::commit(txn3);
				auto weapon_exists = mods::pq::as_int(check_result,0,0);
				if(weapon_exists){
					auto txn5 = txn();
					sql_compositor comp("object_weapon",&txn5);
					auto sql = comp.update("object_weapon")
					.set({
						{"obj_ammo_type",std::to_string(obj->weapon_type)},
						{"obj_ammo_max",std::to_string(obj->ammo_max)},
						{"obj_cooldown","0"},
						{"obj_can_snipe","0"}
					}).where("obj_fk_id","=",std::to_string(check_i))
					.sql();
					mods::pq::exec(txn5,sql);
					mods::pq::commit(txn5);
				}else{
					auto txn6 = txn();
					sql_compositor comp("object_weapon",&txn6);
					auto sql = comp.insert().into("object_weapon")
						.values({
							{"obj_fk_id",std::to_string(check_i)},
							{"obj_ammo_max",std::to_string(obj->ammo_max)},
							{"obj_ammo_type",std::to_string(obj->weapon_type)},
							{"obj_cooldown",std::to_string(0)},
							{"obj_can_snipe",std::to_string(0)}
						})
						.sql();
					mods::pq::exec(txn6,sql);
					mods::pq::commit(txn6);
				}
				break;
		}
		auto txn6 = mods::pq::transaction(*mods::globals::pq_con);
		std::string sel_weapon_sql = 
			std::string(
				"select id from affected_type where aff_fk_id="	//TODO: !mundane use new shit
			) + txn6.quote(check_i);
		auto check_result = mods::pq::exec(txn6,sel_weapon_sql);
		mods::pq::commit(txn6);
		if(check_result.size()){
			auto txn8 = txn();
			std::string sql = std::string(
				"DELETE FROM affected_type where obj_fk_id=") 	//TODO: !mundane use new shit
				+ txn8.quote(check_i);
			mods::pq::exec(txn8,sql);
			mods::pq::commit(txn8);
		}
		auto txn9 = txn();
		auto sql9 = std::string(
				"DELETE FROM object_flags where obj_fk_id=")
			+ txn9.quote(check_i)
		;
		mods::pq::exec(txn9,sql9);
		mods::pq::commit(txn9);
		auto txn10 = txn();
		auto sql10 = sql_compositor("object_flags",&txn10)
			.insert()
			.into("object_flags")
			.values({
				{"obj_fk_id",std::to_string(check_i)},
				{"value_0",mods::util::itoa(obj->obj_flags.value[0])},
				{"value_1",mods::util::itoa(obj->obj_flags.value[1])},
				{"value_2",mods::util::itoa(obj->obj_flags.value[2])},
				{"value_3",mods::util::itoa(obj->obj_flags.value[3])},
				{"type_flag",mods::util::itoa(obj->obj_flags.type_flag)},
				{"wear_flags",mods::util::itoa(obj->obj_flags.wear_flags)},
				{"extra_flags",mods::util::itoa(obj->obj_flags.extra_flags)},
				{"weight",mods::util::itoa(obj->obj_flags.weight)},
				{"cost",mods::util::itoa(obj->obj_flags.cost)},
				{"cost_per_day",mods::util::itoa(obj->obj_flags.cost_per_day)},
				{"timer",mods::util::itoa(obj->obj_flags.timer)},
				{"bitvector",mods::util::itoa(obj->obj_flags.bitvector)}
			})
			.sql();
		mods::pq::exec(txn10,sql10);
		mods::pq::commit(txn10);
		for(unsigned i =0; i < MAX_OBJ_AFFECT;i++){
			auto txn7 = txn();
			std::string loc,mod;
			if(obj->affected[i].location && obj->affected[i].modifier){
				auto sql = sql_compositor("affected_type",&txn7).insert()
					.into("affected_type")
					.values({
						{"aff_fk_id",std::to_string(check_i)},
						{"aff_location",mods::util::itoa(obj->affected[i].location)},
						{"aff_modifier", mods::util::itoa(obj->affected[i].modifier)}
					})
					.sql();
				mods::pq::exec(txn7,sql);
				mods::pq::commit(txn7);
			}
		}
		/* save ex_description linked lists */
		for(auto ex_desc = obj->ex_description;ex_desc;ex_desc = ex_desc->next){
			if(!ex_desc->keyword || !ex_desc->description){ break; }
			auto txn8 = txn();
			mods::pq::exec(txn8,sql_compositor("extra_description",&txn8)
				.insert()
				.into("extra_description")
				.values({
					{"obj_fk_id",std::to_string(check_i)},
					{"extra_keyword",ex_desc->keyword},
					{"extra_description",ex_desc->description}
				})
				.sql()
			);
		}
		return {true,"saved successfully"};
	}
};

using args_t = std::vector<std::string>;
ACMD(do_mbuild){
	MENTOC_PREAMBLE();
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0
		&& vec_args[1].compare("action") == 0){
		player->pager_start();
		*player << "possible flags:\r\n";
		for(auto & flag : mods::builder::mob_act_flags){
			*player << flag.second << "\r\n";
		}
		*player << "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0
		&& vec_args[1].compare("sex") == 0){
		player->pager_start();
		*player << "possible flags:\r\n";
		for(auto & flag : mods::builder::sex_flags){
			*player << flag.second << "\r\n";
		}
		*player << "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0
		&& vec_args[1].compare("default_position") == 0){
		player->pager_start();
		*player << "possible flags:\r\n";
		for(auto & flag : mods::builder::position_flags){
			*player << flag.second << "\r\n";
		}
		*player << "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0){
		player->pager_start();
		*player << "usage: \r\n" << 
				   " mbuild help\r\n" <<
				   "  |--> this help menu\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: mbuild help\r\n" <<
				   "  |:: (this help menu will show up)\r\n" <<
				   " mbuild new\r\n" <<
				   " mbuild list\r\n" <<
				   " mbuild attr <mob_id> <attr> <value>\r\n" <<
				   "  |:: -:[attributes]:-\r\n" <<
				   "  |:: name\r\n" << 
				   "  |:: short_description\r\n" << 
				   "  |:: long_description\r\n" << 
				   "  |:: description\r\n" << 
				   "  |:: mana\r\n" << 
				   "  |:: max_mana\r\n" << 
				   "  |:: hit\r\n" << 
				   "  |:: max_hit\r\n" << 
				   "  |:: move\r\n" << 
				   "  |:: max_move\r\n" << 
				   "  |:: armor\r\n" << 
				   "  |:: gold\r\n" << 
				   "  |:: exp\r\n" << 
				   "  |:: hitroll\r\n" << 
				   "  |:: damroll\r\n" << 
				   "  |:: sex {red}see mbuild help sex{/red}\r\n" << 
				   "  |:: default_position {red}see mbuild help default_position{/red}\r\n" << 
				   "  |:: action {red}see mbuild help action{/red}\r\n" << 
				   " mbuild save <mob_id>\r\n" <<
				   " mbuild show <mob_id>\r\n" <<
			"\r\n"
		;
		player->pager_end();
		player->page(0);
		return;
	}
	auto args = mods::util::subcmd_args<4,args_t>(argument,"new");
	if(args.has_value()){
		mob_proto.push_back(mods::builder::new_npc());
		mods::builder::report_success(player,"Mobile created");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"list");
	if(args.has_value()){
		mods::builder::report_status(player,"listing...");
		unsigned mob_id = 0;
		player->pager_start();
		for(auto& mob_reference : mob_proto){
			auto mob = &mob_reference;
			*player << "{gld}[" << mob_id++ << "]{/gld} :->{red} [" <<
				mob->player.short_descr << "]{/red}\r\n";
		}
		player->pager_end();
		player->page(0);
		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"save");
	if(args.has_value()){
		auto arg_vec = args.value();
		if(arg_vec.size() < 2){
			mods::builder::report_error(player,"Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		if(!index.has_value() || index.value() >= mob_proto.size()){
			mods::builder::report_error(player,"Invalid index");
			return;
		}
		auto obj = &mob_proto[index.value()];
		auto return_pair = mods::builder::save_player(obj);
		if(!return_pair.first){
			mods::builder::report_error(player,return_pair.second);
			return;
		}
		mods::builder::report_success(player,"Object saved.");
		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"show");
	if(args.has_value()){
		auto arg_vec = args.value();
		if(arg_vec.size() < 2){
			mods::builder::report_error(player,"Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		if(!index.has_value() || index.value() >= mob_proto.size()){
			mods::builder::report_error(player,"Invalid index");
			return;
		}
		auto obj = &mob_proto[index.value()];
		player->pager_start();
		*player << "{gld}::player::{/gld}\r\n";
#define MENTOC_SHOW_OBJ_FLAGS(display_name,struct_member,flag_structure) \
		for(auto & obj_flag : flag_structure){\
			if(obj_flag.first == obj->struct_member){\
				*player << "{red}" << #display_name << "{/red}: " <<\
					obj_flag.second << "\r\n";\
				break;\
			}\
		}
#define MENTOC_SHOW_OBJ_BITVECTOR(display_name,struct_member,flag_structure) \
		*player << "{red}" << #display_name << "{/red}: ";\
		for(auto & obj_flag : flag_structure){\
			if(obj_flag.first & obj->struct_member){\
				*player << obj_flag.second << " ";\
			}\
		}\
		*player << "\r\n";
#define MENTOC_SHOW_OBJ(display_name,struct_member) \
		*player << "{red}" << #display_name << "{/red}: " << obj->struct_member << "\r\n";
		MENTOC_SHOW_OBJ(name,player.name);
		MENTOC_SHOW_OBJ(short_description,player.short_descr);
		MENTOC_SHOW_OBJ(long_description,player.long_descr);
		MENTOC_SHOW_OBJ(description,player.description);
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
		MENTOC_SHOW_OBJ(armor,points.armor);
		MENTOC_SHOW_OBJ(gold,points.gold);
		MENTOC_SHOW_OBJ(exp,points.exp);
		MENTOC_SHOW_OBJ(hitroll,points.hitroll);
		MENTOC_SHOW_OBJ(damroll,points.damroll);
		player->pager_end();
		player->page(0);
		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"attr");
	if(args.has_value()){
		auto arg_vec = args.value();
		if(arg_vec.size() < 4){
			mods::builder::report_error(player,"Invalid number of arguments");
			return;
		}
		auto get_intval = [&](std::string_view str) -> std::optional<int>{
			if(arg_vec[2].compare(str.data()) == 0){
				auto i_value = mods::util::stoi(arg_vec[3]);
				if(!i_value.has_value()){
					mods::builder::report_error(player,"Please use a valid numeric value.");
					return std::nullopt;
				}
				return i_value.value();
			}
			return std::nullopt;
		};
		auto get_strval = [&](std::string_view str) -> std::optional<char*>{
			if(arg_vec[2].compare(str.data()) == 0){
				return strdup(arg_vec[3].c_str());
			}
			return std::nullopt;
		};
		auto index = mods::util::stoi(arg_vec[1]);
		if(index.has_value() && index.value() < mob_proto.size()){
			auto obj = &mob_proto[index.value()];
			MENTOC_OBS2(player.name,name);
			MENTOC_OBS2(player.short_descr,short_description);
			MENTOC_OBS2(player.long_descr,long_description);
			MENTOC_OBS2(player.description,description);
			MENTOC_OBI2(points.mana,mana);
			MENTOC_OBI2(points.max_mana,max_mana);
			MENTOC_OBI2(points.hit,hit);
			MENTOC_OBI2(points.max_hit,max_hit);
			MENTOC_OBI2(points.move,move);
			MENTOC_OBI2(points.max_move,max_move);
			MENTOC_OBI2(points.armor,armor);
			MENTOC_OBI2(points.gold,gold);
			MENTOC_OBI2(points.exp,exp);
			MENTOC_OBI2(points.hitroll,hitroll);
			MENTOC_OBI2(points.damroll,damroll);
			if(arg_vec[2].compare("sex") == 0){
				if(arg_vec.end() <= arg_vec.begin() + 3){
					mods::builder::report_error(player,"Please supply a flag");
					return;
				}
				auto flag = arg_vec.begin() + 3;
				bool found = false;
				for(auto & ex_flag : mods::builder::sex_flags){
					if(ex_flag.second.compare(*flag) == 0){
						obj->player.sex = ex_flag.first;
						found = true;
						break;
					}
				}
				if(!found){
					mods::builder::report_error(player,std::string("Unrecognized flag: ") + *flag );
				}
			}
			if(arg_vec[2].compare("action") == 0){
				if(arg_vec.end() <= arg_vec.begin() + 3){
					mods::builder::report_error(player,"Please supply action flags");
					return;
				}
				auto flag = arg_vec.begin() + 3;
				bool found = false;
				obj->char_specials.saved.act = 0;
				for(auto & ex_flag : mods::builder::mob_act_flags){
					if(ex_flag.second.compare(*flag) == 0){
						obj->char_specials.saved.act |= ex_flag.first;
						found = true;
						break;
					}
				}
				if(!found){
					mods::builder::report_error(player,std::string("Unrecognized flag: ") + *flag );
				}
			}
			if(arg_vec[2].compare("default_position") == 0){
				if(arg_vec.end() <= arg_vec.begin() + 3){
					mods::builder::report_error(player,"Please supply a flag");
					return;
				}
				auto flag = arg_vec.begin() + 3;
				bool found = false;
				for(auto & ex_flag : mods::builder::position_flags){
					if(ex_flag.second.compare(*flag) == 0){
						obj->mob_specials.default_pos = ex_flag.first;
						found = true;
						break;
					}
				}
				if(!found){
					mods::builder::report_error(player,std::string("Unrecognized flag: ") + *flag );
				}
			}
		}
		return;
	}
}

ACMD(do_obuild){
	MENTOC_PREAMBLE();
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("affected") == 0){
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
"usage: obuild affected <object_id> <affected_slot> <location> <modifier>\r\n" << 
"example: obuild affected 0 3 CHAR_WEIGHT 15\r\n" <<
"(this will set the affected slot number 3 on object zero to modify \r\n" <<
"the character's weight by 15)\r\n" <<
"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
		
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("extra_flags") == 0){
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
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("wear_flags") == 0){
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
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("type_flags") == 0){
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
	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("flags") == 0){
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
    if(!vec_args.size() || vec_args[0].compare("help") == 0){
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
				   "  |:: flags\r\n" << 
				   " obuild ex <object_id> set <index> <keyword> <description>\r\n" <<
				   " obuild ex <object_id> del <index>\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: obuild ex 6 set 0 \"keyword\" \"my description\"\r\n" <<
				   "  |:: obuild ex 6 del 0\r\n" <<
				   " obuild affected <object_id> set <affected_slot> <location> <modifier>\r\n" <<
				   " obuild affected <object_id> del <affected_slot>\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: obuild affected 6 3 STR 25\r\n" <<
				   "  |:: (this will set the affected slot number 3 on object 6 \r\n" << 
				   "      to modify strength by 25)\r\n" <<
				   "  |:: {red}see obuild help affected{/red}\r\n" <<
				   " obuild flag <object_id> extra_flags <value>\r\n"<<
				   " {red}see obuild help extra_flags{/red}\r\n" <<	
				   " obuild flag <object_id> <attr> <value>\r\n"<<
				   " {red}see obuild help flags{/red}\r\n" <<
				   "  |:: -:[attributes]:-\r\n" <<
				   "  |:: value_0\r\n" <<
				   "  |:: value_1\r\n" <<
				   "  |:: value_2\r\n" <<
				   "  |:: value_3\r\n" <<
				   "  |:: type_flags {red}see: obuild help type_flags{/red}\r\n" <<
				   "  |:: wear_flags {red}see: obuild help wear_flags{/red}\r\n" <<
				   "  |:: extra_flags\r\n" <<
				   "  |:: weight\r\n" <<
				   "  |:: cost\r\n" <<
				   "  |:: cost_per_day\r\n" <<
				   "  |:: timer\r\n" <<
				   "  |:: bitvector {red}see: obuild help bitvector{/red}\r\n" <<
				   " obuild save <object_id>\r\n" <<
				   " obuild show <object_id>\r\n" <<
				   "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	auto args = mods::util::subcmd_args<4,args_t>(argument,"new");
	if(args.has_value()){
		mods::builder::report_status(player,"Creating new object");
		obj_proto.push_back({});
		mods::builder::report_success(player,"Object created");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"save");
	if(args.has_value()){
		//TODO: !mundane make this a function
		auto arg_vec = args.value();
		if(arg_vec.size() == 1){
			mods::builder::report_error(player,"Please supply an object id");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		obj_data * obj = nullptr;
		unsigned ctr = 0;
		if(index.has_value()){
			if(index.value() >= obj_proto.size()){
				mods::builder::report_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		}else{
			mods::builder::report_error(player,std::string(args.value()[1]) + " is not a valid number");
			return;
		}
		if(!obj){
			mods::builder::report_error(player,"Index not found.");
			return;
		}
		mods::builder::report_status(player,"Saving object");
		auto return_status = mods::builder::save_object(obj);
		if(!return_status.first){
			mods::builder::report_error(player,return_status.second);
			return;
		}
		mods::builder::report_success(player,"Object saved");
		return;
	}
	args = mods::util::subcmd_args<3,args_t>(argument,"ex");
	if(args.has_value()){
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		struct obj_data* obj = nullptr;
		if(!i_value.has_value()){
			mods::builder::report_error(player,"Please use a valid numeric value");
			return;
		}else{
			auto index = i_value.value();
			if(index >= obj_proto.size()){
				mods::builder::report_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index];
		}
		if(arg_vec.size() < 3){
			mods::builder::report_error(player,"Invalid number of arguments");
			return;
		}
		if(arg_vec[2].compare("create") == 0 && arg_vec.size() == 4){
			//ex 0 create 10
			auto ex_desc = obj->ex_description;
			auto ex_create = mods::util::stoi(arg_vec[3]);
			if(ex_create.has_value()){
				if(ex_create.value() > 10){
					mods::builder::report_error(player,"Only 10 maximum ex_descriptions are possible");
					return;
				}
				extra_descr_data* previous = ex_desc;
				for(int i = ex_create.value(); i > 0; --i){
					if(!ex_desc){
						ex_desc = (extra_descr_data*)calloc(1,sizeof(extra_descr_data));
						ex_desc->keyword = ex_desc->description = strdup("<default>");
						ex_desc->next = nullptr;
						if(i < ex_create.value()){ previous->next = ex_desc; }
						previous = ex_desc;
						ex_desc = ex_desc->next;
						continue;
					}
					previous = ex_desc;
					ex_desc = ex_desc->next;
				}
				mods::builder::report_success(player,"Extra descriptions created");
				return;
			}else{
				mods::builder::report_error(player,"Invalid create value");
				return;
			}
			return;
		}
		if(arg_vec[2].compare("set") == 0 && arg_vec.size() == 6){
		//obuild ex <object_id> set <index> <keyword> <description>
		//       0    1          2   3         4         5
			auto index = mods::util::stoi(arg_vec[3]);
			if(index.has_value()){
				int iterations = 0;
				auto ex_desc = obj->ex_description;
				for(;iterations < index.value();iterations++){
					if(!ex_desc){
						mods::builder::report_error(player,"Out of bounds");
						return;
					}
					ex_desc = ex_desc->next;
				}
				ex_desc->keyword = strdup(arg_vec[4].c_str());
				ex_desc->description = strdup(arg_vec[5].c_str());
			}
		}
		if(arg_vec[2].compare("del") == 0 && arg_vec.size() == 4){
			//obuild ex <object_id> del <index>
			//        0    1         2     3
			auto index = mods::util::stoi(arg_vec[3]);
			if(index.has_value()){
				int iterations = 0;
				auto ex_desc = obj->ex_description;
				auto previous = ex_desc;
				for(;iterations < index.value();iterations++){
					if(!ex_desc){
						mods::builder::report_error(player,"Out of bounds");
						return;
					}
					ex_desc = ex_desc->next;
				}
				mods::util::linked_list_remove<extra_descr_data*>(ex_desc,obj->ex_description);
				mods::builder::report_success(player,"ex_description removed");
				return;
			}
			mods::builder::report_error(player,"Invalid index");
			return;
		}
	}
	args = mods::util::subcmd_args<12,args_t>(argument,"instantiate");
	if(args.has_value()){
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);
		if(!i_value.has_value()){
			mods::builder::report_error(player,"Please use a valid numeric value.");
			return;
		}else{
			auto index = i_value.value();
			if(index >= obj_proto.size()){
				mods::builder::report_error(player,"Out of bounds");
				return;
			}
			object_list.push_back(obj_proto[index]);
			auto obj = &(*(object_list.end() -1));
			obj->carried_by = obj->worn_by = nullptr;
			obj_to_room(obj,IN_ROOM(player->cd()));
			mods::builder::report_success(player,"Object created, look on the floor");
		}
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"list");
	if(args.has_value()){
		mods::builder::report_status(player,"listing...");
		unsigned obj_id = 0;
		player->pager_start();
		for(auto& obj_reference : obj_proto){
			auto obj = &obj_reference;
			*player << "{gld}[" << obj_id++ << "]{/gld} :->{red} [" <<
				obj->short_description << "]{/red}";
		}
		player->pager_end();
		player->page(0);
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"flag");
	if(args.has_value()){
		auto arg_vec = args.value();
		auto get_intval = [&](std::string_view str) -> std::optional<int>{
			if(arg_vec[3].compare(str.data()) == 0){
				*player << "matches: " << str.data() << "\r\n";
				auto i_value = mods::util::stoi(arg_vec[3]);
				if(!i_value.has_value()){
					mods::builder::report_error(player,"Please use a valid numeric value.");
					return std::nullopt;
				}
				return i_value.value();
			}
		};
		auto index = mods::util::stoi(arg_vec[1]);
		obj_data * obj = nullptr;
		unsigned ctr = 0;
		if(index.has_value()){
			if(index.value() >= obj_proto.size()){
				mods::builder::report_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		}else{
			mods::builder::report_error(player,"not a valid number");
			return;
		}
		if(!obj){
			mods::builder::report_error(player,"Index not found.");
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
		if(arg_vec[2].compare("extra_flags") == 0){
			if(arg_vec.end() <= arg_vec.begin() + 3){
				mods::builder::report_error(player,"Please supply one or more flags");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			obj->obj_flags.bitvector = 0;
			for(;flag != arg_vec.end();++flag){
				bool found = false;
				for(auto & ex_flag : mods::builder::extra_flags){
					if(ex_flag.second.compare(*flag) == 0){
						obj->obj_flags.bitvector |= ex_flag.first;
						found = true;
						break;
					}
				}
				if(!found){
					mods::builder::report_error(player,std::string("Unrecognized flag: ") + *flag );
				}
			}

		}
		if(arg_vec[2].compare("type_flags") == 0){
			if(arg_vec.end() <= arg_vec.begin() + 3){
				mods::builder::report_error(player,"Please supply a type flag");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			obj->obj_flags.type_flag = 0;
			bool found = false;
			for(auto & type_flag : mods::builder::type_flags){
				if(type_flag.second.compare(*flag) == 0){
					obj->obj_flags.type_flag = type_flag.first;
					found = true;
					break;
				}
			}
			if(!found){
				mods::builder::report_error(player,std::string("Unrecognized type flag: ") + *flag );
			}
		}
		if(arg_vec[2].compare("wear_flags") == 0){
			if(arg_vec.end() <= arg_vec.begin() + 3){
				mods::builder::report_error(player,"Please supply wear flags");
				return;
			}
			auto flag = arg_vec.begin() + 3;
			obj->obj_flags.wear_flags = 0;
			for(;flag != arg_vec.end();++flag){
				bool found = false;
				for(auto & wear_flag : mods::builder::wear_flags){
					if(wear_flag.second.compare(*flag) == 0){
						obj->obj_flags.wear_flags |= wear_flag.first;
						found = true;
						break;
					}
				}
				if(!found){
					mods::builder::report_error(player,std::string("Unrecognized wear flag:") + *flag);
				}
			}
		}
		mods::builder::report_success(player,"Object updated");
		return;
	}
	args = mods::util::subcmd_args<9,args_t>(argument,"affected");
	if(args.has_value()){
		auto arg_vec = args.value();
		if(arg_vec.size() > 3 && arg_vec[2].compare("del") == 0){
			//User is trying to delete an affected slot
			auto slot = mods::util::stoi(arg_vec[3]);
			if(!slot.has_value() || slot.value() > MAX_OBJ_AFFECT){
				mods::builder::report_error(player," Invalid slot index");
				return;
			}
			auto index = mods::util::stoi(arg_vec[1]);
			obj_data * obj = nullptr;
			unsigned ctr = 0;
			if(index.has_value()){
				if(index.value() >= obj_proto.size()){
					mods::builder::report_error(player,"Object index Out of bounds");
					return;
				}
				obj = &obj_proto[index.value()];
			}else{
				mods::builder::report_error(player,"not a valid number");
				return;
			}
			obj->affected[slot.value()].location = 0;
			obj->affected[slot.value()].modifier = 0;
			mods::builder::report_error(player,"Slot cleared");
			return;
		}
		if(arg_vec.size() < 6){
			mods::builder::report_error(player,"Not enough arguments. See: obuild help affected");
			return;
		}
		if(arg_vec[2].compare("set") == 0){
			auto index = mods::util::stoi(arg_vec[1]);
			obj_data * obj = nullptr;
			unsigned ctr = 0;
			if(index.has_value()){
				if(index.value() >= obj_proto.size()){
					mods::builder::report_error(player," Out of bounds");
					return;
				}
				obj = &obj_proto[index.value()];
			}else{
				mods::builder::report_error(player,"not a valid number");
				return;
			}
			if(!obj){
				mods::builder::report_error(player,"Index not found.");
				return;
			}
			auto aff_index = mods::util::stoi(arg_vec[3]);
			auto location = mods::util::stoi(arg_vec[4]);
			auto modifier = mods::util::stoi(arg_vec[5]);
			if(aff_index.has_value() && 
					location.has_value() && modifier.has_value()){
				if(aff_index.value() >= MAX_OBJ_AFFECT){
					mods::builder::report_error(player,std::string("aff_index must be less than ") + std::to_string(MAX_OBJ_AFFECT));
					return;
				}
				obj->affected[aff_index.value()].location = location.value();
				obj->affected[aff_index.value()].modifier = modifier.value();
				mods::builder::report_success(player,"Object updated");
				return;
			}else{
				mods::builder::report_error(player,"location and modifier are required");
				return;
			}
		}
		mods::builder::report_error(player,"Unknown mode");
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"show");
	if(args.has_value()){
		//TODO: !mundane make this a function
		auto arg_vec = args.value();
		if(arg_vec.size() == 1){
			mods::builder::report_error(player,"Please supply an object id");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
		obj_data * obj = nullptr;
		unsigned ctr = 0;
		if(index.has_value()){
			if(index.value() >= obj_proto.size()){
				mods::builder::report_error(player," Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		}else{
			mods::builder::report_error(player,"Not a valid number");
			return;
		}
		if(!obj){
			mods::builder::report_error(player,"Index not found.");
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
			auto ex_desc = obj->ex_description;
			unsigned ex_ctr = 0;
			for(; ex_desc ; ex_desc = ex_desc->next){
				*player << "{red}[" << ex_ctr << "]keyword: {/red}" << ex_desc->keyword << "\r\n";
				*player << "{red}[" << ex_ctr << "]description: {/red}" << ex_desc->description << "\r\n";
				++ex_ctr;
			}
			if(!ex_ctr){
				*player << "\r\n";
			}
			*player << "{red}item_number: {/red}" << obj->item_number << "\r\n" << 
			"{red}weapon_type: {/red}" << obj->weapon_type << "\r\n" << 
			"{red}worn_on: {/red}" << obj->worn_on << "\r\n" << 
			"{red}weapon_ammo: {/red}" << obj->ammo << "\r\n" << 
			"{red}weapon_ammo_max: {/red} " << obj->ammo_max << "\r\n" << 
			"{red}weapon_holds_ammo: {/red}: " << obj->holds_ammo << "\r\n" << 
			"{gld}::Wear Flags::{/gld}\r\n" << 
			"{red}value: {/red}" << std::to_string(obj->obj_flags.wear_flags ) <<  "\r\n";
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
		for(unsigned index = 0;
			index < MAX_OBJ_AFFECT; index++){
			*player << "affected[" << index << "]->location: {/red} " <<
				obj->affected[index].location << 
				" {red}modifier: {/red} ";
			for(auto & flag : mods::builder::affected_flags){
				if(flag.first == obj->affected[index].modifier){
					*player << flag.second;
					break;
				}
			}
			*player << "\r\n";
		}
		player->pager_end();
		player->page(0);
		return;
	}
	args = mods::util::subcmd_args<5,args_t>(argument,"attr");
	if(args.has_value()){
		auto arg_vec = args.value();
		auto get_intval = [&](std::string_view str) -> std::optional<int>{
			if(arg_vec[2].compare(str.data()) == 0){
				*player << "matches: " << str.data() << "\r\n";
				auto i_value = mods::util::stoi(arg_vec[3]);
				if(!i_value.has_value()){
					mods::builder::report_error(player,"Please use a valid numeric value.");
					return std::nullopt;
				}
				return i_value.value();
			}
			return std::nullopt;
		};
		auto get_strval = [&](std::string_view str) -> std::optional<char*>{
			*player << arg_vec[2] << "\r\n";
			if(arg_vec[2].compare(str.data()) == 0){
				*player << "matches: " << str.data() << "\r\n";
				return strdup(arg_vec[3].c_str());
			}
			return std::nullopt;
		};
		auto index = mods::util::stoi(args.value()[1]);
		obj_data * obj = nullptr;
		unsigned ctr = 0;
		if(index.has_value()){
			if(index.value() >= obj_proto.size()){
				mods::builder::report_error(player,"Out of bounds");
				return;
			}
			obj = &obj_proto[index.value()];
		}
		if(!obj){
			mods::builder::report_error(player,"Invalid index");
			return;
		}
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
		if(arg_vec[2].compare("affected") == 0){
			if(arg_vec.size() < 6){
				mods::builder::report_error(player,"Not enough arguments");
				return;
			}
			auto index = mods::util::stoi(arg_vec[3]);
			if(!index.has_value()){
				mods::builder::report_error(player,"Invalid index");
				return;
			}else{
				if(index.value() > MAX_OBJ_AFFECT){
					mods::builder::report_error(player,std::string(
						"index cannot be larger than ") + std::to_string(MAX_OBJ_AFFECT));
					return;
				}
			}
			auto location = mods::util::stoi(arg_vec[4]);
			if(!location.has_value()){
				mods::builder::report_error(player,"Invalid location");
				return;
			}
			auto modifier = mods::util::stoi(arg_vec[5]);
			if(!modifier.has_value()){
				mods::builder::report_error(player,"Invalid modifier");
				return;
			}
			obj->affected[index.value()].location = location.value();
			obj->affected[index.value()].modifier = modifier.value();
		}
		mods::builder::report_success(player,"Set attribute");
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
				    
				   " zbuild list\r\n" <<
				   "  |--> lists the current zones saved to the db\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: zbuild list\r\n"<<
				    
				   " zbuild delete <id>...<N>\r\n" <<
				   "  |--> deletes the zone from the db with the id <id>. Multiple IDs can be specified\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: zbuild delete 1\r\n" <<
				    
				   " zbuild place <zone_id> <command> <if_flag> <arg1> <arg2> <arg3>\r\n" <<
				   "  |--> creates a reset command for the zone 'zone_id'.\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: zbuild place 5 M 0 1500 500 300\r\n" <<
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
"\r\n"<<
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
			mods::builder::report_error(player," Invalid zone id");
			return;
		}
		auto zone_command = args[1];
		auto if_flag = args[2];
		auto arg1 = args[3];
		auto arg2 = args[4];
		auto arg3 = args[5];
		mods::builder::zone_place(zone_id.value(),zone_command,if_flag,arg1,arg2,arg3);
		mods::builder::report_success(player,"Placed object in zone");
		return;
	}
	if(std::string(&command[0]).compare("delete") == 0){
		std::string arg = argument;
		auto past = arg.substr(arg.find("delete ")+7);
		for(auto id : mods::util::arglist<std::vector<std::string>>(past)){
			auto zone = mods::util::stoi(id);
			if(zone.value_or(-1) == -1){
				mods::builder::report_error(player,"Unable to delete id");
				return;
			}
			mods::builder::delete_zone(zone.value());
			mods::builder::report_success(player,"Deleted zone");
		}
		return;
	}
	if(std::string(&command[0]).compare("new") == 0){
		std::string arg = argument;
		auto past = arg.substr(arg.find("new ")+4);
		//TODO: take this logic and store it in interpreter.cpp so we can reuse it
		auto arglist = mods::util::arglist<std::vector<std::string>>(past);
		if(arglist.size() != 5){
			mods::builder::report_error(player,"Argument list is invalid, please use the correct number of arguments");
			for(auto vstr: arglist){
				*player << vstr << "\r\n";
			}
		}else{
			// " rbuildzone <new> <zone start> <zone end> <zone name> <zone lifespan> <zone reset mode>\r\n" <<
			auto arg_0 = mods::util::stoi(arglist[0]);
			if(arg_0.value_or(-1) == -1){
				mods::builder::report_error(player,"Invalid parameter 1");
				return;
			}
			auto arg_1 = mods::util::stoi(arglist[1]);
			if(arg_1.value_or(-1) == -1){
				mods::builder::report_error(player,"Invalid parameter 2");
				return;
			}
			auto arg_3 = mods::util::stoi(arglist[3]);
			if(arg_3.value_or(-1) == -1){
				mods::builder::report_error(player,"Invalid parameter 4");
				return;
			}
			auto arg_4 = mods::util::stoi(arglist[4]);
			if(arg_4.value_or(-1) == -1){
				mods::builder::report_error(player,"Invalid parameter 5");
				return;
			}
			if(!mods::builder::save_zone_to_db(arglist[2],arg_0.value(),arg_1.value(),arg_3.value(),arg_4.value())){
				mods::builder::report_error(player,"Unable to save new zone");
			}else{
				mods::builder::report_success(player,"New zone created");
			}
		}
		return;
	}
	if(std::string(&command[0]).compare("list") == 0){
		mods::builder::report_status(player,"listing...");
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
				    
				   " rbuild <room>\r\n" <<
				   "  |--> get the real room number of the room\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: rbuild room\r\n"<<
				    
				   " rbuild <save>\r\n" <<
				   "  |--> save the current room you are standing in\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: rbuild save\r\n"<<
				    
				   " rbuild <create> <direction>\r\n" <<
		           "  |--> creates a room to the direction you choose (neswud)\r\n" << 
				   "  |____[example]\r\n" <<
				   "  |:: rbuild create north\r\n" <<
				   "  |:: (the room to the north will be a brand new defaulted room)\r\n" <<
				    
				   " rbuild <bind> <direction> <room_rnum>\r\n" << 
				   "  |--> bind a room to a direction\r\n" <<
				   "  |____[example]\r\n"<<
				   "  |:: rbuild bind north 27\r\n"<<
				   "  |:: (the room to the north will lead to room 27)\r\n" <<
				    
			       " rbuild <title> <string>\r\n" <<
				   "  |--> set the current room title to string\r\n" << 
				   "  |____[example]\r\n" <<
				   "  |:: rbuild title Taco Bell Employee Lounge\r\n" <<
				   "  |:: (the room's title will be the above title)\r\n" <<
				   
				   " rbuild <description> <string>\r\n" <<
				   "  |--> set the current room description to string\r\n" <<
				   "  |____[example]\r\n" <<
				   "  |:: rbuild description The room is filled with boxes of taco bell...\r\n" <<
				   "  |:: (the room's description will be the above description)\r\n" <<
				   
				   " rbuild <destroy> <direction>\r\n" <<
				   "  |--> destroy a room direction\r\n" <<
				   "  |____[examples]\r\n" <<
				   "  |:: rbuild destroy north\r\n" <<
				   "  |:: (north will no longer be an exit)\r\n" <<
				   
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
		mods::builder::report_error(player,"Room created");
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
			mods::builder::report_error(player,"Invalid number");
			return;
		}
		world[IN_ROOM(ch)].number = number.value();
		mods::builder::report_success(player,std::string("real room number set to ") + std::to_string(number.value()));
		return;
	}
	if(std::string(&command[0]).compare("title") == 0){
		auto title = std::string(argument);
		title = title.substr(title.find("title ") + 6);
		if(mods::builder::title(IN_ROOM(ch),title)){
			mods::builder::report_success(player,"Title changed");
		}else{
			mods::builder::report_error(player,"Error");
		}
		return;
	}
	if(std::string(&command[0]).compare("description") == 0){
		auto description = std::string(argument);
		description = description.substr(description.find("description ") + 12);
		if(mods::builder::description(IN_ROOM(ch),description + "\r\n")){
			mods::builder::report_success(player,"Description changed");
		}else{
			mods::builder::report_error(player,"Error");
		}
		return;
	}
	if(std::string(&command[0]).compare("save") == 0){
		auto ret = mods::builder::save_to_db(IN_ROOM(ch));
		if(ret != 0){
			mods::builder::report_error(player,std::string("Error saving room: ") + std::to_string(ret));
		}else{
			mods::builder::report_success(player,"Room saved");
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
			mods::builder::report_error(player,"Invalid direction");
			return;
		}
		auto room = mods::util::stoi(&room_id[0]);
		if(room.value_or(-1) == -1 || room > mods::globals::room_list.size()){
			mods::builder::report_error(player,"Invalid room number");
			return;
		}
		if(mods::builder::create_direction(IN_ROOM(ch),mods::globals::dir_int(direction[0]),room.value())){
			mods::builder::report_success(player,"Direction created");
		}else{
			mods::builder::report_error(player,"Error");
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
			mods::builder::report_error(player,"Invalid direction");
			return;
		}
		if(!mods::builder::destroy_direction(IN_ROOM(ch),mods::globals::dir_int(direction[0]))){
			mods::builder::report_error(player,"Unable to destroy direction");
		}else{
			mods::builder::report_success(player,"Direction destroyed");
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
				mods::builder::report_success(player,"General Description changed");
			}else{
				mods::builder::report_error(player,ret);
			}
			return;
		}
		if(str_item.compare("keyword") == 0){
			description = description.substr(description.find("keyword ") + 8);
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				description,std::nullopt,std::nullopt,std::nullopt).value_or("success");
			if(ret.compare("success") == 0){
				mods::builder::report_success(player,"Keyword changed");
			}else{
				mods::builder::report_error(player,ret);
			}
			return;
		}
		if(str_item.compare("einfo") == 0){
			description = description.substr(description.find("einfo ") + 6);
			auto exit_info = mods::util::stoi(description);
			if(!exit_info.has_value()){
				mods::builder::report_error(player,"Invalid exit info. Must be 0-3");
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				std::nullopt,exit_info.value(),std::nullopt,std::nullopt).value_or("success");
			if(ret.compare("success") == 0){
				mods::builder::report_success(player,"exit_info changed to: EX_ISDOOR");
			}else{
				mods::builder::report_error(player,ret);
			}
			return;
		}
		if(str_item.compare("key") == 0){
			description = description.substr(description.find("key ") + 4);
			auto key = mods::util::stoi(description);
			if(key.value_or(-1) == -1){
				mods::builder::report_error(player,"Invalid key");
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				std::nullopt,std::nullopt,key.value(),std::nullopt).value_or("success");
			if(ret.compare("success") == 0){
				mods::builder::report_success(player,"key changed");
			}else{
				mods::builder::report_error(player,ret);
			}
			return;
		}
		if(str_item.compare("to_room") == 0){
			description = description.substr(description.find("to_room ") + 8);
			auto to_room = mods::util::stoi(description);
			if(to_room.value_or(-1) == -1){
				mods::builder::report_error(player,"Invalid room number");
				return;
			}
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
				std::nullopt,std::nullopt,std::nullopt,to_room.value()).value_or("success");
			if(ret.compare("success") == 0){
				mods::builder::report_success(player,"to_room changed");
			}else{
				mods::builder::report_error(player,ret);
			}
			return;
		}
	}
};
