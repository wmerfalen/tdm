#include "lmdb.hpp"
#include "db.hpp"
#include <sstream>
#include "util.hpp"
#ifndef __MENTOC_USE_LMDB_INSTEAD_OF_POSTGRES__
#include "sql.hpp"
#include "pq.hpp"
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
#endif
namespace mods::lmdb {
	using aligned_int_t = uint64_t;
	selector_type_t transaction_t::selector(const key_type_t & column,const uint64_t & row_id){
		return {m_table,column,0,row_id};
	}

	transaction_t::transaction_t(std::string ce_table,transact_type_t type) : m_type(type){
		m_str_table = ce_table;
	}
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
	template <typename T>
		result_container_t transaction_t::get_all(T consumer){
			std::string csv_list;
			csv_list = consumer->get(db_key({table_cstr(),"id_list"}));
			//std::cout << "info: get_all() retrieved: " << csv_list.size() << "\n";
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
	tuple_status_t transaction_t::set(
			mutable_map_t & values,
			std::string where_id_equals){
		auto opt_id = mods::util::stoi_optional<aligned_int_t>(where_id_equals);
		if(opt_id.has_value()){
			values["id"] = opt_id.value();
		}else{
			std::cerr << "warning: set() was supplied with an unparseable pk: '" << where_id_equals << "'\n";
		}
		return  mods::db::save_record(std::string(table_cstr()),
			&values,
			where_id_equals
		);
	}
	tuple_status_t transaction_t::values(
			mutable_map_t & values){
		return mods::db::new_record(std::string(table_cstr()),
			&values);
	}
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
	std::vector<std::string> _db_handle::fields_to_grab(const std::string& table){
		auto ptr_db = mods::globals::db.get();
		if(ptr_db->is_using_pluck_filter()){
			return get_pluck_filter();
		}else{
			return mods::schema::db[table];
		}
	}

	void _db_handle::set_pluck_filter(const std::vector<std::string> & cols){
		m_pluck = cols;
		m_use_pluck = true;
	}
	void _db_handle::clear_pluck_filter(){
		m_pluck.clear();
		m_use_pluck = false;
	}
	const std::vector<std::string> & _db_handle::get_pluck_filter() const {
		return m_pluck;
	}
		void _db_handle::use_pluck_filter(bool b){
			m_use_pluck = b;
		}
		bool _db_handle::is_using_pluck_filter() const { 
			return m_use_pluck;
		}
	_db_handle::_db_handle(std::string directory,std::string db_name,const uint64_t & flags,const uint16_t & mode,bool unused) 
		: m_use_pluck(false), m_good(false), m_closed(true), 
		m_dir(directory), m_name(db_name), m_flags(flags), m_mode(mode) {
#ifdef __MENTOC_USE_LMDB__
			m_transaction_open = false;
			m_transaction_good = false;
			m_dbi_opened = false;
			open();
#endif
		}
	bool _db_handle::open(){
		//std::cout << "debug: [lmdb]::open()\n";
#ifdef __MENTOC_USE_LMDB__
		m_closed = true;
		m_good = false;
		m_clear_status();
		int r = 0;
		if((r = mdb_env_create(&m_env)) != 0){
			//std::cerr << "error: mdb_env_create\n";
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

		m_good = true;
		return true;
#else
		return true;
#endif
	}
	_db_handle::tuple_return_type_t _db_handle::abort_txn(){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "info: db_handle::abort_txn called\n";
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
	_db_handle::tuple_return_type_t _db_handle::new_txn(){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "debug: db_handle::new_txn() entry\n";
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
			//std::cerr << "[nex_txn]: error: '" << reason << "'\n";
			//m_status[2] = {false,reason};
			//close();
			m_transaction_good = m_transaction_open = false;
			return {false,reason};
		}
		//m_status[2] = {true,"Transaction opened successfully."};
		//std::cout << "debug: db_handle::new_txn() okay\n";
		m_transaction_good = m_transaction_open = true;
		return {true,""};
#else
		return {true,""};
#endif
	}
	_db_handle::tuple_return_type_t _db_handle::open_dbi(){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "debug: db_handle::open_dbi() called\n";
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
			//std::cerr << "error: db_handle::open_dbi() failed: " << reason << "\n";
			return {false,reason};
		}
		m_status[4] = {true,"DBI handle opened successfully. Ready for get/put."};
		m_dbi_opened = true;
		//std::cout << "debug: db_handle::open_dbi() okay\n";
		return {true,""};
#else
		return {true,""};
#endif
	}
	std::string _db_handle::get(std::string key){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "debug: db_handle::get(str) called\n";
		std::string value;
		this->get(key,value);
		return value;
#else
		return mods::globals::ram_db[key];
#endif
	}
	int _db_handle::get(std::string key,std::string & in_value){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "debug: db_handle::get(key,value) called\n";
		static int recursion_count = 0;
		if(!m_good){
			//std::cout << "debug: db_handle::get(key,value) renewing transaction\n";
			this->renew_txn();
		}
		if(m_good){
			//std::cout << "debug: db_handle::get(key,value) m_good okay\n";
			MDB_val k;
			k.mv_size = key.length();
			k.mv_data = (void*)key.c_str();
			MDB_val v;
			memset(&v,0,sizeof(v));
			int ret = mdb_get(m_txn,m_dbi,&k,&v);
			switch(ret){
				case MDB_NOTFOUND:
					return _db_handle::KEY_NOT_FOUND;
				case EINVAL:
					//std::cerr << "[lmdb] invalid parameter to mdb_get\n";
					this->renew_txn();
					++recursion_count;
					if(recursion_count > 3){
						recursion_count = 0;
						//std::cout << "info: stopping after recursing 3 times via db_handle::get()\n";
						return EINVAL;
					}
					//std::cout << "debug: recursion level: " << recursion_count << "\n";
					return this->get(key,in_value);
					return EINVAL;
				default:
					char buf[v.mv_size + 1];
					memset(buf,0,v.mv_size +1);
					bcopy(v.mv_data,buf,v.mv_size);
					in_value = buf;
					//std::cout << "debug: db_handle::get(key,value) got '" << key << "' as '" << in_value << "'\n";
					return _db_handle::KEY_FETCHED_OKAY;
			}
		}else{
			//std::cerr << "error: [lmdb]get::m_good NOT okay\n";
			return -2;
		}
#else
		in_value = mods::globals::ram_db[key];
		return _db_handle::KEY_FETCHED_OKAY;
#endif
	}
	_db_handle::tuple_return_type_t _db_handle::renew_txn(){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "debug: db_handle::renew_txn() called\n";
		clear_pluck_filter();
		if(m_transaction_good || m_transaction_open){
			mdb_txn_commit(m_txn);
			m_transaction_good = m_transaction_open = false;
		}
		auto tuple = new_txn();
		if(!std::get<0>(tuple)){
			//std::cerr << "[lmdb]: warning new_txn failed via renew_txn\n";
			m_good = false;
			return tuple;
		}
		if(!m_dbi_opened){
			return open_dbi();
		}
		//std::cout << "debug: db_handle::renew_txn() okay\n";
		return {true,""};
#else
		return {true,""};
#endif
	}
	int _db_handle::del(std::string key){
#ifdef __MENTOC_USE_LMDB__
		MDB_val k;
		k.mv_data = (void*)key.c_str();
		k.mv_size = key.length();
		return mdb_del(m_txn,m_dbi,&k,nullptr);
#else
		mods::globals::ram_db[key] = "";
		return 0;
#endif
	}
	int _db_handle::put(std::string key,std::string value,bool renew){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "debug: db_handle::put(key,value,renew) called\n";
		if(renew){
			if(m_transaction_good || m_transaction_open){
				mdb_txn_commit(m_txn);
				m_transaction_good = m_transaction_open = false;
			}
			renew_txn();
		}
		return put(std::string(key),value);
#else
		mods::globals::ram_db[key] = value;
		return 0;
#endif
	}
	int _db_handle::put(std::string key,std::string value){
#ifdef __MENTOC_USE_LMDB__
		if(!m_good){
			//std::cout << "debug: db_handle::put -- renewing transaction\n";
			this->renew_txn();
		}
		if(m_good){
			//std::cout << "debug: db_handle::m_good is true\n";
			MDB_val k;
			k.mv_size = key.length();
			k.mv_data = (void*)key.c_str();
			MDB_val v;
			v.mv_size = value.length();
			v.mv_data = (void*)value.c_str();
			int ret = mdb_put(m_txn,m_dbi,&k,&v,0);
			switch(ret){
				case MDB_MAP_FULL:
					//std::cerr << "[lmdb] database is full, see mdb_env_set_mapsize()\n";
					return ret;
				case EINVAL:
					//std::cerr << "[lmdb] invalid parameter to mdb_get\n";
					return ret;
				case EACCES:
					//std::cerr << "[lmdb] invalid parameter to mdb_get\n";
					return ret;
				case MDB_TXN_FULL:
					//std::cerr << "[lmdb] transaction has too many dirty pages\n";
					return ret;
				default:
					/*
						 std::cerr << "[lmdb]::default error: " << ret << "\n";
						 std::cerr << "[lmdb]::default strerror: " << mdb_strerror(ret) << "\n";
						 */
					if(std::string("Successful").compare(mdb_strerror(ret)) == 0){
						//std::cout << "debug: put: '" << key << "' as '" << value << "'\n";
						return 0;
					}
			}
		}else{
			//std::cerr << "[lmdb]::put m_good NOT okay\n";
			return -2;
		}
		return 0;
#else
		mods::globals::ram_db[key] = value;
		return 0;
#endif
	}
	_db_handle::tuple_return_type_t _db_handle::commit(){
#ifdef __MENTOC_USE_LMDB__
		if(m_good){
			//std::cout << "debug: db_handle::commit -- m_good is good\n";
			auto ret = mdb_txn_commit(m_txn);
			if(ret){
				//std::cerr << "error: db_handle::commit -- failed:" << mdb_strerror(ret) << "\n";
				m_transaction_good = false;
				return {false,std::string("[lmdb]::commit failed: ") + mdb_strerror(ret)};
			}else{
				//std::cout << "debug: commit good\n";
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
	_db_handle::~_db_handle(){
#ifdef __MENTOC_USE_LMDB__
		//std::cout << "debug: [~_db_handle]: destructor yo\n";
		m_good = false;
		if(m_transaction_open){
			mdb_txn_abort(m_txn);
		}
		if(std::get<0>(m_status[0])){
			mdb_env_close(m_env);
		}
		this->close();
#else
		;;
#endif
	}
	void _db_handle::close(){
#ifdef __MENTOC_USE_LMDB__
		if(m_closed){
			//std::cerr << "[_db_handle]::close already closed\n";
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
#endif
	}
	void _db_handle::dump_status() const {
#ifdef __MENTOC_USE_LMDB__
		for(unsigned i=0; i < status_step_count;++i){
			//std::cerr << "[" << i << "]: " ;
			if(std::get<0>(m_status[i])){
				//std::cerr << " good -> " << std::get<1>(m_status[i]);
			}else{
				//std::cerr << " bad -> " << std::get<1>(m_status[i]);
			}
			//std::cerr << "\n";
		}
#endif
	}


};

namespace mods::globals {
	extern std::unique_ptr<mods::lmdb::db_handle> db;
};

std::string db_key(const std::vector<std::string> & parts){
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

void db_renew_txn(){
	mods::globals::db->renew_txn();
}

std::string db_get(std::string key){
	return mods::globals::db->get(key);
}
void db_put(std::string key,std::string value){
	mods::globals::db->put(key,value);
}
mods::pq::result db_get_all(std::string table){
#ifndef __MENTOC_USE_LMDB_INSTEAD_OF_POSTGRES__
		try{
			auto up_txn = txn();
			sql_compositor comp(table,&up_txn);
			auto room_sql = comp.select("*")
				.from(table)
				.sql();
			return mods::pq::exec(up_txn,room_sql.data());
			//auto result = mods::pq::exec(up_txn,room_sql.data());
			////result_container_t r;
			//std::map<std::string,std::string> row;
			//std::size_t i_ctr = 0;
			//return result;
			////for(auto i : result){
			//	//std::cerr << "pqxx::to_string(" << pqxx::to_string(i).as<int>() << "\n";
			//	row[std::to_string(i_ctr++)] = result[0]["id"].c_str();
			////}
			//r.emplace_back(row);
			////row.clear();
			//return r;
		}catch(std::exception& e){
			//std::cerr << "error selecting room from db: '" << e.what() << "'\n";
			return mods::pq::result(); //result_container_t();
		}
#else
	mods::lmdb::transaction_t t(table,mods::lmdb::transact_type_t::EXEC);
	return t.get_all<mods::lmdb::db_handle*>(mods::globals::db.get());
#endif
}

auto db_get_all_pluck(std::string table,const std::vector<std::string>& pluck){
	mods::globals::db->set_pluck_filter(pluck);
	auto r = db_get_all(table);
	mods::globals::db->clear_pluck_filter();
	return r;
}

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
			//std::cerr << "error selecting room from db: '" << e.what() << "'\n";
			return mods::pq::result();
		}
}
pqxx::result db_get_by_meta(std::string table, std::string col,const pqxx::tuple::reference & value){
		try{
			auto up_txn = txn();
			sql_compositor comp(table,&up_txn);
			auto room_sql = comp.select("*")
				.from(table)
				.where(col,"=",value.c_str())
				.sql();
			return mods::pq::exec(up_txn,room_sql.data());
			//auto result = mods::pq::exec(up_txn,room_sql.data());
			////result_container_t r;
			//std::map<std::string,std::string> row;
			//std::size_t i_ctr = 0;
			//return result;
			////for(auto i : result){
			//	//std::cerr << "pqxx::to_string(" << pqxx::to_string(i).as<int>() << "\n";
			//	row[std::to_string(i_ctr++)] = result[0]["id"].c_str();
			////}
			//r.emplace_back(row);
			////row.clear();
			//return r;
		}catch(std::exception& e){
			//std::cerr << "error selecting room from db: '" << e.what() << "'\n";
			return mods::pq::result();
		}
}

//result_container_t db_get_by_meta(std::string table, std::string col,std::string value){
//#ifndef __MENTOC_USE_LMDB_INSTEAD_OF_POSTGRES__
//		try{
//			auto up_txn = txn();
//			sql_compositor comp(table,&up_txn);
//			auto room_sql = comp.select("*")
//				.from(table)
//				.where(col,"=",value)
//				.sql();
//			return mods::pq::exec(up_txn,room_sql.data());
//			//auto result = mods::pq::exec(up_txn,room_sql.data());
//			////result_container_t r;
//			//std::map<std::string,std::string> row;
//			//std::size_t i_ctr = 0;
//			//return result;
//			////for(auto i : result){
//			//	//std::cerr << "pqxx::to_string(" << pqxx::to_string(i).as<int>() << "\n";
//			//	row[std::to_string(i_ctr++)] = result[0]["id"].c_str();
//			////}
//			//r.emplace_back(row);
//			////row.clear();
//			//return r;
//		}catch(std::exception& e){
//			std::cerr << "error selecting room from db: '" << e.what() << "'\n";
//			return result_container_t();
//		}
//#else
//	result_container_t r;
//	mutable_map_t where,out;
//	where[col] = value;
//	mods::db::load_record_by_meta(table,&where,out);
//	r.emplace_back(out);
//	return r;
//#endif 
//}
/*

mods::lmdb::result_container_t db_get_by_id(std::string table,std::string id){
	mods::globals::db.get()->renew_txn();
	return mods::lmdb::exec(table)->get_by_id<mods::lmdb::db_handle*>(mods::globals::db.get(),id);
}

mods::lmdb::result_container_t db_get_by_meta(std::string table,
		std::string column,std::string equals){
	return mods::lmdb::exec(table)->get_by_meta<mods::lmdb::db_handle*>(mods::globals::db.get(),column,equals);
}


bool db_exists_by_meta(std::string table,
		std::string column,std::string equals){
	mutable_map_t where_data,row;
	where_data[column] = equals;
	mods::db::load_record_by_meta(table,&where_data,row);
	if(row.size()){
		return true;
	}else{
		return false;
	}
}

bool db_exists_by_id(std::string table,std::string id){
	std::cerr << "[deprecated] db_exists_by_id\n";
	mutable_map_t row;
	mods::db::load_record(table,id,row);
	if(row.size()){
		return true;
	}
	return false;
}

tuple_status_t db_update(std::string table,
		mods::lmdb::mutable_map_t & values,
		std::string id){
	return mods::lmdb::update(table)->set(values,id);
}

tuple_status_t db_insert(std::string table,
		mods::lmdb::mutable_map_t & values){
	return mods::lmdb::insert(table)->values(values);
}
*/
