#include "extended-types.hpp"
#include "../util.hpp"


namespace mods::mobs::extended_types {
	std::vector<std::string> strings(){
		std::vector<std::string> m;
		for(auto pair : ext_map){
			m.emplace_back(pair.second);
		}
		return m;
	}

	std::optional<extended_types_t> from_string(std::string str){
		for(auto pair : ext_map){
			if(mods::util::is_lower_match(pair.second.c_str(),str.c_str())){
				return pair.first;
			}
		}
		return std::nullopt;
	}
	std::string description(std::string str){
		auto opt = from_string(str);
		if(opt.has_value() == false){
			return "<unknown>";
		}
		return description_strings[opt.value()];
	}
	std::string description(extended_types_t type){
		return description_strings[type];
	}
	
	std::string to_string(extended_types_t t){
		return ext_map[t];
	}
};
