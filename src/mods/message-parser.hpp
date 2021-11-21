#ifndef __MENTOC_MODS_MESSAGE_parser_HEADER__
#define __MENTOC_MODS_MESSAGE_parser_HEADER__

#include <vector>
#include <string>

namespace mods::message_parser {
	/**
		EBNF GRAMMAR
		digit = "1","2","3","4","5","6","7","8","9","0";
		field_separator = ":";
		string_size_start = "{";
		string_size_close = "}";


		string_size = string_size_start , { digit }, string_size_close;

		entry = string_size , field_separator;

		-----------------------------------------------------------------
		| after parsing is successful and the character pointer is pointed
		| at one char past the last symbol in 'entry' (field_separator),
		| the parser should take the parsed string_size and iterate over
		| the string until it has captured string_size bytes of data.
		|-----------------------------------------------------------------
		*/
	std::vector<std::string> extract_entries(std::string_view msg);
}; //end namespace mods::message_parser

#endif
