#include "encode.hpp"

namespace mods::builder {
	using strmap_t = std::map<std::string,std::string>;
	std::string dump(const strmap_t& data) {
		std::string dump;
		for(const auto& pair : data) {
			dump += CAT("[",pair.first,"]:->",pair.second,"\r\n");
		}
		return dump;
	}
	std::string encode(const strmap_t& data) {
		std::string dump;
		std::size_t ctr = 0;
		dump = "[";
		for(const auto& pair : data) {
			dump += CAT("{klen:",pair.first.length(),",key:\"",pair.first,"\",vlen:",pair.second.length(),",value:\"",pair.second,"\"}");
			if(++ctr == data.size()) {
				break;
			}
			dump += ",";
		}
		dump += "]";
		return dump;
	}
};
