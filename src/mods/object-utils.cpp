#include "object-utils.hpp"
#include "weapon-types.hpp"

namespace mods::object_utils {
	void report_yaml_message(std::string_view msg){
		yaml_feed_exceptions.emplace_back(CAT(mods::util::time_string(),"->",msg.data()));
	}

};//End namespace
