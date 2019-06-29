#include "world-configuration.hpp"
#include "pq.hpp"
#include "util.hpp"
#include "object.hpp"
#include <stdlib.h> //For itoa
#include "sql.hpp"
#include "../structs.h"
#include "extern.hpp"
#include "builder_util.hpp"
#include <tuple>

using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
namespace mods::world_conf {
	std::pair<bool,std::string> select_value(std::string_view table,
			std::string_view column,
			std::string_view where_column,std::string_view where_value){
		try{

			auto select_transaction = txn();
			sql_compositor comp(table.data(),&select_transaction);
			auto sql = comp.select(column.data())
				.from(table.data())
				.where(where_column.data(),"=",where_value.data())
				.sql();
			auto record = mods::pq::exec(select_transaction,sql);
			mods::pq::commit(select_transaction);
			return {true,record[0][column.data()].c_str()};
		}catch(std::exception& e){
			std::cerr << __FILE__ << __LINE__ << " exception: '" << e.what() << "'\n";
			return {false,e.what()};
		}
	}
	std::pair<bool,std::string> set_immortal_start_room(room_vnum room_id){
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
				"mortal_start_room","is_active","1");
		std::string mortal_start_room = "";
		if(record.first){
			mortal_start_room = record.second;
		}else{
			mortal_start_room = std::to_string(room_id);
		}
		return mods::world_conf::update_or_insert(
				mods::world_conf::START_ROOM_TABLE,
				{
					{"mortal_start_room",mortal_start_room},
					{"immortal_start_room",std::to_string(room_id)},
					{"is_active","1"}
				},
				"is_active","1");
	}
	std::pair<bool,std::string> set_mortal_start_room(room_vnum room_id){
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
				"immortal_start_room","is_active","1");
		std::string immortal_start_room = "";
		if(record.first){
			immortal_start_room = record.second;
		}else{
			immortal_start_room = std::to_string(room_id);
		}
		return mods::world_conf::update_or_insert(
				mods::world_conf::START_ROOM_TABLE,
				{
					{"immortal_start_room",immortal_start_room},
					{"mortal_start_room",std::to_string(room_id)},
					{"is_active","1"}
				},
				"is_active","1");
	}
	std::pair<bool,std::string> update_or_insert(std::string_view table,
			mutable_map_t values,std::string_view where_column,
			std::string_view where_value)
	{
		try{
			auto select_transaction = txn();
			sql_compositor comp(table.data(),&select_transaction);
			auto world_conf_sql = comp.select("*")
				.from(table.data())
				.where(where_column.data(),"=",where_value.data())
				.sql();
			std::cerr << "[debug]: sql: " << world_conf_sql << "\n";
			auto record = mods::pq::exec(select_transaction,world_conf_sql);
			mods::pq::commit(select_transaction);
			{
			if(record.size()){
				auto up_txn = txn();
				sql_compositor comp(table.data(),&up_txn);
				auto up_sql = comp
					.update(table.data())
					.set(values)
					.where(where_column.data(),"=",where_value.data())
					.sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return {true,"Updated record"};
			}else{
				auto txn3 = txn();
				auto sql = sql_compositor(table.data(),&txn3)
					.insert()
					.into(table.data())
					.values(values)
				.sql();
				mods::pq::exec(txn3,sql);
				mods::pq::commit(txn3);
				return {true,"Saved new record"};
			}
			}
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
			return {false,e.what()};
		}
	}
};
