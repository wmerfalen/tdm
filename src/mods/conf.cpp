#include "conf.hpp"

namespace mods::conf {
	std::string pq_connection(std::map<std::string,std::string> values){
		return "user=" + values["user"] + " " +
			"host=" + values["host"] +  " " + 
			"password=" + values["password"] + " " +
			"dbname=" + values["dbname"];
	}
};
