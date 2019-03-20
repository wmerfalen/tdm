#include "builder.hpp"
#include "quests.hpp"
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
namespace mods {  struct player; };
namespace mods { struct extra_desc_data; }; 
#define MENTOC_OBI(i) obj->i = get_intval(#i).value_or(obj->i);
#define MENTOC_OBI2(i,a) obj->i = get_intval(#a).value_or(obj->i);
#define MENTOC_OBS(i) obj->i = get_strval(#i).value_or(obj->i);
#define MENTOC_OBS2(i,a) obj->i = get_strval(#a).value_or(obj->i.c_str());
using objtype = mods::object::type;
using shrd_ptr_player_t = std::shared_ptr<mods::player>;
using jxcomp = mods::jx::compositor;
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
extern std::tuple<int16_t,std::string> parse_sql_zones();
void r_error(const shrd_ptr_player_t & player,std::string_view msg){
	mods::builder::report_error<shrd_ptr_player_t>(player,msg.data());
}
void r_success(const shrd_ptr_player_t & player,std::string_view msg){
	mods::builder::report_success<shrd_ptr_player_t>(player,msg.data());
}
void r_status(const shrd_ptr_player_t & player,std::string_view msg){
	mods::builder::report_status<shrd_ptr_player_t>(player,msg.data());
}
namespace mods::builder {
	bool has_flag(std::shared_ptr<mods::player> player,uint64_t flag){
		return player->has_flag(mods::flags::chunk_type_t::BUILDER,flag);
	}
	std::array<std::pair<int,std::string>,4> weapon_type_flags = { {
			{mods::weapon::SMG,"SMG"},
			{mods::weapon::SHOTGUN,"SHOTGUN"},
			{mods::weapon::SNIPE,"SNIPE"},
			{mods::weapon::GRENADE,"GRENADE"}
		}
	};
	std::array<std::pair<int,std::string>,3> sex_flags = { {
			{SEX_NEUTRAL,"NEUTRAL"},
			{SEX_MALE,"MALE"},
			{SEX_FEMALE,"FEMALE"}
		}
	};

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
		}
	};

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
		}
	};
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
		}
	};

	std::array<std::pair<int,std::string>,3> type2_flags = { {
			{objtype::WEAPON,"WEAPON"},
			{objtype::CONTAINER,"CONTAINER"},
			{objtype::SENTINEL,"SENTINEL"}
		}
	};
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
		}
	};

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
		}
	};
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
		}
	};
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
		}
	};
	sandbox_list_t sandboxes;
	sandbox_data_t::sandbox_data_t(
				std::shared_ptr<mods::player> player,
				std::string_view name,
				room_vnum starting_room_number){
		 new_sandbox(player,name,starting_room_number);
	}
	sandbox_data_t::sandbox_data_t() : m_name("") {}
	void sandbox_data_t::set_name(std::string_view n){
		m_name = n;
	}
	std::string_view sandbox_data_t::name() const {
		return m_name;
	}
	int8_t sandbox_data_t::new_sandbox(
				std::shared_ptr<mods::player> player,
				std::string_view name,
				int starting_room_number){
		m_player = player;
			m_builder_data = std::make_shared<builder_data_t>();
			m_name = name;
			bool status = save_zone_to_db(
					starting_room_number,
					name,
					starting_room_number,
					starting_room_number + 100,
					100,
					2
			);
			m_builder_data->room_pave_mode = true;
			m_builder_data->room_pavements.start_room = starting_room_number;
			m_builder_data->room_pavements.zone_id = 0;
			if(!status){
				r_error(player,"Unable to create new zone!");
				m_builder_data->room_pavements.zone_id = 0;
				return -1;
			}else{
				for(auto && row : db_get_by_meta("zone","zone_name",name.data())){
					m_builder_data->room_pavements.zone_id = row["id"].as<int>(0);
					break;
				}
			}
			mods::builder::new_room(player,NORTH);
			auto & room = world.back();
			room.number = starting_room_number;
			room.zone = m_builder_data->room_pavements.zone_id;
			room.name.assign("new room");
			room.description.assign("description goes here");
			return 0;
		}
	/* Factory method to generate a room for us */
	room_data new_room(std::shared_ptr<mods::player> player,int direction);
	bool flush_to_db(struct char_data *ch,room_vnum room);
	std::optional<obj_data*> instantiate_object_by_index(int index) {
		std::size_t i = index;

		if(i >= obj_proto.size()) {
			return std::nullopt;
		}

		object_list.push_back(obj_proto[index]);
		return &(*(object_list.end()-1));
	}
	std::optional<obj_data*> instantiate_object_by_vnum(obj_vnum vnum) {
		obj_data* obj;

		for(auto& object_reference : obj_proto) {
			if(object_reference.item_number == vnum) {
				obj = &object_reference;
				object_list.push_back(object_reference);
				return obj;
			}
		}

		return std::nullopt;
	}

	inline void initialize_builder(std::shared_ptr<mods::player> player){
		//if(!player->has_builder_data()){
		//	std::cerr << "initialize_builder: creating shared_ptr\n";
		//	player->builder_data = std::make_shared<builder_data_t>();
		//}else{
		//	std::cerr << "initialize_builder: shared_ptr already built\n";
		//}
		player->set_bui_mode(true);
	}
	void pave_to(std::shared_ptr<mods::player> player,room_data * current_room,int direction) {
		initialize_builder(player);
		r_status(player,
				std::string("start_room: ") + 
				std::to_string(player->builder_data->room_pavements.start_room)
		);
		mods::builder::new_room(player,direction);
		
		auto & builder_data = player->builder_data;
		if(has_flag(player,mods::builder::HAS_SANDBOX)){
			builder_data = mods::builder::sandboxes[player->name().c_str()].begin()->builder_data();
		}
		builder_data->room_pavements.rooms.push_back(world.size()-1);
		auto created_room = world.back();
		created_room.number = builder_data->room_pavements.start_room + builder_data->room_pavements.current_room_number;
		created_room.zone = builder_data->room_pavements.zone_id;

		if(!create_direction(player->room(),direction,world.size() -1)) {
			r_error(player,"Couldn't create direction to that room!");
			return;
		}

		if(!create_direction(world.size()-1,OPPOSITE_DIR(direction),player->room())) {
			r_error(player,"Couldn't create bind-back direction to that room!");
			return;
		}

		world.back().dir_option[OPPOSITE_DIR(direction)]->general_description = world[player->room()].name;
		world.back().dir_option[OPPOSITE_DIR(direction)]->keyword = "door";
		r_status(player,"Paved room to that direction");
	}

	std::pair<bool,std::string> update_zone_commands(int zone_id) {
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
					{"zone_id",std::to_string(zone_table[zone_id].number)},
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
			return {false,std::string("Saving zone commands failed: ") + e.what()};
		}

		return {true,"Saved all zone commands successfully"};
	}
	bool save_zone_to_db(int64_t virtual_number,std::string_view name,int room_start,int room_end,int lifespan,int reset_mode) {
		try{
		auto txn2 = txn();
		sql_compositor comp("room_direction_data",&txn2);
		auto sql = comp
			.insert()
			.into("zone")
			.values({
					{"zone_virtual_number",std::to_string(virtual_number)},
					{"zone_start",std::to_string(room_start)},
					{"zone_end",std::to_string(room_end)},
					{"zone_name",name.data()},
					{"lifespan",std::to_string(lifespan)},
					{"reset_mode",std::to_string(reset_mode)}})
		.sql();
		mods::pq::exec(txn2,sql);
		mods::pq::commit(txn2);
		}catch(std::exception& e){
			std::cerr << "Exception with save_zone_to_db: " << e.what() << "\n";
			return false;
		}
		return true;
	}
	int save_to_db(room_rnum in_room) {
		auto world_top = mods::globals::room_list.size();
		std::size_t ir = in_room;

		if(ir >= world_top) {
			return mods::builder::ROOM_NUMBER_OUT_OF_RANGE;
		}

		if(!world[in_room].name) {
			return mods::builder::ROOM_NAME_EMPTY;
		}

		if(!world[in_room].description) {
			return mods::builder::ROOM_DESC_EMPTY;
		}

		std::map<std::string,std::string> values;
		values["zone"] = std::to_string(world[in_room].zone);
		values["sector_type"] = std::to_string(world[in_room].sector_type);
		values["name"] = world[in_room].name.c_str();
		values["description"] = world[in_room].description.c_str();
		values["room_number"] = std::to_string(world[in_room].number);


		std::array<char,16> num;
		std::fill(num.begin(),num.end(),0);
		sprintf(&num[0],"%d",world[in_room].light);
		values["light"] = &num[0];
		std::fill(num.begin(),num.end(),0);
		sprintf(&num[0],"%d",world[in_room].room_flags);
		values["room_flag"] = &num[0];
		pqxx::result room_record;

		try{
			auto up_txn = txn();
			sql_compositor comp("room",&up_txn);
			auto room_sql = comp.select("room_number")
				.from("room")
				.where("room_number","=",std::to_string(world[in_room].number))
				.sql();
			room_record = mods::pq::exec(up_txn,room_sql);
		}catch(std::exception& e){
			std::cerr << "error selecting room from db: '" << e.what() << "'\n";
			return -4;
		}
		if(room_record.size()){
			/* update the record */
			try{
				auto up_txn = txn();
				sql_compositor comp("room",&up_txn);
				auto up_sql = comp
					.update("room")
					.set(values)
					.where("id","=",room_record[0]["id"].c_str())
					.sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
			}catch(std::exception& e){
				std::cerr << "error updating room in db: '" << e.what() << "'\n";
				return -3;
			}
		} else {
			auto txn2 = txn();
			sql_compositor comp("room",&txn2);
			auto sql = comp
				.insert()
				.into("room")
				.values(values).sql();
			mods::pq::exec(txn2,sql);
			mods::pq::commit(txn2);
		}

		auto del_txn = txn();
		mods::pq::exec(del_txn,std::string("DELETE FROM room_direction_data where room_number=") + std::to_string(world[in_room].number));
		mods::pq::commit(del_txn);

		for(auto direction = 0; direction < NUM_OF_DIRS; direction++) {
			if(world[in_room].dir_option[direction] &&
					world[in_room].dir_option[direction]->general_description) {
				auto check_txn = txn();
				sql_compositor comp("room_direction_data",&check_txn);
				std::string check_sql = comp.
					select("room_number")
					.from("room_direction_data")
					.where("room_number","=",std::to_string(world[in_room].number))
					.op_and("exit_direction","=",std::to_string(direction))
					.sql().data();
				check_sql += " AND exit_direction=";
				check_sql += check_txn.quote(direction);
				auto check_result = mods::pq::exec(check_txn,check_sql);
				auto real_room_num = world[world[in_room].dir_option[direction]->to_room].number;
				std::map<std::string,std::string> values = {
					{"general_description",static_cast<std::string>(world[in_room].dir_option[direction]->general_description)},
					{"keyword",static_cast<std::string>(world[in_room].dir_option[direction]->keyword)},
					{"exit_info",std::to_string(world[in_room].dir_option[direction]->exit_info)},
					{"exit_key",std::to_string(world[in_room].dir_option[direction]->key)},
					{"to_room",std::to_string(real_room_num)},
					{"room_number",std::to_string(world[in_room].number)},
					{"exit_direction",std::to_string(direction)}
				};

				if(check_result.size()) {
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
				} else {
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

		if(world[in_room].ex_descriptions().size()){
			try{
				auto del_txn = txn();
				mods::pq::exec(del_txn,std::string("DELETE FROM room_extra_desc_data where room_number=") 
						+ std::to_string(world[in_room].number));
			}catch(std::exception &e){ }
			mods::pq::commit(del_txn);
			for(const auto & ex_desc : world[in_room].ex_descriptions()){
				values.clear();
				values["ex_keyword"] = ex_desc.keyword.c_str();
				values["ex_description"] = ex_desc.description.c_str();
				values["room_number"] = std::to_string(world[in_room].number);
				try{
					auto txn2 = txn();
					sql_compositor comp("room_extra_desc_data",&txn2);
					auto sql = comp
						.insert()
						.into("room_extra_desc_data")
						.values(values).sql();
					mods::pq::exec(txn2,sql);
					mods::pq::commit(txn2);
				}catch(std::exception& e){
					std::cerr << "[room_extra_desc_data]-> error: '" << e.what() << "'\n";
				}
			}
		}

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
		mods::globals::register_room(world_top);
		return 0;
	}
	room_data new_room(std::shared_ptr<mods::player> player,int direction) {
		world.emplace_back();
		mods::globals::register_room(world.size());
		world.back().init();
		return world.back();
	}
	bool title(room_rnum room_id,std::string_view str_title) {
		std::size_t rid = room_id;

		if(rid < mods::globals::room_list.size()) {
			world[room_id].name = strdup(str_title.data());
			return true;
		}

		return false;
	}
	bool description(room_rnum room_id,std::string_view str_description) {
		std::size_t rid = room_id;

		if(rid < mods::globals::room_list.size()) {
			world[room_id].description = strdup(str_description.data());
			return true;
		}

		return false;
	}
	bool flush_to_db(struct char_data *ch,room_vnum room) {
		MENTOC_PREAMBLE();
		*player << "flush_to_db[stub]\r\n";
		return true;
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

		if(description.value_or("-1").compare("-1") != 0) {
			world[room_id].dir_option[direction]->general_description = strdup(description.value().data());
		}

		if(keywords.value_or("-1").compare("-1") != 0) {
			world[room_id].dir_option[direction]->keyword = strdup(keywords.value().data());
		}

		world[room_id].dir_option[direction]->exit_info = exit_info.value_or(world[room_id].dir_option[direction]->exit_info);
		world[room_id].dir_option[direction]->key = key.value_or(world[room_id].dir_option[direction]->key);
		world[room_id].dir_option[direction]->to_room = to_room.value_or(world[room_id].dir_option[direction]->to_room);
		return std::nullopt;
	}
	bool create_direction(room_rnum room_id,byte direction,room_rnum to_room) {
		if(direction > NUM_OF_DIRS) {
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
	bool delete_zone(int id) {
		auto t = mods::pq::transaction(*mods::globals::pq_con);
		std::string delete_sql = std::string("DELETE FROM zone where id=") + t.quote(id);
		mods::pq::exec(t,delete_sql);
		mods::pq::commit(t);
		return true;
	}
	std::pair<bool,std::string> zone_place(int zone_id,std::string_view zone_command,std::string_view if_flag,std::string_view arg1,std::string_view arg2,std::string_view arg3) {
		try{
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
			return {false,std::string("Exception occurred: ") + e.what()};
		}
		return {true,"Saved zone successfully."};
	}
	char_data new_blank_character(bool npc = false) {
		char_data proto;
		memset(&proto,0,sizeof(proto));
		const char* foo = "foo";
		proto.player.name = strdup(foo);
		proto.player.short_descr = strdup(foo);
		proto.player.long_descr = strdup(foo);
		proto.player.description = strdup(foo);

		if(npc) {
			SET_BIT(proto.char_specials.saved.act, MOB_ISNPC);
		} else {
			REMOVE_BIT(proto.char_specials.saved.act, MOB_ISNPC);
		}

		REMOVE_BIT(proto.char_specials.saved.act, MOB_NOTDEADYET);
		unsigned j = 0;

		for(; j < 5; j++) {
			GET_SAVE(&proto, j) = 0;
		}

		proto.aff_abils = proto.real_abils;
		return proto;
	}
	char_data new_npc() {
		return new_blank_character(true);
	}
	std::pair<bool,std::string> save_player(char_data* obj) {
		try {
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
			p_map["mob_load_position"] = p_map["mob_default_position"];//mods::util::itoa(GET_POS(obj));
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

			if(result_01.size()) {
				//Update
				sql = sql_compositor("mobile",&txn_02)
					.update("mobile")
					.set(p_map)
					.where("mob_virtual_number","=",mods::util::itoa(obj->nr))
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
			return {false,e.what()};
		}

		return {true,"Successfully saved player."};
	}
	std::pair<bool,std::string> save_object(obj_data* obj) {
		try {
			auto txn_01 = txn();
			sql_compositor comp3("object",&txn_01);
			auto sql = comp3.select("id").from("object").
				where("object_item_number","=",std::to_string(obj->item_number))
				.sql();
			auto check_result_01 = mods::pq::exec(txn_01,sql);
			mods::pq::commit(txn_01);
			auto check_i = 0;
			sql_compositor::value_map my_map;
			my_map["object_item_number"] = std::to_string(obj->item_number);
			my_map["obj_flags"] = "0";
#define MENTOC_CHK_OBJ(item) if(!obj->item){ return {false,std::string(#item) + " is empty"}; }
			MENTOC_CHK_OBJ(name);
			MENTOC_CHK_OBJ(description);
			MENTOC_CHK_OBJ(short_description);
			MENTOC_CHK_OBJ(action_description);
			my_map["object_name"] = obj->name;
			my_map["object_description"] = obj->description;
			my_map["object_short_description"] = obj->short_description;

			if(obj->action_description) {
				my_map["object_action_description"] = obj->action_description;
			}

			my_map["object_type"] = std::to_string(obj->type);
			my_map["object_worn_on"] = std::to_string(obj->worn_on);
			my_map["object_type_data"] = "0";

			if(obj->ex_description && obj->ex_description->keyword) {
				my_map["object_extra_keyword"] = obj->ex_description->keyword;
			} else {
				return {false,"atleast one ex_description->keyword is required"};
			}

			if(obj->ex_description && obj->ex_description->description) {
				my_map["object_extra_description"] = obj->ex_description->description;
			} else {
				return {false,"atleast one ex_description->description is required"};
			}

			if(check_result_01.size()) {
				check_i = mods::pq::as_int(check_result_01,0,0);
				/* update the fields */
				auto t = txn();
				sql_compositor comp("object",&t);
				auto update_sql = comp
					.update("object")
					.set(my_map)
					.where("object_item_number","=",std::to_string(obj->item_number))
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
					.where("object_item_number","=",std::to_string(obj->item_number))
					.sql();
				auto res = mods::pq::exec(txn4,sql);
				mods::pq::commit(txn4);

				if(res.size()) {
					check_i = mods::pq::as_int(res,0,0);
				}
			}

			switch(obj->obj_flags.type_flag) {
				case ITEM_WEAPON:
					auto txn3 = txn();
					sql_compositor comp3("object_weapon",&txn3);
					auto sql = comp3.select("id").from("object_weapon").
						where("object_number","=",std::to_string(check_i))
						.sql();
					auto check_result = mods::pq::exec(txn3,sql);
					mods::pq::commit(txn3);

					if(check_result.size()) {
						auto txn5 = txn();
						sql_compositor comp("object_weapon",&txn5);
						auto sql = comp.update("object_weapon")
							.set({
									{"object_ammo_type",std::to_string(obj->weapon_type)},
									{"object_ammo_max",std::to_string(obj->ammo_max)},
									{"object_cooldown","0"},
									{"object_can_snipe","0"}
									}).where("object_number","=",std::to_string(check_i))
						.sql();
						mods::pq::exec(txn5,sql);
						mods::pq::commit(txn5);
					} else {
						auto txn6 = txn();
						sql_compositor comp("object_weapon",&txn6);
						auto sql = comp.insert().into("object_weapon")
							.values({
									{"object_number",std::to_string(check_i)},
									{"object_ammo_max",std::to_string(obj->ammo_max)},
									{"object_ammo_type",std::to_string(obj->weapon_type)},
									{"object_cooldown","0"},
									{"object_can_snipe","0"}
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
						"select id from affected_type where object_number="	//TODO: !mundane use new shit
						) + txn6.quote(check_i);
			auto check_result = mods::pq::exec(txn6,sel_weapon_sql);
			mods::pq::commit(txn6);

			if(check_result.size()) {
				auto txn8 = txn();
				std::string sql = std::string(
						"DELETE FROM affected_type where object_number=") 	//TODO: !mundane use new shit
					+ txn8.quote(check_i);
				mods::pq::exec(txn8,sql);
				mods::pq::commit(txn8);
			}

			auto txn9 = txn();
			auto sql9 = std::string(
					"DELETE FROM obj_flags where object_number=")
				+ txn9.quote(check_i)
				;
			mods::pq::exec(txn9,sql9);
			mods::pq::commit(txn9);
			auto txn10 = txn();
			auto sql10 = sql_compositor("obj_flags",&txn10)
				.insert()
				.into("obj_flags")
				.values({
						{"object_number",std::to_string(check_i)},
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

			for(unsigned i =0; i < MAX_OBJ_AFFECT; i++) {
				auto txn7 = txn();
				std::string loc,mod;

				if(obj->affected[i].location && obj->affected[i].modifier) {
					auto sql = sql_compositor("affected_type",&txn7).insert()
						.into("affected_type")
						.values({
								{"object_number",std::to_string(check_i)},
								{"aff_location",mods::util::itoa(obj->affected[i].location)},
								{"aff_modifier", mods::util::itoa(obj->affected[i].modifier)}
								})
					.sql();
					mods::pq::exec(txn7,sql);
					mods::pq::commit(txn7);
				}
			}

			/* save ex_description linked lists */
			for(auto ex_desc = obj->ex_description; ex_desc; ex_desc = ex_desc->next) {
				if(!ex_desc->keyword || !ex_desc->description) {
					break;
				}

				auto txn8 = txn();
				mods::pq::exec(txn8,sql_compositor("extra_description",&txn8)
						.insert()
						.into("extra_description")
						.values({
							{"object_number",std::to_string(check_i)},
							{"extra_keyword",ex_desc->keyword},
							{"extra_description",ex_desc->description}
							})
						.sql()
						);
			}
		} catch(std::exception& e) {
			return {false,std::string("Exception occurred: ") + e.what()};
		}

		return {true,"saved successfully"};
	}
};

using args_t = std::vector<std::string>;

ACMD(do_rbuild_sandbox) {
	MENTOC_PREAMBLE();
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
			" {grn}rbuild_sandbox{/grn} {red}new <name> <room_number_start>{/red}\r\n" <<
			" {grn}rbuild_sandbox{/grn} {red}list{/red}\r\n" <<
			" {grn}rbuild_sandbox{/grn} {red}delete <id>{/red}\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
	}

	{
	auto args = mods::util::subcmd_args<4,args_t>(argument,"new");

	if(args.has_value()) {
		auto cmd_args = args.value();
		if(cmd_args.size() == 3){
			/**
			 * cmd_args will be: [0] => new, [1] => <name>, [2] => <starting_room_number>
			 */
			mods::builder::sandbox_data_t sandbox;
			auto status = sandbox.new_sandbox(player,cmd_args[1],mods::util::stoi<int>(cmd_args[2]));
			if(status < 0){
				r_error(player,std::string("Failed to create sandbox! Error: #") + std::to_string(status));
				return;
			}else{
				mods::builder::sandboxes[player->name().c_str()].emplace_back(std::move(sandbox));
				player->set_flag(mods::flags::chunk_type_t::BUILDER,mods::builder::HAS_SANDBOX);
				r_success(player,"Sandbox created");
				return;
			}
		}else{
			r_error(player,"Invalid arguments. Arguments should be: new <name> <starting_room_number>");
			return;
		}
	}
	}

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
		if(args.has_value()){
			auto cmd_args = args.value();
			if(cmd_args.size() == 2){
				std::size_t sandbox_count = mods::builder::sandboxes[player->name().c_str()].size();
				std::size_t index = mods::util::stoi<std::size_t>(cmd_args[1]);
				if(index >= sandbox_count){
					r_error(player,"Index is too large");
					return;
				}else{
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
ACMD(do_sbuild) {
	MENTOC_PREAMBLE();
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));

	if(vec_args.size() == 0 || vec_args[0].compare("help") == 0) {
		player->pager_start();
		*player << "usage: \r\n" <<
			" {red}sbuild{/red} {grn}help{/grn}\r\n" <<
			"  |--> this help menu\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {red}sbuild{/red} help\r\n" <<
			"  |:: (this help menu will show up)\r\n" <<
			" {red}sbuild{/red} {grn}new{/grn}\r\n" <<
			" {red}sbuild{/red} {grn}list{/grn}\r\n" <<
			" {red}sbuild{/red} {grn}attr{/grn} <shop_id> <attr> <value>\r\n" <<
			"  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
			"  {gld}|:: vnum{/gld}\r\n" <<
			"  {gld}|:: profit_buy{/gld}\r\n" <<
			"  {gld}|:: profit_sell{/gld}\r\n" <<
			"  {gld}|:: type{/gld}\r\n" <<
			"  {gld}|:: no_such_item1{/gld}\r\n" <<
			"  {gld}|:: no_such_item2{/gld}\r\n" <<
			"  {gld}|:: missing_cash1{/gld}\r\n" <<
			"  {gld}|:: missing_cash2{/gld}\r\n" <<
			"  {gld}|:: do_not_buy{/gld}\r\n" <<
			"  {gld}|:: message_buy{/gld}\r\n" <<
			"  {gld}|:: message_sell{/gld}\r\n" <<
			"  {gld}|:: temper1{/gld}\r\n" <<
			"  {gld}|:: bitvector{/gld}\r\n" <<
			"  {gld}|:: keeper{/gld}\r\n" <<
			"  {gld}|:: with_who{/gld}\r\n" <<
			"  {gld}|:: in_room{/gld}\r\n" <<
			"  {gld}|:: open1{/gld}\r\n" <<
			"  {gld}|:: open22r\n" <<
			"  {gld}|:: close1{/gld}\r\n" <<
			"  {gld}|:: close2{/gld}\r\n" <<
			"  {gld}|:: bankAccount{/gld}\r\n" <<
			"  {gld}|:: lastsort{/gld}\r\n" <<
			" {red}sbuild{/red} {grn}save{/grn} <shop_id>\r\n" <<
			" {red}sbuild{/red} {grn}show{/grn} <shop_id>\r\n" <<
			"\r\n"
			;
		player->pager_end();
		player->page(0);
	}

	mods::builder_util::list_object_vector<std::vector<shop_data>,std::string>(
			player,
			std::string(argument),
			shop_index,
			[](shop_data & shop) -> std::string {
			return std::string(mods::util::itoa(shop.vnum));
			}
			);

	mods::builder_util::save_object_vector<std::vector<shop_data>>(
			player,
			std::string(argument),
			shop_index,
			[](shop_data & shop) -> std::pair<bool,std::string> {
			mods::builder_util::post_modify_callback pm_callback = []() -> std::pair<bool,std::string> {
			return {true,""};
			};
			mods::builder_util::value_callback value_setter = [&](sql_compositor::value_map & s_map) {
			s_map["shop_vnum"] = mods::util::itoa(shop.vnum);
			s_map["shop_profit_buy"] = mods::util::itoa(shop.profit_buy);
			s_map["shop_profit_sell"] = mods::util::itoa(shop.profit_sell);
			//s_map["shop_type"] = mods::util::itoa(shop.type);
			s_map["shop_no_such_item1"] = (shop.no_such_item1);
			s_map["shop_no_such_item2"] = (shop.no_such_item2);
			s_map["shop_missing_cash1"] = (shop.missing_cash1);
			s_map["shop_missing_cash2"] = (shop.missing_cash2);
			s_map["shop_do_not_buy"] =(shop.do_not_buy);
			s_map["shop_message_buy"] = (shop.message_buy);
			s_map["shop_message_sell"] =(shop.message_sell);
			s_map["shop_temper1"] = mods::util::itoa(shop.temper1);
			s_map["shop_bitvector"] = mods::util::itoa(shop.bitvector);
			s_map["shop_keeper"] = mods::util::itoa(shop.keeper);
			s_map["shop_with_who"] = mods::util::itoa(shop.with_who);
			//s_map["shop_in_room"] = mods::util::itoa(shop.in_room);
			s_map["shop_open1"] = mods::util::itoa(shop.open1);
			s_map["shop_open2"] = mods::util::itoa(shop.open2);
			s_map["shop_close1"] = mods::util::itoa(shop.close1);
			s_map["shop_close2"] = mods::util::itoa(shop.close2);
			s_map["shop_bankAccount"] = mods::util::itoa(shop.bankAccount);
			s_map["shop_lastsort"] = mods::util::itoa(shop.lastsort);
			};
			return mods::builder_util::save_to_db<std::string>(
					"shops",
					"shop_id",
					std::string(mods::util::itoa(shop.vnum)),
					value_setter,
					pm_callback
					);
			}
	);
	mods::builder_util::show_object_vector<std::vector<shop_data>>(
			player,
			std::string(argument),
			shop_index,
			[](shop_data & shop) -> void {

			}
			);
}
ACMD(do_mbuild) {
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
	MENTOC_PREAMBLE();
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));

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
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}mbuild{/wht} {gld}help{/gld}\r\n" <<
			"  |:: (this help menu will show up)\r\n" <<
			" {grn}mbuild{/grn} {red}new{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}list{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}attr <mob_id> <attr> <value>{/red}\r\n" <<
			"  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
			"  {gld}|:: virtual_number{/gld}\r\n" <<
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
			"  {gld}|:: charisma{/gld}\r\n" <<
			"  {gld}|:: damnodice{/gld}\r\n" <<
			"  {gld}|:: damsizedice{/gld}\r\n" <<
			"  {gld}|:: sex {red}see mbuild help sex{/red}{/gld}\r\n" <<
			"  {gld}|:: default_position {red}see mbuild help default_position{/red}{/gld}\r\n" <<
			"  {gld}|:: action {red}see mbuild help action{/red}{/gld}\r\n" <<
			" {grn}mbuild{/grn} {red}save <mob_id>{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}show <mob_id>{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}instantiate <mob_id>{/red}\r\n" <<
			"\r\n"
			;
		player->pager_end();
		player->page(0);
		return;
	}

	auto args = mods::util::subcmd_args<4,args_t>(argument,"new");

	if(args.has_value()) {
		mob_proto.push_back(mods::builder::new_npc());
		r_success(player,"Mobile created");
		return;
	}

	args = mods::util::subcmd_args<12,args_t>(argument,"instantiate");

	if(args.has_value()) {
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);

		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value.");
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
			auto index = i_value.value();
			std::size_t i = index;

			if(i >= mob_proto.size()) {
				r_error(player,"Out of bounds");
				return;
			}

			auto obj = read_mobile(index,VIRTUAL - 1);
			GET_POS(obj) = POS_STANDING;

			for(i = 0; i < NUM_WEARS; i++) {
				GET_EQ(obj, i) = nullptr;
			}

			obj->carrying = nullptr;
			IN_ROOM(obj) = player->room();
			mods::globals::rooms::char_from_room(obj);
			mods::globals::rooms::char_to_room(player->room(),obj);
			r_success(player,"Object created, look on the floor");
		}

		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"list");

	if(args.has_value()) {
		r_status(player,"listing...");
		unsigned mob_id = 0;
		player->pager_start();

		for(auto& mob_reference : mob_proto) {
			auto mob = &mob_reference;
			*player << "{gld}[" << mob_id++ << "]{/gld} :->{red} [" <<
				mob->player.short_descr.c_str() << "]{/red}\r\n";
		}

		player->pager_end();
		player->page(0);
		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"save");

	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 2) {
			r_error(player,"Invalid number of arguments");
			return;
		}

		auto index = mods::util::stoi(arg_vec[1]);

		if(!index.has_value()) {
			r_error(player,"Invalid index");
			return;
		}

		std::size_t i = index.value();

		if(i  >= mob_proto.size()) {
			r_error(player,"Invalid index");
			return;
		}

		auto obj = &mob_proto[index.value()];
		auto return_pair = mods::builder::save_player(obj);

		if(!return_pair.first) {
			r_error(player,return_pair.second);
			return;
		}

		r_success(player,"Object saved.");
		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"show");

	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 2) {
			r_error(player,"Invalid number of arguments");
			return;
		}

		auto index = mods::util::stoi(arg_vec[1]);

		if(!index.has_value()) {
			r_error(player,"Invalid index");
			return;
		}

		std::size_t i = index.value();

		if(i >= mob_proto.size()) {
			r_error(player,"Invalid index");
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
		MENTOC_SHOW_OBJ(virtual_number,nr);
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

	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 4) {
			r_error(player,"Invalid number of arguments");
			return;
		}

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
			MENTOC_OBI2(player.level,level);
			MENTOC_OBI2(player.weight,weight);
			MENTOC_OBI2(player.height,height);
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

			if(arg_vec[2].compare("virt") == 0) {
				if(arg_vec.end() <= arg_vec.begin() + 3) {
					r_error(player,"Please supply a virtual number");
					return;
				}
				auto opt_vr_number = mods::util::stoi(arg_vec[3]);
				obj->nr = opt_vr_number.value();
				r_success(player,"Saved");
				return;
			}


			if(arg_vec[2].compare("sex") == 0) {
				if(arg_vec.end() <= arg_vec.begin() + 3) {
					r_error(player,"Please supply a flag");
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
					r_error(player,std::string("Unrecognized flag: ") + *flag);
				}
			}

			if(arg_vec[2].compare("action") == 0) {
				if(arg_vec.end() <= arg_vec.begin() + 3) {
					r_error(player,"Please supply action flags");
					return;
				}

				auto flag = arg_vec.begin() + 3;
				bool found = false;
				obj->char_specials.saved.act = 0;

				for(auto& ex_flag : mods::builder::mob_act_flags) {
					if(ex_flag.second.compare(*flag) == 0) {
						obj->char_specials.saved.act |= ex_flag.first;
						found = true;
						break;
					}
				}

				if(!found) {
					r_error(player,std::string("Unrecognized flag: ") + *flag);
				}
			}

			if(arg_vec[2].compare("default_position") == 0) {
				if(arg_vec.end() <= arg_vec.begin() + 3) {
					r_error(player,"Please supply a flag");
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
					r_error(player,std::string("Unrecognized flag: ") + *flag);
				}
			}
		}

		return;
	}
}

