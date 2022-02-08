#ifndef __MENTOC_MODS_SHOP_HEADER__
#define __MENTOC_MODS_SHOP_HEADER__

#include "../globals.hpp"
#include "player.hpp"

extern obj_ptr_t read_object_ptr(obj_vnum nr, int type);
namespace mods::shop  {
	static inline int object_cost(obj_data& obj) {
		return 50;
	}
	template <typename TShopMap>
	void buy_item(player_ptr_t& player, TShopMap& shop_map, std::string_view argument) {
		//std::cerr << "[buy_item] argument: '" << argument << "'\n";
		int16_t s_arg_index = mods::util::stoi(argument).value_or(-1);
		auto room_id = player->room();
		auto it = shop_map.find((room_rnum)world[room_id].number);
		if(it != shop_map.end()) {
			auto& s_obj_list = it->second->object_info.objects;
			auto s_obj_it = s_obj_list.begin();
			s_obj_it += (s_arg_index -1);
			if(s_obj_it >= s_obj_list.end()) {
				player->sendln("That id doesn't exist");
				return;
			}

			auto s_obj_rnum = real_object(*s_obj_it);
			if(s_obj_rnum == NOTHING) {
				player->sendln("That id doesn't exist");
				return;
			}
			auto mp = object_cost(obj_proto[s_obj_rnum]);
			if(player->mp() < mp) {
				player->sendln("You don't have enough mp for that!");
				return;
			}
			player->mp() -= mp;
			player->sendln(
			    CAT(
			        "You spend ", mp,
			        " mp."
			    )
			);
			player->carry(read_object_ptr(s_obj_rnum,REAL));
			//"You carry a %s in your inventory.\r\n", obj_proto[s_obj_rnum].name.c_str()
			player->sendln(
			    CAT(
			        "You carry a ", obj_proto[s_obj_rnum].name.c_str(),", in your inventory."
			    )
			);
			return;
		}
	}
	template <typename TShopMap>
	static inline void list_shop_items(player_ptr_t& player,TShopMap& shop_map) {
		auto room_id = player->room();
		auto it = shop_map.find((room_rnum)world[room_id].number);
		if(it != shop_map.end()) {
			it->second->list_to_char(player);
			return;
		}
		player->sendln("You cannot do that here.");
	}
	template <typename TShopMap>
	static inline void show_item(player_ptr_t& player,TShopMap& shop_map, int16_t item_number) {
		auto room_id = player->room();
		auto it = shop_map.find((room_rnum)world[room_id].number);
		if(it != shop_map.end()) {
			auto& shop_object_vnums = it->second->object_info.objects;
			if(item_number <= 0 || item_number >= shop_object_vnums.size()) {
				player->sendln("Invalid item number");
				return;
			}
			auto obj = real_object(shop_object_vnums[item_number]);
			if(obj == NOTHING) {
				player->sendln("That item number doesn't exist. Please contact an admin.");
				return;
			}
			player->sendln(mods::globals::obj_stat_pages[shop_object_vnums[item_number]]);
			return;
		}
		player->sendln("You cannot do that here.");
	}
};

#endif
