#include "lmdb.hpp"
namespace mods::lmdb {
		selector_type_t transaction_t::selector(const key_type_t & column,const uint64_t & row_id){
			return {m_table,column,0,row_id};
		}
		uint64_t transaction_t::get_row_id_list(row_id_list_t & rows){
			//auto key = selector(key_type_t::row_id_list,0);
			return 0;
		}

		transaction_t::transaction_t(std::string_view ce_table,transact_type_t type) : m_type(type){
			m_str_table = ce_table;
		}
		transaction_t::transaction_t(table_type_t e_table,transact_type_t type) : m_table(e_table), m_type(type){
			set_str_table(e_table);
		}

		/**
		 * get(key,val) is basically like the sql version of "where".
		 * i.e.: get("user_id",123); //sql equiv: select * from users where user_id = 123
		 */
			result_container_t transaction_t::get(std::string_view key){
				result_container_t r;
				
				return result_container_t();
			}

		/**
		 * The sql equivalent of this is 'select * from table'
		 */
		template <typename T>
		result_container_t transaction_t::get_all(T consumer){
			//TODO: fetch results
			std::string id_list_selector  = std::string(table_cstr()) + "|id_list";
			std::string csv_list;
			if(consumer->get(id_list_selector,csv_list) < 0){
				std::cerr << "[lmdb]get_all failed with query: " << id_list_selector << "\n";
			}
			else{
				std::cerr << "[lmdb]revrieved: " << csv_list << "\n";
			}
			return  result_container_t();
		}
		template <typename T>
			bool transaction_t::set(const mutable_map_t & values,const key_type_t & field,T value){
				//TODO: write the values
				return true;
			}
		bool transaction_t::values(const mutable_map_t & values){
			return true;
		}
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

/*
 #####   #####           #    #    ##    #    #  #####   #       ######
 #    #  #    #          #    #   #  #   ##   #  #    #  #       #
 #    #  #####           ######  #    #  # #  #  #    #  #       #####
 #    #  #    #          #    #  ######  #  # #  #    #  #       #
 #    #  #    #          #    #  #    #  #   ##  #    #  #       #
 #####   #####  #######  #    #  #    #  #    #  #####   ######  ######
*/

		_db_handle::_db_handle(std::string_view directory,std::string_view db_name,const uint64_t & flags,const uint16_t & mode,bool unused) 
			:m_good(false), m_closed(true), m_dir(directory), m_name(db_name), m_flags(flags), m_mode(mode) {
				m_transaction_open = false;
				m_transaction_good = false;
				open();
			}
		/*
		std::tuple<bool,std::string,MDB_txn*> _db_handle::new_txn(){
			int r = 0;
			std::string reason{""};
			if((r = mdb_txn_begin(m_env,0,0,&m_txn)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_PANIC: reason = "a fatal error occurred earlier and the environment must be shut down."; break;
					case MDB_MAP_RESIZED: reason = "another process wrote data beyond this MDB_env's mapsize and this environment's map must be resized as well. See mdb_env_set_mapsize()."; break;
					case MDB_READERS_FULL: reason = "a read-only transaction was requested and the reader lock table is full. See mdb_env_set_maxreaders()."; break;
					case ENOMEM: reason = "out of memory."; break;
					default: reason += "|mdb_strerror:";
									 reason += mdb_strerror(r); break;
				}
				reason += " [via:mdb_txn_begin]";
				return {false,reason,m_txn};
			}
			return {true,reason,m_txn};
		}

		bool _db_handle::new_dbi(){
			int r = 0;
			std::string reason{""};
			if((r = mdb_dbi_open(m_txn,0,0,&m_dbi)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_NOTFOUND : reason = "the specified database doesn't exist in the environment and MDB_CREATE was not specified."; break;
					case MDB_DBS_FULL : reason = "too many databases have been opened. See mdb_env_set_maxdbs()."; break;
					default: reason += "|mdb_strerror:";
									 reason += mdb_strerror(r); break;
				}
				reason += " [via:mdb_dbi_open]";
				reason += " m_name: '";
				reason += m_name + "'";
				m_status[3] = {false,reason};
				m_good = false;
				return false;
			}
			m_status[4] = {true,"DBI handle opened successfully. Ready for get/put."};
			m_good = true;
			return true;
		}
		*/
		bool _db_handle::open(){
			std::cerr << "[lmdb]::open()\n";
			m_closed = true;
			m_good = false;
			m_clear_status();
			int r = 0;
			if((r = mdb_env_create(&m_env)) != 0){
				m_status[0] = {false,"There was an error in mdb_env_create(): "_s + std::to_string(r) + \
				"|mdb_strerror: " + mdb_strerror(r) + "[via:mdb_env_create]"};
				close();
				m_good = false;
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
					default: reason += "|mdb_strerror:";
									 reason += mdb_strerror(r); break;
				}
				reason += " [via:mdb_env_open]";
				m_status[1] = {false,reason};
				close();
				m_good = false;
				return false;
			}
			m_status[1] = {true,"Environment opened successfully."};
			m_closed = false;

