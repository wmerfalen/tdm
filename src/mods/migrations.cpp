#include "migrations.hpp"
#include "filesystem.hpp"

namespace mods::migrations {
#define m_debug(a) std::cerr << "migrations-debug: " << a << "\n";

	void REPORT_DB_ISSUE(std::string issue,std::string exception_message){
		log("SYSERR: MIGRATIONS ISSUE '%s' '%s'", issue.c_str(), exception_message.c_str());
	}
	std::tuple<int,std::string> strip_identifier(std::string identifier){
		std::string stripped;
		for(auto ch : identifier){
			if(isalpha(ch) || ch == '-'){
				stripped += ch;
			}
		}
		return {stripped.length(),stripped};
	}
	void report_migration_status(std::string message,std::string type){
		if(type.compare("error") == 0){
			log("SYSERR: Migrations error: '%s'",message.c_str());
			return;
		}
		if(type.compare("status") == 0){
			log("Migrations status: '%s'",message.c_str());
			return;
		}
		log("migrations success: '%s'", message.c_str());
	}
	std::tuple<bool,int,std::string> run_migration(std::string identifier, std::string purpose,std::string direction){
		bool proper_direction = false;
		if(direction.compare("up") == 0 || direction.compare("down") == 0){
			proper_direction = true;
		}
		if(!proper_direction){
			auto s = CAT("migration:'",purpose,"' tried to call db::migrate without proper direction. Must be either 'up' or 'down'.");
			return {false,-1,s};
		}
		bool valid_identifier = false;
		auto stripped_tuple = strip_identifier(identifier);
		valid_identifier = std::get<0>(stripped_tuple);
		if(!valid_identifier){
			auto s = CAT("migration:'",purpose,"' doesn't have a valid identifier:'",identifier,"'. most likely contains garbage characters");
			return {false,-1,s};
		}

		std::string path = CAT(__MENTOC_SQL_MIGRATIONS_DIR,"/",direction,"/",std::get<1>(stripped_tuple),".sql");
		m_debug("path:" << path.data());
		std::string sql;
		std::string error;
		int size = mods::filesystem::file_get_contents(path,sql,error);
		if(size < 0){
			auto s = CAT("error getting migration file",error);
			return {false,size,s};
		}
		m_debug("sql:'" << sql << "'");
		try{
			auto t = txn();
			auto status = mods::pq::exec(t,sql);
			m_debug("affected_rows:" << status.affected_rows());
			m_debug("size:" << status.size());
			mods::pq::commit(t);
			return {true,0,"success"};
		}catch(std::exception& e){
			auto s = CAT("migration:'",purpose,"' failed. exception:'",e.what(),"'");
			return {false,-2,s};
		}
		return {false,-3,"unreachable code reached."};
	}
#undef m_debug
};
