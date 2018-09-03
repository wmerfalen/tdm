#ifndef __MENTOC_MODS_LMDB_HEADER__
#define  __MENTOC_MODS_LMDB_HEADER__

#include <iostream>
#include <variant>
#include <vector>
#include <map>
#include <string_view>
#include <memory>
#include "string.hpp"
#include "../liblmdb/lmdb.h"
#include <tuple>

namespace mods::lmdb {
	std::string operator "" _s(const char*,std::size_t);
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

	struct consumer {};
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
	 * A typical transaction
	 */
	struct transaction_t {
		selector_type_t selector(const key_type_t & column,const uint64_t & row_id){
			return {m_table,column,0,row_id};
		}
		uint64_t get_row_id_list(row_id_list_t & rows){
			auto key = selector(key_type_t::row_id_list,0);
			std::cerr << key.row_id << "\n";
			return 0;
		}
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

		transaction_t(table_type_t e_table,transact_type_t type) : m_table(e_table), m_type(type){
			set_str_table(e_table);
		}
		transaction_t() = delete;
		std::string_view table_cstr() const { return m_str_table; }
		table_type_t table() const { return m_table; }
		transact_type_t type() const { return m_type; }
		/**
		 * get(key,val) is basically like the sql version of "where".
		 * i.e.: get("user_id",123); //sql equiv: select * from users where user_id = 123
		 */
		template <typename T>
			result_container_t get(const key_type_t & key,T val){
				result_container_t r;

				return result_container_t();
			}

		/**
		 * The sql equivalent of this is 'select * from table'
		 */
		result_container_t get_all(){
			//TODO: fetch results
			return  result_container_t();
		}
		template <typename T>
			bool set(const mutable_map_t & values,const key_type_t & field,T value){
				//TODO: write the values
				return true;
			}
		bool values(const mutable_map_t & values){
			return true;
		}
		private: 
		table_type_t m_table;
		std::string m_str_table;
		transact_type_t m_type;
		MDB_env* m_env;
	};

	/**
	 * Transactions
	 */
	using transaction_ptr = std::unique_ptr<transaction_t>;
	using transaction_pool_t = std::vector<std::unique_ptr<transaction_t>>;
	inline static transaction_pool_t transact_pool;

	inline transaction_ptr exec(table_type_t table){
		return std::move(transact_pool.emplace_back(std::make_unique<transaction_t>(table,transact_type_t::EXEC)));
	}

	inline transaction_ptr update(table_type_t table){
		return std::move(transact_pool.emplace_back(std::make_unique<transaction_t>(table,transact_type_t::UPDATE)));
	}
	inline transaction_ptr insert(table_type_t table){
		return std::move(transact_pool.emplace_back(std::make_unique<transaction_t>(table,transact_type_t::INSERT)));
	}
	struct _db_handle {
		using status_type_t = std::array<std::tuple<bool,std::string>,3>;
		_db_handle(std::string_view directory,std::string_view db_name,const uint64_t & flags,const uint16_t & mode,bool unused) 
			:m_dir(directory), m_name(db_name), m_flags(flags), m_mode(mode) {
				open();
			}
		bool open(){
			m_clear_status();
			int r = 0;
			if((r = mdb_env_create(&m_env)) != 0){
				m_status[0] = {false,"There was an error in mdb_env_create(): "_s + std::to_string(r)};
				close();
				return false;
			}
			m_status[1] = {true,"Environment created successfully."};
			if((r = mdb_env_open(m_env,m_dir.c_str(),m_flags,m_mode)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_VERSION_MISMATCH:
						reason = "the version of the LMDB library doesn't match the version that created the database environment.";
						break;
					case MDB_INVALID:
						reason = "the environment file headers are corrupted.";
						break;
					case ENOENT:
						reason = "the directory specified by the path parameter doesn't exist.";
						break;
					case EACCES:
						reason = "the user didn't have permission to access the environment files.";
						break;
					case EAGAIN: 
						reason = "the environment was locked by another process.";
						break;
					default: break;
				}
				m_status[1] = {false,reason};
				close();
				return false;
			}
			m_status[1] = {true,"Environment opened successfully."};

			if((r = mdb_txn_begin(m_env,nullptr,0,&m_txn)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_PANIC: reason = "a fatal error occurred earlier and the environment must be shut down."; break;
					case MDB_MAP_RESIZED: reason = "another process wrote data beyond this MDB_env's mapsize and this environment's map must be resized as well. See mdb_env_set_mapsize()."; break;
					case MDB_READERS_FULL: reason = "a read-only transaction was requested and the reader lock table is full. See mdb_env_set_maxreaders()."; break;
					case ENOMEM: reason = "out of memory."; break;
					default: break;
				}
				m_status[2] = {false,reason};
				close();
				return false;
			}
			m_status[2] = {true,"Transaction opened successfully."};
			if((r = mdb_dbi_open(m_txn,nullptr,m_flags,&m_dbi)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_NOTFOUND : reason = "the specified database doesn't exist in the environment and MDB_CREATE was not specified."; break;
					case MDB_DBS_FULL : reason = "too many databases have been opened. See mdb_env_set_maxdbs()."; break;
					default: break;
				}
				m_status[3] = {false,reason};
				close();
				return false;
			}
			m_status[4] = {true,"DBI handle opened successfully. Ready for get/put."};
			return true;
		}
		status_type_t status() const {
			return m_status;
		}
		_db_handle() = delete;
		~_db_handle(){
			if(std::get<0>(m_status[0])){
				mdb_env_close(m_env);
			}
		}
		void close(){
			if(std::get<0>(m_status[3])){
				mdb_dbi_close(m_env,m_dbi);
			}
			if(std::get<0>(m_status[0])){
				mdb_env_close(m_env);
			}
		}
		private:
		void m_clear_status(){
			for(unsigned i = 0; i < 5; ++i){
				m_status[i] = {false,nullptr};
			}
		}
		status_type_t m_status;
		MDB_env*	m_env;
		MDB_txn*	m_txn;
		MDB_dbi	m_dbi;
		std::string m_dir;
		std::string m_name;
		uint64_t m_flags;
		uint16_t m_mode;
	};

	using handle = _db_handle;
};



template <typename T>
inline mods::lmdb::result_container_t db_get(mods::lmdb::table_type_t table,const mods::lmdb::key_type_t & field,T& value){
	return mods::lmdb::exec(table)->get<T>(field,value);
}

inline mods::lmdb::result_container_t db_get_all(mods::lmdb::table_type_t table){
	return mods::lmdb::exec(table)->get_all();
}

template <typename T>
inline bool db_update(mods::lmdb::table_type_t table,const mods::lmdb::mutable_map_t & values,const mods::lmdb::key_type_t & field,const T & value ){
	return mods::lmdb::update(table)->set<const T&>(values,field,value);
}

inline bool db_insert(mods::lmdb::table_type_t table,const mods::lmdb::mutable_map_t & values){
	return mods::lmdb::insert(table)->values(values);
}

#endif

