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
namespace mods {  struct player; };
namespace mods { struct extra_desc_data; }; 
#define MENTOC_OBI(i) obj->i = get_intval(#i).value_or(obj->i);
#define MENTOC_OBI2(i,a) obj->i = get_intval(#a).value_or(obj->i);
#define MENTOC_OBS(i) obj->i = get_strval(#i).value_or(obj->i);
#define MENTOC_OBS2(i,a) obj->i = get_strval(#a).value_or(obj->i.c_str());
using objtype = mods::object::type;
using player_ptr_t = std::shared_ptr<mods::player>;
using jxcomp = mods::jx::compositor;
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

extern int next_mob_number();
extern int next_room_number();
extern int next_zone_number();
extern std::tuple<int16_t,std::string> parse_sql_zones();
extern std::vector<int> zone_id_blacklist;
extern bool disable_all_zone_resets;
extern obj_ptr_t create_object_from_index(std::size_t proto_index);
using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
extern std::deque<shop_data_t> shop_proto;	/* prototypes for objs		 */

using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
#define in_room(a) IN_ROOM(a)
#define nowhere NOWHERE
#define is_direction(A) IS_DIRECTION(A)
void show_shop_by_index(std::size_t i,player_ptr_t& player){
	mods::builder_util::show_object_vector<std::deque<shop_data_t>>(
			player,
			CAT("show ",tostr(i)),
			shop_proto,
			[&player](shop_data_t & shop) -> void {
#define MENTOC_LAZY_SHOP(a) mods::builder_util::list_line(player,#a, shop.a)
			MENTOC_LAZY_SHOP(db_id);
			MENTOC_LAZY_SHOP(vnum);
			MENTOC_LAZY_SHOP(title);
			MENTOC_LAZY_SHOP(description);
			MENTOC_LAZY_SHOP(flags);
			MENTOC_LAZY_SHOP(profit_buy);
			MENTOC_LAZY_SHOP(profit_sell);
			MENTOC_LAZY_SHOP(no_such_item1);
			MENTOC_LAZY_SHOP(no_such_item2);
			MENTOC_LAZY_SHOP(missing_cash1);
			MENTOC_LAZY_SHOP(missing_cash2);
			MENTOC_LAZY_SHOP(do_not_buy);
			MENTOC_LAZY_SHOP(message_buy);
			MENTOC_LAZY_SHOP(message_sell);
			MENTOC_LAZY_SHOP(temper1);
			MENTOC_LAZY_SHOP(bitvector);
			MENTOC_LAZY_SHOP(keeper);
			MENTOC_LAZY_SHOP(with_who);
			MENTOC_LAZY_SHOP(open1);
			MENTOC_LAZY_SHOP(open2);
			MENTOC_LAZY_SHOP(close1);
			MENTOC_LAZY_SHOP(close2);
			MENTOC_LAZY_SHOP(bankAccount);
			MENTOC_LAZY_SHOP(lastsort);
#undef MENTOC_LAZY_SHOP
			if(shop.room_info.rooms.size() == 0){
				mods::builder_util::list_line(player, "room list", "zero rooms");
			}
			if(shop.object_info.objects.size() == 0){
				mods::builder_util::list_line(player, "object list", "zero objects");
			}
			for(auto & room_vnum : shop.room_info.rooms){
				auto real_room_id =  real_room(room_vnum);
				if(real_room_id >= world.size()){
					mods::builder_util::list_line(player, "invalid room vnum:", room_vnum);
				}else{
					mods::builder_util::list_line(player, "room", world[real_room_id].name);
				}
			}
			for(auto & obj_vnum : shop.object_info.objects){
				auto real_obj_id = real_object(obj_vnum);
				if(real_obj_id >= obj_list.size()){
					mods::builder_util::list_line(player, "invalid object vnum:", obj_vnum);
				}else{
					mods::builder_util::list_line(player, "object", obj_list[real_obj_id]->name);
				}
			}
			}
		);//end show_object_vector call
}
int next_zone_vnum(){
		static int next_zone_vnum = 0;
		if(next_zone_vnum == 0){
			try{
				auto up_txn = txn();
				auto record = mods::pq::exec(up_txn,"SELECT max(zone_virtual_number) + 1 as z from zone;");
				if(record.size()){
					std::cerr << ("Max room number: " + std::string(record[0]["z"].c_str()));
					next_zone_vnum = mods::util::stoi<int>(record[0]["z"].c_str());
				}else{
					next_zone_vnum = 1;
				}
			}catch(std::exception& e){
				REPORT_DB_ISSUE("error grabbing next max room number '",e.what());
				next_zone_vnum = 1;
			}
		}
		std::cerr << "next_zone_vnum: " << next_zone_vnum + 1 << "\n";
		return ++next_zone_vnum;
}
int next_room_vnum(){
		static int next_room_number = 0;
		if(next_room_number == 0){
			try{
				auto up_txn = txn();
				auto record = mods::pq::exec(up_txn,"SELECT max(room_number) + 1 as r from room;");
				if(record.size()){
					std::cerr << ("Max room number: " + std::string(record[0]["r"].c_str()));
					next_room_number = mods::util::stoi<int>(record[0]["r"].c_str());
				}else{
					next_room_number = 1;
				}
			}catch(std::exception& e){
				REPORT_DB_ISSUE("error grabbing next max room number '",e.what());
				next_room_number = 1;
			}
		}
		std::cerr << "next_room_number: " << next_room_number + 1 << "\n";
		return ++next_room_number;
}

void disable_zone_resets(){
	disable_all_zone_resets = true;
}
void enable_zone_resets(){
	disable_all_zone_resets = false;
}
void blacklist_zone(int zone){
	auto it = std::find(zone_id_blacklist.begin(),
			zone_id_blacklist.end(),
			zone);
	if(it == zone_id_blacklist.end()){
		zone_id_blacklist.emplace_back(zone);
	}
}

void release_zone(int zone){
	auto copy = zone_id_blacklist;
	copy.clear();
	for(auto &zone_id : zone_id_blacklist){
		if(zone_id == zone){
			continue;
		}
		copy.emplace_back(zone_id);
	}
	zone_id_blacklist.clear();
	zone_id_blacklist.assign(
		copy.begin(),
		copy.end()
	);

}
void r_error(const player_ptr_t & player,std::string_view msg){
	mods::builder::report_error<player_ptr_t>(player,msg.data());
}
void r_success(const player_ptr_t & player,std::string_view msg){
	mods::builder::report_success<player_ptr_t>(player,msg.data());
}
void r_status(const player_ptr_t & player,std::string_view msg){
	mods::builder::report_status<player_ptr_t>(player,msg.data());
}
namespace mods::builder {
	bool has_flag(player_ptr_t player,uint64_t flag){
		return player->has_flag(mods::flags::chunk_type_t::BUILDER,flag);
	}
#define MENTOC_WEAPON(name) {mods::weapon::name,#name}
	std::array<std::pair<int,std::string>,13> weapon_type_flags = { {
		MENTOC_WEAPON(SMG),
			MENTOC_WEAPON(SHOTGUN),
			MENTOC_WEAPON(SNIPE),
			MENTOC_WEAPON(GRENADE),
			MENTOC_WEAPON(ASSAULT_RIFLE),
			MENTOC_WEAPON(PISTOL),
			MENTOC_WEAPON(MACHINE_PISTOL),
			MENTOC_WEAPON(ENERGY),
			MENTOC_WEAPON(FUMES),
			MENTOC_WEAPON(FLAME),
			MENTOC_WEAPON(CLAYMORE),
			MENTOC_WEAPON(REMOTE_EXPLOSIVE),
			MENTOC_WEAPON(ATTACK_DRONE)
	}};
#undef MENTOC_WEAPON

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
			{AFF_INTIMIDATED,"INTIMIDATED"},
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

