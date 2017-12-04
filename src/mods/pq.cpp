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
};
