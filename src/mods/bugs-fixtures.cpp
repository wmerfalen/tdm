#include "bugs-fixtures.hpp"

namespace mods::bugs {
	static std::shared_ptr<std::ofstream> out;
	static bool opened = false;
	void fixtures(std::string_view from, std::string_view msg){
		fixtures(CAT("from:",from.data(),msg.data()));
	}
	void fixtures(std::string_view msg){
		if(!opened){
			out = std::make_shared<std::ofstream>("fixtures.log",std::ios::app | std::ios::out);
			opened = true;
			(*out) << CAT("[[[--------------- FIXTURES ----------](date:",ctime(nullptr),")\n");
		}
		std::string buffer = CAT("[[[bug::fixture::->",msg.data(),"]]]\n");
		std::cerr << red_str(buffer);
		(*out) << buffer;
		out->flush();
	}
	void close(){
		if(out && out->is_open()){
			out->flush();
			out->close();
			opened = false;
			out = nullptr;
		}
	}
};
