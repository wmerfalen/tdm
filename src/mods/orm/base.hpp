#ifndef __MENTOC_MODS_ORM_BASE_HEADER__
#define __MENTOC_MODS_ORM_BASE_HEADER__
#include "../db-report.hpp"
#include <iostream>
#include <vector>
#include "../player.hpp"
#include "../../structs.h"
#include "../pq.hpp"
#include <tuple>
#include "../sql.hpp"

//#define MENTOC_ORM_STR(key) key = result[ ##key ];
//#define MENTOC_ORM_UINT(key) key = static_cast<uint64_t>(result[ ##key ]);
namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct base {
		virtual int16_t feed(const pqxx::result::reference&);
		base() : loaded(0) {}
		~base() = default;
		bool loaded;
		template <typename TObject>
		static inline std::tuple<int16_t,std::string> insert_returing(TObject obj, std::string_view returning_field) {
			try {
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
				std::cerr << "insert_returning: '" << up_sql << "'\n";
				auto record = mods::pq::exec(insert_transaction,up_sql);
				mods::pq::commit(insert_transaction);
				if(record.size()) {
					return {record.size(),record[0][returning_field.data()].c_str()};
				}
			} catch(std::exception& e) {
				REPORT_DB_ISSUE(": error initializing sentinel class row: '",e.what());
				return {-1,e.what()};
			}
			return {-2,"unknown"};
		}

		template <typename TObject>
		static inline std::tuple<int16_t,std::string> delete_from(TObject& obj) {
			try {
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
			} catch(std::exception& e) {
				return {-1,e.what()};
			}
			return {-2,"unknown"};
		}
		template <typename TObject>
		static inline std::tuple<int16_t,std::string> delete_where(
		    std::string_view table_name,
		    std::string_view where,
		    std::string_view sql_operator,
		    std::string_view value) {
			try {
				auto del_txn = txn();
				sql_compositor comp(table_name.data(),&del_txn);
				auto up_sql = comp
				              .del()
				              .from(table_name.data())
				              .where(where.data(), sql_operator.data(),value.data())
				              .sql();
				mods::pq::exec(del_txn,up_sql);
				mods::pq::commit(del_txn);
				return {0,"okay"};
			} catch(std::exception& e) {
				return {-1,e.what()};
			}
			return {-2,"unknown"};
		}
		template <typename TObject>
		static inline std::tuple<int16_t,std::string> update(TObject& s, std::map<std::string,std::string>& values) {
			try {
				auto up_txn = txn();
				sql_compositor comp(s.table_name(),&up_txn);
				auto up_sql = comp
				              .update(s.table_name())
				              .set(values)
				              .where(s.primary_key_name(),"=",s.primary_key_value())
				              .sql();
				std::cerr << "update:'" << up_sql << "'\n";
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
				return {0,"okay"};
			} catch(std::exception& e) {
				REPORT_DB_ISSUE(": error updating room by pkid: '",e.what());
				return {-1,e.what()};
			}
			return {-2,"unknown"};
		}
		template <typename TContainerObject>
		static inline std::tuple<int16_t,std::string> load_where(TContainerObject& s,
		                                                         std::string_view where,
		                                                         std::string_view sql_operator,
		                                                         std::string_view value) {
			try {
				auto select_transaction = txn();
				sql_compositor comp(s[0].table_name(),&select_transaction);
				auto player_sql = comp.select("*")
				                  .from(s[0].table_name())
				                  .where(where.data(), sql_operator.data(), value.data())
				                  .sql();
				std::cerr << "load_where:'" << player_sql << "'\n";
				auto player_record = mods::pq::exec(select_transaction,player_sql);
				mods::pq::commit(select_transaction);
				for(auto row : player_record) {
					s.emplace_back(row);
				}
				return {player_record.size(),"okay"};
			} catch(std::exception& e) {
				REPORT_DB_ISSUE(": error loading character by pkid: '",e.what());
				return {-1,e.what()};
			}
			return {-2,"unknown"};
		}
		template <typename TObject>
		static inline std::tuple<int16_t,std::string> load_by_pkid(TObject& s,std::string pkid) {
			try {
				auto select_transaction = txn();
				sql_compositor comp(s.table_name(),&select_transaction);
				auto player_sql = comp.select("*")
				                  .from(s.table_name())
				                  .where(s.primary_key_name(),"=",pkid)
				                  .sql();
				std::cerr << "load_by_pkid'" << player_sql << "'\n";
				auto player_record = mods::pq::exec(select_transaction,player_sql);
				if(player_record.size()) {
					s.feed(player_record[0]);
					return {player_record.size(),"okay"};
				}
			} catch(std::exception& e) {
				REPORT_DB_ISSUE(": error loading character by pkid: '",e.what());
				return {-1,e.what()};
			}
			return {-2,"unknown"};
		}
	};

};

#endif
