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

