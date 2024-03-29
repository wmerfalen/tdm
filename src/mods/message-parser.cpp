#include "message-parser.hpp"
#include <iostream>
#include "util.hpp"

namespace mods::message_parser {
#ifdef __MENTOC_SHOW_ZEROMQ_DEBUG__
#define m_debug(A) mentoc_prefix_debug(CAT(__FILE__,":",__LINE__,":",__FUNCTION__)) << A << "\n";
#define debug_dump_list() 		for(auto i = 0; i < list.size(); i++) {\
				m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");\
				m_debug("[" << i << "]->'" << list[i] << "'\n");\
				m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");\
			}
#else
#define m_debug(A)
#define debug_dump_list()
#endif

	std::vector<std::string> extract_entries(std::string_view msg) {
		m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");
		m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");
		m_debug("msg: '" << msg << "'\n");
		m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");
		m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");

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

		std::size_t i = 0;
		for(; i < msg.length(); i++) {
			auto ch = msg[i];
			if(!size_parsed) {
				if(ch == '{') {
					capture_size = 1;
					size_length = 0;
					continue;
				}
				if(capture_size) {
					if(isdigit(ch) && ++size_length >= MAX_SIZE_LENGTH) {
						std::cerr << "Funky looking size specifier " << __FILE__ << ":" << __LINE__ << ". Aborting...\n";
						return {"funky-looking-spec for ","size spec weirdly high",msg.data()};
					}
					if(isdigit(ch)) {
						size += ch;
						continue;
					}
					if(ch == '}') {
						auto opt_size = mods::util::stoi_optional<uint16_t>(size);
						if(!opt_size.has_value()) {
							std::cerr << "Funky looking size specifier " << __FILE__ << ":" << __LINE__ << ". Aborting...\n";
							return {"funky-looking-spec for ","closing brace",msg.data()};
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
					return {"syntax error","expecting digit or close stirng size at offset:",std::to_string(i),"instead",msg.data()};
				}
				continue;
			}
			if(capture_payload) {
				payload_buffer += ch;
				if(++payload_counter >= actual_size) {
					entries.emplace_back(payload_buffer);
					payload_buffer.clear();

					size_parsed = 0;
					capture_size = 0;
					payload_counter = 0;
					capture_payload = 0;
					actual_size = 0;
					size_length = 0;
					size.clear();
				}
			}
		}
		return entries;
	}
#undef m_debug
};//end namespace mods::message_parser
