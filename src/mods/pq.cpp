#include "pq.hpp"

namespace mods::pq {

	pqxx::result exec(transaction& txn,std::string_view query){
		return txn.exec(query.data());
	}
	int as_int(result& r,int index, int index_y){
		return r[index][index_y].as<int>();
	}
	void commit(transaction& txn){
		txn.commit();
	}
	int count(std::string_view query){
		auto txn = mods::pq::transaction(*mods::globals::pq_con);
		auto check_result = mods::pq::exec(txn,query.data());
		mods::pq::commit(txn);
		return mods::pq::as_int(check_result,0,0);
	}

};
