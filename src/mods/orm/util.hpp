#ifndef __MENTOC_MODS_ORM_UTIL_HEADER__
#define __MENTOC_MODS_ORM_UTIL_HEADER__
#include <forward_list>

#define NULLABLE_U32(ITEM) row[ITEM].is_null() ? 0 : row[ITEM].as<uint32_t>();
namespace mods::orm::util {
	struct statement_t {
		std::string where;
		std::string op;
		std::string value;
		bool op_and;
		bool op_or;
		statement_t() = delete;
		statement_t(std::string c_where,
		            std::string c_op,
		            std::string c_value,
		            bool c_op_and,
		            bool c_op_or) : where(c_where), op(c_op),
			value(c_value), op_and(c_op_and), op_or(c_op_or) {

		}
	};
	using compound_statement_t = std::forward_list<statement_t>;
	std::optional<unsigned> stoul(std::string_view str);
	template <typename T>
	T stoi(const std::string& i) {
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
	static inline std::tuple<int16_t,std::string,uint64_t> insert_returning(TObject* obj, std::string_view returning_field) {
		try {
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
			std::cerr << "insert_returning: '" << up_sql << "'\n";
			auto record = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(record.size()) {
				auto i_value = stoi<uint64_t>(record[0][returning_field.data()].c_str());
				return {FETCHED_OKAY,"okay",i_value};
			}
			return {NO_RESULTS,"no results",0};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what(),0};
		}
		return {UNKNOWN_ERROR,"unknown",0};
	}

