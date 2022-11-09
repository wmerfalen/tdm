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


void show_shop_by_index(std::size_t i,player_ptr_t& player) {
	mods::builder_util::show_object_vector<std::deque<shop_data_t>>(
	        player,
	        CAT("show ",tostr(i)),
	        shop_proto,
	[&player](shop_data_t& shop) -> void {
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
		if(shop.room_info.rooms.size() == 0) {
			mods::builder_util::list_line(player, "room list", "zero rooms");
		}
		if(shop.object_info.objects.size() == 0) {
			mods::builder_util::list_line(player, "object list", "zero objects");
		}
		for(auto& room_vnum : shop.room_info.rooms) {
			auto real_room_id =  real_room(room_vnum);
			if(real_room_id >= world.size()) {
				mods::builder_util::list_line(player, "invalid room vnum:", room_vnum);
			} else {
				mods::builder_util::list_line(player, "room", world[real_room_id].name);
			}
		}
		for(auto& obj_vnum : shop.object_info.objects) {
			auto real_obj_id = real_object(obj_vnum);
			if(real_obj_id >= obj_list.size()) {
				mods::builder_util::list_line(player, "invalid object vnum:", obj_vnum);
			} else {
				mods::builder_util::list_line(player, "object", obj_list[real_obj_id]->name);
			}
		}
	}
	    );//end show_object_vector call
}
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
		}
	};
#undef MENTOC_WEAPON

	std::array<std::pair<int,std::string>,3> sex_flags = { {
			{SEX_NEUTRAL,"NEUTRAL"},
			{SEX_MALE,"MALE"},
			{SEX_FEMALE,"FEMALE"}
		}
	};

	std::array<std::pair<int,std::string>,20> mob_act_flags = { {
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
			{MOB_NOTDEADYET,"NOTDEADYET"},
			{MOB_NODROP,"NODROP"},
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

SUPERCMD(do_sbuild) {
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
		if(world.size() > room) {
			r_status(player,"checking of shop installed in this room...");
			if(world[room].shop_vnum) {
				r_success(player,"Found shop installed in room...printing...");
				for(std::size_t i = 0; i < shop_proto.size(); ++i) {
					if(shop_proto[i].vnum == world[room].shop_vnum) {
						show_shop_by_index(i,player);
						break;
					}
				}
				r_success(player,"done");
				return;
			}
			r_error(player, "no shop installed in this room");
		} else {
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
		if(std::get<0>(result)) {
			player->sendln(CAT(std::get<1>(result)));
			r_success(player,"Shop saved.");
			return;
		}
		r_error(player, std::get<2>(result));
		return;
	}
	/** sbuild delete */
	args = mods::util::subcmd_args<7,args_t>(argument,"delete");
	if(args.has_value()) {
		if(shop_proto.size() == 0) {
			r_error(player, "Shop size is zero.");
			return;
		}
		auto arg_vec = args.value();
		auto index = mods::util::stoi(arg_vec[1]);
		if(index.has_value() == false) {
			r_error(player, "Invalid index. Expected zero-based integer.");
			return;
		}
		if(index.value() >= shop_proto.size()) {
			r_error(player, "Index out of bounds.");
			return;
		}
		auto it = shop_proto.begin();
		it = it + index.value();
		if(it == shop_proto.end()) {
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
		if(index.has_value() && index.value() < shop_proto.size()) {
			auto& shop = shop_proto[index.value()];
			auto room = player->room();
			auto r_vnum = world[room].number;
			auto it = std::find(shop.room_info.rooms.begin(),shop.room_info.rooms.end(),r_vnum);
			if(it == shop.room_info.rooms.end()) {
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
		if(index.has_value() && index.value() < shop_proto.size()) {
			auto& shop = shop_proto[index.value()];
			auto room = player->room();
			auto r_vnum = world[room].number;
			auto it = std::find(shop.room_info.rooms.begin(),shop.room_info.rooms.end(),r_vnum);
			if(it != shop.room_info.rooms.end()) {
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
		if(index.has_value() == false || (index.has_value() && index.value() >= shop_proto.size())) {
			r_error(player, "Invalid shop index");
			return;
		}
		if(o_index.has_value() == false || (o_index.value() >= obj_list.size())) {
			r_error(player, "Invalid object index");
		}
		auto& shop = shop_proto[index.value()];
		auto obj = obj_list[o_index.value()];
		auto r_vnum = obj->item_number;
		auto it = std::find(shop.object_info.objects.begin(),shop.object_info.objects.end(),r_vnum);
		if(it == shop.object_info.objects.end()) {
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
		if(index.has_value() == false || (index.has_value() && index.value() >= shop_proto.size())) {
			r_error(player, "Invalid shop index");
			return;
		}
		if(o_index.has_value() == false || (o_index.value() >= obj_list.size())) {
			r_error(player, "Invalid object index");
		}
		auto& shop = shop_proto[index.value()];
		auto obj = obj_list[o_index.value()];
		auto r_vnum = obj->item_number;
		auto it = std::find(shop.object_info.objects.begin(),shop.object_info.objects.end(),r_vnum);
		if(it != shop.object_info.objects.end()) {
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
		if(index.has_value() && index.value() < shop_proto.size()) {
			auto& shop = shop_proto[index.value()];
			if(arg_vec[2].compare("profit_buy") == 0) {
				shop.profit_buy = std::stol(arg_vec[3]);
				r_success(player, "Successfully set profit_buy.");
				return;
			}
			if(arg_vec[2].compare("profit_sell") == 0) {
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
	[](shop_data_t& shop) -> std::string {
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
	[](shop_data_t& shop) -> std::pair<bool,std::string> {
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
	[&player](shop_data_t& shop) -> void {
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
		if(shop.room_info.rooms.size() == 0) {
			mods::builder_util::list_line(player, "room list", "zero rooms");
		}
		if(shop.object_info.objects.size() == 0) {
			mods::builder_util::list_line(player, "object list", "zero objects");
		}
		for(auto& room_vnum : shop.room_info.rooms) {
			auto real_room_id =  real_room(room_vnum);
			if(real_room_id >= world.size()) {
				mods::builder_util::list_line(player, "invalid room vnum:", room_vnum);
			} else {
				mods::builder_util::list_line(player, "room", world[real_room_id].name);
			}
		}
		for(auto& obj_vnum : shop.object_info.objects) {
			auto real_obj_id = real_object(obj_vnum);
			if(real_obj_id >= obj_list.size()) {
				mods::builder_util::list_line(player, "invalid object vnum:", obj_vnum);
			} else {
				mods::builder_util::list_line(player, "object", obj_list[real_obj_id]->name);
			}
		}
	}
	    );
}

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
		ENCODE_MAP(str_map_t({{"uuid",std::to_string(obj->uuid())}}));
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
