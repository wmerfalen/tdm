#include "lmdb.hpp"
#include "db.hpp"
#include <sstream>
#include "util.hpp"
#ifndef __MENTOC_USE_LMDB_INSTEAD_OF_POSTGRES__
#include "sql.hpp"
#include "pq.hpp"
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
#endif
#define __MENTOC_USE_LMDB__ 1
namespace mods::lmdb {
	using aligned_int_t = uint64_t;
	/**
	 * @brief builds a selector for grabbing values by metadata
	 *
	 * @param column
	 * @param row_id
	 *
	 * @return 
	 */
	selector_type_t transaction_t::selector(const key_type_t & column,const uint64_t & row_id){
		return {m_table,column,0,row_id};
	}

	/**
	 * @brief constructor. initializes table and transaction type
	 *
	 * @param ce_table
	 * @param type
	 */
	transaction_t::transaction_t(std::string ce_table,transact_type_t type) : m_type(type){
		m_str_table = ce_table;
	}
	/**
	 * @brief constructor. initializes table and transcation type
	 *
	 * @param e_table
	 * @param type
	 */
	transaction_t::transaction_t(table_type_t e_table,transact_type_t type) : m_table(e_table), m_type(type){
		set_str_table(e_table);
	}

	/**
	 * get(key,val) is basically like the sql version of "where".
	 * i.e.: get("user_id",123); //sql equiv: select * from users where user_id = 123
	 */

	/**
	 * The sql equivalent of this is 'select * from table'
	 */
	/**
	 * @brief grabs all items from the db using consumer for extra data
	 *
	 * @tparam T
	 * @param consumer
	 *
	 * @return 
	 */
	template <typename T>
		result_container_t transaction_t::get_all(T consumer){
			std::string csv_list;
			csv_list = consumer->get(db_key({table_cstr(),"id_list"}));
			result_container_t results;
			mutable_map_t mapped;
			std::vector<aligned_int_t> deserialized_id_list;
			if(!csv_list.empty()){
				std::copy(csv_list.begin(),csv_list.end(),std::back_inserter(deserialized_id_list));
			}
			csv_list.clear();
			for(auto & id: deserialized_id_list){
				for(auto & field_name : consumer->fields_to_grab(table_cstr())){
					std::string element_selector  = db_key({table_cstr(),
							field_name.c_str(),
							std::to_string(id)});
					std::string value;
					value.clear();
					value = consumer->get(db_key({table_cstr(),field_name,std::to_string(id)}));
					mapped.insert(std::make_pair(
								field_name,
								value));
				}
				if(mapped.size()){
					results.emplace_back(mapped);
					mapped.clear();
				}
			}
			return  results;
		}

	/*
#####   #####           #    #    ##    #    #  #####   #       ######
#    #  #    #          #    #   #  #   ##   #  #    #  #       #
#    #  #####           ######  #    #  # #  #  #    #  #       #####
#    #  #    #          #    #  ######  #  # #  #    #  #       #
#    #  #    #          #    #  #    #  #   ##  #    #  #       #
#####   #####  #######  #    #  #    #  #    #  #####   ######  ######
*/
	/**
	 * @brief returns list of fields that we are interested in grabbing
	 *
	 * @param table
	 *
	 * @return 
	 */
	std::vector<std::string> _db_handle::fields_to_grab(const std::string& table){
		auto ptr_db = mods::globals::db.get();
		if(ptr_db->is_using_pluck_filter()){
			return get_pluck_filter();
		}else{
			return mods::schema::db[table];
		}
	}

#ifdef __MENTOC_SHOW_LMDB_DEBUG__
	void lmdb_debug(std::string f){ std::cerr << "[lmdb_debug]:'" << f << "'\n"; }
#else
	#define lmdb_debug(a) /** */
#endif

