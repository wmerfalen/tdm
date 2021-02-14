#include "prefs.hpp"
#include "player.hpp"
namespace mods {
	std::string prefs::namify(const std::string& key,char_data* c) {
		std::string format = PREFS_FORMAT_STRING;
		std::string named = mods::globals::replace_all(format,"{name}",c->player.name.c_str());
		named = mods::globals::replace_all(named,"{key}",key);
		return named;
	}
	void prefs::set(const std::string& key,const std::string& value,char_data* ch) {
		char_data * c = ch;
		auto named = namify(key,c);
		mods::db::lmdb_put(named,value);
	}

	std::string prefs::get(const std::string& key,char_data* ch) {
		char_data * c = ch;
		std::string value;
		auto k = namify(key,c);
		return mods::db::lmdb_get(k);
	}
	std::string prefs::dynamic_namify(const std::string& key,const std::string& section,char_data* c) {
		std::string format = DYNAMIC_FORMAT_STRING;
		std::string named = mods::globals::replace_all(format,"{name}",c->player.name.c_str());
		named = mods::globals::replace_all(named,"{prefs}",section);
		named = mods::globals::replace_all(named,"{key}",key);
		return named;
	}
	void prefs::dynamic_set(const std::string& key,const std::string& value,const std::string& section,char_data* ch) {
		auto k = dynamic_namify(key,section,ch);
		mods::db::lmdb_put(k,value);
	}
	std::string prefs::dynamic_get(const std::string& key,const std::string& section,char_data* ch) {
		auto k = dynamic_namify(key,section,ch);
		std::string value;
		return mods::db::lmdb_get(k);
	}
};

ACMD(do_pref) {
	auto vec_args = PARSE_ARGS();
	str_vec_t no_comms{"nochat","nogossip","nogratz","nonewbie"};
	DO_HELP("pref");

	if(vec_args.size() == 0) {
		*player << "{gld}::[ preferences ]::{/gld}\r\n";
		std::string line;

		line = "=================================================================================";
		*player << "{gld}" << line << "{/gld}\r\n";
		auto width = PLAYER_GET("screen_width");
		int w = mods::util::stoi(width).value_or(80);
		for(auto pref : no_comms) {
			std::string v = PLAYER_GET(pref);
			player->send("      %s: %s\r\n",pref.c_str(),v.c_str());
		}
		*player << "     width: " << w << "\r\n";
		*player << "{gld}" << line << "{/gld}\r\n";
		*player << "{gld} usage: pref <item> <value> {/gld}{blu}(Example: pref width 80){/blu}\r\n";
		return;
	}
	if(vec_args.size() < 2) {
		player->sendln("{gld} usage: pref <item> <value> {/gld}{blu}(Example: pref width 80){/blu}");
		return;
	}

	std::string item = vec_args[0],value = vec_args[1];
	if(item.compare("width") == 0) {
		PLAYER_SET("screen_width",&value[0]);
		auto v_int = mods::util::stoi(&value[0]);

		if(v_int.value_or(-1) == -1) {
			*player << "{red}Invalid width{/red}\r\n";
			return;
		}

		PLAYER_SET("screen_width",&value[0]);
		*player << "{gld}Screen width set to: " << &value[0] << "\r\n";
		return;
	}
	if(invec(item,no_comms)) {
		if(value.compare("1") != 0 && value.compare("0") != 0) {
			player->sendln("{red}Use either 1 or 0.{/red}");
			return;
		}
		PLAYER_SET(item,&value[0]);
		*player << "{gld}" << item << " set to: " << &value[0] << "\r\n";
		return;
	}

}
