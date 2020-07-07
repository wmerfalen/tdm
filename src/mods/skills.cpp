#include "skills.hpp"

ACMD(do_skills){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() >= 2 && ICMP(vec_args[0],"help")){
		for(unsigned i = 1; i < vec_args.size();++i){
			for(auto & skillset : mods::skills::proficiencies::list) {
				for(auto & prof : skillset){
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
		for(auto & prof : skillset){
			player->send("[%d]\t\t[%s]\t[%d]\r\n",
					/** FIXME: mods::skills::get_player_level(prof.name),*/ 0,
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
	/** PLAYER BOOTSTRAP */
	void init_player_levels(player_ptr_t& player){
		init_player_levels(player->name().c_str());
	}
	void init_player_levels(std::string_view player_name){
		strmap_t m;
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : skillset){
				m[prof.name.str()] = "0";
			}
		}
		put_player_map(player_name.data(),"skills",m);
	}
	int get_enum_by_name(std::string_view name){
		for(auto & skillset : mods::skills::proficiencies::list) {
			for(auto & prof : skillset){
				if(ICMP(prof.name.c_str(),name.data())){
					return prof.e_name;
				}
			}
		}
		return -1;
	}
	void load_player_levels(player_ptr_t& player){
		strmap_t skills;
		if(0 == get_player_map(player->name(),"skills",skills)){
			init_player_levels(player->name().c_str());
			get_player_map(player->name(),"skills",skills);
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
	uint16_t get_player_level(player_ptr_t& player,std::string_view skill){
		return 0;
	}
};
