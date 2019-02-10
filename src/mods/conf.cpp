#include "conf.hpp"

namespace mods::conf {
	std::string pq_connection(std::map<std::string,std::string> values){
		return "user=" + values["user"] + " " +
			"host=" + values["host"] +  " " + 
			"port=" + values["port"] + " " + 
			"dbname=" + values["dbname"] + " " + 
			"password=" + values["password"] + " " ;
	}
};
