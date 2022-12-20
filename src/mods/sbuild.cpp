#include "sbuild.hpp"
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
int get_raid_id(player_ptr_t player);
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

	extern std::optional<obj_data*> instantiate_object_by_index(int index) ;
	extern std::optional<obj_data*> instantiate_object_by_vnum(int vnum) ;
	extern void initialize_builder(player_ptr_t& player);

	using values_t = std::map<std::string,std::string>;

	static std::map<int,int> room_mappings;

	extern int save_to_db(room_rnum in_room,std::string& error_string) ;

};

using args_t = std::vector<std::string>;

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
		if(arg_vec.size() < 2) {
			r_error(player,"Invalid number of arguments");
			return;
		}
		auto index = mods::util::stoi(arg_vec[1]);
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

extern char_data* grab_mobile(std::size_t index,bool& fetched);
extern void present_action(player_ptr_t& player, std::size_t index);
extern void present_mob_specials(player_ptr_t& player, std::size_t index);
