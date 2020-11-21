#ifndef __MENTOC_MODS_PQ_HEADER__
#define __MENTOC_MODS_PQ_HEADER__

#include "lmdb.hpp"

//inline void transaction(void){}
#include <pqxx/pqxx>
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"
#include <optional>


namespace mods::pq {
	using transaction = pqxx::work;
	using result = pqxx::result;
	pqxx::result exec(transaction& txn,std::string_view query);
	int as_int(result& r,int index, int index_y);
	void commit(transaction&);
	int count(std::string_view query);
};

namespace mods::globals {
		extern std::unique_ptr<pqxx::connection> pq_con;
};
inline mods::pq::transaction txn() { 
	return mods::pq::transaction(*mods::globals::pq_con);
}

static inline void REPORT_DB_ISSUE(std::string issue,std::string exception_message){
	std::cerr << red_str("[DATABASE ISSUE]->") << issue << ", " << red_str("EXCEPTION: '") << red_str(exception_message) << "'\n";
}

using str_map_t = std::map<std::string,std::string>;
#define ORM_SUCCESS(a) std::get<0>(a) == 0
#define ORM_NO_RESULTS(a) std::get<0>(a) == NO_RESULTS
#define ORM_FAILURE(a) std::get<0>(a) < 0
	template <typename sql_compositor>
	static inline bool db_exists(std::string_view table, std::string_view primary_key,std::string_view field,std::string_view operator_type, std::string_view value){
			try{
				auto up_txn = txn();
				sql_compositor comp(table.data(),&up_txn);
				auto up_sql = comp.select(primary_key)
					.from(table.data())
					.where(field.data(),operator_type.data(),value.data())
					.sql();
				auto rows = mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return rows.size() > 0;
			}catch(std::exception& e){
				return 0;
			}
	}
	template <typename sql_compositor>
	static inline int db_update(std::string_view table, str_map_t values,std::string_view field,std::string_view operator_type,std::string_view value){
			try{
				auto up_txn = txn();
				sql_compositor comp(table.data(),&up_txn);
				auto up_sql = comp
					.update(table.data())
					.set(values)
					.where(field.data(),operator_type.data(),value.data())
					.sql();
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return 0;
			}catch(std::exception& e){
				return -1;
			}
	}
	template <typename sql_compositor>
	static inline int db_create(std::string_view table, str_map_t values){
			try {
			auto txn2 = txn();
			sql_compositor comp(table.data(),&txn2);
			auto sql = comp
				.insert()
				.into(table.data())
				.values(values)
				.sql();
			auto row = mods::pq::exec(txn2,sql);
			mods::pq::commit(txn2);
			return 0;
			}catch(std::exception& e){
				return -1;
			}
	}

	template <typename sql_compositor,typename PKValueType,typename value_callback,typename post_modify_callback>
	std::pair<bool,std::string> insert_or_update_existing(
	    const std::string& table_name,
	    const std::string& primary_key,
	    const PKValueType& pk_value,
			std::map<std::string,std::string>& values
	) {
		try {
			auto check_txn = txn();
			auto check_sql = sql_compositor(table_name,&check_txn)
			                 .select("COUNT(*)")
			                 .from(table_name)
			                 .where(primary_key,"=",pk_value)
			                 .sql();
			auto check_result = mods::pq::exec(check_txn,check_sql);
			mods::pq::commit(check_txn);

			if(check_result.size()) {
				/* update the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				           .update(table_name)
				           .set(values)
				           .where(primary_key,"=",pk_value)
				           .sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			} else {
				/* insert the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				           .insert()
				           .into(table_name)
				           .values(values)
				           .sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			}

			return {true,"success"};
		} catch(const std::exception& e) {
			return {false,e.what()};
		}

		return {true,"success"};
	}

	template <typename sql_compositor,typename PKValueType>
	std::tuple<bool,std::string,std::string> insert_or_update_existing_returning(
	    const std::string& table_name,
	    const std::string& primary_key,
	    const PKValueType& pk_value,
			std::map<std::string,std::string>& values,
			const std::string& returning_column
	) {
		try {
			auto check_txn = txn();
			auto check_sql = sql_compositor(table_name,&check_txn)
			                 .select("COUNT(*)")
			                 .from(table_name)
			                 .where(primary_key,"=",pk_value)
			                 .sql();
			auto check_result = mods::pq::exec(check_txn,check_sql);
			mods::pq::commit(check_txn);

			if(check_result.size()) {
				/* update the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				           .update(table_name)
				           .set(values)
				           .where(primary_key,"=",pk_value)
				           .sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			} else {
				/* insert the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				           .insert()
				           .into(table_name)
				           .values(values)
									 .returning(returning_column)
				           .sql();
				auto record = mods::pq::exec(t,sql);
				mods::pq::commit(t);
				if(record.size()){
					return {true,"success",record[0][returning_column.data()].c_str()};
				}
			return {true,"success",""};
			}
		} catch(const std::exception& e) {
			return {false,e.what(),""};
		}

		return {true,"success",""};
	}
	template <typename T>
		T stoi(const std::string& i){
			T m;
			std::stringstream ss;
			ss.str(i);
			ss >> m;
			return m;
		}
	static constexpr int8_t NO_RESULTS = 1;
	static constexpr int8_t FETCHED_OKAY = 0;
	static constexpr int8_t EXCEPTION_OCCURRED = -1;
	static constexpr int8_t UNKNOWN_ERROR = -2;
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string,uint64_t> insert_returning(TObject& obj, std::string_view returning_field){
				try{
					auto insert_transaction = txn();
					auto mapped = obj->export_class();
					mapped.erase(obj->primary_key_name().data());
					sql_compositor comp(obj->table_name(),&insert_transaction);
					auto up_sql = comp
						.insert()
						.into(obj->table_name())
						.values(mapped)
						.returning(returning_field)
						.sql();
					auto record = mods::pq::exec(insert_transaction,up_sql);
					mods::pq::commit(insert_transaction);
					if(record.size()){
						auto i_value = stoi<uint64_t>(record[0][returning_field.data()].c_str());
						return {FETCHED_OKAY,"okay",i_value};
					}
					return {NO_RESULTS,"no results",0};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what(),0};
				}
				return {UNKNOWN_ERROR,"unknown",0};
			}

		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> delete_from(TObject& obj){
				try{
					auto del_txn = txn();
					sql_compositor comp(obj->table_name(),&del_txn);
					auto up_sql = comp
						.del()
						.from(obj->table_name())
						.where(obj->primary_key_name(),"=",obj->primary_key_value())
						.sql();
					auto record = mods::pq::exec(del_txn,up_sql);
					mods::pq::commit(del_txn);
					return {FETCHED_OKAY,"okay"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> delete_where(
					std::string_view table_name,
					std::string_view where, 
					std::string_view sql_operator,
					std::string_view value) {
				try{
					auto del_txn = txn();
					sql_compositor comp(table_name.data(),&del_txn);
					auto up_sql = comp
						.del()
						.from(table_name.data())
						.where(where.data(), sql_operator.data(),value.data())
						.sql();
					mods::pq::exec(del_txn,up_sql);
					mods::pq::commit(del_txn);
					return {FETCHED_OKAY,"okay"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> update(TObject& s){
				try{
					auto up_txn = txn();
					sql_compositor comp(s->table_name(),&up_txn);
					auto up_sql = comp
						.update(s->table_name())
						.set(s->export_class())
						.where(s->primary_key_name(),"=",s->primary_key_value())
						.sql();
					mods::pq::exec(up_txn,up_sql);
					mods::pq::commit(up_txn);
					return {FETCHED_OKAY,"okay"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_all(TObject& s){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s->table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s->table_name())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					mods::pq::commit(select_transaction);
					if(player_record.size()){
						s->feed_multi(player_record);
						return {FETCHED_OKAY,"okay"};
					}
					return {NO_RESULTS,"no results"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}
		template <typename TContainerObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_where(TContainerObject& s,
					std::string_view where,
					std::string_view sql_operator,
					std::string_view value){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s[0].table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s[0].table_name())
						.where(where.data(), sql_operator.data(), value.data())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					mods::pq::commit(select_transaction);
					for(auto row : player_record){
						s.emplace_back(row);
					}
					return {FETCHED_OKAY,"okay"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_by_column(TObject& s,std::string_view column,std::string_view value){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s->table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s->table_name())
						.where(column.data(),"=",value.data())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					if(player_record.size()){
						s->feed(player_record[0]);
						return {FETCHED_OKAY,"okay"};
					}
					return {NO_RESULTS,"no results"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_multi_by_column(TObject& s,std::string_view column,std::string_view value){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s->table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s->table_name())
						.where(column.data(),"=",value.data())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					if(player_record.size()){
						s->feed_multi(player_record);
						return {FETCHED_OKAY,"okay"};
					}
					return {NO_RESULTS,"no results"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}
		template <typename TObject,typename sql_compositor>
			static inline std::tuple<int16_t,std::string> load_by_pkid(TObject& s){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s->table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s->table_name())
						.where(s->primary_key_name(),"=",s->primary_key_value())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					if(player_record.size()){
						s->feed(player_record[0]);
						return {FETCHED_OKAY,"okay"};
					}
					return {NO_RESULTS,"no results"};
				}catch(std::exception& e){
					return {EXCEPTION_OCCURRED,e.what()};
				}
				return {UNKNOWN_ERROR,"unknown"};
			}

		template <typename sql_compositor,typename TObject>
			static inline std::tuple<int16_t,std::string> insert_returing(TObject& obj, std::string_view returning_field){
				try{
					auto insert_transaction = txn();
					sql_compositor comp(obj.table_name(),&insert_transaction);
					auto mapped = obj.export_insert();
					mapped.erase(obj.primary_key_name());
					auto up_sql = comp
						.insert()
						.into(obj.table_name())
						.values(obj.export_class())
						.returning(returning_field)
						.sql();
					auto record = mods::pq::exec(insert_transaction,up_sql);
					mods::pq::commit(insert_transaction);
					if(record.size()){
						return {record.size(),record[0][returning_field.data()].c_str()};
					}
				}catch(std::exception& e){
					REPORT_DB_ISSUE(": error initializing sentinel class row: '",e.what());
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}

		template <typename sql_compositor,typename TObject>
			static inline std::tuple<int16_t,std::string> delete_from(TObject& obj){
				try{
					auto del_txn = txn();
					sql_compositor comp(obj.table_name(),&del_txn);
					auto up_sql = comp
						.del()
						.from(obj.table_name())
						.where(obj.primary_key_name(),"=",obj.primary_key_value())
						.sql();
					auto record = mods::pq::exec(del_txn,up_sql);
					mods::pq::commit(del_txn);
					return {0,"okay"};
				}catch(std::exception& e){
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename sql_compositor,typename TObject>
			static inline std::tuple<int16_t,std::string> update(TObject& s, std::map<std::string,std::string>& values){
				try{
					auto up_txn = txn();
					sql_compositor comp(s.table_name(),&up_txn);
					auto up_sql = comp
						.update(s.table_name())
						.set(values)
						.where(s.primary_key_name(),"=",s.primary_key_value())
						.sql();
					mods::pq::exec(up_txn,up_sql);
					mods::pq::commit(up_txn);
					return {0,"okay"};
				}catch(std::exception& e){
					REPORT_DB_ISSUE(": error updating room by pkid: '",e.what());
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename sql_compositor,typename TContainerObject>
			static inline std::tuple<int16_t,std::string> load_where(TContainerObject& s,
					std::string_view where,
					std::string_view sql_operator,
					std::string_view value){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s[0].table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s[0].table_name())
						.where(where.data(), sql_operator.data(), value.data())
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					mods::pq::commit(select_transaction);
					for(auto row : player_record){
						s.emplace_back(row);
					}
					return {player_record.size(),"okay"};
				}catch(std::exception& e){
					REPORT_DB_ISSUE(": error loading character by pkid: '",e.what());
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}
		template <typename sql_compositor,typename TObject>
			static inline std::tuple<int16_t,std::string> load_by_pkid(TObject& s,std::string pkid){
				try{
					auto select_transaction = txn();
					sql_compositor comp(s.table_name(),&select_transaction);
					auto player_sql = comp.select("*")
						.from(s.table_name())
						.where(s.primary_key_name(),"=",pkid)
						.sql();
					auto player_record = mods::pq::exec(select_transaction,player_sql);
					if(player_record.size()){
						s.feed(player_record[0]);
						return {player_record.size(),"okay"};
					}
				}catch(std::exception& e){
					REPORT_DB_ISSUE(": error loading character by pkid: '",e.what());
					return {-1,e.what()};
				}
				return {-2,"unknown"};
			}

	using strmap_t = std::map<std::string,std::string>;
	template <typename sql_compositor,typename TClassType,typename TPrimaryType>
	struct orm_base {
		template <typename TClass>
		static inline std::tuple<int16_t,std::string,uint64_t> create(TClass* c){
			auto status = insert_returning<TClass,sql_compositor>(c,c->primary_key_name());
			if(ORM_FAILURE(status)){
				std::cerr << "[orm_base::insert] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read(TClass* c){
			auto status = load_by_pkid<TClass,sql_compositor>(c);
			if(ORM_NO_RESULTS(status)){
				std::cout << "[load_by_pkid] no results.\n";
			}
			if(ORM_FAILURE(status)){
				std::cerr << "[load_by_pkid] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read_normalized(TClass* c,std::string column,std::string value){
			auto status = load_multi_by_column<TClass,sql_compositor>(c,column,value);
			if(ORM_NO_RESULTS(status)){
				std::cout << "[read_normalized] no results.\n";
			}
			if(ORM_FAILURE(status)){
				std::cerr << "[read_normalized] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read_all(TClass* c){
			auto status = load_all<TClass,sql_compositor>(c);
			if(ORM_NO_RESULTS(status)){
				std::cout << "[mods::orm::orm_base::read] no results.\n";
			}
			if(ORM_FAILURE(status)){
				std::cerr << "[mods::orm::orm_base::read] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		std::tuple<int16_t,std::string> read(TClass* c,std::string_view column,std::string_view value){
			auto status = load_by_column<TClass,sql_compositor>(c,column,value);
			if(ORM_NO_RESULTS(status)){
				std::cout << "[mods::orm::orm_base::read] no results.\n";
			}
			if(ORM_FAILURE(status)){
				std::cerr << "[mods::orm::orm_base::read] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		template <typename TClass>
		static inline std::tuple<int16_t,std::string> update(TClass* c){
			auto status = update<TClass,sql_compositor>(c);
			if(ORM_FAILURE(status)){
				std::cerr << "[orm_base::update] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}
		std::tuple<int16_t,std::string> remove(){
			auto status = delete_from<orm_base,sql_compositor>(this);
			if(ORM_FAILURE(status)){
				std::cerr << "[orm_base::remove] failed: '" << std::get<1>(status) << "'\n";
			}
			return status;
		}

		virtual std::string table_name() { return "player"; }
		virtual std::string column_prefix(){ return "player_"; }
		virtual std::string id_column(){ return "id"; }
		using primary_choice_t = TPrimaryType;

		virtual std::string primary_key_name() { return id_column(); }
		virtual std::string primary_key_value(){ return std::to_string(this->id); }
		virtual TPrimaryType primary_type(){ return (TPrimaryType)0; }
		virtual int16_t feed(const pqxx::result::reference &){
			return 0;
		}
		virtual strmap_t export_class(){
			strmap_t f;
			f["id"] = std::to_string(id);
			return f;
		}

		bool loaded;
		uint64_t id;
		long created_at;
		long updated_at;

	};
#undef ORM_SUCCESS
#undef ORM_NO_RESULTS
#undef ORM_FAILURE

#endif
