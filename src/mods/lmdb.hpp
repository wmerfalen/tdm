#ifndef __MENTOC_MODS_LMDB_HEADER__
#define  __MENTOC_MODS_LMDB_HEADER__

#include <iostream>
#include <vector>
#include <map>
#include <string_view>
#include <memory>
#include "../structs.h"
#include "string.hpp"
#include "../liblmdb/lmdb.h"
#include <tuple>
#include "schema.hpp"
#include "pq.hpp"
using aligned_int_t = uint64_t;
using tuple_status_t = std::tuple<bool,std::string,aligned_int_t>;
	using mutable_map_t = std::map<std::string,std::string>;
	using result_container_t = std::vector<std::map<std::string,std::string>>;
namespace mods::lmdb {
	using mutable_map_t = std::map<std::string,std::string>;
	inline static std::string operator "" _s(const char* str,std::size_t size){ 
		return std::string(str);
	}
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

		transaction_t(std::string ce_table,transact_type_t type);
		transaction_t(table_type_t e_table,transact_type_t type);
		transaction_t() = delete;

		std::string table_cstr() const { return m_str_table; }
		table_type_t table() const { return m_table; }
		transact_type_t type() const { return m_type; }
		/**
		 * get(key,val) is basically like the sql version of "where".
		 * i.e.: get("user_id",123); //sql equiv: select * from users where user_id = 123
		 */
		template <typename T>
			std::string get(T consumer,std::string key);

		/**
		 * The sql equivalent of this is 'select * from table'
		 */
		template <typename T>
		result_container_t get_all(T consumer);
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
	/*
	inline transaction_ptr exec(std::string table){
		return std::move(std::make_unique<transaction_t>(table,transact_type_t::EXEC));
	}

	inline transaction_ptr update(table_type_t table){
		return std::make_unique<transaction_t>(table,transact_type_t::UPDATE);
	}
	inline transaction_ptr update(std::string table){
		return std::make_unique<transaction_t>(table,transact_type_t::UPDATE);
	}
	inline transaction_ptr insert(std::string table){
		return std::make_unique<transaction_t>(table,transact_type_t::INSERT);
	}
	*/




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
		_db_handle(std::string directory,std::string db_name,const uint64_t & flags,const uint16_t & mode,bool unused);
		std::vector<std::string> fields_to_grab(const std::string& table);
		void set_pluck_filter(const std::vector<std::string> &);
		void clear_pluck_filter();
		const std::vector<std::string>& get_pluck_filter() const;
		void use_pluck_filter(bool b);
		bool is_using_pluck_filter() const;
		bool open();
		int put(std::string key,std::string value,bool renew);
		int del(std::string key);
		status_type_t status() const;
		int get(std::string key,std::string & in_value);
		int put(std::string key,std::string value);

		int get(MDB_txn*,std::string key,std::string & in_value);
		int put(MDB_txn*,std::string key,std::string value);
		std::string get(std::string key);
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
		bool m_use_pluck;
		std::vector<std::string> m_pluck;
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

//namespace mods::globals {
//	extern std::unique_ptr<mods::lmdb::db_handle> db;
//};

void db_renew_txn();
std::string db_key(const std::vector<std::string> & parts);
std::string db_get(std::string key);
void db_put(std::string key,std::string value);
//mods::lmdb::result_container_t db_get_by_id(std::string table,std::string id);
//mods::lmdb::result_container_t db_get_by_meta(std::string table,std::string col,std::string equals);
pqxx::result db_get_all(std::string table);
//result_container_t db_get_by_meta(std::string table, std::string col,std::string value);
//result_container_t db_get_by_meta(std::string table, std::string col,const pqxx::tuple::reference& value);
pqxx::result db_get_by_meta(std::string table, std::string col,const std::string & value);
namespace mods::pq {
	using transaction = pqxx::work;
	using result = pqxx::result;
	extern pqxx::result exec(transaction& txn,std::string_view query);
};
template <typename TSqlCompositor,typename TIDListContainer>
pqxx::result db_get_by_meta_multi(std::string table, std::string col,const TIDListContainer& values){
	using txn = mods::pq::transaction;
	try{
		auto select_txn = txn();
		TSqlCompositor comp(table,&select_txn);
		auto sql_string = comp.select("*")
			.from(table)
			.where_in(col,values)
			.sql();
		return mods::pq::exec(select_txn,sql_string.data());
	}catch(std::exception& e){
		return {};
	}
}
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::tuple::reference& value);
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::field& value);
#ifdef __MENTOC_USE_PQXX_RESULT__
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::result::reference& value);
#else
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::row::reference& value);
#endif
//mods::lmdb::result_container_t db_get_all_pluck(std::string table,const std::vector<const std::string&>& pluck);
//bool db_update(mods::lmdb::table_type_t table,
//		mods::lmdb::mutable_map_t & values,
//		std::string value);
//bool db_insert(std::string table,const mods::lmdb::mutable_map_t & values);
//
//extern tuple_status_t new_record_with_values(std::string table,
//		mods::lmdb::db_handle* ptr_db,
//		mods::lmdb::mutable_map_t& values,
//		bool & error,
//		std::optional<aligned_int_t> use_this_id);
//
#endif
