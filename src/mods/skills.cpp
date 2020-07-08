#include "skills.hpp"

ACMD(do_skills){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() >= 2 && ICMP(vec_args[0],"help")){
		for(unsigned i = 1; i < vec_args.size();++i){
			for(auto & skillset : mods::skills::proficiencies::list) {
				for(auto & prof : std::get<1>(skillset)){
					if(ICMP(prof.name.c_str(),vec_args[i])){
						player->send("{grn}skill-name: '{yel}%s{/yel}'\r\n", prof.name.c_str());
						player->send("{grn}skill-description: '{yel}%s{/yel}'\r\n", prof.description.c_str());
						player->send("{grn}minium-proficiency: {yel}%d{/yel}\r\n", prof.minimum_profiency);
						continue;
					}
				}
			}
		}
		return;
	}

	for(auto & skillset : mods::skills::proficiencies::list) {
		player->send("{grn}-[ %s ]:----------------------------------------{/grn}\r\n",std::get<0>(skillset).c_str());
		for(auto & prof : std::get<1>(skillset)){
			player->send("{gld}[%d]\t\t[%s]\t[%d]\r\n{/gld}",
					mods::skills::get_player_level(player,prof.e_name),
					prof.name.c_str(),
					prof.minimum_profiency
			);
		}
	}
	player->sendln("To see a detailed description of a skill, type: {grn}skills help <skill>{/grn}");
	player->sendln("Example: {yel}skills help spray-chance{/yel}");
	player->sendln("NOTE: you can specify multiple skills");
	player->sendln("Example: {yel}skills help spray-chance basic-armor mold{/yel}");
	player->sendln("this documentation was written on 2020-07-06.");
}
namespace mods::skills {
	constexpr static const char* DB_PREFIX = "skills";
	static std::vector<std::string> skill_name_list;

	void refresh_skill_name_list() {
		skill_name_list.clear();
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : std::get<1>(skillset)){
				skill_name_list.emplace_back(prof.name);
			}
		}
	}

	/** PLAYER BOOTSTRAP */
	void init_player_levels(player_ptr_t& player){
		init_player_levels(player->name().c_str());
	}
	void init_player_levels(std::string_view player_name){
		strmap_t m;
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : std::get<1>(skillset)){
				m[prof.name.str()] = "0";
			}
		}
		put_player_map(player_name.data(),DB_PREFIX,m);
	}
	static std::map<std::string,int> mappings;
	static std::map<int,std::string> to_string_mappings;
	static std::map<int,proficiencies::proficiency_t> e_name_to_proficiency;

	/** called by game initialization sequence */
	void game_init(){
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : std::get<1>(skillset)){
				mappings[prof.name.c_str()] = prof.e_name;
				to_string_mappings[prof.e_name] = prof.name.c_str();
				e_name_to_proficiency[prof.e_name] = prof;
				skill_name_list.emplace_back(prof.name.str());
			}
		}
	}
	int get_enum_by_name(std::string_view name){
		return mappings[name.data()];
	}
	std::string to_string(int e_name){
		return to_string_mappings[e_name];
	}
	uint16_t get_minimum_proficiency(int e_name){
		return e_name_to_proficiency[e_name].minimum_profiency;
	}
	void load_player_levels(player_ptr_t& player){
		strmap_t skills;
		if(0 == get_player_map(player->name(),DB_PREFIX,skills)){
			init_player_levels(player->name().c_str());
			get_player_map(player->name(),DB_PREFIX,skills);
		}
		for(auto & pair : skills){
			int e_name = get_enum_by_name(pair.first);
			if(e_name == -1){
				continue;
			}
			auto level_opt = mods::util::stoi_optional<int>(pair.second.c_str());
			int level = 0;
			if(level_opt.has_value() == false){
				level = 0;
			}
			player->set_skill(e_name,level);
		}
	}
	uint16_t get_player_level(player_ptr_t& player,int skill){
		return player->skill(skill);
	}
	uint16_t get_player_level(player_ptr_t& player,std::string_view skill){
		return player->skill(get_enum_by_name(skill));
	}
	bool npc_can(player_ptr_t& player,int e_name){
		return true;/** TODO */
	}
	bool player_can(player_ptr_t& player,int e_name){
		if(player->is_npc()){
			return npc_can(player,e_name);
		}
		return player->skill(e_name) >= get_minimum_proficiency(e_name);
	}
	std::string pkey(std::string player_name,std::string key){
		return player_key(player_name,DB_PREFIX,"map") + key;
	}
	int update_schema_for_player(std::string player_name) {
		int ctr = 0;
		for(auto & skill_name : skill_name_list){
			if(!mods::db::lmdb_exists(pkey(player_name,skill_name))){
				mods::db::lmdb_put(pkey(player_name,skill_name),"0");
				++ctr;
			}
		}
		return ctr;
	}
};
