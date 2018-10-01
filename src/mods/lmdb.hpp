#ifndef __MENTOC_MODS_LMDB_HEADER__
#define  __MENTOC_MODS_LMDB_HEADER__

#include <iostream>
#include <variant>
#include <vector>
#include <map>
#include <string_view>
#include <memory>
#include "../structs.h"
#include "string.hpp"
#include "../liblmdb/lmdb.h"
#include <tuple>
#include "schema.hpp"

namespace mods::lmdb {
	inline static std::string operator "" _s(const char* str,std::size_t size){ 
		return std::string(str);
	}
	using variant_t = std::variant<std::string_view,mods::string,std::string,const char*,uint64_t,int64_t,int,nullptr_t,short,char,unsigned char,char*,unsigned char*,sbyte>;
	enum transact_type_t {
		EXEC, UPDATE, INSERT 
	};
	enum table_type_t : uint16_t {
		player,mobile,object,affected_type,
		extra_description,affected_type_object_flags,
		zone,zone_data,rooms,room_direction_data
	};
	enum key_type_t : uint16_t {
		id,name,short_description,long_description,description,item_number,
		object_id,room_number,row_id_list
	};

	using mutable_map_t = std::map<std::string_view,variant_t>;
	using result_container_t = std::vector<std::map<std::string_view,variant_t>>;

	struct _selector {
		table_type_t table;
		key_type_t column;
		uint32_t reserved;
		uint64_t row_id;
	};

	using selector_type_t = _selector;

	using row_id_list_t = std::vector<uint64_t>;

	/**
	 * ##############################
	 * A typical transaction
	 * ##############################
	 */
	/**
	 * ##############################
	 * A typical transaction
	 * ##############################
	 */
	/**
	 * ##############################
	 * A typical transaction
	 * ##############################
	 */
	struct transaction_t {
		selector_type_t selector(const key_type_t & column,const uint64_t & row_id);
		uint64_t get_row_id_list(row_id_list_t & rows);
		void set_str_table(table_type_t e_table){
#define MENTOC_T(_e) case t::_e: m_str_table = #_e; break;
			using t = table_type_t;
			switch(e_table){
				MENTOC_T(player);
				MENTOC_T(mobile);
				MENTOC_T(object);
				MENTOC_T(affected_type);
				MENTOC_T(extra_description);
				MENTOC_T(affected_type_object_flags);
				MENTOC_T(zone);
				MENTOC_T(zone_data);
				MENTOC_T(rooms);
				MENTOC_T(room_direction_data);
				default: m_str_table = "unknown";	
			}
#undef MENTOC_T
		}

		transaction_t(std::string_view ce_table,transact_type_t type);
		transaction_t(table_type_t e_table,transact_type_t type);
		transaction_t() = delete;

		std::string_view table_cstr() const { return m_str_table; }
		table_type_t table() const { return m_table; }
		transact_type_t type() const { return m_type; }
		/**
		 * get(key,val) is basically like the sql version of "where".
		 * i.e.: get("user_id",123); //sql equiv: select * from users where user_id = 123
		 */
			result_container_t get(std::string_view key);

		/**
		 * The sql equivalent of this is 'select * from table'
		 */
		template <typename T>
		result_container_t get_all(T consumer);
		template <typename T>
			bool set(const mutable_map_t & values,const key_type_t & field,T value);
		bool values(const mutable_map_t & values);
		private: 
		table_type_t m_table;
		std::string m_str_table;
		transact_type_t m_type;
		MDB_env* m_env;
	};
	/**
	 * ##############################
	 * A typical transaction
	 * ##############################
	 */
	/**
	 * ##############################
	 * A typical transaction
	 * ##############################
	 */
	/**
	 * ##############################
	 * A typical transaction
	 * ##############################
	 */

	/**
	 * Transactions
	 */
	using transaction_ptr = std::unique_ptr<transaction_t>;
	using transaction_pool_t = std::vector<std::unique_ptr<transaction_t>>;
	inline transaction_ptr exec(std::string_view table){
		return std::move(std::make_unique<transaction_t>(table,transact_type_t::EXEC));
	}

	inline transaction_ptr update(table_type_t table){
		return std::make_unique<transaction_t>(table,transact_type_t::UPDATE);
	}
	inline transaction_ptr update(std::string_view table){
		return std::make_unique<transaction_t>(table,transact_type_t::UPDATE);
	}
	inline transaction_ptr insert(std::string_view table){
		return std::make_unique<transaction_t>(table,transact_type_t::INSERT);
	}




/*
 #####   #####           #    #    ##    #    #  #####   #       ######
 #    #  #    #          #    #   #  #   ##   #  #    #  #       #
 #    #  #####           ######  #    #  # #  #  #    #  #       #####
 #    #  #    #          #    #  ######  #  # #  #    #  #       #
 #    #  #    #          #    #  #    #  #   ##  #    #  #       #
 #####   #####  #######  #    #  #    #  #    #  #####   ######  ######
*/

	struct _db_handle {
		constexpr static std::size_t status_step_count = 5;
		constexpr static int KEY_NOT_FOUND = 0;
		constexpr static int KEY_FETCHED_OKAY = 1;
		using tuple_return_type_t = std::tuple<bool,std::string>; 
		using status_type_t = std::array<std::tuple<bool,std::string>,status_step_count>;
		_db_handle(std::string_view directory,std::string_view db_name,const uint64_t & flags,const uint16_t & mode,bool unused);
		bool open();
		int put(std::string_view key,const std::string & value,bool renew);
		int del(std::string_view key);
		status_type_t status() const;
		int get(std::string_view key,std::string & in_value);
		int put(std::string_view key,const std::string & value);
		int put(const std::string& key,const std::string & value);

		int get(MDB_txn*,std::string_view key,std::string & in_value);
		int put(MDB_txn*,std::string_view key,const std::string & value);
		tuple_return_type_t commit();
		tuple_return_type_t new_txn();
		tuple_return_type_t new_txn(MDB_txn**);
		tuple_return_type_t open_dbi();
		tuple_return_type_t abort_txn();
		_db_handle() = delete;
		~_db_handle();
		void close();
		void dump_status() const;
		tuple_return_type_t renew_txn();
		private:
		bool m_dbi_opened;
		bool m_good;
		bool m_closed;
		void m_clear_status(){
			for(unsigned i = 0; i < status_step_count; ++i){
				m_status[i] = {false,""};
			}
		}
		status_type_t m_status;
		bool m_transaction_open;
		bool m_transaction_good;
		MDB_env*	m_env;
		MDB_txn*	m_txn;
		MDB_dbi	m_dbi;
		std::string m_dir;
		std::string m_name;
		uint64_t m_flags;
		uint16_t m_mode;
	};


	using db_handle = _db_handle;

};

namespace mods::globals {
	extern std::unique_ptr<mods::lmdb::db_handle> db;
};


std::string db_key(const std::vector<std::string_view> & parts);

mods::lmdb::result_container_t db_get(std::string_view table,std::string_view key);
mods::lmdb::result_container_t db_get_all(std::string_view table);
bool db_update(std::string_view table,const mods::lmdb::mutable_map_t & values,
		std::string_view field,std::string_view value );
bool db_update(mods::lmdb::table_type_t table,const mods::lmdb::mutable_map_t & values,const mods::lmdb::key_type_t & field,std::string_view value );
bool db_insert(std::string_view table,const mods::lmdb::mutable_map_t & values);


#endif