			if((r = mdb_txn_begin(m_env,0,0,&m_txn)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_PANIC: reason = "a fatal error occurred earlier and the environment must be shut down."; break;
					case MDB_MAP_RESIZED: reason = "another process wrote data beyond this MDB_env's mapsize and this environment's map must be resized as well. See mdb_env_set_mapsize()."; break;
					case MDB_READERS_FULL: reason = "a read-only transaction was requested and the reader lock table is full. See mdb_env_set_maxreaders()."; break;
					case ENOMEM: reason = "out of memory."; break;
					default: reason += "|mdb_strerror:";
									 reason += mdb_strerror(r); break;
				}
				reason += " [via:mdb_txn_begin]";
				m_status[2] = {false,reason};
				close();
				m_transaction_good = m_transaction_open = m_good = false;
				return false;
			}
			m_status[2] = {true,"Transaction opened successfully."};
			m_transaction_good = m_transaction_open = true;
			if((r = mdb_dbi_open(m_txn,0,0,&m_dbi)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_NOTFOUND : reason = "the specified database doesn't exist in the environment and MDB_CREATE was not specified."; break;
					case MDB_DBS_FULL : reason = "too many databases have been opened. See mdb_env_set_maxdbs()."; break;
					default: reason += "|mdb_strerror:";
									 reason += mdb_strerror(r); break;
				}
				reason += " [via:mdb_dbi_open]";
				reason += " m_name: '";
				reason += m_name + "'";
				m_status[3] = {false,reason};
				close();
				m_good = false;
				return false;
			}
			m_status[4] = {true,"DBI handle opened successfully. Ready for get/put."};
			m_good = true;
			return true;
		}
		bool _db_handle::renew_txn(){
			if(m_transaction_open){
				mdb_txn_abort(m_txn);
				m_transaction_open = false;
				m_transaction_good = false;
			}
			int r = 0;
			std::string reason{""};
			if((r = mdb_txn_begin(m_env,0,0,&m_txn)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_PANIC: reason = "a fatal error occurred earlier and the environment must be shut down."; break;
					case MDB_MAP_RESIZED: reason = "another process wrote data beyond this MDB_env's mapsize and this environment's map must be resized as well. See mdb_env_set_mapsize()."; break;
					case MDB_READERS_FULL: reason = "a read-only transaction was requested and the reader lock table is full. See mdb_env_set_maxreaders()."; break;
					case ENOMEM: reason = "out of memory."; break;
					default: reason += "|mdb_strerror:";
									 reason += mdb_strerror(r); break;
				}
				reason += " [via:mdb_txn_begin]";
				m_status[2] = {false,reason};
				return false;
			}
			m_status[2] = {true,"Transaction opened successfully."};
			/*
			if((r = mdb_dbi_open(m_txn,0,0,&m_dbi)) != 0){
				std::string reason = "An unknown error occured: "_s + std::to_string(r);
				switch(r){
					case MDB_NOTFOUND : reason = "the specified database doesn't exist in the environment and MDB_CREATE was not specified."; break;
					case MDB_DBS_FULL : reason = "too many databases have been opened. See mdb_env_set_maxdbs()."; break;
					default: reason += "|mdb_strerror:";
									 reason += mdb_strerror(r); break;
				}
				reason += " [via:mdb_dbi_open]";
				reason += " m_name: '";
				reason += m_name + "'";
				m_status[3] = {false,reason};
				return false;
			}
			m_status[4] = {true,"DBI handle opened successfully. Ready for get/put."};
			m_good = true;
			*/
			return true;
		}
			int _db_handle::get(std::string_view key,std::string & in_value){
				if(m_good){
					std::cerr << "[lmdb]get::m_good okay\n";
					MDB_val k;
					k.mv_size = key.length();
					k.mv_data = (void*)key.data();
					std::cerr << "[debug]db_handle::get key.data: '" << key.data() << "'";
					std::cerr << "[debug]key.length: " << key.length() << "\n";
					MDB_val v;
					int ret = mdb_get(m_txn,m_dbi,&k,&v);
					switch(ret){
						case MDB_NOTFOUND:
							return 0;
						case EINVAL:
							std::cerr << "[lmdb] invalid parameter to mdb_get\n";
							return -1;
						default:
							char buf[v.mv_size + 1];
							memset(buf,0,v.mv_size +1);
							bcopy(v.mv_data,buf,v.mv_size);
							in_value = buf;
							return 1;
					}
				}else{
					std::cerr << "[lmdb]get::m_good NOT okay\n";
					return -2;
				}
			}
			int _db_handle::put(std::string_view key,const std::string & value,bool renew){
				if(renew){
					if(m_transaction_good || m_transaction_open){
						mdb_txn_commit(m_txn);
						m_transaction_good = m_transaction_open = false;
					}
					renew_txn();
				}
				return put(std::string(key.data()),value);
			}
			int _db_handle::put(std::string_view key,const std::string & value){
				return put(std::string(key.data()),value);
			}
			int _db_handle::put(const std::string& key,const std::string & value){
				if(m_good){
					std::cerr << "[lmdb]::put m_good okay\n";
					MDB_val k;
					k.mv_size = key.length();
					k.mv_data = (void*)key.data();
					MDB_val v;
					v.mv_size = value.length();
					v.mv_data = (void*)value.data();
					int ret = mdb_put(m_txn,m_dbi,&k,&v,0);
					switch(ret){
						case MDB_MAP_FULL:
							std::cerr << "[lmdb] database is full, see mdb_env_set_mapsize()\n";
							return -3;
						case EINVAL:
							std::cerr << "[lmdb] invalid parameter to mdb_get\n";
							return -1;
						case EACCES:
							std::cerr << "[lmdb] invalid parameter to mdb_get\n";
							return -4;
						case MDB_TXN_FULL:
							std::cerr << "[lmdb] transaction has too many dirty pages\n";
							return -5;
						default:
							std::cerr << "[lmdb]::default error: " << ret << "\n";
							std::cerr << "[lmdb]::default strerror: " << mdb_strerror(ret) << "\n";
							if(std::string("Successful").compare(mdb_strerror(ret)) == 0){
								return 0;
							}
					}
				}else{
					std::cerr << "[lmdb]::put m_good NOT okay\n";
					return -2;
				}
				return 0;
			}
			std::tuple<bool,std::string_view> _db_handle::commit(){
				if(m_good){
					auto ret = mdb_txn_commit(m_txn);
					if(ret){
						m_transaction_good = false;
						return {false,std::string("[lmdb]::commit failed: ") + mdb_strerror(ret)};
					}else{
						m_transaction_good = false;
						return {true,"[lmdb]::commit good"};
					}
				}else{
					return {false,"[lmdb]::commit [m_good is not true]-> Will not commit due to this invalid state."};
				}
			}
			_db_handle::~_db_handle(){
			m_good = false;
			if(std::get<0>(m_status[0])){
				mdb_env_close(m_env);
			}
			if(m_transaction_good || m_transaction_open){
				mdb_txn_abort(m_txn);
			}
		}
		void _db_handle::close(){
			if(m_closed){
				std::cerr << "[_db_handle]::close already closed\n";
				return;
			}
			if(std::get<0>(m_status[3])){
				mdb_dbi_close(m_env,m_dbi);
			}
			if(std::get<0>(m_status[0])){
				mdb_env_close(m_env);
			}
			if(m_transaction_open){
				mdb_txn_abort(m_txn);
			}
			m_transaction_open = false;
			m_transaction_good = false;
			m_good = false;
			m_closed = true;
		}
		void _db_handle::dump_status() const {
			for(unsigned i=0; i < status_step_count;++i){
				std::cerr << "[" << i << "]: " ;
				if(std::get<0>(m_status[i])){
					std::cerr << " good -> " << std::get<1>(m_status[i]);
				}else{
					std::cerr << " bad -> " << std::get<1>(m_status[i]);
				}
				std::cerr << "\n";
			}
		}


};

