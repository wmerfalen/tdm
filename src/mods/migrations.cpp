#include "migrations.hpp"

namespace mods::migrations {
#define m_debug(a) std::cerr << "migrations-debug: " << a << "\n";

	void REPORT_DB_ISSUE(std::string issue,std::string exception_message){
		log("SYSERR: MIGRATIONS ISSUE '%s' '%s'", issue.c_str(), exception_message.c_str());
	}
	std::tuple<bool,int> test(std::string identifier, std::string up_sql,std::string down_sql,std::string purpose){
		std::string sql = "CREATE TABLE karma (\n"
		"karma_id SERIAL,\n"
		"karma_player_id INTEGER NOT NULL,\n"
		"karma_alignment INTEGER NOT NULL DEFAULT 0,\n"
		"karma_cold_blooded_index INTEGER NOT NULL DEFAULT 0,\n"
		"created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,\n"
		"updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP\n"
		");\n"
		;
		try{
			auto t = txn();
			auto status = mods::pq::exec(t,sql);
			m_debug("affected_rows:" << status.affected_rows());
			m_debug("size:" << status.size());
			mods::pq::commit(t);
			return {true,0};
		}catch(std::exception& e){
			REPORT_DB_ISSUE(purpose.c_str(),e.what());
			return {false,-2};
		}
		return {false,-3};
	}
#undef m_debug
};
