#include "conf.hpp"

namespace mods::conf {
	static std::map<std::string,std::string> credentials;
	void set_credentials(std::map<std::string,std::string> values) {
		credentials = values;
	}
	std::string pq_connection(std::map<std::string,std::string> values) {
		return "user=" + values["user"] + " " +
		       "host=" + values["host"] +  " " +
		       "port=" + values["port"] + " " +
		       "dbname=" + values["dbname"] + " " +
		       "password=" + values["password"] + " " ;
	}
	std::string pq_connection() {
		return "user=" + credentials["user"] + " " +
		       "host=" + credentials["host"] +  " " +
		       "port=" + credentials["port"] + " " +
		       "dbname=" + credentials["dbname"] + " " +
		       "password=" + credentials["password"] + " " ;
	}
};