	template <typename TObject,typename sql_compositor>
	static inline std::tuple<int16_t,std::string> delete_from(TObject* obj) {
		try {
			auto del_txn = txn();
			sql_compositor comp(obj->table_name(),&del_txn);
			auto up_sql = comp
			              .del()
			              .from(obj->table_name())
			              .where(obj->primary_key_name(),"=",obj->primary_key_value())
			              .sql();
			std::cerr << "delete_from: '" << up_sql << "'\n";
			auto record = mods::pq::exec(del_txn,up_sql);
			mods::pq::commit(del_txn);
			return {FETCHED_OKAY,"okay"};
		} catch(std::exception& e) {
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
		try {
			auto del_txn = txn();
			sql_compositor comp(table_name.data(),&del_txn);
			auto up_sql = comp
			              .del()
			              .from(table_name.data())
			              .where(where.data(), sql_operator.data(),value.data())
			              .sql();
			std::cerr << "delete_where: '" << up_sql << "'\n";
			mods::pq::exec(del_txn,up_sql);
			mods::pq::commit(del_txn);
			return {FETCHED_OKAY,"okay"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TObject,typename sql_compositor,typename TCompoundStatement>
	static inline std::tuple<int16_t,std::string> delete_where(
	    std::string_view table_name,
	    TCompoundStatement& statements) {
		try {
			auto del_txn = txn();
			sql_compositor comp(table_name.data(),&del_txn);
			auto builder = comp
			               .del()
			               .from(table_name.data());
			bool first = true;
			bool op_and = false, op_or = false;
			for(const auto& element : statements) {
				if(first) {
					first = false;
					builder.where(element.where.data(),element.op.data(),element.value.data());
					op_and = element.op_and;
					op_or = element.op_or;
					continue;
				}
				if(op_and) {
					builder.op_and(element.where.data(),element.op.data(),element.value.data());
				} else if(op_or) {
					builder.op_or(element.where.data(),element.op.data(),element.value.data());
				}
				op_and = element.op_and;
				op_or = element.op_or;
			}
			std::cerr << "delete_where(builder): '" << builder.sql() << "'\n";
			mods::pq::exec(del_txn,builder.sql());
			mods::pq::commit(del_txn);
			return {FETCHED_OKAY,"okay"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
	}

	template <typename TObject,typename sql_compositor>
	static inline std::tuple<int16_t,std::string> update(TObject* s) {
		try {
			auto up_txn = txn();
			sql_compositor comp(s->table_name(),&up_txn);
			auto up_sql = comp
			              .update(s->table_name())
			              .set(s->export_class())
			              .where(s->primary_key_name(),"=",s->primary_key_value())
			              .sql();
			std::cerr << "update: '" << up_sql << "'\n";
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
			return {FETCHED_OKAY,"okay"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TObject,typename sql_compositor>
	static inline std::tuple<int16_t,std::string> load_all(TObject* s) {
		try {
			auto select_transaction = txn();
			sql_compositor comp(s->table_name(),&select_transaction);
			auto player_sql = comp.select("*")
			                  .from(s->table_name())
			                  .sql();
			std::cerr << "load_all: '" << player_sql << "'\n";
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			mods::pq::commit(select_transaction);
			if(player_record.size()) {
				s->feed_multi(player_record);
				return {FETCHED_OKAY,"okay"};
			}
			return {NO_RESULTS,"no results"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TContainerObject,typename sql_compositor>
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
			std::cerr << "load_where: '" << player_sql << "'\n";
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			mods::pq::commit(select_transaction);
			for(auto row : player_record) {
				s.emplace_back(row);
			}
			return {FETCHED_OKAY,"okay"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TContainerObject,typename sql_compositor,typename TCompoundStatement>
	static inline std::tuple<int16_t,std::string> load_where(TContainerObject& s, std::string_view table_name,TCompoundStatement& compound_statement_list) {
		try {
			auto select_transaction = txn();
			sql_compositor comp(table_name.data(),&select_transaction);
			auto builder = comp.select("*")
			               .from(table_name.data());
			bool first = true;
			bool op_and = false, op_or = false;
			for(const auto& element : compound_statement_list) {
				std::cerr << "[load_where] where:'" << element.where.data() << "', " <<
				          "op:'" << element.op.data() << "', " <<
				          "value:'" << element.value.data() << "'\n";
				if(first) {
					first = false;
					builder.where(element.where.data(),element.op.data(),element.value.data());
					op_and = element.op_and;
					op_or = element.op_or;
					continue;
				}
				if(op_and) {
					builder.op_and(element.where.data(),element.op.data(),element.value.data());
				} else if(op_or) {
					builder.op_or(element.where.data(),element.op.data(),element.value.data());
				}
				op_and = element.op_and;
				op_or = element.op_or;
			}

			auto player_sql = builder.sql();
			std::cerr << "load_where(builder): '" << player_sql << "'\n";
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			mods::pq::commit(select_transaction);
			for(auto row : player_record) {
				s.emplace_back(row);
			}
			return {FETCHED_OKAY,"okay"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TObject,typename sql_compositor>
	static inline std::tuple<int16_t,std::string> load_by_column(TObject* s,std::string_view column,std::string_view value) {
		try {
			auto select_transaction = txn();
			sql_compositor comp(s->table_name(),&select_transaction);
			auto player_sql = comp.select("*")
			                  .from(s->table_name())
			                  .where(column.data(),"=",value.data())
			                  .sql();
			std::cerr << "load_by_column: '" << player_sql << "'\n";
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			if(player_record.size()) {
				s->feed(player_record[0]);
				return {FETCHED_OKAY,"okay"};
			}
			return {NO_RESULTS,"no results"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TObject,typename sql_compositor>
	static inline std::tuple<int16_t,std::string> load_multi_by_column(TObject* s,std::string_view column,std::string_view value) {
		try {
			auto select_transaction = txn();
			sql_compositor comp(s->table_name(),&select_transaction);
			auto player_sql = comp.select("*")
			                  .from(s->table_name())
			                  .where(column.data(),"=",value.data())
			                  .sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			if(player_record.size()) {
				s->feed_multi(player_record);
				return {FETCHED_OKAY,"okay"};
			}
			return {NO_RESULTS,"no results"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TObject,typename sql_compositor>
	static inline std::tuple<int16_t,std::string> load_multi_by_column_order_by(TObject* s,std::string_view column,std::string_view value,std::string order_by,std::string direction) {
		try {
			auto select_transaction = txn();
			sql_compositor comp(s->table_name(),&select_transaction);
			auto player_sql = comp.select("*")
			                  .from(s->table_name())
			                  .where(column.data(),"=",value.data())
			                  .order_by(order_by,direction)
			                  .sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			if(player_record.size()) {
				s->feed_multi(player_record);
				return {FETCHED_OKAY,"okay"};
			}
			return {NO_RESULTS,"no results"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}
	template <typename TObject,typename sql_compositor>
	static inline std::tuple<int16_t,std::string> load_by_pkid(TObject* s) {
		try {
			auto select_transaction = txn();
			sql_compositor comp(s->table_name(),&select_transaction);
			auto player_sql = comp.select("*")
			                  .from(s->table_name())
			                  .where(s->primary_key_name(),"=",s->primary_key_value())
			                  .sql();
			std::cerr << "load_by_pkid: '" << player_sql << "'\n";
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			if(player_record.size()) {
				s->feed(player_record[0]);
				return {FETCHED_OKAY,"okay"};
			}
			return {NO_RESULTS,"no results"};
		} catch(std::exception& e) {
			return {EXCEPTION_OCCURRED,e.what()};
		}
		return {UNKNOWN_ERROR,"unknown"};
	}

}; //end namespace
#endif
