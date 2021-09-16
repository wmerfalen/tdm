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
	rooms_t rooms;
	std::pair<bool,std::string> select_value(std::string_view table,
	                                         std::string_view column,
	                                         std::string_view where_column,std::string_view where_value) {
		try {

			auto select_transaction = txn();
			sql_compositor comp(table.data(),&select_transaction);
			auto sql = comp.select(column.data())
			           .from(table.data())
			           .where(where_column.data(),"=",where_value.data())
			           .sql();
			auto record = mods::pq::exec(select_transaction,sql);
			mods::pq::commit(select_transaction);
			return {true,record[0][column.data()].c_str()};
		} catch(std::exception& e) {
			std::cerr << __FILE__ << __LINE__ << " exception: '" << e.what() << "'\n";
			return {false,e.what()};
		}
	}
	rooms_t load_all() {
		rooms[WC_FROZEN] = real_room(get_frozen_room_vnum());
		rooms[WC_IDLE] = real_room(get_idle_room_vnum());
		rooms[WC_IMM] = real_room(get_immortal_start_room_vnum());
		rooms[WC_MOR] = real_room(get_mortal_start_room_vnum());
		std::cerr << "Mortal room: " << rooms[WC_MOR] << "\n";
		return rooms;
	}
	int real_frozen() {
		return rooms[WC_FROZEN];
	}
	int real_idle() {
		return rooms[WC_IDLE];
	}
	int real_immortal_start() {
		return rooms[WC_IMM];
	}
	int real_mortal_start() {
		return rooms[WC_MOR];
	}
	/**
	 * frozen room
	 */
	int get_frozen_room_vnum() {
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
		                           "frozen_room","is_active","1");
		std::string frozen_room = "";
		if(record.first) {
			frozen_room = record.second;
		}
		return mods::util::stoi(frozen_room).value_or(-1);
	}
	/**
	 * immortal start
	 */
	int get_immortal_start_room_vnum() {
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
		                           "immortal_start_room","is_active","1");
		std::string immortal_start_room = "";
		if(record.first) {
			immortal_start_room = record.second;
		}
		return mods::util::stoi(immortal_start_room).value_or(-1);
	}
	/**
	 * mortal start
	 */
	int get_mortal_start_room_vnum() {
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
		                           "mortal_start_room","is_active","1");
		std::string mortal_start_room = "";
		if(record.first) {
			mortal_start_room = record.second;
		}
		return mods::util::stoi(mortal_start_room).value_or(-1);
	}

	/**
	 * idle room
	 */
	int get_idle_room_vnum() {
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
		                           "idle_room","is_active","1");
		std::string mortal_start_room = "";
		if(record.first) {
			mortal_start_room = record.second;
		}
		return mods::util::stoi(mortal_start_room).value_or(-1);
	}

	std::pair<bool,std::string> set_value(std::string_view column, room_vnum room_id) {
		{
			std::map<std::string,std::string> values;
			values[column.data()] = std::to_string(room_id);
			try {
				auto up_txn = txn();
				sql_compositor comp("world_configuration_start_rooms",&up_txn);
				auto up_sql = comp
				              .update("world_configuration_start_rooms")
				              .set(values)
				              .where("is_active","=","1")
				              .sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return {true,"Updated record"};
			} catch(std::exception& e) {
				return {false,e.what()};
			}
		}
	}

	std::pair<bool,std::string> set_frozen_room(room_vnum room_id) {
		return set_value("frozen_room",room_id);
	}
	std::pair<bool,std::string> set_idle_room(room_vnum room_id) {
		return set_value("idle_room",room_id);
	}

	std::pair<bool,std::string> set_immortal_start_room(room_vnum room_id) {
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
		                           "mortal_start_room","is_active","1");
		std::string mortal_start_room = "";
		if(record.first) {
			mortal_start_room = record.second;
		} else {
			mortal_start_room = std::to_string(room_id);
		}
		return mods::world_conf::update_or_insert(
		mods::world_conf::START_ROOM_TABLE, {
			{"mortal_start_room",mortal_start_room},
			{"immortal_start_room",std::to_string(room_id)},
			{"is_active","1"}
		},
		"is_active","1");
	}

	std::pair<bool,std::string> set_mortal_start_room(room_vnum room_id) {
		auto record = select_value(mods::world_conf::START_ROOM_TABLE,
		                           "mortal_start_room","is_active","1");
		std::string immortal_start_room = "";
		if(record.first) {
			immortal_start_room = record.second;
		} else {
			immortal_start_room = std::to_string(room_id);
		}
		return mods::world_conf::update_or_insert(
		mods::world_conf::START_ROOM_TABLE, {
			{"immortal_start_room",immortal_start_room},
			{"mortal_start_room",std::to_string(room_id)},
			{"is_active","1"}
		},
		"is_active","1");
	}
	std::pair<bool,std::string> update_or_insert(std::string_view table,
	                                             mutable_map_t values,std::string_view where_column,
	                                             std::string_view where_value) {
		try {
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
				if(record.size()) {
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
				} else {
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
		} catch(std::exception& e) {
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
			return {false,e.what()};
		}
	}
	namespace toggle {
		bool status_obj_from_room;
		bool get_obj_from_room() {
			return mods::world_conf::toggle::status_obj_from_room;
		}
		void set_obj_from_room(bool b) {
			mods::world_conf::toggle::status_obj_from_room = b;
		}
	};
};