	/**
	 * @brief set list of fields we are interested in
	 *
	 * @param cols
	 */
	void _db_handle::set_pluck_filter(const std::vector<std::string> & cols){
		m_pluck = cols;
		m_use_pluck = true;
	}
	/**
	 * @brief clears fields we're interested in
	 */
	void _db_handle::clear_pluck_filter(){
		m_pluck.clear();
		m_use_pluck = false;
	}
	/**
	 * @brief grabs fields we're interested in
	 *
	 * @return 
	 */
	const std::vector<std::string> & _db_handle::get_pluck_filter() const {
		return m_pluck;
	}
	/**
	 * @brief toggle whether or not to use the pluck filter
	 *
	 * @param b
	 */
		void _db_handle::use_pluck_filter(bool b){
			m_use_pluck = b;
		}
		/**
		 * @brief get status of toggle
		 *
		 * @return 
		 */
		bool _db_handle::is_using_pluck_filter() const { 
			return m_use_pluck;
		}
		/**
		 * @brief constructor. dictates director, name of db, flags, mode.
		 *
		 * @param directory
		 * @param db_name
		 * @param flags
		 * @param mode
		 * @param unused
		 */
	_db_handle::_db_handle(std::string directory,std::string db_name,const uint64_t & flags,const uint16_t & mode,bool unused) 
		: m_use_pluck(false), m_good(false), m_closed(true), 
		m_dir(directory), m_name(db_name), m_flags(flags), m_mode(mode) {
#ifdef __MENTOC_USE_LMDB__
			lmdb_debug("using lmdb. opening..");
			m_transaction_open = false;
			m_transaction_good = false;
			m_dbi_opened = false;
			auto status = open();
			if(status){
				lmdb_debug("open successfully");
			}else{
				lmdb_debug("Not opened successfully!");
			}
#endif
		}
	/**
	 * @brief opens the connection to lmdb
	 *
	 * @return 
	 */
	bool _db_handle::open(){
#ifdef __MENTOC_USE_LMDB__
		m_closed = true;
		m_good = false;
		m_clear_status();
		int r = 0;
		if((r = mdb_env_create(&m_env)) != 0){
			m_status[0] = {false,"There was an error in mdb_env_create(): "_s + std::to_string(r) + \
				"|mdb_strerror: " + mdb_strerror(r) + "[via:mdb_env_create]"};
			close();
			lmdb_debug(std::string("env_create failed"));
			lmdb_debug(std::get<1>(m_status[0]));
			m_good = false;
			return false;
		}
		m_status[1] = {true,"Environment created successfully."};
			lmdb_debug("env_create good!");
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

		m_good = true;
		return true;
#else
		return true;
#endif
	}
	/**
	 * @brief aborts transaction
	 *
	 * @return 
	 */
	_db_handle::tuple_return_type_t _db_handle::abort_txn(){
#ifdef __MENTOC_USE_LMDB__
		if(m_transaction_open){
			mdb_txn_abort(m_txn);
			m_transaction_open = false;
			m_transaction_good = false;
			return {true,""};
		}
		return {false,"A transaction wasn't open to begin with."};
#else
		return {true,"abort_txn stub"};
#endif
	}
	/**
	 * @brief creates a brand new transaction
	 *
	 * @return 
	 */
	_db_handle::tuple_return_type_t _db_handle::new_txn(){
#ifdef __MENTOC_USE_LMDB__
		int r = 0;
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
			m_transaction_good = m_transaction_open = false;
			return {false,reason};
		}
		m_transaction_good = m_transaction_open = true;
		return {true,""};
#else
		return {true,""};
#endif
	}
	/**
	 * @brief opens dbi. required by lmdb
	 *
	 * @return 
	 */
	_db_handle::tuple_return_type_t _db_handle::open_dbi(){
#ifdef __MENTOC_USE_LMDB__
		int r = 0;
		if((r = mdb_dbi_open(m_txn,0,0,&m_dbi)) != 0){
			m_dbi_opened= false;
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
			return {false,reason};
		}
		m_status[4] = {true,"DBI handle opened successfully. Ready for get/put."};
		m_dbi_opened = true;
		return {true,""};
#else
		return {true,""};
#endif
	}
	/**
	 * @brief grabs a key from lmdb and returns as (non-reference) string
	 *
	 * @param key
	 *
	 * @return 
	 */
	std::string _db_handle::get(std::string key){
#ifdef __MENTOC_USE_LMDB__
		std::string value;
		this->get(key,value);
		return value;
#else
		return mods::globals::ram_db[key];
#endif
	}
	/**
	 * @brief grabs a key and stores in untyped buffer in_value
	 *
	 * @param key
	 * @param k_size
	 * @param in_value
	 *
	 * @return 
	 */
	int _db_handle::nget(void* key,std::size_t k_size,void* in_value){
#ifdef __MENTOC_USE_LMDB__
		lmdb_debug("nget entry");
		if(!m_good){
			lmdb_debug("nget - renewed txn");
			this->renew_txn();
		}
		if(m_good){
			lmdb_debug("nget - good. fetching..");
			MDB_val k;
			k.mv_size = k_size;
			k.mv_data = key;
			MDB_val v;
			memset(&v,0,sizeof(v));
			int ret = mdb_get(m_txn,m_dbi,&k,&v);
			switch(ret){
				case MDB_NOTFOUND:
					lmdb_debug("nget - key not found...");
					break;
				case EINVAL:
					lmdb_debug("nget - EINVAL");
					lmdb_debug("EINVAL == " << EINVAL);
					break;
				default:
					lmdb_debug("nget - fetched okay");
					//char buf[v.mv_size + 1];
					//memset(buf,0,v.mv_size +1);
					bcopy(v.mv_data,in_value,v.mv_size);
					//std::copy(buf,buf + v.mv_size,in_value.begin());
					return _db_handle::KEY_FETCHED_OKAY;
			}
			return ret;
		}else{
			return -2;
		}
#else
		in_value = mods::globals::ram_db[key];
		return _db_handle::KEY_FETCHED_OKAY;
#endif
	}
	/**
	 * @brief grabs key and stores in std::string in_value ref
	 *
	 * @param key
	 * @param k_size
	 * @param in_value
	 *
	 * @return 
	 */
	int _db_handle::nget(void* key,std::size_t k_size,std::string& in_value){
#ifdef __MENTOC_USE_LMDB__
		lmdb_debug("nget entry");
		if(!m_good){
			lmdb_debug("nget - renewed txn");
			this->renew_txn();
		}
		if(m_good){
			lmdb_debug("nget - good. fetching..");
			MDB_val k;
			k.mv_size = k_size;
			k.mv_data = key;
			MDB_val v;
			memset(&v,0,sizeof(v));
			int ret = mdb_get(m_txn,m_dbi,&k,&v);
			switch(ret){
				case MDB_NOTFOUND:
					lmdb_debug("nget - key not found...");
					break;
				case EINVAL:
					lmdb_debug("nget - EINVAL");
					lmdb_debug("EINVAL == " << EINVAL);
					break;
				default:
					lmdb_debug("nget - fetched okay");
					char buf[v.mv_size + 1];
					memset(buf,0,v.mv_size +1);
					bcopy(v.mv_data,buf,v.mv_size);
					std::copy(buf,buf + v.mv_size,in_value.begin());
					return _db_handle::KEY_FETCHED_OKAY;
			}
			return ret;
		}else{
			return -2;
		}
#else
		in_value = mods::globals::ram_db[key];
		return _db_handle::KEY_FETCHED_OKAY;
#endif
	}

	bool _db_handle::exists(std::string key){
#ifdef __MENTOC_USE_LMDB__
		lmdb_debug("exists entry");
		if(!m_good){
			lmdb_debug("get - renewed txn");
			this->renew_txn();
		}
		if(m_good){
			MDB_val k;
			k.mv_size = key.length()+1;
			k.mv_data = (void*)key.data();
			MDB_val v;
			memset(&v,0,sizeof(v));
			int ret = mdb_get(m_txn,m_dbi,&k,&v);
			switch(ret){
				case MDB_NOTFOUND:
					return false;
				case EINVAL:
					return false;
				default:
					return true;
			}
		}else{
			return false;
		}
#else
		mods::globals::ram_db[key];
		return std::find(mods::globals::ram_db.begin(),mods::globals::ram_db.end(),key) != mods::globals::ram_db.end();
#endif
	}

	/**
	 * @brief grabs string key and stores in std::string in_value ref
	 *
	 * @param key
	 * @param in_value
	 *
	 * @return 
	 */
	int _db_handle::get(std::string key,std::string & in_value){
#ifdef __MENTOC_USE_LMDB__
		lmdb_debug("get entry");
		if(!m_good){
			lmdb_debug("get - renewed txn");
			this->renew_txn();
		}
		if(m_good){
			lmdb_debug("get - good. fetching..");
			MDB_val k;
			lmdb_debug("key.length: " <<  key.length() << "key.size: " << key.size());
			k.mv_size = key.length()+1;
			k.mv_data = (void*)key.data();
			MDB_val v;
			memset(&v,0,sizeof(v));
			int ret = mdb_get(m_txn,m_dbi,&k,&v);
			switch(ret){
				case MDB_NOTFOUND:
					lmdb_debug("key not found...");
					in_value = "";
					break;
				case EINVAL:
					lmdb_debug("get - EINVAL");
					lmdb_debug("EINVAL == " << EINVAL);
					in_value="";
					break;
				default:
					lmdb_debug("get - fetched okay");
					char buf[v.mv_size + 1];
					memset(buf,0,v.mv_size +1);
					bcopy(v.mv_data,buf,v.mv_size);
					in_value.resize(v.mv_size);
					std::copy(buf,buf + v.mv_size,in_value.begin());
					return _db_handle::KEY_FETCHED_OKAY;
			}
			return ret;
		}else{
			return -2;
		}
#else
		in_value = mods::globals::ram_db[key];
		return _db_handle::KEY_FETCHED_OKAY;
#endif
	}
	/**
	 * @brief renews transaction
	 *
	 * @return 
	 */
	_db_handle::tuple_return_type_t _db_handle::renew_txn(){
#ifdef __MENTOC_USE_LMDB__
		clear_pluck_filter();
		if(m_transaction_good || m_transaction_open){
			mdb_txn_commit(m_txn);
			m_transaction_good = m_transaction_open = false;
		}
		auto tuple = new_txn();
		if(!std::get<0>(tuple)){
			m_good = false;
			return tuple;
		}
		if(!m_dbi_opened){
			return open_dbi();
		}
		return {true,""};
#else
		return {true,""};
#endif
	}
	/**
	 * @brief delete key by untyped key and size
	 *
	 * @param key
	 * @param key_size
	 */
	void _db_handle::ndel(void* key,std::size_t key_size){
#ifdef __MENTOC_USE_LMDB__
		MDB_val k;
		k.mv_data = key;
		k.mv_size = key_size;
		mdb_del(m_txn,m_dbi,&k,nullptr);
#else
		mods::globals::ram_db[key] = "";
#endif
	}
	/**
	 * @brief deletes a string key
	 *
	 * @param key
	 *
	 * @return 
	 */
	int _db_handle::del(std::string key){
#ifdef __MENTOC_USE_LMDB__
		MDB_val k;
		k.mv_data = (void*)key.c_str();
		k.mv_size = key.length()+1;
		return mdb_del(m_txn,m_dbi,&k,nullptr);
#else
		mods::globals::ram_db[key] = "";
		return 0;
#endif
	}
	/**
	 * @brief uses untyped key/value to grab value from lmdb and stores them in value
	 *
	 * @param key
	 * @param key_size
	 * @param value
	 * @param v_size
	 *
	 * @return 
	 */
	int _db_handle::nput(void* key,std::size_t key_size,void* value,std::size_t v_size){
#ifdef __MENTOC_USE_LMDB__
		lmdb_debug("nput function entry");
		if(m_good){
			lmdb_debug("m_good");
			MDB_val k;
			k.mv_size = key_size;
			k.mv_data = key;
			MDB_val v;
			v.mv_size = v_size;
			v.mv_data = value;
			int ret = mdb_put(m_txn,m_dbi,&k,&v,0);
			switch(ret){
				case MDB_MAP_FULL:
						lmdb_debug("nput full");
						break;
				case EINVAL:
						lmdb_debug("nput einval");
						break;
				case EACCES:
						lmdb_debug("nput eaccess");
						break;
				case MDB_TXN_FULL:
						lmdb_debug("nput txn full");
						break;
				default:
					if(std::string("Successful").compare(mdb_strerror(ret)) == 0){
						lmdb_debug("nput success");
						return 0;
					}
			}
			return ret;
		}else{
			lmdb_debug("not good");
			return -2;
		}
#endif
		return 0;
	}
	/**
	 * @brief places a value into key, optionally renewing the transaction beforehand
	 *
	 * @param key
	 * @param value
	 * @param renew
	 *
	 * @return 
	 */
	int _db_handle::put(std::string key,std::string value,bool renew){
		lmdb_debug("put function entry");
#ifdef __MENTOC_USE_LMDB__
		lmdb_debug("put function entry got the okay to proceed");
		if(renew){
			lmdb_debug("renewing...");
			if(m_transaction_good || m_transaction_open){
				mdb_txn_commit(m_txn);
				m_transaction_good = m_transaction_open = false;
			}
			renew_txn();
		}
		return this->put(key,value);
#else
		mods::globals::ram_db[key] = value;
		return 0;
#endif
	}
	/**
	 * @brief places a string key and string value 
	 *
	 * @param key
	 * @param value
	 *
	 * @return 
	 */
	int _db_handle::put(std::string key,std::string value){
		lmdb_debug("put function entry (no renew)");
#ifdef __MENTOC_USE_LMDB__
		/*
		if(!m_good){
			lmdb_debug("put renewing txn");
			this->renew_txn();
		}
		*/
		if(m_good){
			lmdb_debug("m_good");
			MDB_val k;
			k.mv_size = key.length()+1;
			k.mv_data = (void*)key.c_str();
			MDB_val v;
			v.mv_size = value.length()+1;
			v.mv_data = (void*)value.c_str();
			int ret = mdb_put(m_txn,m_dbi,&k,&v,0);
			switch(ret){
				case MDB_MAP_FULL:
						lmdb_debug("put full");
						break;
				case EINVAL:
						lmdb_debug("put einval");
						break;
				case EACCES:
						lmdb_debug("put eaccess");
						break;
				case MDB_TXN_FULL:
						lmdb_debug("put txn full");
						break;
				default:
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
					const auto e = mdb_strerror(ret);
					lmdb_debug(e);
					lmdb_debug(std::string("strerror: ") + e);
#endif
					if(std::string("Successful return: 0").compare(mdb_strerror(ret)) == 0){
						lmdb_debug("put success");
						return 0;
					}else{
						lmdb_debug("error:...");
						lmdb_debug(mdb_strerror(ret));
						lmdb_debug("WTF");
					}

			}
			return ret;
		}else{
			lmdb_debug("not good");
			return -2;
		}
		return 0;
#else
		mods::globals::ram_db[key] = value;
		return 0;
#endif
	}
	/**
	 * @brief commits current transactions
	 *
	 * @return 
	 */
	_db_handle::tuple_return_type_t _db_handle::commit(){
#ifdef __MENTOC_USE_LMDB__
		if(m_good){
			auto ret = mdb_txn_commit(m_txn);
			if(ret){
				m_transaction_good = false;
				return {false,std::string("[lmdb]::commit failed: ") + mdb_strerror(ret)};
			}else{
				m_transaction_open = m_transaction_good = false;
				return {true,"[lmdb]::commit good"};
			}
		}else{
			return {false,"[lmdb]::commit [m_good is not true]-> Will not commit due to this invalid state."};
		}
#else
		return {true,"stub commit"};
#endif
	}
	/**
	 * @brief destructor, calls commit and close
	 */
	_db_handle::~_db_handle(){
#ifdef __MENTOC_USE_LMDB__
		this->commit();
		this->close();
#else
		;;
#endif
	}
	/**
	 * @brief closes lmdb handle
	 */
	void _db_handle::close(){
#ifdef __MENTOC_USE_LMDB__
		lmdb_debug("close entry");
		if(m_closed){
			lmdb_debug("already closed. returning");
			return;
		}
		lmdb_debug("commiting final transaction");
		//this->commit();
		lmdb_debug("dumping statusi");
		this->dump_status();
		if(std::get<0>(m_status[3])){
			lmdb_debug("closing dbi");
			mdb_dbi_close(m_env,m_dbi);
		}
		if(std::get<0>(m_status[0])){
			lmdb_debug("closing env");
			mdb_env_close(m_env);
		}
		m_transaction_open = false;
		m_transaction_good = false;
		m_good = false;
		m_closed = true;
		lmdb_debug("final dump status");
		this->dump_status();
#endif
	}
	/**
	 * @brief debug function to print all status messages (if activated)
	 */
	void _db_handle::dump_status() const {
		lmdb_debug("dump-status");
		for(unsigned i =0; i < status_step_count; i++){
			lmdb_debug(std::get<1>(m_status[i]));
		}
		lmdb_debug("dump-status/done");
	}


};

