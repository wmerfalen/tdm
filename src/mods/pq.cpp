#include "pq.hpp"

namespace mods::pq {

	pqxx::result exec(transaction& txn,std::string_view query) {
		return txn.exec(query.data());
	}
	int as_int(result& r,int index, int index_y) {
		return r[index][index_y].as<int>();
	}
	void commit(transaction& txn) {
		txn.commit();
	}
	int count(std::string_view query) {
		auto txn = mods::pq::transaction(*mods::globals::pq_con);
		auto check_result = mods::pq::exec(txn,query.data());
		mods::pq::commit(txn);
		return mods::pq::as_int(check_result,0,0);
	}

};
std::tuple<bool,std::string> upsert(
    const std::string& table_name,
    const std::string&  primary_key,
    const std::map<std::string,std::string>& criteria,
    const std::map<std::string,std::string>& values
) {
	try {
		auto check_txn = txn();
		auto builder = sql_compositor(table_name,&check_txn)
		               .select(primary_key)
		               .from(table_name);
		bool first = true;
		for(const auto& pair : criteria) {
			if(first) {
				builder.where(pair.first,"=",pair.second);
				first = false;
				continue;
			}
			builder.op_and(pair.first,"=",pair.second);
		}
		auto check_sql = builder.sql();
		auto check_result = mods::pq::exec(check_txn,check_sql);
		mods::pq::commit(check_txn);

		if(check_result.size()) {
			/* update the record */
			auto t = txn();
			auto sql = sql_compositor(table_name,&t)
			           .update(table_name)
			           .set(values)
			           .where(primary_key,"=",check_result[0][primary_key.data()].c_str())
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
			auto record = mods::pq::exec(t,sql);
			mods::pq::commit(t);
		}
	} catch(const std::exception& e) {
		return {false,e.what()};
	}

	return {true,"success"};
}


