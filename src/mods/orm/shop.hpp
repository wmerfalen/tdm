#ifndef __MENTOC_MODS_ORM_SHOP_HEADER__
#define __MENTOC_MODS_ORM_SHOP_HEADER__

#include <iostream>
#include <vector>
#include "../player.hpp"
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include "base.hpp"
#include <map>
#include <memory>

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	using db_id_t = uint64_t;

	struct shop_rooms : public base {
		static constexpr const char* table_name = "shop_rooms";
		using room_list_t = std::vector<room_vnum>;
		shop_rooms() : base(), shop_vnum_id(0) {}
		~shop_rooms() = default;
		int16_t load_by_shop_vnum(shop_vnum in_shop_vnum);
		shop_vnum shop_vnum_id;
		room_list_t rooms;
		int16_t place_keepers_in_rooms(mob_vnum);
		std::map<room_vnum,std::shared_ptr<mods::npc>> room_keeper_map;
	};

	struct shop_objects : public base {
		static constexpr const char* table_name = "shop_objects";
		using object_list_t = std::vector<obj_vnum>;
		shop_objects() : base(), shop_vnum_id(0) {}
		~shop_objects() = default;
		int16_t load_by_shop_vnum(shop_vnum in_shop_vnum);
		shop_vnum shop_vnum_id;
		object_list_t objects;
	};


	struct shop : public base {
		static constexpr const char* table_name = "shops";
		db_id_t shop_id;
		shop_vnum shop_vnum_id;		/* Virtual number of this shop		*/
		mods::string shop_title;
		mods::string shop_description;
		float shop_profit_buy;		/* Factor to multiply cost with		*/
		float shop_profit_sell;		/* Factor to multiply cost with		*/
		mods::string shop_no_such_item1;		/* Message if keeper hasn't got an item	*/
		mods::string shop_no_such_item2;		/* Message if player hasn't got an item	*/
		mods::string shop_missing_cash1;		/* Message if keeper hasn't got cash	*/
		mods::string shop_missing_cash2;		/* Message if player hasn't got cash	*/
		mods::string shop_do_not_buy;		/* If keeper dosn't buy such things	*/
		mods::string shop_message_buy;		/* Message when player buys item	*/
		mods::string shop_message_sell;		/* Message when player sells item	*/
		int shop_temper1;		/* How does keeper react if no money	*/
		bitvector_t shop_bitvector;	/* Can attack? Use bank? Cast here?	*/
		mob_rnum shop_keeper;	/* The mobile who owns the shop (rnum)	*/
		int shop_with_who;		/* Who does the shop trade with?	*/
		room_vnum shop_in_room;
		int shop_open1;
		int shop_open2;		/* When does the shop open?		*/
		int	 shop_close1;
		int shop_close2;	/* When does the shop close?		*/
		int shop_bankAccount;		/* Store all gold over 15000 (disabled)	*/
		int shop_lastsort;		/* How many items are sorted in inven?	*/
		int shop_type;
		bool loaded;
		uint64_t shop_flags;

		shop_rooms room_info;
		shop_objects object_info;

		void init();
		shop() : base(), loaded(false) {}
		~shop() = default;
		int16_t feed(const pqxx::result::reference &);

		/** database routines */
		int64_t db_id() const;
		std::tuple<bool,db_id_t,std::string> save();
		int16_t delete_shop_record();
		int16_t remove_room(room_vnum room);
	};


	void save_shop_rooms(shop_rooms& sr_list);
	void save_shop_objects(shop_objects& so_list);
	void delete_shop_rooms(shop_vnum room);
	void delete_shop_objects(shop_vnum room);
};

#endif