namespace mods::globals {
	extern std::unique_ptr<mods::lmdb::db_handle> db;
};

std::string sanitize_key(std::string key){
	std::string k = "";
	for(auto ch : key){
		if(!isascii(ch)){
			continue;
		}
		if(!isalnum(ch) && ch != '_' && ch != '-'){
			continue;
		}
		k += ch;
	}
	return k;
}

/**
 * @brief create a key by list of strings
 *
 * @param parts
 *
 * @return 
 */
std::string db_key(const std::vector<std::string> & parts){
	std::string query = "";
	std::size_t i = 0,s=parts.size();
	for(auto & unsanitized_part : parts){
		std::string part = sanitize_key(unsanitized_part);
		query.append(part);
		if(++i == s){ break; }
		query.append("|");
	}
	lmdb_debug("db_key:'" << query << "'");
	return query;
}

/**
 * @brief alias of renew_txn call
 */
void db_renew_txn(){
	mods::globals::db->renew_txn();
}

/**
 * @brief grabs a key and returns as string
 *
 * @param key
 *
 * @return 
 */
std::string db_get(std::string key){
	return mods::globals::db->get(key);
}
/**
 * @brief stores a value in key
 *
 * @param key
 * @param value
 */
void db_put(std::string key,std::string value){
	mods::globals::db->put(key,value);
}
/**
 * @brief grabs all results from postgres for table
 *
 * @param table
 *
 * @return 
 */
