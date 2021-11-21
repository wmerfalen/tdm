#include "message-parser.hpp"
#include <iostream>
#include "util.hpp"

namespace mods::message_parser {
	std::vector<std::string> extract_entries(std::string_view msg) {
		std::vector<std::string> entries;
		bool size_parsed = 0;
		std::string size;
		static constexpr std::size_t MAX_SIZE_LENGTH = 6;
		uint16_t actual_size = 0;
		bool capture_size = 0;
		bool capture_payload = 0;
		std::size_t payload_counter = 0;
		std::string payload_buffer;
		std::size_t size_length = 0;

		for(const auto& ch : msg) {
			if(!size_parsed) {
				if(ch == '{') {
					capture_size = 1;
					size_length = 0;
					continue;
				}
				if(capture_size) {
					if(isdigit(ch) && ++size_length >= MAX_SIZE_LENGTH) {
						std::cerr << "Funky looking size specifier " << __FILE__ << ":" << __LINE__ << ". Aborting...\n";
						return {};
					}
					if(isdigit(ch)) {
						size += ch;
						continue;
					}
					if(ch == '}') {
						auto opt_size = mods::util::stoi_optional<uint16_t>(size);
						if(!opt_size.has_value()) {
							std::cerr << "Funky looking size specifier " << __FILE__ << ":" << __LINE__ << ". Aborting...\n";
							return {};
						}
						actual_size = opt_size.value();
						capture_size = false;
						size_parsed = true;
						capture_payload = true;
						payload_counter = 0;
						payload_buffer.clear();
						continue;
					}
					std::cerr << "SYNTAX-ERROR: expecting digit or close string size. instead, got: '" << ch << "'. Aborting...\n";
					return {};
				}
				continue;
			}
			if(capture_payload) {
				payload_buffer += ch;
				if(++payload_counter >= actual_size) {
					entries.emplace_back(payload_buffer);
					payload_buffer.clear();

					size_parsed = 0;
					capture_size = 1;
					payload_counter = 0;
					capture_payload = 0;
					actual_size = 0;
					size.clear();
					continue;
				}
				continue;
			}
		}
		return entries;
	}
};//end namespace mods::message_parser
