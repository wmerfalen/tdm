#include "bugs-fixtures.hpp"

namespace mods::bugs {
	void fixtures(std::string_view msg){
		static std::shared_ptr<std::ofstream> out;
		static bool opened = false;
		if(!opened){
			out = std::make_shared<std::ofstream>("fixtures.log",std::ios::app | std::ios::out);
			opened = true;
		}
		std::string buffer = red_str("[[[---- FIXTURES ----]]]\n") +
								 red_str("bug::fixture::->") + red_str(msg.data()) + "\n" +
								 red_str("[[[---- FIXTURES ----]]]\n");
		std::cerr << buffer ;
		(*out) << buffer;
		out->flush();
	}
};
