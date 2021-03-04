#include "encode.hpp"
#include "../../globals.hpp"

namespace mods::builder {
	using strmap_t = std::map<std::string,std::string>;
	std::string dump(const strmap_t& data) {
		std::string dump;
		for(const auto& pair : data) {
			dump += CAT("[",pair.first,"]:->",pair.second,"\r\n");
		}
		return dump;
	}
	std::string encode_map(const strmap_t& data) {
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
	std::string encode(std::string_view data) {
		return CAT("{vlen:",data.length(),",value:\"",data.data(),"\"}");
	}

#if 0
	strmap_t decode(std::string_view data) {
		bool open_array = 0, open_row = 0,capture_klen = 0,capture_vlen =0;
		uint32_t ctr = 0;
		std::string key_contents, value_contents;
		uint32_t key_length = 0, value_length = 0;
		for(auto ch : data) {
			if(!open_array && ch == '[') {
				open_array = 1;
				++ctr;
				continue;
			}
			if(open_array && !open_row && ch == '{') {
				open_row = 1;
				capture_klen = 1;
				++ctr;
				continue;
			}
			if(capture_vlen) {
				if(data.size() > ctr + 4 && data.substr(ctr,ctr+4).compare("vlen:") == 0) {
					ctr += 4;
					uint8_t vlen_ctr = 0;
					static constexpr uint8_t max_int_capture_length = 4;
					std::string vlen_capture;
					do {
						if(isspace(data.at(vlen_ctr))) {
							if(vlen_capture.length()) {
								break;
							}
							++vlen_ctr;
							continue;
						}
						if(isdigit(data.at(vlen_ctr))) {
							vlen_capture += data.at(vlen_ctr);
						}
						vlen_ctr++;
					} while(isdigit(data.at(vlen_ctr++)) && vlen_capture.length() < max_int_capture_length);
					if(vlen_capture.length() == 0) {
						std::cerr << red_str("invalid vlen captured. ignoring") << "\n";
						continue;
					}
					value_length = mods::util::stoi(vlen_capture).value_or(0);
				}
				if(value_length) {

				}
			}
			if(capture_klen) {
				if(data.size() > ctr + 4 && data.substr(ctr,ctr+4).compare("klen:") == 0) {
					ctr += 4;
					uint8_t klen_ctr = 0;
					static constexpr uint8_t max_int_capture_length = 4;
					std::string keylen_capture;
					do {
						if(isspace(data.at(klen_ctr))) {
							if(keylen_capture.length()) {
								break;
							}
							++klen_ctr;
							continue;
						}
						if(isdigit(data.at(klen_ctr))) {
							keylen_capture += data.at(klen_ctr);
						}
						klen_ctr++;
					} while(isdigit(data.at(klen_ctr++)) && keylen_capture.length() < max_int_capture_length);
					if(keylen_capture.length() == 0) {
						std::cerr << red_str("invalid keylen captured. ignoring") << "\n";
						continue;
					}
					key_length = mods::util::stoi(keylen_capture).value_or(0);
				}
			}
			++ctr;
		}
	}
#endif
};