namespace mods::globals {
	extern std::unique_ptr<mods::lmdb::db_handle> db;
};

std::string db_key(const std::vector<std::string_view> & parts){
	std::string query = "";
	std::size_t count = 0;
	for(auto & part : parts){
		query.append(part);
		if(++count == parts.size()){
			break;
		}
		query.append("|");
	}
	return query;
}

mods::lmdb::result_container_t db_get(std::string_view table,std::string_view key){
	return mods::lmdb::exec(table)->get(key);
}

mods::lmdb::result_container_t db_get_all(std::string_view table){
	return mods::lmdb::exec(table)->get_all<mods::lmdb::db_handle*>(mods::globals::db.get());
}

bool db_update(std::string_view table,const mods::lmdb::mutable_map_t & values,
		std::string_view field,std::string_view value ){
	//return mods::lmdb::update(table)->set<const T&>(values,field,value);
	return true;
}
bool db_update(mods::lmdb::table_type_t table,const mods::lmdb::mutable_map_t & values,const mods::lmdb::key_type_t & field,std::string_view value ){
	//return mods::lmdb::update(table)->set<const T&>(values,field,value);
	return true;
}

bool db_insert(std::string_view table,const mods::lmdb::mutable_map_t & values){
	return mods::lmdb::insert(table)->values(values);
}
