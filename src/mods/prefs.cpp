#include "prefs.hpp"
namespace mods {
		static std::string prefs::namify(const std::string & key,struct char_data* c){
			std::string format = PREFS_FORMAT_STRING;
			std::string named = mods::globals::replace_all(format,"{name}",c->player.name);
			named = mods::globals::replace_all(named,"{key}",key);
			return named;
		}
		static void prefs::set(const std::string& key,const std::string& value,struct char_data* ch){
			struct char_data * c = ch;
			auto named = namify(key,c);
			mods::globals::db->put(named,value);
		}

		static std::string prefs::get(const std::string& key,struct char_data* ch){
			struct char_data * c = ch;
			std::string value;
			auto k = namify(key,c);
			mods::globals::db->get(key,value);
			return value;
		}
		static std::string prefs::dynamic_namify(const std::string & key,const std::string& section,struct char_data* c){
			std::string format = DYNAMIC_FORMAT_STRING;
			std::string named = mods::globals::replace_all(format,"{name}",c->player.name);
			named = mods::globals::replace_all(named,"{prefs}",section);
			named = mods::globals::replace_all(named,"{key}",key);
			return named;
		}
		static void prefs::dynamic_set(const std::string& key,const std::string &value,const std::string& section,struct char_data* ch){
			auto k = dynamic_namify(key,section,ch);
			mods::globals::db->put(k,value);
		}
		static std::string prefs::dynamic_get(const std::string& key,const std::string& section,struct char_data* ch){
			auto k = dynamic_namify(key,section,ch);
			std::string value;
			mods::globals::db->get(k,value);
			return value;
		}
    };

ACMD(do_pref){
	MENTOC_PREAMBLE();
	constexpr unsigned int max_char = 11;
	std::array<char,max_char> item;
	std::array<char,max_char> value;
	one_argument(one_argument(argument,&item[0],max_char),&value[0],max_char);
	if(std::string(&item[0]).compare("width") == 0){
		PLAYER_SET("screen_width",&value[0]);
		auto v_int = mods::util::stoi(&value[0]);
		if(v_int.value_or(-1) == -1){
			*player << "{red}Invalid width{/red}\r\n";
			return;
		}
		PLAYER_SET("screen_width",&value[0]);
		*player << "{gld}Screen width set to: " << &value[0] << "\r\n";
	}

	*player << "{gld}::[ preferences ]::{/gld}\r\n";
	std::string line = "";
	for(unsigned i =0; i < 80;i++){
		line += "=";
	}
	*player << "{gld}" << line << "{/gld}\r\n";
	auto width = PLAYER_GET("screen_width");
	*player << "     width: " << width << "\r\n";
	*player << "{gld}" << line << "{/gld}\r\n";
	*player << "{gld} usage: pref <item> <value> {/gld}{blu}(Example: pref width 80){/blu}\r\n";
}