ACMD(do_obuild) {
	MENTOC_PREAMBLE();
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));

	if(vec_args.size() == 2 && vec_args[0].compare("help") == 0 && vec_args[1].compare("weapon_type") == 0) {
		player->pager_start();
		*player <<
			"{red}Weapon Type{/red} A hash value of the weapon type.  It must be one of\r\n" <<
			"the following numbers:</P>\r\n" <<
			" SMG                sub machine gun\r\n" <<
			" SHOTGUN            shot gun\r\n" <<
			" SNIPE              sniper rifle\r\n" <<
			" GRENADE            type of frag grenade (not a grenade launcher)\r\n" <<
			"example: obuild attr 1 weapon_type SMG\r\n" <<
			"(this will set the affected slot number 3 on object zero to modify \r\n" <<
			"the character's weight by 15)\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}

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
			"usage: obuild affected <object_id> <affected_slot> <location> <modifier>\r\n" <<
			"example: obuild affected 0 3 CHAR_WEIGHT 15\r\n" <<
			"(this will set the affected slot number 3 on object zero to modify \r\n" <<
			"the character's weight by 15)\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}

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

	if(!vec_args.size() || vec_args[0].compare("help") == 0) {
		player->pager_start();
		*player << "usage: \r\n" <<
			" {grn}obuild{/grn} {red}help{/red}\r\n" <<
			"  |--> this help menu\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}obuild{/wht} {gld}help{/gld}\r\n" <<
			"  |:: (this help menu will show up)\r\n" <<
			" {grn}obuild{/grn} {red}new{/red}\r\n" <<
			" {grn}obuild{/grn} {red}list{/red}\r\n" <<
			" {grn}obuild{/grn} {red}attr <object_id> <attr> <value>{/red}\r\n" <<
			"  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
			"  {gld}|:: item_number{/gld}\r\n" <<
			"  {gld}|:: name{/gld}\r\n" <<
			"  {gld}|:: desc{/gld}\r\n" <<
			"  {gld}|:: short_desc{/gld}\r\n" <<
			"  {gld}|:: action_desc{/gld}\r\n" <<
			"  {gld}|:: worn_on{/gld}\r\n" <<
			"  {gld}|:: weapon_type {red}see: obuild help weapon_type{/red}{/gld}\r\n" <<
			"  {gld}|:: weapon_ammo{/gld}\r\n" <<
			"  {gld}|:: weapon_ammo_max{/gld}\r\n" <<
			"  {gld}|:: weapon_holds_ammo{/gld}\r\n" <<
			"  {gld}|:: flags{/gld}\r\n" <<
			" {grn}obuild{/grn} {red}ex <object_id> create <index>{/red}\r\n" <<
			" {grn}obuild{/grn} {red}ex <object_id> set <index> <keyword> <description>{/red}\r\n" <<
			" {grn}obuild{/grn} {red}ex <object_id> del <index>{/red}\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}obuild{/wht} {gld}ex 6 set 0 {/gld}\"keyword\" \"my description\"\r\n" <<
			"  |:: {wht}obuild{/wht} {gld}ex 6 del 0{/gld}\r\n" <<
			" {grn}obuild{/grn} {red}affected <object_id> set <affected_slot> <location> <modifier>{/red}\r\n" <<
			" {grn}obuild{/grn} {red}affected <object_id> del <affected_slot>{/red}\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}obuild{/wht} {gld}affected 6 3 STR 25{/gld}\r\n" <<
			"  |:: (this will set the affected slot number 3 on object 6 \r\n" <<
			"      to modify strength by 25)\r\n" <<
			"  |:: {red}see obuild help affected{/red}\r\n" <<
			" {grn}obuild{/grn} {red}flag <object_id> extra_flags <value>{/red}\r\n"<<
			" {red}see obuild help extra_flags{/red}\r\n" <<
			" {grn}obuild{/grn} {red}flag <object_id> <attr> <value>{/red}\r\n"<<
			" {red}see obuild help flags{/red}\r\n" <<
			"  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
			"  {gld}|:: value_0{/gld}\r\n" <<
			"  {gld}|:: value_1{/gld}\r\n" <<
			"  {gld}|:: value_2{/gld}\r\n" <<
			"  {gld}|:: value_3{/gld}\r\n" <<
			"  {gld}|:: type_flags {red}see: obuild help type_flags{/red}{/gld}\r\n" <<
			"  {gld}|:: wear_flags {red}see: obuild help wear_flags{/red}{/gld}\r\n" <<
			"  {gld}|:: extra_flags{/gld}\r\n" <<
			"  {gld}|:: weight{/gld}\r\n" <<
			"  {gld}|:: cost{/gld}\r\n" <<
			"  {gld}|:: cost_per_day{/gld}\r\n" <<
			"  {gld}|:: timer{/gld}\r\n" <<
			"  {gld}|:: bitvector {red}see: obuild help bitvector{/red}{/gld}\r\n" <<
			" {grn}obuild{/grn} {red}save <object_id>{/red}\r\n" <<
			" {grn}obuild{/grn} {red}show <object_id>{/red}\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}

	auto args = mods::util::subcmd_args<4,args_t>(argument,"new");

	if(args.has_value()) {
		r_status(player,"Creating new object");
		obj_proto.push_back({});
		if(player->is_executing_js()){
			*player << "{index: " << obj_proto.size() - 1 << "}";
		}
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
		auto return_status = mods::builder::save_object(obj);

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

			auto head = &obj->ex_description;

			if(!*head) {
				*head = (extra_descr_data*)calloc(1,sizeof(extra_descr_data));
				(*head)->keyword = (*head)->description = strdup("<default>");
				(*head)->next = nullptr;
				r_status(player,"Built node.");
			}

			auto new_node = (extra_descr_data*)calloc(1,sizeof(extra_descr_data));
			new_node->keyword = new_node->description = strdup("<default>");
			new_node->next = nullptr;
			auto current = *head;
			unsigned max_iter = size.value()  < 1 ? 0 : size.value() -1;

			while(max_iter--) {
				if(current->next == nullptr) {
					new_node = (extra_descr_data*)calloc(1,sizeof(extra_descr_data));
					new_node->keyword = new_node->description = strdup("<default>");
					new_node->next = nullptr;
					current->next = new_node;
					r_status(player,"Built node.");
				}

				current = current->next;
			}

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
				int iterations = 0;
				auto head = obj->ex_description;

				if(!head) {
					r_error(player,"ex_description index is out of bounds (no ex_descriptions exist)");
					return;
				}

				extra_descr_data* ex_desc = obj->ex_description;

				while(iterations++ < index.value()) {
					if(!ex_desc) {
						r_error(player,"ex_description index out of bounds");
						return;
					}

					ex_desc = ex_desc->next;
				}

				ex_desc->keyword = strdup(arg_vec[4].c_str());
				ex_desc->description = strdup(arg_vec[5].c_str());
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
				int iterations = 0;
				auto ex_desc = obj->ex_description;

				for(; iterations < index.value(); iterations++) {
					if(!ex_desc) {
						r_error(player,"Out of bounds");
						return;
					}

					ex_desc = ex_desc->next;
				}

				mods::util::linked_list_remove<extra_descr_data*>(ex_desc,obj->ex_description);
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

			object_list.push_back(obj_proto[index]);
			auto obj = &(*(object_list.end() -1));
			obj->carried_by = obj->worn_by = nullptr;
			obj_to_room(obj,IN_ROOM(player->cd()));
			r_success(player,"Object created, look on the floor");
		}

		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"list");

	if(args.has_value()) {
		r_status(player,"listing...");
		unsigned object_id = 0;
		if(!player->is_executing_js()){
			player->pager_start();
		}

		jxcomp jx; 
		jx.array_start("objects");
		for(auto& object_reference : obj_proto) {
			auto obj = &object_reference;
			if(player->is_executing_js()){
				jx.object_start("")
					.push("index",object_id)
					.push("item_number",obj->item_number)
					.push("name",obj->name)
					.push("short_description",obj->short_description)
					.object_end();
			}else{
				*player << "{gld}[" << object_id << "]{/gld} :->{red} [" <<
					obj->short_description << "]{/red}";
			}
			object_id++;
		}
		if(player->is_executing_js()){
			jx.array_end();
			*player << jx.get();
		}else{
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
		auto ex_desc = obj->ex_description;
		unsigned ex_ctr = 0;

		for(; ex_desc ; ex_desc = ex_desc->next) {
			*player << "{red}[" << ex_ctr << "]keyword: {/red}" << ex_desc->keyword << "\r\n";
			*player << "{red}[" << ex_ctr << "]description: {/red}" << ex_desc->description << "\r\n";
			++ex_ctr;
		}

		if(!ex_ctr) {
			*player << "\r\n";
		}

		*player << "{red}item_number: {/red}" << obj->item_number << "\r\n" <<
			"{red}weapon_type: {/red}" << obj->weapon_type << "\r\n" <<
			"{red}worn_on: {/red}" << obj->worn_on << "\r\n" <<
			"{red}weapon_ammo: {/red}" << obj->ammo << "\r\n" <<
			"{red}weapon_ammo_max: {/red} " << obj->ammo_max << "\r\n" <<
			"{red}weapon_holds_ammo: {/red}: " << obj->holds_ammo << "\r\n" <<
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
				return strdup(arg_vec[3].c_str());
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

		if(arg_vec[2].compare("weapon_type") == 0) {
			if(arg_vec.end() <= arg_vec.begin() + 3) {
				r_error(player,"Please supply a flag");
				return;
			}

			auto flag = arg_vec.begin() + 3;
			bool found = false;

			for(auto& ex_flag : mods::builder::weapon_type_flags) {
				if(ex_flag.second.compare(*flag) == 0) {
					obj->weapon_type = ex_flag.first;
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

		MENTOC_OBI(worn_on);
		MENTOC_OBI2(ammo,weapon_ammo);
		MENTOC_OBI2(ammo_max,weapon_ammo_max);
		MENTOC_OBI2(holds_ammo,weapon_holds_ammo);
		MENTOC_OBI2(ammo,weapon_ammo);
		MENTOC_OBS(name);
		MENTOC_OBS(description);
		MENTOC_OBS(short_description);
		MENTOC_OBS(action_description);

		if(arg_vec[2].compare("affected") == 0) {
			if(arg_vec.size() < 6) {
				r_error(player,"Not enough arguments");
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
ACMD(do_zbuild) {
	MENTOC_PREAMBLE();
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
			" {grn}zbuild{/grn} {red}obj <zone_id> <obj_vnum> <room_vnum> <max> <if_flag>{/red}\r\n" <<
			"  |--> places object obj_vnum in room room_vnum\r\n" <<
			" {grn}zbuild{/grn} {red}obj2mob <zone_id> <obj_vnum> <mob_vnum> <max> <if_flag>{/red}\r\n" <<
			"  |--> gives object obj_vnum to mob mob_vnum\r\n" <<
			" {grn}zbuild{/grn} {red}obj2obj <zone_id> <obj_vnum> <obj_vnum2> <max> <if_flag>{/red}\r\n" <<
			"  |--> places object obj_vnum into object obj_vnum2\r\n" <<
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
			" {grn}zbuild{/grn} {red}place-list <zone_id>{/red}\r\n" <<
			"  |--> lists all place commands for 'zone_id'.\r\n" <<
			" {grn}zbuild{/grn} {red}place-remove <zone_id> <place_id>{/red}\r\n" <<
			"  |--> removes the place command 'place_id' in zone 'zone_id'\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
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
			r_error(player,"Please provide a zone id");
			return;
		}

		auto zone_id = mods::util::stoi(arg_vec[1]);

		if(!zone_id.has_value()) {
			r_error(player,"Invalid zone id");
			return;
		}

		std::size_t z = zone_id.value();

		if(z >= zone_table.size()) {
			r_error(player,"Out of bounds");
			return;
		}

		mods::builder_util::value_callback value_callback = [&](sql_compositor::value_map & values) {
			values["zone_virtual_number"] = mods::util::itoa(zone_table[zone_id.value()].number);
			values["zone_start"] = mods::util::itoa(zone_table[zone_id.value()].bot);
			values["zone_end"] = mods::util::itoa(zone_table[zone_id.value()].top);
			values["zone_name"] = zone_table[zone_id.value()].name;
			values["lifespan"] = std::to_string(zone_table[zone_id.value()].lifespan);
			values["reset_mode"] = std::to_string(zone_table[zone_id.value()].reset_mode);
		};
		mods::builder_util::post_modify_callback post_modify_callback = []() -> std::pair<bool,std::string> {
			return {true,""};
		};
		auto status = mods::builder_util::save_to_db<std::string>(
				"zone",
				"id",
				std::to_string(zone_table[zone_id.value()].get_id()),
				value_callback,
				post_modify_callback
				);

		if(status.first) {
			r_success(player,"Saved zone");
		} else {
			r_error(player,status.second);
		}

		auto status2 = mods::builder::update_zone_commands(zone_id.value());

		if(status2.first) {
			r_success(player,"Saved zone commands");
		} else {
			r_error(player,status2.second);
		}

		return;
	}

	if(std::string(&command[0]).compare("mob") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("mob ")+4);
		auto args = mods::util::arglist<std::vector<std::string>>(past);
		auto zone_id = mods::util::stoi(args[0]);

		if(!zone_id.has_value() || static_cast<unsigned>(zone_id.value()) >= zone_table.size()) {
			r_error(player," Invalid zone id");
			return;
		}

		if(args.size() < 6){
			r_error(player,"Not enough arguments");
			return;
		}

		auto zone_command = "M";
		auto if_flag = args[4];
		auto mob_vnum = args[1];
		auto max_existing = args[2];
		auto room_vnum = args[3];
		auto result = mods::builder::zone_place(zone_id.value(),zone_command,if_flag,mob_vnum,max_existing,room_vnum);
		if(!result.first){
			r_error(player,result.second);
		}else{
			r_success(player,"Placed mob in zone successfully");
		}
		return;
	}

	if(std::string(&command[0]).compare("place") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("place ")+6);
		auto args = mods::util::arglist<std::vector<std::string>>(past);
		auto zone_id = mods::util::stoi(args[0]);

		if(!zone_id.has_value() || static_cast<unsigned>(zone_id.value()) >= zone_table.size()) {
			r_error(player," Invalid zone id");
			return;
		}

		if(args.size() < 6){
			r_error(player,"Not enough arguments");
			return;
		}

		auto zone_command = args[1];
		auto if_flag = args[2];
		auto arg1 = args[3];
		auto arg2 = args[4];
		auto arg3 = args[5];
		mods::builder::zone_place(zone_id.value(),zone_command,if_flag,arg1,arg2,arg3);
		r_success(player,"Placed object in zone");
		return;
	}

	if(std::string(&command[0]).compare("delete") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("delete ")+7);

		for(auto id : mods::util::arglist<std::vector<std::string>>(past)) {
			auto zone = mods::util::stoi(id);

			if(zone.value_or(-1) == -1) {
				r_error(player,"Unable to delete id");
				return;
			}

			mods::builder::delete_zone(zone.value());
			r_success(player,"Deleted zone");
		}

		return;
	}

	if(std::string(&command[0]).compare("new") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("new ")+4);
		//TODO: take this logic and store it in interpreter.cpp so we can reuse it
		auto arglist = mods::util::arglist<std::vector<std::string>>(past);

		if(arglist.size() != 6) {
			r_error(player,"Argument list is invalid, please use the correct number of arguments");

			for(auto vstr: arglist) {
				*player << vstr << "\r\n";
			}
		} else {
			// " rbuildzone <new> <virtual_number> <zone start> <zone end> <zone name> <zone lifespan> <zone reset mode>\r\n" <<

			if(!mods::builder::save_zone_to_db(
						mods::util::stoi<int64_t>(arglist[0]),
						arglist[3],
						mods::util::stoi<int>(arglist[1]),
						mods::util::stoi<int>(arglist[2]),
						mods::util::stoi<int>(arglist[4]),
						mods::util::stoi<int>(arglist[5]))
			){
				r_error(player,"Unable to save new zone");
			} else {
				parse_sql_zones();
				r_success(player,"New zone created");
			}
		}

		return;
	}

	if(std::string(&command[0]).compare("list") == 0) {
		if(!player->is_executing_js()){
			r_status(player,"listing...");
		}
		auto t = mods::pq::transaction(*mods::globals::pq_con);
		std::string check_sql = "SELECT id,zone_start,zone_end,zone_name,lifespan,reset_mode FROM zone";
		auto check_result = mods::pq::exec(t,check_sql);
		mods::pq::commit(t);

		if(!player->is_executing_js()){
			player->pager_start();
		}

		jxcomp jx; 
		jx.array_start("zones");
		for(auto row : check_result) {
			if(!player->is_executing_js()){
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
			}else{
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

		if(player->is_executing_js()){
			jx.array_end();
			*player << jx.get();
		}else{
			player->pager_end();
			player->page(0);
		}
		return;
	}

	args = mods::util::subcmd_args<11,args_t>(argument,"place-list");

	if(args.has_value()) {
		auto arg_vec = args.value();

		//place-list <zone_id>
		// 0            1
		if(arg_vec.size() < 2) {
			r_error(player,"Invalid number of arguments");
			return;
		}

		auto index = mods::util::stoi(arg_vec[1]);

		if(!index.has_value()) {
			r_error(player,"Invalid index");
			return;
		}

		std::size_t i = index.value();

		if(i >= zone_table.size()) {
			r_error(player,"Out of bounds");
			return;
		}

		int current_mobile = 0;
		int ctr = 0;

		for(auto ZCMD : zone_table[index.value()].cmd) {
			if(ZCMD.command == '*') {
				break;
			}

			*player << "{red}[{/red}" << ctr << "{red}] ->{/red}";

			switch(ZCMD.command) {
				case 'M':
					current_mobile = ZCMD.arg1;
					*player << "{red}mobile{/red}: " << ZCMD.arg1 << "{red} to room: {/red}" << ZCMD.arg3 << "\r\n";
					break;

				case 'O':
					*player << "{red}object{/red}: " << ZCMD.arg1 << "{red} to room: {/red}" << ZCMD.arg3 << "\r\n";
					break;

				case 'G':
					*player << "{red}give object{/red}: " << ZCMD.arg1 << "{red} to mobile{/red}: " << current_mobile << "\r\n";
					break;

				case 'E':
					*player << "{red}equip mobile({/red}" << current_mobile << "{red}) with object{/red}: " << ZCMD.arg1 << "\r\n";
					break;

				case 'P':
					*player << "{red}put object({/red}" << ZCMD.arg1 << "{red}) in object{/red}: " << ZCMD.arg3 << "\r\n";
					break;

				case 'D':
					*player << "{red}set state of door{/red}:" << ZCMD.arg1 << " {red}to{/red}:" << ZCMD.arg2 << "\r\n";
					break;

				case 'R':
					*player << "{red}remove object({/red}" << ZCMD.arg2 << "{red}) from room{/red}:" << ZCMD.arg1 << "\r\n";
					break;
			}
		}

		return;
	}

	args = mods::util::subcmd_args<13,args_t>(argument,"place-remove");

	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 3) {
			r_error(player,"Not enough arguments");
			return;
		}

		//place-remove <zone_id> <command_index>
		//     0           1           2
		auto index = mods::util::stoi(arg_vec[1]);

		if(!index.has_value()) {
			r_error(player,"Invalid index");
			return;
		}

		std::size_t i = index.value();

		if(i >= zone_table.size()) {
			r_error(player,"Out of bounds");
			return;
		}

		auto command_index = mods::util::stoi(arg_vec[2]);

		if(!command_index.has_value()) {
			r_error(player,"Invalid command index");
			return;
		}

		auto command_vec = zone_table[index.value()].cmd;
		std::size_t c = command_index.value();

		if(c >= command_vec.size()) {
			r_error(player,"Command index out of bounds");
			return;
		}

		zone_table[index.value()].cmd.erase(
				zone_table[index.value()].cmd.begin() + command_index.value()
				);
		r_status(player,"Index removed");
		return;
	}
};

ACMD(do_rbuild) {
	MENTOC_PREAMBLE();
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));

	if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0) {
		player->pager_start() << "usage: \r\n" <<
			" {grn}rbuild{/grn} {red}help{/red}\r\n" <<
			"  |--> this help menu\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}help{/gld}\r\n" <<
			"  |:: (this help menu will show up)\r\n" <<

			" {grn}rbuild{/grn} {red}<list> {yel}[page]{/red}\r\n" <<
			"  |--> list " << std::to_string(mods::builder::RNUMLIST_MAX_PER_CALL) << " rooms per page. The page argument is zero-indexed\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}list{/gld}\r\n" << 
			"  |:: (list the first " << std::to_string(mods::builder::RNUMLIST_MAX_PER_CALL) << " rooms)\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}list 2{/gld}\r\n" << 
			"  |:: (list the third page)\r\n" <<

			" {grn}NEW FEATURE [as of: 2019-03]{/grn}\r\n" <<
			" {red}FEATURE: Room textures{/red}\r\n" <<
			" {grn}rbuild{/grn} {red}<texture> <type1> ... <typeN>{/red}\r\n" <<
			"  |--> Set the room's texture type. A room can have multiple texture types.\r\n" <<
			"  |:: {yel}-:[{/yel}{grn}types{/grn}{yel}]:-{/yel}\r\n" <<
			"  |:: \r\n" <<
			"  {gld}|:: GRASS{/gld}\r\n" <<
			"  {gld}|:: CEMENT{/gld}\r\n" <<
			"  {gld}|:: OUTSIDE{/gld}\r\n" <<
			"  {gld}|:: INSIDE{/gld}\r\n" <<
			"  {gld}|:: SEWER{/gld}\r\n" << 
			"  {gld}|:: VOLATILE{/gld}\r\n" << 
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}texture GRASS OUTSIDE{/gld}\r\n" <<

			" {grn}rbuild{/grn} {red}<texture> <delete> <type1> ... <typeN>{/red}\r\n" <<
			"  |--> Delete the textures listed after the delete keyword.\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}texture delete GRASS OUTSIDE{/gld}\r\n" <<

			" {grn}rbuild{/grn} {red}<texture> <clear>{/red}\r\n" <<
			"  |--> Deletes all textures in the current room. WARNING: This does not prompt for confirmation!\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}texture clear{/gld}\r\n" <<

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

			" {grn}rbuild{/grn} {red}<bind> <direction> <room_rnum>{/red}\r\n" <<
			"  |--> bind a room to a direction\r\n" <<
			"  {grn}|____[example]{/grn}\r\n"<<
			"  |:: {wht}rbuild{/wht} {gld}bind north 27{/gld}\r\n"<<
			"  |:: (the room to the north will lead to room 27)\r\n" <<

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
			"  |:: einfo               -> Currently only accepts ISDOOR\r\n" << //TODO Accept more than just ISDOOR
			"  |:: key                 -> Integer key that is accepted for this exit\r\n" <<
			"  |:: to_room             -> The room number that this exit leads to\r\n" <<
			"  {grn}|____[examples]{/grn}\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}dopt north gen To the north you see the Taco Bell bathroom.{/gld}\r\n" <<
			"  |:: (When you do 'look north' you will see the above description)\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}dopt north keyword bathroom{/gld}\r\n" <<
			"  |:: (when you do 'open bathroom' it will open the door to the north)\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}dopt north einfo ISDOOR{/gld}\r\n" <<
			"  |:: (the north exit will be a door)\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}dopt north key 123{/gld}\r\n" <<
			"  |:: (the north exit will require a key numbered 123)\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}dopt north to_room 27{/gld}\r\n" <<
			"  |:: (the north room will lead to room number 27)\r\n" <<


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
			"[documentation written on 2018-01-19]\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
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

	if(std::string(&command[0]).compare("room") == 0) {
		*player << world[IN_ROOM(ch)].number << "\r\n";
		return;
	}

	if(std::string(&command[0]).compare("set") == 0) {
		auto set = std::string(argument);
		set = set.substr(set.find("rnum ") + 5);
		auto number = mods::util::stoi(set);

		if(number.value_or(-1) == -1) {
			r_error(player,"Invalid number");
			return;
		}

		world[IN_ROOM(ch)].number = number.value();
		r_success(player,std::string("real room number set to ") + std::to_string(number.value()));
		return;
	}

	if(std::string(&command[0]).compare("title") == 0) {
		auto title = std::string(argument);
		title = title.substr(title.find("title ") + 6);

		if(mods::builder::title(IN_ROOM(ch),title)) {
			r_success(player,"Title changed");
		} else {
			r_error(player,"Error");
		}

		return;
	}

	if(std::string(&command[0]).compare("description") == 0) {
		auto description = std::string(argument);
		description = description.substr(description.find("description ") + 12);

		if(mods::builder::description(IN_ROOM(ch),description + "\r\n")) {
			r_success(player,"Description changed");
		} else {
			r_error(player,"Error");
		}

		return;
	}

	//" {grn}rbuild{/grn} {red}ed <list>{/red}\r\n" << 	/** TODO needs impl */
	//" {grn}rbuild{/grn} {red}ed <save-all>{/red}\r\n" << /** TODO: needs impl */
	//" {grn}rbuild{/grn} {red}ed <show> <N>{/red}\r\n" <<  /** TODO: needs impl */
	//" {grn}rbuild{/grn} {red}ed <N> <keyword> <value>{/red}\r\n" <<  /** TODO: needs impl */
	//" {grn}rbuild{/grn} {red}ed <N> <description> <value>{/red}\r\n" <<  /** TODO: needs impl */
	//

	/** HOw positional parameters are parsed: */
	/* On the command: rbuild ed foo bar 1 
		 vec_args[0] == 'ed'
		 vec_args[1] == 'foo'
		 vec_args[2] == 'bar'
		 vec_args[3] == '1'
		 */

	/**
	 *  Command line: rbuild ed [...] 
	 */
	if(std::string(&command[0]).compare("ed") == 0) {
		/**
		 *  Command line: rbuild ed new [N]
		 * ------------------------------------------
		 *  the N is for optionally how many new ones to create
		 */
		if(vec_args.size() >= 2 && vec_args[1].compare("new") == 0){
			// rbuild ed <new>\r\n" <<  /** TODO: needs impl */
			/** TODO: add mutex lock so that other builders cant lock this room */

			auto size_before = world[IN_ROOM(ch)].ex_descriptions().size();
			world[IN_ROOM(ch)].ex_descriptions().emplace_back();
			auto size_after = world[IN_ROOM(ch)].ex_descriptions().size();
			std::cerr << "before: " << size_before << " after: " << size_after << "\n";
			if(size_after > size_before){
				r_success(player,"Room ed saved");
				return;
			}else { r_error(player,std::string("Error creating ed: ")); }
			return;
		}
		/**
		 *  Command line: rbuild ed list [N]
		 * --------------------------------------------------
		 *  the N is for which page to list. 25 to a page
		 */
		if(vec_args.size() >= 3 && vec_args[1].compare("list") == 0){
			r_status(player,"listing...");
			unsigned ex_id = 0;
			player->pager_start();

			for(const auto& ex : world[IN_ROOM(ch)].ex_descriptions()) {
				*player << "{gld}[" << ex_id++ << "]{/gld} :->{red} [" <<
					ex.keyword.c_str() <<  "]->'" << 
					ex.description.c_str() << "'{/red}\r\n";
			}
			player->pager_end();
			player->page(0);
			return;
		}
		if(vec_args.size() >= 3 && vec_args[1].compare("delete") == 0){
			/** Accepts: rbuild ed delete N */
			// rbuild ed <delete> <N>\n" <<  /** TODO: needs impl */
			/**
			 *  Command line: rbuild ed delte N
			 * --------------------------------------------------
			 *  where N is the index you want to delete. accepts csv (TODO)
			 */
			int32_t target = mods::util::stoi<int>(vec_args[2]);
			int32_t i = target;
			if(i < 0){
				r_error(player,std::string("Value must be greater than zero"));
				return;
			}
			else{
				auto temp = world[IN_ROOM(ch)].ex_descriptions();
				auto size_temp = temp.size();
				if(temp.size()){
					r_error(player,std::string(
								"Nothing to delete"));
					return;
				}
				world[IN_ROOM(ch)].ex_descriptions().clear();
				int32_t ctr = 0; 
				for(auto && m : temp){
					if(ctr == target){
						continue;
					}else{
						world[IN_ROOM(ch)].ex_descriptions().emplace_back(std::move(m));
					}
				}
				std::string before = std::string("Before: [") + std::to_string(size_temp) + "] items.";
				std::string items = std::string("Now: [") + std::to_string(world[IN_ROOM(ch)].ex_descriptions().size() ) + "] items."; 
				r_success(player,before + items);
			}
		}


	}/** All code that handles ed should be in above statement */


	if(std::string(&command[0]).compare("save") == 0) {
		auto ret = mods::builder::save_to_db(IN_ROOM(ch));

		if(ret != 0) {
			r_error(player,std::string("Error saving room: ") + std::to_string(ret));
		} else {
			r_success(player,"Room saved");
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
		if(!IS_DIRECTION(&direction[0])) {
			r_error(player,"Invalid direction");
			return;
		}

		auto room = mods::util::stoi(&room_id[0]);
		std::size_t r = room.value();

		if(room.value_or(-1) == -1 || r > mods::globals::room_list.size()) {
			r_error(player,"Invalid room number");
			return;
		}

		if(mods::builder::create_direction(IN_ROOM(ch),mods::globals::dir_int(direction[0]),room.value())) {
			r_success(player,"Direction created");
		} else {
			r_error(player,"Error");
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

		if(!IS_DIRECTION(&direction[0])) {
			r_error(player,"Invalid direction");
			return;
		}

		if(!mods::builder::destroy_direction(IN_ROOM(ch),mods::globals::dir_int(direction[0]))) {
			r_error(player,"Unable to destroy direction");
		} else {
			r_success(player,"Direction destroyed");
		}

		return;
	}

	if(std::string(&command[0]).compare("dopt") == 0) {
		constexpr unsigned int max_char_item = 20;
		std::array<char,max_char_item> direction;
		std::array<char,max_char_item> item;
		one_argument(one_argument(one_argument(argument,&command[0],max_char),&direction[0],max_char_item),&item[0],max_char_item);
		auto description = std::string(argument);
		std::string str_item = &item[0];

		if(str_item.compare("gen") == 0) {
			description = description.substr(description.find("gen ") + 4);
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
			description = description.substr(description.find("keyword ") + 8);
			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
					description,std::nullopt,std::nullopt,std::nullopt).value_or("success");

			if(ret.compare("success") == 0) {
				r_success(player,"Keyword changed");
			} else {
				r_error(player,ret);
			}

			return;
		}

		if(str_item.compare("einfo") == 0) {
			description = description.substr(description.find("einfo ") + 6);
			auto exit_info = mods::util::stoi(description);

			if(!exit_info.has_value()) {
				r_error(player,"Invalid exit info. Must be 0-3");
				return;
			}

			auto ret = mods::builder::dir_option(IN_ROOM(ch),mods::globals::dir_int(direction[0]),std::nullopt,
					std::nullopt,exit_info.value(),std::nullopt,std::nullopt).value_or("success");

			if(ret.compare("success") == 0) {
				r_success(player,"exit_info changed to: EX_ISDOOR");
			} else {
				r_error(player,ret);
			}

			return;
		}

		if(str_item.compare("key") == 0) {
			description = description.substr(description.find("key ") + 4);
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

		if(str_item.compare("to_room") == 0) {
			description = description.substr(description.find("to_room ") + 8);
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

	auto args = mods::util::subcmd_args<11,args_t>(argument,"list-paved");

	if(args.has_value()) {
		if(player->builder_data){
			for(auto room_id : player->builder_data->room_pavements.rooms) {
				r_status(player,std::to_string(room_id));
			}
		}
		return;
	}

	args = mods::util::subcmd_args<11,args_t>(argument,"save-paved");

	if(args.has_value()) {
		unsigned saved_room_counter = 0;

		if(player->builder_data){
			for(auto room_id : player->builder_data->room_pavements.rooms) {
				std::size_t rid = room_id;

				if(rid >= world.size()) {
					r_error(player,std::string("Cannot save room id#: ") + std::to_string(room_id));
					continue;
				} else {
					if(mods::builder::save_to_db(room_id) < 0) {
						r_error(player,std::string("Unable to save room id#: ") + std::to_string(world[room_id].number));
						break;
					} else {
						saved_room_counter++;
					}
				}
			}
		}

		r_success(player,std::string("Saved ") + std::to_string(saved_room_counter) + " rooms");
		return;
	}

	args = mods::util::subcmd_args<5,args_t>(argument,"pave");

	if(args.has_value()) {
		auto arg_vec = args.value();

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

			if(real_zone(zone_id.value()) == NOWHERE) {
				r_error(player,"Couldn't find a zone id with the virtual number you specified.");
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

			r_status(player,"Starting pave mode. You can start moving around now. To stop, type 'rbuild pave off'");
			return;
		}

		if(arg_vec[1].compare("off") == 0) {
			if(player->builder_data){
				player->builder_data->room_pave_mode = false;
				r_status(player,"Stopped pave mode.");
				r_status(player,"Transaction ID: 0");
			}
			return;
		}

		return;
	}
};
