#include "prefs.hpp"
#include "player.hpp"
#include "players/db-load.hpp"
#include "../interpreter.hpp"

namespace mods::prefs {
	static constexpr uint8_t MAX_SCREEN_WIDTH = 200;
	static std::map<std::string,int> pref_map{
		{"automap",PRF_OVERHEAD_MAP},
		{"autoexit",PRF_AUTOEXIT},
		{"brief",PRF_BRIEF},
		{"compact",PRF_COMPACT},
		{"deaf",PRF_DEAF},
		{"dispauto",PRF_DISPAUTO},
		{"dispmove",PRF_DISPMOVE},
		{"disphp",PRF_DISPHP},
		{"noauct",PRF_NOAUCT},
		{"nochat",PRF_NOCHAT},
		{"nogossip",PRF_NOGOSS},
		{"nogratz",PRF_NOGRATZ},
		{"nohassle",PRF_NOHASSLE},
		{"nonewbie",PRF_NONEWBIE},
		{"noreminder",PRF_NOREMINERS},
		{"notell",PRF_NOTELL},
		{"nowiz",PRF_NOWIZ},
	};
	std::string namify(const std::string& key,char_data* c) {
		std::string format = PREFS_FORMAT_STRING;
		std::string named = mods::globals::replace_all(format,"{name}",c->player.name.c_str());
		named = mods::globals::replace_all(named,"{key}",key);
		return named;
	}
	void set(const std::string& key,const std::string& value,char_data* ch) {
		char_data * c = ch;
		auto named = namify(key,c);
		mods::db::lmdb_put(named,value);
	}

	std::string get(const std::string& key,char_data* ch) {
		char_data * c = ch;
		std::string value;
		auto k = namify(key,c);
		return mods::db::lmdb_get(k);
	}
	std::string dynamic_namify(const std::string& key,const std::string& section,char_data* c) {
		std::string format = DYNAMIC_FORMAT_STRING;
		std::string named = mods::globals::replace_all(format,"{name}",c->player.name.c_str());
		named = mods::globals::replace_all(named,"{prefs}",section);
		named = mods::globals::replace_all(named,"{key}",key);
		return named;
	}
	void dynamic_set(const std::string& key,const std::string& value,const std::string& section,char_data* ch) {
		auto k = dynamic_namify(key,section,ch);
		mods::db::lmdb_put(k,value);
	}
	std::string dynamic_get(const std::string& key,const std::string& section,char_data* ch) {
		auto k = dynamic_namify(key,section,ch);
		std::string value;
		return mods::db::lmdb_get(k);
	}
	void update_pref(player_ptr_t& player,int constant, long value) {
		for(const auto& pair : mods::prefs::pref_map) {
			if(pair.second == constant) {
				if(player->get_prefs() & pair.second) {
					PLAYER_SET(pair.first,"1");
				} else {
					PLAYER_SET(pair.first,"0");
				}
			}
		}
	}
	bool get_player_pref(player_ptr_t& player, std::string_view pref_name) {
		auto str_value = mods::prefs::dynamic_get(pref_name.data(),"player",player->cd());
		if(str_value.length() == 0) {
			return false;
		}
		auto i_opt = mods::util::stoi(str_value.data());
		return i_opt.value_or(false);
	}
	ACMD(do_pref) {
		auto vec_args = PARSE_ARGS();
		str_vec_t no_comms{"brief","compact","deaf","notell","disphp","dispmove","nohassle","nowiz","noauct","dispauto","nochat","nogossip","nogratz","nonewbie","noreminder"};
		DO_HELP("pref");

		if(vec_args.size() == 0) {
			*player << "{gld}::[ preferences ]::{/gld}\r\n";
			std::string line;

			line = "=================================================================================";
			*player << "{gld}" << line << "{/gld}\r\n";
			auto width = PLAYER_GET("screen_width");
			int w = mods::util::stoi(width).value_or(80);
			if(w < 0) {
				w = 80;
			}
			for(const auto& pair : mods::prefs::pref_map) {
				std::string v = PLAYER_GET(pair.first);
				player->sendln(CAT("      ",pair.first.c_str(),": ",v.c_str()));
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

			if(v_int.value_or(-1) <= 0) {
				*player << "{red}Invalid width{/red}\r\n";
				return;
			}
			if(v_int.value() > MAX_SCREEN_WIDTH) {
				player->sendln(CAT("{yel}Please note, we capped your screen width to ",MAX_SCREEN_WIDTH,"{/yel}"));
				PLAYER_SET("screen_width",std::to_string(MAX_SCREEN_WIDTH));
				player->sendln(CAT("{gld}Screen width set to: ",MAX_SCREEN_WIDTH));
			} else {
				PLAYER_SET("screen_width",&value[0]);
				player->sendln(CAT("{gld}Screen width set to: ",value[0]));
			}
			return;
		}
		for(const auto& pair : mods::prefs::pref_map) {
			if(pair.first.compare(vec_args[0].c_str()) == 0) {
				if(value[0] == '1') {
					player->get_prefs() |= pair.second;
				} else {
					player->get_prefs() &= ~(pair.second);
				}
				PLAYER_SET(item,&value[0]);
				mods::players::db_load::save_prefs(player);
				*player << "{gld}" << item << " set to: " << &value[0] << "\r\n";
				return;
			}
		}

	}
	void init() {
		mods::interpreter::add_command("pref", POS_RESTING, do_pref, 0, 0);

	}
};


namespace mods::prefs::events {
	void prefs_loaded_from_db(player_ptr_t& player) {
		for(const auto& pair : mods::prefs::pref_map) {
			if(player->get_prefs() & pair.second) {
				PLAYER_SET(pair.first,"1");
			} else {
				PLAYER_SET(pair.first,"0");
			}
		}
	}
};