mods::pq::result db_get_all(std::string table){
#ifndef __MENTOC_USE_LMDB_INSTEAD_OF_POSTGRES__
		try{
			auto up_txn = txn();
			sql_compositor comp(table,&up_txn);
			auto room_sql = comp.select("*")
				.from(table)
				.sql();
			return mods::pq::exec(up_txn,room_sql.data());
		}catch(std::exception& e){
			return mods::pq::result(); //result_container_t();
		}
#else
	mods::lmdb::transaction_t t(table,mods::lmdb::transact_type_t::EXEC);
	return t.get_all<mods::lmdb::db_handle*>(mods::globals::db.get());
#endif
}

/**
 * @brief grabs all values from table but plucks only list of columns in pluck
 *
 * @param table
 * @param pluck
 *
 * @return 
 */
auto db_get_all_pluck(std::string table,const std::vector<std::string>& pluck){
	mods::globals::db->set_pluck_filter(pluck);
	auto r = db_get_all(table);
	mods::globals::db->clear_pluck_filter();
	return r;
}

/**
 * @brief this is equivalent to: 'select * from table where col = value'
 *
 * @param table
 * @param col
 * @param value
 *
 * @return 
 */
pqxx::result db_get_by_meta(std::string table, std::string col,const std::string & value){
		try{
			auto up_txn = txn();
			sql_compositor comp(table,&up_txn);
			auto room_sql = comp.select("*")
				.from(table)
				.where(col,"=",value.c_str())
				.sql();
			return mods::pq::exec(up_txn,room_sql.data());
		}catch(std::exception& e){
			return mods::pq::result();
		}
}
/*
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::field & value){
	return db_get_by_meta(table,col,value.c_str());
}
*/
#ifdef __MENTOC_USE_PQXX_RESULT__
/**
 * @brief this is equivalent to: 'select * from table where col = value'
 *
 * @param table
 * @param col
 * @param value
 *
 * @return 
 */
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::result::reference & value){
#else
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::row::reference & value){
#endif
		try{
			auto up_txn = txn();
			sql_compositor comp(table,&up_txn);
			auto room_sql = comp.select("*")
				.from(table)
#ifdef __MENTOC_USE_PQXX_RESULT__
				.where(col,"=",value[0].c_str())
#else
				.where(col,"=",value.c_str())
#endif
				.sql();
			return mods::pq::exec(up_txn,room_sql.data());
		}catch(std::exception& e){
			return mods::pq::result();
		}
}

