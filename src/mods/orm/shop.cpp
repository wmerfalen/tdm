#include "shop.hpp"
#include <vector>
#include <map>
#include <string>

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	void delete_shop_rooms(shop_vnum shop_vnum_id){
		try{
			auto del_transaction = txn();
			sql_compositor comp(shop_rooms::table_name,&del_transaction);
			auto up_sql = comp
				.del()
				.from(shop_rooms::table_name)
				.where("shop_vnum","=",std::to_string(shop_vnum_id))
				.sql();
			mods::pq::exec(del_transaction,up_sql);
			mods::pq::commit(del_transaction);
		}catch(std::exception& e){
			REPORT_DB_ISSUE(": error deleting from '",e.what());
		}
	}
	void delete_shop_objects(shop_vnum shop_vnum_id){
		try{
			auto del_transaction = txn();
			sql_compositor comp(shop_objects::table_name,&del_transaction);
			auto up_sql = comp
				.del()
				.from(shop_rooms::table_name)
				.where("shop_vnum","=",std::to_string(shop_vnum_id))
				.sql();
			mods::pq::exec(del_transaction,up_sql);
			mods::pq::commit(del_transaction);
		}catch(std::exception& e){
			REPORT_DB_ISSUE(": error deleting from '",e.what());
		}
	}
	int64_t				shop::db_id() const { return shop_id; }
	int16_t       shop::delete_shop_record(){
		try{
			auto del_transaction = txn();
			sql_compositor comp(shop::table_name,&del_transaction);
			auto up_sql = comp
				.del()
				.from(shop::table_name)
				.where("shop_id","=",std::to_string(shop_id))
				.sql();
			mods::pq::exec(del_transaction,up_sql);
			mods::pq::commit(del_transaction);
			return 1;
		}catch(std::exception& e){
			REPORT_DB_ISSUE(": error deleting from '",e.what());
		}
		return 0;
	}
	int16_t shop::remove_room(room_vnum room){
		if(room_info.rooms.size() == 0){
			return 0;
		}
		int16_t count = 0;
		bool found = 0;

		do{
			auto it = std::find(room_info.rooms.begin(), room_info.rooms.end(), room);
			found = 0;
			if(it != room_info.rooms.end()) {
				found = 1;
				room_info.rooms.erase(it);
			}
			++count;
		}while(found);
		return count;
	}
	std::tuple<bool,db_id_t,std::string> shop::save(){
		this->delete_shop_record();
		delete_shop_rooms(shop_vnum_id);
		delete_shop_objects(shop_vnum_id);
		{
			sql_compositor::value_map s_map;
			s_map["shop_vnum"] = mods::util::itoa(shop_vnum_id);
			s_map["shop_title"] = shop_title.str();
			s_map["shop_description"] = shop_description.str();
			s_map["shop_flags"] = std::to_string(shop_flags);
			s_map["shop_profit_buy"] = mods::util::itoa(shop_profit_buy);
			s_map["shop_profit_sell"] = mods::util::itoa(shop_profit_sell);
			s_map["shop_no_such_item1"] = (shop_no_such_item1.str());
			s_map["shop_no_such_item2"] = (shop_no_such_item2.str());
			s_map["shop_missing_cash1"] = (shop_missing_cash1.str());
			s_map["shop_missing_cash2"] = (shop_missing_cash2.str());
			s_map["shop_do_not_buy"] =(shop_do_not_buy.str());
			s_map["shop_message_buy"] = (shop_message_buy.str());
			s_map["shop_message_sell"] =(shop_message_sell.str());
			s_map["shop_temper1"] = mods::util::itoa(shop_temper1);
			s_map["shop_bitvector"] = mods::util::itoa(shop_bitvector);
			s_map["shop_keeper"] = mods::util::itoa(shop_keeper);
			s_map["shop_with_who"] = mods::util::itoa(shop_with_who);
			s_map["shop_open1"] = mods::util::itoa(shop_open1);
			s_map["shop_open2"] = mods::util::itoa(shop_open2);
			s_map["shop_close1"] = mods::util::itoa(shop_close1);
			s_map["shop_close2"] = mods::util::itoa(shop_close2);
			s_map["shop_bankAccount"] = mods::util::itoa(shop_bankAccount);
			s_map["shop_lastsort"] = mods::util::itoa(shop_lastsort);
			s_map["shop_type"] = std::to_string(shop_type);
			try{
				auto insert_transaction = txn();
				sql_compositor comp(shop::table_name,&insert_transaction);
				auto ins_sql = comp
					.insert()
					.into(shop::table_name)
					.values(s_map)
					.returning("shop_id")
					.sql();
				auto row = mods::pq::exec(insert_transaction,ins_sql);
			 	mods::pq::commit(insert_transaction);
				if(room_info.rooms.size()){
					save_shop_rooms(room_info);
				}
				if(object_info.objects.size()){
					save_shop_objects(object_info);
				}
				for(auto && record : row){
					return {true,record["shop_id"].as<uint64_t>(),""};
				}
				return {false,0,"unreachable"};
			}catch(std::exception& e){
				REPORT_DB_ISSUE(": error inserting into '",e.what());
				return {false,0,e.what()};
			}
			return {false,0,"unreachable"};
		}
	}

	void save_shop_objects(shop_objects& so_list){
		for(auto & shop_object_vnum : so_list.objects){
			try{
				auto insert_transaction = txn();
				sql_compositor comp(shop_objects::table_name,&insert_transaction);
				auto ins_sql = comp
					.insert()
					.into(shop_objects::table_name)
					.values({{"shop_vnum", std::to_string(so_list.shop_vnum_id)},
							{"shop_object_vnum", std::to_string(shop_object_vnum)}
							})
				.sql();
				mods::pq::exec(insert_transaction,ins_sql);
				mods::pq::commit(insert_transaction);
			}catch(std::exception& e){
				REPORT_DB_ISSUE(": error inserting into '",e.what());
			}
		}
	}
	void save_shop_rooms(shop_rooms& sr_list){
		for(auto & shop_room_vnum : sr_list.rooms){
			try{
				auto insert_transaction = txn();
				sql_compositor comp(shop_rooms::table_name,&insert_transaction);
				auto ins_sql = comp
					.insert()
					.into(shop_rooms::table_name)
					.values({{"shop_vnum", std::to_string(sr_list.shop_vnum_id)},
							{"shop_room_vnum", std::to_string(shop_room_vnum)}
							})
				.sql();
				mods::pq::exec(insert_transaction,ins_sql);
				mods::pq::commit(insert_transaction);
			}catch(std::exception& e){
				REPORT_DB_ISSUE(": error inserting into '",e.what());
			}
		}
	}
	int16_t shop::feed(const pqxx::result::reference & row){
		shop_id = row["shop_id"].as<db_id_t>();
		shop_vnum_id = row["shop_vnum"].as<shop_vnum>();

		shop_title.assign(row["shop_title"].as<std::string>());
		shop_description.assign(row["shop_description"].as<std::string>());
		shop_flags = row["shop_flags"].as<uint64_t>();

		shop_profit_buy = row["shop_profit_buy"].as<float>();		/* Factor to multiply cost with		*/
		shop_profit_sell = row["shop_profit_sell"].as<float>();		/* Factor to multiply cost with		*/
		shop_no_such_item1.assign(row["shop_no_such_item1"].as<std::string>());		/* Message if keeper hasn't got an item	*/
		shop_no_such_item2.assign(row["shop_no_such_item2"].as<std::string>());		/* Message if player hasn't got an item	*/
		shop_missing_cash1.assign(row["shop_missing_cash1"].as<std::string>());		/* Message if keeper hasn't got cash	*/
		shop_missing_cash2.assign(row["shop_missing_cash2"].as<std::string>());		/* Message if player hasn't got cash	*/
		shop_do_not_buy.assign(row["shop_do_not_buy"].as<std::string>());		/* If keeper dosn't buy such things	*/
		shop_message_buy.assign(row["shop_message_buy"].as<std::string>());		/* Message when player buys item	*/

		shop_message_sell.assign(row["shop_message_sell"].as<std::string>());		/* Message when player sells item	*/
		shop_temper1 = row["shop_temper1"].as<int>();		/* How does keeper react if no money	*/
		shop_bitvector = row["shop_bitvector"].as<bitvector_t>();	/* Can attack? Use bank? Cast here?	*/
		shop_keeper = row["shop_keeper"].as<mob_rnum>();	/* The mobile who owns the shop (rnum)	*/
		shop_with_who = row["shop_with_who"].as<int>();		/* Who does the shop trade with?	*/
		shop_open1 = row["shop_open1"].as<int>();;
		shop_open2 = row["shop_open2"].as<int>();		/* When does the shop open?		*/
		shop_close1 = row["shop_close1"].as<int>();
		shop_close2 = row["shop_close2"].as<int>();	/* When does the shop close?		*/
		shop_bankAccount = row["shop_bankAccount"].as<int>();		/* Store all gold over 15000 (disabled)	*/
		shop_lastsort = row["shop_lastsort"].as<int>();		/* How many items are sorted in inven?	*/
		shop_type = row["shop_type"].as<int>();
		room_info.load_by_shop_vnum(shop_vnum_id);
		object_info.load_by_shop_vnum(shop_vnum_id);
		loaded = true;
		return 1;
	}

	int16_t shop_rooms::place_keepers_in_rooms(mob_vnum mob_vnum_id){
#ifdef __MENTOC_SHOPS_HAVE_KEEPERS__
		int16_t count = 0;
		for(const auto & room_vnum_id : rooms){
			auto &keeper_ptr = mods::globals::read_mobile_ptr(mob_vnum_id,VIRTUAL);
			room_keeper_map[room_vnum_id] = std::move(keeper_ptr);
			++count;
		}
		return count;
#else
		return 0;
#endif
	}
	int16_t shop_rooms::load_by_shop_vnum(shop_vnum in_shop_vnum){
		shop_vnum_id = in_shop_vnum;
		int16_t count = 0;
		for(auto && record : db_get_by_meta(shop_rooms::table_name, "shop_vnum", std::to_string(in_shop_vnum))){
			rooms.emplace_back(record["shop_room_vnum"].as<room_vnum>());
			++count;
		}
		return count;
	}

	int16_t shop_objects::load_by_shop_vnum(shop_vnum in_shop_vnum){
		shop_vnum_id = in_shop_vnum;
		int16_t count = 0;
		for(auto && record : db_get_by_meta(shop_objects::table_name, "shop_vnum", std::to_string(in_shop_vnum))){
			objects.emplace_back(record["shop_object_vnum"].as<room_vnum>());
			++count;
		}
		return count;
	}

};