	/**
	 * !introspection It's questionable why this is here within the builder.cpp file...
	 * If there is a reason that we're setting position flags on a mob,
	 * then this is fine, but it smells of bad design. A position flag
	 * is set throughout battle and not as a mob's prototype. In rare
	 * cases this may make sense, i.e.: necromancers will have a default
	 * position of DEAD.
	 */
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
	std::array<std::pair<int,std::string>,24> type_flags = { {
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
			{ITEM_FOUNTAIN,"FOUNTAIN"},
			{ITEM_WEAPON_ATTACHMENT,"WEAPON_ATTACHMENT"}
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
	std::array<std::pair<int,std::string>,38> affected_flags = { {
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
			{APPLY_SAVING_SPELL,"SAVING_SPELL"},
			{APPLY_SCOPE,"SCOPE"},
			{APPLY_RECOIL,"RECOIL"},
			{APPLY_BARREL,"BARREL"},
			{APPLY_MAGAZINE_SIZE,"MAGAZINE_SIZE"},
			{APPLY_INCENDIARY,"INCENDIARY"},
			{APPLY_SIDERAIL,"SIDERAIL"},
			{APPLY_UNDER_BARREL,"UNDER_BARREL"},
			{APPLY_PENETRATION,"PENETRATION"},
			{APPLY_RANGE,"RANGE"},
			{APPLY_CRITICAL_RANGE,"CRITICAL_RANGE"},
			{APPLY_OPTIMAL_RANGE,"OPTIMAL_RANGE"},
			{APPLY_VISION,"VISION"},
			{APPLY_STEALTH,"STEALTH"}

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
			int zone_virtual_number){
		new_sandbox(player,name,starting_room_number,zone_virtual_number);
	}
	sandbox_data_t::sandbox_data_t() : m_name("") {}
	void sandbox_data_t::set_name(std::string_view n){
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
			int zone_virtual_number){
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

	void initialize_builder(player_ptr_t& player){
		if(!player->has_builder_data()){
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
			std::cerr << "initialize_builder: creating shared_ptr\n";
#endif
			player->builder_data = std::make_shared<builder_data_t>();
		}else{
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
			std::cerr << "initialize_builder: shared_ptr already built\n";
#endif
		}
		player->set_bui_mode(true);
	}
	void add_room_to_pavements(player_ptr_t& player, int room_id){
		auto & r = player->builder_data->room_pavements.rooms;
		if(std::find(r.begin(),r.end(),room_id) == r.end()){
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


	template <typename T>
	void rb_debug(T s){
#ifdef __MENTOC_SHOW_MODS_BUILDER_DEBUG_OUTPUT__
		std::cerr << "[builder-debug]: " << s << "\n";
#endif
	}
	template <typename T>
	void rb_map_debug(T & t){
		rb_debug("dumping map");
		for(auto & pair: t){
			rb_debug(std::string("values[") + pair.first + "]=" + pair.second);
		}
		rb_debug("Done dumping map");
	}

	using values_t = std::map<std::string,std::string>;
	std::tuple<bool,zone_pkid_t,std::string> update_zone_with_placements(int virtual_number,player_ptr_t player) {
		auto zone_start = player->builder_data->room_pavements.start_room;
		auto zone_end = zone_start;
		for(auto & room : player->builder_data->room_pavements.rooms){
			if(zone_end < room){
				zone_end = room;
			}
		}
		player->sendln("Zone start: " + std::to_string(zone_start));
		player->sendln("Zone end: " + std::to_string(zone_end));
		{
			try{
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
			}catch(std::exception& e){
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
			try{
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
			}catch(std::exception &e){
				REPORT_DB_ISSUE("saving zone in db",e.what());
				rb_debug("EXCEPTION");
				rb_debug(e.what());
				return {false,-1};
			}
		}
		{
			try{
				auto grab_zone_pk_id_txn = txn();
				sql_compositor comp("zone",&grab_zone_pk_id_txn);
				auto zone_select_sql = comp.select("id")
					.from("zone")
					.where("zone_virtual_number","=",std::to_string(virtual_number))
					.sql();
				rb_debug(zone_select_sql);
				auto zone_record = mods::pq::exec(grab_zone_pk_id_txn,zone_select_sql);
				return {true,mods::util::stoi<zone_pkid_t>(zone_record[0]["id"].c_str())};
			}catch(std::exception& e){
				REPORT_DB_ISSUE("selecting zone in db",e.what());
				rb_debug("EXCEPTION");
				rb_debug(e.what());
				return {false,-2};
			}

		}
		return {false,-3};
	}


	static std::map<int,int> room_mappings;

	/** SAVE ROOM TO DB */
	/** SAVE ROOM TO DB */
	/** SAVE ROOM TO DB */
	/** SAVE ROOM TO DB */
	int save_to_db(room_rnum in_room,std::string & error_string) {
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


		auto number = tostr(world[in_room].number);
		{
			try{
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
			}catch(std::exception& e){
				REPORT_DB_ISSUE("selecting room in db",e.what());
				error_string = "error selecting room from db: '";
				error_string += e.what();
				rb_debug("EXCEPTION");
				rb_debug(e.what());
				return -4;
			}
		}
		if(room_record.size()){
			/* update the record */
			rb_debug("Attempting to update record");
			try{
				auto up_txn = txn();
				sql_compositor comp("room",&up_txn);
				auto up_sql = comp
					.update("room")
					.set(values)
					.where("room_number","=",number)
					.sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
			}catch(std::exception& e){
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
			rb_debug("committing (didnt update, inserted instead)");
		}

		rb_debug("delete transaction");
		{
			auto del_txn = txn();
			mods::pq::exec(del_txn,std::string("DELETE FROM room_direction_data where room_number=") + std::to_string(world[in_room].number));
			mods::pq::commit(del_txn);
		}

		rb_debug("Deleted room_direction-data... now, insert some");
		for(auto direction = 0; direction < NUM_OF_DIRS; direction++) {
			if(world[in_room].dir_option[direction] &&
					world[in_room].dir_option[direction]->general_description) {

				//rb_debug("Saving general description");
				//auto check_txn = txn();
				//sql_compositor comp("room_direction_data",&check_txn);
				//std::string check_sql = comp.
				//	select("room_number")
				//	.from("room_direction_data")
				//	.where("room_number","=",std::to_string(world[in_room].number))
				//	.op_and("exit_direction","=",std::to_string(direction))
				//	.sql().data();
				//check_sql += " AND exit_direction=";
				//check_sql += check_txn.quote(direction);
				//auto check_result = mods::pq::exec(check_txn,check_sql);
				auto vnum = world[world[in_room].dir_option[direction]->to_room].number;
				rb_debug("real room num:" + std::to_string(vnum));
				std::map<std::string,std::string> values = {
					{"general_description",static_cast<std::string>(world[in_room].dir_option[direction]->general_description)},
					{"keyword",static_cast<std::string>(world[in_room].dir_option[direction]->keyword)},
					{"exit_info",std::to_string(world[in_room].dir_option[direction]->exit_info)},
					{"exit_key",std::to_string(world[in_room].dir_option[direction]->key)},
					{"to_room",std::to_string(vnum)},
					{"room_number",number},
					{"exit_direction",std::to_string(direction)}
				};

				//if(check_result.size()) {
				//	/* update the row instead of inserting it */
				//	auto up_txn = txn();
				//	sql_compositor comp("room_direction_data",&up_txn);
				//	auto up_sql = comp
				//		.update("room_direction_data")
				//		.set(values)
				//		.where("exit_direction","=",std::to_string(direction))
				//		.op_and("room_number","=",std::to_string(world[in_room].number))
				//		.sql();
				//	mods::pq::exec(up_txn,up_sql);
				//	mods::pq::commit(up_txn);
				//} else {
				{
					rb_debug("Inserting room dir data");
					rb_map_debug(values);
					try{
						auto txn2 = txn();
						sql_compositor comp("room_direction_data",&txn2);
						auto sql = comp
							.insert()
							.into("room_direction_data")
							.values(values)
							.sql();
						mods::pq::exec(txn2,sql);
						mods::pq::commit(txn2);
					}catch(std::exception& e){
						REPORT_DB_ISSUE("room dir data in db",e.what());
					}
				}
				//}
			}
		}

		if(world[in_room].ex_descriptions().size()){
			rb_debug("ex desc delete..");
			try{
				auto room_ex_desc_d_del_txn = txn();
				mods::pq::exec(room_ex_desc_d_del_txn,std::string("DELETE FROM room_extra_desc_data where room_number=") 
						+ std::to_string(world[in_room].number));
				mods::pq::commit(room_ex_desc_d_del_txn);
				rb_debug("deleted..");
			}catch(std::exception &e){
				REPORT_DB_ISSUE("deleting room extra desc data in db",e.what());
			}
			for(const auto & ex_desc : world[in_room].ex_descriptions()){
				values.clear();
				values["ex_keyword"] = ex_desc.keyword.c_str();
				values["ex_description"] = ex_desc.description.c_str();
				values["room_number"] = std::to_string(world[in_room].number);
				{
					rb_debug("inserting room ex desc data...");
					try{
						auto room_ex_desc_data_txn = txn();
						sql_compositor comp("room_extra_desc_data",&room_ex_desc_data_txn);
						auto sql = comp
							.insert()
							.into("room_extra_desc_data")
							.values(values).sql();
						mods::pq::exec(room_ex_desc_data_txn,sql);
						mods::pq::commit(room_ex_desc_data_txn);
					}catch(std::exception& e){
						REPORT_DB_ISSUE("[room_extra_desc_data]-> error",e.what());
						error_string = "[room_extra_desc_data]->error: '";
						error_string += e.what();
						rb_debug(error_string);
						return -5;
					}
				}
			}
		}

		error_string = "";
		return 0;
	}
	/** SAVE ROOM TO DB */
	/** SAVE ROOM TO DB */
	/** SAVE ROOM TO DB */
	/** SAVE ROOM TO DB */



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
	room_data new_room(player_ptr_t player,int direction) {
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
	bool flush_to_db(char_data *ch,int room) {
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
		if(!world[room_id].dir_option[direction]){
			std::string d = "description", k= "keyword";
			if(description.has_value()){
				d = description.value().data();
			}
			if(keywords.has_value()){
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
			REPORT_DB_ISSUE("error",e.what());
			return {false,std::string("Exception occurred: ") + e.what()};
		}
		return {true,"Saved zone successfully."};
	}
	char_data new_npc(){
		char_data proto;
		const char* foo = "foo";
		proto.player.name = strdup(foo);
		proto.player.short_descr = strdup(foo);
		proto.player.long_descr = strdup(foo);
		proto.player.description = strdup(foo);

		SET_BIT(proto.char_specials.saved.act, MOB_ISNPC);
		REMOVE_BIT(proto.char_specials.saved.act, MOB_NOTDEADYET);

		for(unsigned j=0; j < 5; j++) {
			GET_SAVE(&proto, j) = 0;
		}

		proto.aff_abils = proto.real_abils;
		return proto;
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
			p_map["mob_special_extended_type"] = std::to_string(obj->mob_specials.extended_mob_type);
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
			REPORT_DB_ISSUE("error",e.what());
			return {false,e.what()};
		}

		return {true,"Successfully saved player."};
	}
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

int next_room_pavement_transaction_id(){
	static int transaction_id = 0;
	return ++transaction_id;
}
ACMD(do_rbuild_sandbox) {
	
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
		if(args.has_value()){
			auto cmd_args = args.value();
			if(cmd_args.size() > 0 && cmd_args[0].compare("save") == 0){

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
				for(auto & room : player->builder_data->room_pavements.rooms){
					error_string = "";
					auto status = mods::builder::save_to_db(
							room,
							error_string
							);
					if(status != 0){
						r_error(player, std::string("Couldn't save room ") + std::to_string(room) + " ->" + error_string);
						continue;
					}
					r_success(player, "Saved room " + std::to_string(room));
				}
				auto status = mods::builder::update_zone_with_placements(
						player->builder_data->room_pavements.zone_id,
						player
				);
				if(std::get<0>(status)){
					r_success(player,std::string("Sandbox saved. Message: ") + std::get<2>(status));
				}else{
					r_error(player, std::string("Sandbox creation failed. Message: ") + std::get<2>(status));
				}
				return;
			}//end command is save
		}
	}

	{
		auto args = mods::util::subcmd_args<5,args_t>(argument,"pave");

		if(args.has_value()) {
			auto cmd_args = args.value();
			if(cmd_args.size() == 3 && cmd_args[1].compare("on") == 0){
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
						cmd_args[2],
						room_id,
						zone_vnum
				);
				sandbox.set_name(cmd_args[2]);
				auto status = mods::builder::save_zone_to_db(
						zone_vnum,
						cmd_args[2],
						room_id,
						room_id + 100,
						100,
						2
						);
				if(!std::get<0>(status)){
					r_error(player,std::string("Failed to create sandbox!"));
					return;
				}
				mods::builder::sandboxes[player->name().c_str()].emplace_back(std::move(sandbox));
				player->set_flag(mods::flags::chunk_type_t::BUILDER,mods::builder::HAS_SANDBOX);
				player->builder_data->room_pavements.rooms.clear();
				player->builder_data->room_pave_mode = true;
				player->builder_data->room_pavements.start_room = room_id;
				player->builder_data->room_pavements.zone_id = zone_vnum;
				if(player->builder_data->room_pavements.zone_id == -1){
					r_error(player, "Zone id is -1");
				}
				player->builder_data->room_pavements.transact_id = next_room_pavement_transaction_id();
				r_success(player,"Sandbox created");
				return;
			}//end pave on
			if(cmd_args.size() == 2 && cmd_args[1].compare("off") == 0){
				player->builder_data->room_pave_mode = false;
				r_success(player,"Turned off pave mode. Don't forget to run rbuild_sandbox save <id>.");
			}
		}
	}//end pave block
	{
		auto args = mods::util::subcmd_args<4,args_t>(argument,"new");

		if(args.has_value()) {
			auto cmd_args = args.value();
			if(cmd_args.size() == 4){
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
			"  {gld}|:: db_id{/gld}\r\n" <<
			"  {gld}|:: vnum{/gld}\r\n" <<
			"  {gld}|:: title{/gld}\r\n" <<
			"  {gld}|:: description{/gld}\r\n" <<
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
			"  {gld}|:: open1{/gld}\r\n" <<
			"  {gld}|:: open22r\n" <<
			"  {gld}|:: close1{/gld}\r\n" <<
			"  {gld}|:: close2{/gld}\r\n" <<
			"  {gld}|:: bankAccount{/gld}\r\n" <<
			"  {gld}|:: lastsort{/gld}\r\n" <<
			" {red}sbuild{/red} {grn}save{/grn} <shop_id>\r\n" <<
			" {red}sbuild{/red} {grn}show{/grn} <shop_id>\r\n" <<
			" {red}sbuild{/red} {grn}install{/grn} <shop_id>\r\n" <<
			"  |--> install shop in the room you're standing in\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {red}sbuild{/red} install 2\r\n" <<
			"  |:: (install shop 2 in this room)\r\n" <<
			" {red}sbuild{/red} {grn}uninstall{/grn} <shop_id>\r\n" <<
			"  |--> uninstall shop in the room you're standing in\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {red}sbuild{/red} uninstall 2\r\n" <<
			"  |:: (uninstall shop 2 in this room)\r\n" <<
			" {red}sbuild{/red} {grn}add-object{/grn} <shop_id> <object_id>\r\n" <<
			"  |--> add object_id to shop_id\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {red}sbuild{/red} add-object 2 3\r\n" <<
			"  |:: (add object 3 into shop number 2)\r\n" <<
			"  |:: (for a list of objects, type obuild)\r\n" <<
			" {red}sbuild{/red} {grn}del-object{/grn} <shop_id> <object_id>\r\n" <<
			"  |--> delete object_id from shop_id\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {red}sbuild{/red} del-object 2 3\r\n" <<
			"  |:: (removes object 3 from shop number 2)\r\n" <<
			"  |:: (for a list of objects, type obuild)\r\n" <<
			" {red}sbuild{/red} {grn}info{/grn}\r\n" <<
			"  |--> prints information about the shop installed in this room (if any).\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {red}sbuild{/red} info\r\n" <<
			"  |:: (prints info (if any))\r\n" <<
			"\r\n"
			;
		player->pager_end();
		player->page(0);
	}

	/** sbuild info */
	auto args = mods::util::subcmd_args<5,args_t>(argument,"info");

	if(args.has_value()) {
		auto room = player->room();

		if(world.size() > room){
			r_status(player,"checking of shop installed in this room...");
			if(world[room].shop_vnum){
				r_success(player,"Found shop installed in room...printing...");
				for(std::size_t i = 0; i < shop_proto.size(); ++i){
					if(shop_proto[i].vnum == world[room].shop_vnum){
						show_shop_by_index(i,player);
						break;
					}
				}
				r_success(player,"done");
				return;
			}
			r_error(player, "no shop installed in this room");
		}else{
			r_error(player, "cannot get reliable room number. Are you in a weird room?");
			return;
		}
		return;
	}

	/** sbuild save */
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

		if(i  >= shop_proto.size()) {
			r_error(player,"Invalid index");
			return;
		}

		auto result = shop_proto[index.value()].save();
		if(std::get<0>(result)){
			player->send("%d\n", std::get<1>(result));
			r_success(player,"Shop saved.");
			return;
		}
		r_error(player, std::get<2>(result));
		return;
	}

	/** sbuild delete */
	args = mods::util::subcmd_args<7,args_t>(argument,"delete");

	if(args.has_value()){
		if(shop_proto.size() == 0){
			r_error(player, "Shop size is zero.");
			return;
		}
		auto arg_vec = args.value();
		auto index = mods::util::stoi(arg_vec[1]);
		if(index.has_value() == false){
			r_error(player, "Invalid index. Expected zero-based integer.");
			return;
		}
		if(index.value() >= shop_proto.size()){
			r_error(player, "Index out of bounds.");
			return;
		}
		auto it = shop_proto.begin();
		it = it + index.value();
		if(it == shop_proto.end()){
			r_error(player, "Index out of bounds.");
			return;
		}
		shop_proto.erase(it);
		r_success(player,"Shop deleted.");
		return;
	}


	/** sbuild new */
	args = mods::util::subcmd_args<4,args_t>(argument,"new");

	if(args.has_value()) {
		shop_proto.emplace_back();
		r_success(player,"Shop created");
		return;
	}

	/** sbuild install */
	args = mods::util::subcmd_args<8,args_t>(argument,"install");

	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 3) {
			r_error(player,"Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[2]);
		if(index.has_value() && index.value() < shop_proto.size()){ 
			auto & shop = shop_proto[index.value()];
			auto room = player->room();
			auto r_vnum = world[room].number;
			auto it = std::find(shop.room_info.rooms.begin(),shop.room_info.rooms.end(),r_vnum);
			if(it == shop.room_info.rooms.end()){
				shop.room_info.rooms.emplace_back(r_vnum);
				r_success(player, "Installed shop in this room's vnum");
				return;
			}
			r_error(player, "That shop is already installed here.");
			return;
		}
		r_error(player, "Invalid shop index");
		return;
	}

	/** sbuild uninstall */
	args = mods::util::subcmd_args<10,args_t>(argument,"uninstall");
	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 3) {
			r_error(player,"Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[2]);
		if(index.has_value() && index.value() < shop_proto.size()){ 
			auto & shop = shop_proto[index.value()];
			auto room = player->room();
			auto r_vnum = world[room].number;
			auto it = std::find(shop.room_info.rooms.begin(),shop.room_info.rooms.end(),r_vnum);
			if(it != shop.room_info.rooms.end()){
				shop.room_info.rooms.erase(it);
				r_success(player, "Uninstalled shop in this room's vnum");
				return;
			}
			r_error(player, "That shop isn't installed here.");
			return;
		}
		r_error(player, "Invalid shop index");
		return;
	}


	/** sbuild add-object */
	args = mods::util::subcmd_args<11,args_t>(argument,"add-object");

	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 3) {
			r_error(player,"Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[2]);
		auto o_index = mods::util::stoi(arg_vec[3]);
		if(index.has_value() == false || (index.has_value() && index.value() >= shop_proto.size())){
			r_error(player, "Invalid shop index");
			return;
		}
		if(o_index.has_value() == false || (o_index.value() >= obj_list.size())){
			r_error(player, "Invalid object index");
		}
		auto & shop = shop_proto[index.value()];
		auto obj = obj_list[o_index.value()];
		auto r_vnum = obj->item_number;
		auto it = std::find(shop.object_info.objects.begin(),shop.object_info.objects.end(),r_vnum);
		if(it == shop.object_info.objects.end()){
			shop.object_info.objects.emplace_back(r_vnum);
			r_success(player, "Added object to this shop");
			return;
		}
		r_error(player, "Didn't do anything.");
		return;
	}

	/** sbuild del-object */
	args = mods::util::subcmd_args<11,args_t>(argument,"del-object");

	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 3) {
			r_error(player,"Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[2]);
		auto o_index = mods::util::stoi(arg_vec[3]);
		if(index.has_value() == false || (index.has_value() && index.value() >= shop_proto.size())){
			r_error(player, "Invalid shop index");
			return;
		}
		if(o_index.has_value() == false || (o_index.value() >= obj_list.size())){
			r_error(player, "Invalid object index");
		}
		auto & shop = shop_proto[index.value()];
		auto obj = obj_list[o_index.value()];
		auto r_vnum = obj->item_number;
		auto it = std::find(shop.object_info.objects.begin(),shop.object_info.objects.end(),r_vnum);
		if(it != shop.object_info.objects.end()){
			shop.object_info.objects.erase(it);
			r_success(player, "Removed object from shop");
			return;
		}
		r_error(player, "Didn't do anything");
		return;
	}

	/** sbuild attr */
	args = mods::util::subcmd_args<5,args_t>(argument,"attr");


	if(args.has_value()) {
		auto arg_vec = args.value();

		if(arg_vec.size() < 4) {
			r_error(player,"Invalid number of arguments");
			return;
		}

		auto index = mods::util::stoi(arg_vec[1]);
		if(index.has_value() && index.value() < shop_proto.size()){ 
			auto & shop = shop_proto[index.value()];
			if(arg_vec[2].compare("profit_buy") == 0){
				shop.profit_buy = std::stol(arg_vec[3]);
				r_success(player, "Successfully set profit_buy.");
				return;
			}
			if(arg_vec[2].compare("profit_sell") == 0){
				shop.profit_sell = std::stol(arg_vec[3]);
				r_success(player, "Successfully set profit_sell.");
				return;
			}
#define MENTOC_LAZY_STR(a)\
			if(arg_vec[2].compare(#a) == 0){\
				shop.a = (char*)arg_vec[3].c_str();\
				r_success(player, std::string("Successfully set ") + #a);\
				return;\
			}
	MENTOC_LAZY_STR(title);
	MENTOC_LAZY_STR(description);
	MENTOC_LAZY_STR(no_such_item1);		/* Message if keeper hasn't got an item	*/
	MENTOC_LAZY_STR(no_such_item2);		/* Message if player hasn't got an item	*/
	MENTOC_LAZY_STR(missing_cash1);		/* Message if keeper hasn't got cash	*/
	MENTOC_LAZY_STR(missing_cash2);		/* Message if player hasn't got cash	*/
	MENTOC_LAZY_STR(do_not_buy);		/* If keeper dosn't buy such things	*/
	MENTOC_LAZY_STR(message_buy);		/* Message when player buys item	*/
	MENTOC_LAZY_STR(message_sell);		/* Message when player sells item	*/
#undef MENTOC_LAZY_STR
#define MENTOC_LAZY_INT(a)\
	if(arg_vec[2].compare(#a) == 0){\
		shop.a = mods::util::stoi(arg_vec[3]).value_or(0);\
		r_success(player, std::string("Successfully set ") + #a);\
		return;\
	}

	MENTOC_LAZY_INT(vnum);		/* How does keeper react if no money	*/
	MENTOC_LAZY_INT(temper1);		/* How does keeper react if no money	*/
	MENTOC_LAZY_INT(with_who);		/* Who does the shop trade with?	*/
	MENTOC_LAZY_INT(open1); 
	MENTOC_LAZY_INT(open2);		/* When does the shop open?		*/
	MENTOC_LAZY_INT(close1);
	MENTOC_LAZY_INT(close2);	/* When does the shop close?		*/
	MENTOC_LAZY_INT(bankAccount);		/* Store all gold over 15000 (disabled)	*/
	MENTOC_LAZY_INT(lastsort);		/* How many items are sorted in inven?	*/
	MENTOC_LAZY_INT(bitvector);	/* Can attack? Use bank? Cast here?	*/
	MENTOC_LAZY_INT(keeper);	/* The mobile who owns the shop (rnum)	*/
	MENTOC_LAZY_INT(flags);
#undef MENTOC_LAZY_INT
		}
		r_error(player, "Didn't do anything.");
		return;
	}

	mods::builder_util::list_object_vector<std::deque<shop_data_t>,std::string>(
		player,
		std::string(argument),
		shop_proto,
		[](shop_data_t & shop) -> std::string {
			return std::string(mods::util::itoa(shop.vnum));
		}
	);

	/**
	 * not obvious, but this handles the "save" sub-command
	 */
	mods::builder_util::save_object_vector<std::deque<shop_data_t>>(
			player,
			std::string(argument),
			shop_proto,
			[](shop_data_t & shop) -> std::pair<bool,std::string> {
				mods::builder_util::post_modify_callback pm_callback = []() -> std::pair<bool,std::string> {
					return {true,""};
				};
				mods::builder_util::value_callback value_setter = [&](sql_compositor::value_map & s_map) {
				/** TODO: make sure that if this is an existing shop, that we update it via db_id */
					//TODO: figure out how to handle thiss_map["shop_id"] = mods::util::itoa(shop.db_id);
					s_map["shop_vnum"] = mods::util::itoa(shop.vnum);
					s_map["shop_profit_buy"] = mods::util::itoa(shop.profit_buy);
					s_map["shop_profit_sell"] = mods::util::itoa(shop.profit_sell);
					s_map["shop_title"] = (shop.title.str());
					s_map["shop_description"] = (shop.description.str());
					s_map["shop_no_such_item1"] = (shop.no_such_item1.str());
					s_map["shop_no_such_item2"] = (shop.no_such_item2.str());
					s_map["shop_missing_cash1"] = (shop.missing_cash1.str());
					s_map["shop_missing_cash2"] = (shop.missing_cash2.str());
					s_map["shop_do_not_buy"] =(shop.do_not_buy.str());
					s_map["shop_message_buy"] = (shop.message_buy.str());
					s_map["shop_message_sell"] =(shop.message_sell.str());
					s_map["shop_temper1"] = mods::util::itoa(shop.temper1);
					s_map["shop_bitvector"] = mods::util::itoa(shop.bitvector);
					s_map["shop_flags"] = mods::util::itoa(shop.flags);
					s_map["shop_keeper"] = mods::util::itoa(shop.keeper);
					s_map["shop_with_who"] = mods::util::itoa(shop.with_who);
					s_map["shop_open1"] = mods::util::itoa(shop.open1);
					s_map["shop_open2"] = mods::util::itoa(shop.open2);
					s_map["shop_close1"] = mods::util::itoa(shop.close1);
					s_map["shop_close2"] = mods::util::itoa(shop.close2);
					s_map["shop_bankAccount"] = mods::util::itoa(shop.bankAccount);
					s_map["shop_lastsort"] = mods::util::itoa(shop.lastsort);
					s_map["shop_flags"] = mods::util::itoa(shop.flags);
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
	/**
	 * not obvious, but this handles the "show" sub command
	 */
	mods::builder_util::show_object_vector<std::deque<shop_data_t>>(
			player,
			std::string(argument),
			shop_proto,
			[&player](shop_data_t & shop) -> void {
#define MENTOC_LAZY_SHOP(a) mods::builder_util::list_line(player,#a, shop.a)
			MENTOC_LAZY_SHOP(db_id);
			MENTOC_LAZY_SHOP(vnum);
			MENTOC_LAZY_SHOP(title);
			MENTOC_LAZY_SHOP(description);
			MENTOC_LAZY_SHOP(flags);
			MENTOC_LAZY_SHOP(profit_buy);
			MENTOC_LAZY_SHOP(profit_sell);
			MENTOC_LAZY_SHOP(no_such_item1);
			MENTOC_LAZY_SHOP(no_such_item2);
			MENTOC_LAZY_SHOP(missing_cash1);
			MENTOC_LAZY_SHOP(missing_cash2);
			MENTOC_LAZY_SHOP(do_not_buy);
			MENTOC_LAZY_SHOP(message_buy);
			MENTOC_LAZY_SHOP(message_sell);
			MENTOC_LAZY_SHOP(temper1);
			MENTOC_LAZY_SHOP(bitvector);
			MENTOC_LAZY_SHOP(keeper);
			MENTOC_LAZY_SHOP(with_who);
			MENTOC_LAZY_SHOP(open1);
			MENTOC_LAZY_SHOP(open2);
			MENTOC_LAZY_SHOP(close1);
			MENTOC_LAZY_SHOP(close2);
			MENTOC_LAZY_SHOP(bankAccount);
			MENTOC_LAZY_SHOP(lastsort);
#undef MENTOC_LAZY_SHOP
			if(shop.room_info.rooms.size() == 0){
				mods::builder_util::list_line(player, "room list", "zero rooms");
			}
			if(shop.object_info.objects.size() == 0){
				mods::builder_util::list_line(player, "object list", "zero objects");
			}
			for(auto & room_vnum : shop.room_info.rooms){
				auto real_room_id =  real_room(room_vnum);
				if(real_room_id >= world.size()){
					mods::builder_util::list_line(player, "invalid room vnum:", room_vnum);
				}else{
					mods::builder_util::list_line(player, "room", world[real_room_id].name);
				}
			}
			for(auto & obj_vnum : shop.object_info.objects){
				auto real_obj_id = real_object(obj_vnum);
				if(real_obj_id >= obj_list.size()){
					mods::builder_util::list_line(player, "invalid object vnum:", obj_vnum);
				}else{
					mods::builder_util::list_line(player, "object", obj_list[real_obj_id]->name);
				}
			}
			}
		);
}

char_data* grab_mobile(std::size_t index,bool &fetched){
	if(index >= mob_proto.size()){
		auto obj = mods::globals::read_mobile(index,VIRTUAL - 1);
		fetched = obj == nullptr;
		return obj;
	}else{
		fetched = true;
		return &mob_proto[index];
	}
}
void present_action(player_ptr_t & player, std::size_t index){
		std::string flag_values;
		bool found = false;
		auto obj = grab_mobile(index,found);
		if(!found){
			return;
		}
		for(auto& ex_flag : mods::builder::mob_act_flags) {
			if(obj->char_specials.saved.act & ex_flag.first){
				flag_values += ex_flag.second;
				flag_values += ",";
			}
		}
		mods::builder_util::list_line(player, "action", flag_values);
		return;
}
void present_mob_specials(player_ptr_t & player, std::size_t index){
		std::string flag_values;
		bool found = false;
		auto obj = grab_mobile(index,found);
		if(!found){
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
			save_function(in_save_function)
	{
		
	}
	bool ready_to_save(str_t argument){
		auto args = mods::util::subcmd_args<50,args_t>(argument,this->name.c_str());
		if(!args.has_value() || args.value().size() < 3){
			return false;
		}
		auto arg_vec = args.value();
		int m_id = mods::util::stoi(arg_vec[2]).value_or(-1);
		return arg_vec[1].compare("save") == 0 && m_id == this->mob_id;
	}

	parse_response_t parse(str_t argument,player_ptr_t& player){
		auto args = mods::util::subcmd_args<50,args_t>(argument,this->name.c_str());
		if(!args.has_value()){
			return NOT_OURS;
		}
		auto arg_vec = args.value();
		if(arg_vec.size() >= 3 && arg_vec[2].compare("save") == 0){
			return NOT_OURS;
		}
		if(arg_vec.size() < this->arguments + 1){
			r_error(player,"Not enough arguments.");
			return INCOMPLETE;
		}
		auto i_value = mods::util::stoi(arg_vec[this->mob_id_argument + 1]);
		if(!i_value.has_value()){
			r_error(player,"Please specify a valid mob-id");
			return INCOMPLETE;
		}
		this->mob_id = i_value.value();
		if(mob_id >= mob_proto.size()){
			r_error(player,"Mob id out of range");
			return INCOMPLETE;
		}
		this->m_vnum = mob_proto[this->mob_id].nr;
		std::string field = arg_vec[2];
		std::string value = arg_vec[3];
		if(mods::util::in_array<std::string>(field,map_keys(this->column_mappings)) == false){
			r_error(player, "Please use a valid key");
			return INCOMPLETE;
		}
		for(auto & pair : this->column_mappings){
			if(field.compare(pair.first) == 0){
				this->values[this->column_mappings[field]] = value;
				r_success(player,CAT("Successfully set '", field, "'."));
				return HANDLED;
			}
		}
		r_error(player,"Didn't process any data.");
		return INCOMPLETE;
	}
	void print_command(player_ptr_t& player){
		player->send(" {grn}mbuild{/grn} {red}%s %s{/red}\r\n",this->name.c_str(),this->format.c_str());
		player->send("  {gld}|:: -:[keys]:-{/gld}\r\n");
		for(auto & pair : column_mappings){
			player->send("  {gld}|::%s {/gld}\r\n",pair.first.c_str());
		}
		player->send(" {grn}mbuild{/grn} {red}%s <mob-id> save{/red}\r\n",this->name.c_str());
	}
};
static std::list<std::shared_ptr<command_t>> mob_commands;

void initialize_mob_commands(){
	static bool initialized = false;
	if(initialized){
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
					if(!mods::mobs::mg::orm::db_exists<sql_compositor>(cmd->m_vnum)){
						mods::mobs::mg::orm::db_create<sql_compositor>(cmd->m_vnum,cmd->values);
					}
					mods::mobs::mg::orm::db_update<sql_compositor>(cmd->m_vnum,cmd->values);
			}
		)
	);
	initialized = true;
}

ACMD(do_mbuild) {
	static bool initialized = false;
	if(!initialized){
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
			"  {gld}|:: virt{/gld}\r\n" <<
			"  {gld}|:: vnum{/gld} {grn}this is an alias of {/grn}{gld}virt{/gld}\r\n" <<
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

			" {grn}NEW FEATURE [as of: 2020-07-10]{/grn}\r\n" <<
			" {red}FEATURE: extended-type{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}extended-type <mob_id> <type>{/red}\r\n" <<
			"  |--> will set the mob's extended type to <type>. The list of\r\n" <<
			"  available mob types follow.\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}mbuild{/wht} {gld}extended-type 5 INNOCENT{/gld}\r\n" <<
			"  |:: (will mark the mob (using the rnum) as INNOCENT)\r\n" <<
			"  {gld}|:: -:[extended-types]:-{/gld}\r\n";
			for(auto type : mods::mobs::extended_types::strings()){
				*player << "  {gld}|:: " << type << "{/gld}\r\n";
			}
			*player << 
			"  |::{red} for a description, type mbuild describe <type>\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}mbuild{/wht} {gld}describe INNOCENT{/gld}\r\n" <<
			"[documentation written on 2020-07-10]\r\n" <<
			" {grn}mbuild{/grn} {red}save <mob_id>{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}show <mob_id>{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}instantiate <mob_id>{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}action:add <mob_id> <flag>{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}action:remove <mob_id> <flag>{/red}\r\n" <<
			" {grn}mbuild{/grn} {red}action:list <mob_id>{/red}\r\n"
			;
		for(auto & cmd : mob_commands){
			cmd->print_command(player);
		}
		player->sendln("");
		player->pager_end();
		player->page(0);
		return;
	}
	{
		auto args = mods::util::subcmd_args<9,args_t>(argument,"describe");
		if(args.has_value()){
			//[ -  ] [ 0        ] [ 1    ]
			//mbuild <describe>    <type>
			auto arg_vec = args.value();
			if(arg_vec.size() < 2){
				r_error(player,"Please specify a type");
				return;
			}
			auto desc = mods::mobs::extended_types::description(arg_vec[1]);
			r_success(player,desc.c_str());
			return;
		}
	}

	{
		auto args = mods::util::subcmd_args<50,args_t>(argument,"extended-type");
		if(args.has_value()){
			//[ -  ] [ 0        ]    [ 1    ]  [  2   ]
			//mbuild <extended-type> <mob-id>  <type>
			auto arg_vec = args.value();
			if(arg_vec.size() < 3){
				r_error(player,"Please specify a mob-id and extended-type");
				return;
			}
			auto i_value = mods::util::stoi(arg_vec[1]);
			if(!i_value.has_value()){
				r_error(player,"Please specify a valid mob-id");
				return;
			}
			auto mob_id = i_value.value();
			std::string str_type = arg_vec[2];
			auto et_opt = mods::mobs::extended_types::from_string(str_type);
			if(et_opt.has_value() == false){
				r_error(player, "Unrecognized extended type");
				return;
			}
			auto opt = et_opt.value();
			if(mob_proto.size() <= mob_id){
				r_error(player, "Mob-id is out of bounds.");
				return;
			}
			mob_proto[mob_id].mob_specials.extended_mob_type = opt;
			r_success(player,"Set mob type.");
			return;
		}
	}
	for(auto & cmd : mob_commands){
		if(cmd->ready_to_save(argument)){
			cmd->save_function(cmd);
			return;
		}
		switch(cmd->parse(argument,player)){
			case parse_response_t::HANDLED:
				return;
			default:
				break;
		}
	}

	auto args = mods::util::subcmd_args<11,args_t>(argument,"action:add");
	if(args.has_value()){
		//[ -  ] [ 0        ] [ 1    ] [ 2  ]
		//mbuild <action:add> <mob_id> <flag>
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);

		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value.");
			return;
		}
		if(arg_vec.end() <= arg_vec.begin() + 2) {
			r_error(player,"Please supply action flags");
			return;
		}

		auto flag = arg_vec.begin() + 2;
		bool found = false;
		auto index = i_value.value();
		auto obj = grab_mobile(index,found);
		if(!found){
			r_error(player,"Invalid mob number");
			return;
		}

		for(auto& ex_flag : mods::builder::mob_act_flags) {
			if(ex_flag.second.compare(*flag) == 0) {
				obj->char_specials.saved.act |= ex_flag.first;
				found = true;
				r_success(player,"Added flag");
				break;
			}
		}

		if(!found) {
			r_error(player,std::string("Unrecognized flag: ") + *flag);
		}
	}
	args = mods::util::subcmd_args<14,args_t>(argument,"action:remove");
	if(args.has_value()){
		//[ -  ] [ 0           ] [ 1    ] [ 2  ]
		//mbuild <action:remove> <mob_id> <flag>
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);

		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value.");
			return;
		}
		if(arg_vec.end() <= arg_vec.begin() + 2) {
			r_error(player,"Please supply action flags");
			return;
		}

		auto flag = arg_vec.begin() + 2;
		bool found = false;
		auto index = i_value.value();
		auto obj = grab_mobile(index,found);
		if(!found){
			r_error(player,"Invalid mob number");
			return;
		}

		for(auto& ex_flag : mods::builder::mob_act_flags) {
			if(ex_flag.second.compare(*flag) == 0) {
				obj->char_specials.saved.act &= ~ex_flag.first;
				found = true;
				r_success(player,"Removed flag");
				break;
			}
		}

		if(!found) {
			r_error(player,std::string("Unrecognized flag: ") + *flag);
		}
	}

	args = mods::util::subcmd_args<12,args_t>(argument,"action:list");
	if(args.has_value()){
		//[ -  ] [ 0         ] [ 1    ] 
		//mbuild <action:list> <mob_id>
		auto arg_vec = args.value();
		auto i_value = mods::util::stoi(arg_vec[1]);

		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value.");
			return;
		}
		std::string flag_values;
		auto index = i_value.value();
		bool found = false;
		auto obj = grab_mobile(index,found);
		if(!found){
			r_error(player,"Invalid mob number");
			return;
		}
		for(auto& ex_flag : mods::builder::mob_act_flags) {
			if(obj->char_specials.saved.act & ex_flag.first){
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
		r_success(player,"Mobile created");
		return;
	}
	
	args = mods::util::subcmd_args<12,args_t>(argument,"clone");
	{
		if(args.has_value()){
			auto arg_vec = args.value();
			auto i_value = mods::util::stoi(arg_vec[1]);

			if(!i_value.has_value()) {
				r_error(player,"Please use a valid numeric value.");
				return;
			} else {
				auto index = i_value.value();
				std::size_t i = index;

				if(i >= mob_proto.size()) {
					r_error(player,"Out of bounds");
					return;
				}

				mob_vnum v = next_mob_number();
				mob_proto.emplace_back();
				mob_proto.back() = mob_proto[i];
				mob_proto.back().nr = v;
				r_success(player,"Object cloned");
			}
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

			mob_vnum v = mob_proto[index].nr;
			auto obj = mods::globals::read_mobile_ptr(v,VIRTUAL);
			mods::globals::rooms::char_to_room(player->room(),obj->cd());
			r_success(player,"Object created, look on the floor");
		}

		return;
	}

	/** TODO: create copy command */
	args = mods::util::subcmd_args<5,args_t>(argument,"copy");
	if(args.has_value()) {
		//r_status(player,"...");
		auto arg_vec = args.value();
		auto i_value = mods::util::stoul(arg_vec[1]);

		if(!i_value.has_value()) {
			r_error(player,"Please use a valid numeric value.");
			return;
		} 
		if(i_value.value() >= mob_proto.size()){
			r_error(player,"Out of bounds");
			return;
		}
		mob_proto.push_back(mob_proto[i_value.value()]);
		auto return_pair = mods::builder::save_player(&mob_proto[i_value.value()]);

		if(!return_pair.first) {
			r_error(player,return_pair.second);
			return;
		}

		r_success(player,"Mobile created and saved");
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


			if(arg_vec[2].compare("virt") == 0 || arg_vec[2].compare("vnum") == 0) {
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

		for(auto pairedData : mods::builder::weapon_type_flags){
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
		if(r.size() < 3){
			r_error(player, "Invalid number of arguments");
			return;
		}
		int obj_index = mods::util::stoi(r[1]).value_or(-1);
		if(obj_index < 0){
			r_error(player, "Invalid index. Unrecognized.");
			return;
		}
		auto & obj = obj_proto[obj_index];
		obj.set_feed_file(r[2]);
		r_success(player,"Object feed file set.");
		return;
	}


	args = mods::util::subcmd_args<4,args_t>(argument,"new");

	if(args.has_value()) {
		r_status(player,"Creating new object");
		obj_proto.push_back({ITEM_RIFLE,"g36c.yml"});
		if(player->is_executing_js()){
			*player << "{index: " << obj_proto.size() - 1 << "}";
		}
		r_success(player,"Object created");
		return;
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
			if(size.value() > obj->ex_description.size()){
				for(unsigned i=0; i < size.value();i++){
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
				if(index.value() >= obj->ex_description.size()){
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
				if(index.value() >= obj->ex_description.size()){
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
		if(obj_proto.size() == 0){
			r_status(player,"No objects to list");
			return;
		}
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
				if(obj->short_description){
					player->send("{gld}[%d]{/gld} :->{red}%s{/red}\r\n",object_id,obj->short_description);
				}else if(obj->name){
					player->send("{gld}[%d]{/gld} :->{red}%s{/red}\r\n",object_id,obj->name);
				}else{
					player->send("{gld}[%d]{/gld} :->{red}%s{/red}\r\n",object_id,"<null>");
				}
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
		unsigned ex_ctr = 0;
		for(auto & ex_desc : obj->ex_description){
			if(ex_desc.keyword.length()){
				*player << "{red}[" << ex_ctr << "]keyword: {/red}" << ex_desc.keyword.c_str() << "\r\n";
			}
			if(ex_desc.description.length()){
				*player << "{red}[" << ex_ctr << "]description: {/red}" << ex_desc.description.c_str() << "\r\n";
			}
			++ex_ctr;
		}
		*player << "\r\n{red}item_number: {/red}" << obj->item_number << "\r\n"
			"{red}worn_on: {/red}" << obj->worn_on << "\r\n";
		if(obj->rifle()){
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
		player->send("{red}obj_file: {/red} {grn}'%s'{/grn}\r\n",obj->feed_file());

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

		if(arg_vec[2].compare("weapon_type:list") == 0) {
			std::string report = "";
			for(auto& ex_flag : mods::builder::weapon_type_flags) {
				if(!obj->rifle()){ continue; }
				if(obj->rifle()->type & ex_flag.first || 
						obj->obj_flags.weapon_flags & ex_flag.first){
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
					if(!obj->rifle()){
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

		if(!obj->rifle()){
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

			if(!std::get<0>(mods::builder::save_zone_to_db(
							mods::util::stoi<int>(arglist[0]),
							arglist[3],
							mods::util::stoi<int>(arglist[1]),
							mods::util::stoi<int>(arglist[2]),
							mods::util::stoi<int>(arglist[4]),
							mods::util::stoi<int>(arglist[5]))
						)){
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
	mods::builder::initialize_builder(player);
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));

	if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0) {
		player->pager_start() << "usage: \r\n" <<
			" {grn}rbuild{/grn} {red}help{/red}\r\n" <<
			"  |--> this help menu\r\n" <<
			"  {grn}|____[example]{/grn}\r\n" <<
			"  |:: {wht}rbuild{/wht} {gld}help{/gld}\r\n" <<
			"  |:: (this help menu will show up)\r\n" <<
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

			/*
			" {grn}NEW FEATURE [as of: 2019-03]{/grn}\r\n" <<
			" {red}FEATURE: Room textures{/red}\r\n" <<
			" {grn}rbuild{/grn} {red}<texture:add> <type1> ... <typeN>{/red}\r\n" <<
			"  |--> Set the room's texture type. A room can have multiple texture types.\r\n" <<
			"  |:: {yel}-:[{/yel}{grn}types{/grn}{yel}]:-{/yel}\r\n" <<
			"  |:: \r\n"
			;
			for(auto & pair : mods::rooms::texture_strings){
				player->send("  {gld}|:: %s{/gld}\r\n",pair.second.c_str());
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
			*/

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
			for(auto & pair : mods::rooms::sector_strings) {
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
			"[documentation written on 2018-01-19]\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}

	auto args = mods::util::subcmd_args<4,args_t>(argument,"vnum");
	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough arguments to dopt. Expecting 4.");
			return;
		}
		auto str_vnum = vec_args[1];
		auto opt = mods::util::stoi(str_vnum);
		if(opt.value_or(-1) <= 0){
			r_error(player,"Invalid vnum. Must be positive number and not zero.");
			return;
		}
		auto room = player->room();
		if(world.size() <= room){
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
	if(args.has_value() && args.value().size() > 1 && args.value()[0].compare("set-recall") == 0){
		auto arg_vec = args.value();
		if(arg_vec[1].compare("mortal") == 0){
			auto set_status = mods::world_conf::set_mortal_start_room(world[player->room()].number);
			if(set_status.first == false){
				r_error(player,std::string("An error occurred: '") + set_status.second + "'");
				return;
			}
			r_success(player,set_status.second);
			return;
		}else if(arg_vec[1].compare("immortal") == 0){
			auto set_status = mods::world_conf::set_immortal_start_room(world[player->room()].number);
			if(set_status.first == false){
				r_error(player,std::string("An error occurred: '") + set_status.second + "'");
				return;
			}
			r_success(player,set_status.second);
			return;
		}else {
			r_error(player,"Second argument wasn't recognized. Please use either 'mortal' or 'immortal'");
			return;
		}
	}

	/**
	 * rbuild list <page>
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
		if(std::distance(world.begin(),world.end()) > max_per_call * number.value()){
			auto it = world.begin() + max_per_call * number.value();
			auto end_range = world.end();
			if(it + max_per_call < world.end()){
				end_range = it + max_per_call;
			}
			unsigned real_room_number = max_per_call * number.value();
			for(;it != end_range;++it){
				if(player->is_executing_js()){
					jx.object_start("")
						.push("rnum",real_room_number)
						.push("vnum",it->number)
						.object_end();
				}else{
					*player << "{gld}[" << real_room_number << "]{/gld} :->{red} [" <<
						it->number << "]\"" << it->name.c_str() << "\"{/red}";
				}
				real_room_number++;
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
		if(vec_args.size() >= 2 && vec_args[1].compare("new") == 0){
			// rbuild ed <new>\r\n" <<  /** todo: needs impl */
			/** todo: add mutex lock so that other builders cant lock this room */

			auto size_before = world[in_room(ch)].ex_descriptions().size();
			world[in_room(ch)].ex_descriptions().emplace_back();
			auto size_after = world[in_room(ch)].ex_descriptions().size();
#ifdef __mentoc_show_mods_builder_debug_output__
			std::cerr << "before: " << size_before << " after: " << size_after << "\n";
#endif
			if(size_after > size_before){
				r_success(player,"room ed saved");
				return;
			}else { r_error(player,std::string("error creating ed: ")); }
			return;
		}
		/**
		 *  command line: rbuild ed list [n]
		 * --------------------------------------------------
		 *  the n is for which page to list. 25 to a page
		 */
		if(vec_args.size() >= 2 && vec_args[1].compare("list") == 0){
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
		if(vec_args.size() >= 3 && vec_args[1].compare("delete") == 0){
			/** accepts: rbuild ed delete n */
			// rbuild ed <delete> <n>\n" <<  /** todo: needs impl */
			/**
			 *  command line: rbuild ed delte n
			 * --------------------------------------------------
			 *  where n is the index you want to delete. accepts csv (todo)
			 */
			int32_t target = mods::util::stoi<int>(vec_args[2]);
			int32_t i = target;
			if(i < 0){
				r_error(player,std::string("value must be greater than zero"));
				return;
			}
			else{
				auto temp = world[in_room(ch)].ex_descriptions();
				auto size_temp = temp.size();
				if(temp.size()){
					r_error(player,std::string(
								"nothing to delete"));
					return;
				}
				world[in_room(ch)].ex_descriptions().clear();
				int32_t ctr = 0; 
				for(auto && m : temp){
					if(ctr == target){
						continue;
					}else{
						world[in_room(ch)].ex_descriptions().emplace_back(std::move(m));
					}
				}
				std::string before = std::string("before: [") + std::to_string(size_temp) + "] items.";
				std::string items = std::string("now: [") + std::to_string(world[in_room(ch)].ex_descriptions().size() ) + "] items."; 
				r_success(player,before + items);
			}
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
		if(r < 0){
			r_error(player,"invalid vnum. could not convert to a number.");
			return;
		}
		r = real_room((room_vnum)r);
		if(r == nowhere){
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
	if(args.has_value()){
		if(vec_args.size() < 4){
			r_error(player,"Not enough arguments to dopt. Expecting 4.");
			return;
		}
		for(auto a : vec_args){
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

	args = mods::util::subcmd_args<11,args_t>(argument,"list-paved");

	if(args.has_value()) {
		if(player->builder_data){
			for(auto room: player->builder_data->room_pavements.rooms) {
				r_status(player,tostr(room));
			}
			r_status(player, "Transaction id: " + std::to_string(player->builder_data->room_pavements.transact_id));
		}
		return;
	}

	args = mods::util::subcmd_args<50,args_t>(argument,"save-paved");

	if(args.has_value()) {
		if(player->builder_data){
			unsigned c = 0;
			std::string error;
			for(auto & room_id : player->builder_data->room_pavements.rooms){
				auto ret = mods::builder::save_to_db(room_id,error);
				if(ret != 0) {
					r_error(player,std::string("Error saving room: ") + std::to_string(ret) + "->" + error);
				} else {
					++c;
					r_success(player,CAT("Room:", tostr(room_id), " saved"));
				}
			}
			r_success(player, CAT("Saved ",tostr(c), " rooms."));
		}else{
			r_error(player,"You currently don't have any builder data. Try starting the pave process.");
		}
		return;
	}

	/** textures */
	/** textures */
	/** textures */
	args = mods::util::subcmd_args<12,args_t>(argument,"texture:add");

	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64);++i){
			auto opt_flag = mods::rooms::texture_from_string(vec_args[i]);
			if(opt_flag.has_value() == false){
				r_error(player, vec_args[i] + ": unrecognized texture.");
				continue;
			}
			world[room].add_texture(opt_flag.value());
			r_status(player, vec_args[i] + " added.");
			++count;
		}
		if(count == 0){
			r_error(player, "Didn't set any flags");
			return;
		}
		r_success(player, std::string("Set ") + std::to_string(count) + " textures on room.");
		return;
	}
	args = mods::util::subcmd_args<16,args_t>(argument,"texture:remove");

	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64);++i){
			auto opt_flag = mods::rooms::texture_from_string(vec_args[i]);
			if(opt_flag.has_value() == false){
				r_error(player, vec_args[i] + ": unrecognized texture.");
				continue;
			}
			world[room].remove_texture(opt_flag.value());
			r_status(player, vec_args[i] + " removed.");
			++count;
		}
		if(count == 0){
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

	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough args");
			return;
		}
		auto opt_sector = mods::rooms::sector_from_string(vec_args[1]);
		if(!opt_sector.has_value()){
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
	if(args.has_value()){
		r_status(player, "Listing...");
		for(auto & str : mods::rooms::get_room_flags_from_room(player->room())){
			player->sendln(str);
		}
		r_status(player, "Done Listing.");
		return;
	}

	args = mods::util::subcmd_args<10,args_t>(argument,"flag:add");

	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64);++i){
			std::string real_flag = std::string("ROOM_") + vec_args[i];
			auto opt_flag = mods::rooms::room_flag_from_string(real_flag);
			if(opt_flag.has_value() == false){
				r_error(player, "Unrecognized flag");
				continue;
			}
			mods::rooms::set_flag(room, opt_flag.value());
			r_status(player, vec_args[i] + " added.");
			++count;
		}
		if(count == 0){
			r_error(player, "Didn't set any flags");
			return;
		}
		r_success(player, std::string("Set ") + std::to_string(count) + " flags on room. To list: rbuild flag:list");
		return;
	}

	args = mods::util::subcmd_args<12,args_t>(argument,"flag:remove");
	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough args");
			return;
		}
		auto room = player->room();
		unsigned count = 0;
		for(unsigned i=1; i < std::min((int)vec_args.size(),64);++i){
			std::string real_flag = std::string("ROOM_") + vec_args[i];
			auto opt_flag = mods::rooms::room_flag_from_string(real_flag);
			if(opt_flag.has_value() == false){
				r_error(player, "Unrecognized flag");
				continue;
			}
			mods::rooms::remove_flag(room, opt_flag.value());
			r_status(player, vec_args[i] + " removed.");
			++count;
		}
		if(count == 0){
			r_error(player, "Didn't remove any flags");
			return;
		}
		r_success(player, std::string("Removed ") + std::to_string(count) + " flag(s) on door");
		return;
	}


	/** room flags */
	/** room flags */


	args = mods::util::subcmd_args<9,args_t>(argument,"exit:add");

	if(args.has_value()){
		if(vec_args.size() <= 2){
			r_error(player,"Not enough args");
			return;
		}
		auto direction = mods::globals::dir_int(vec_args[1][0]);
		if(direction == -1){
			r_error(player, "Unrecognized direction");
			return;
		}
		if(!world[player->room()].dir_option[direction]){
			r_error(player, "Door doesn't exist there");
			return;
		}

		for(unsigned i=2; i < vec_args.size();i++){
			auto f= mods::doors::from_string(vec_args[i]);
			if(f == -1){
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

	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough args");
			return;
		}
		auto direction = mods::globals::dir_int(vec_args[1][0]);
		if(direction == -1){
			r_error(player, "Unrecognized direction");
			return;
		}
		if(!world[player->room()].dir_option[direction]){
			r_error(player, "Door doesn't exist there");
			return;
		}
		for(auto & str : mods::doors::all_string_flags(world[player->room()].dir_option[direction]->exit_info)){
			r_status(player, str);
		}
		r_success(player, "Listed.");
		return;
	}

	args = mods::util::subcmd_args<11,args_t>(argument,"exit:remove");

	if(args.has_value()){
		if(vec_args.size() < 2){
			r_error(player,"Not enough args");
			return;
		}
		auto direction = mods::globals::dir_int(vec_args[1][0]);
		if(direction == -1){
			r_error(player, "Unrecognized direction");
			return;
		}
		if(!world[player->room()].dir_option[direction]){
			r_error(player, "Door doesn't exist there");
			return;
		}
		for(unsigned i = 2;i < vec_args.size();i++){
			auto f = mods::doors::from_string(vec_args[i]);
			if(f == -1){
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
		if (arg_vec.size() == 2 && ICMP(arg_vec[1],"continue")){
			if(player->builder_data && player->builder_data->room_pave_mode){
				r_error(player,"It looks like you're already paving. Save your existing pavement to begin.");
				return;
			}
			r_success(player,"Continuing pavement.");
			mods::builder::initialize_builder(player);
			player->builder_data->room_pave_mode = true;
			player->builder_data->room_pavements.start_room = player->room();
			player->builder_data->room_pavements.zone_id = world[player->room()].zone;
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
								2))){
					r_error(player,"Couldn't find zone, nor could we create one automatically.");
					return;
				}else{
					/** TODO: transactional behaviour wanted here. (rollback) */
					r_success(player,"Zone automatically created");
				}
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
