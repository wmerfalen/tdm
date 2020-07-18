#ifndef __MENTOC_MODS_DEBUG_PRE_GAME_HEADER__
#define __MENTOC_MODS_DEBUG_PRE_GAME_HEADER__
#include "../globals.hpp"
#include "flashbang.hpp"
#include "orm/room.hpp"
#include "db.hpp"
#define __MENTOC_RUN_PREGAME_SKILL_CODE__
#ifdef __MENTOC_RUN_PREGAME_SKILL_CODE__
#include "skills.hpp"
#include "rand.hpp"
#endif
namespace mods::debug::pre_game {
#define DD(a){ std::cerr << "[debug::pre_game][line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"; }
	namespace fb = ::mods::flashbang;
	bool run(){
#ifdef __MENTOC_RUN_PREGAME_RANDOM_MINIGUNNER_YELLING_CODE__
		mods::rand::init();
		for(int i=0; i < 50; i++){
			std::cerr << "random_string:" << random_key_string(MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS()) << "\n";
		}
#endif
#ifdef __MENTOC_RUN_PREGAME_EXTRACT_YAML_INFO_CODE__
		std::tuple<int,std::string> e = mods::util::extract_yaml_info_from_path("objects/armor/ballistic-helmet.yml");
		std::cerr << "[type]:'" << std::get<0>(e) << "', yaml_file:'" << std::get<1>(e) << "'\n";
#endif
#ifdef __MENTOC_RUN_PREGAME_SKILL_CODE__
		int count = mods::skills::update_schema_for_player("far");
		DD("Number of fields updated: " << count);
		return 1;
		mods::skills::init_player_levels("<fiz>");
		
		//mods::skills::load_player_levels("<fiz>");
		put_player_map("<fiz>","test1",{
				{"foo","bar"},
				{"baz","buz"}
				}
		);
		std::map<std::string,std::string> values;
		get_player_map("<fiz>","test1",values);
		for(auto pair : values){
			DD("map-fetched: '" << pair.first << "'->'" << pair.second << "'");
		}
		std::vector<std::string> vector_vals = {"empathy","apathy","ceased-to-be","","flower"},
			fetched;
		put_player_vector("<fiz>","test1",vector_vals);
		get_player_vector("<fiz>","test1",fetched);
		for(auto v : fetched){
			DD("vector-fetched: '" << v << "'");
		}
#endif

#ifndef __MENTOC_RUN_PREGAME__
		return 1;
#endif
#if 0
		mods::orm::foobar();

		auto o = create_object(ITEM_EXPLOSIVE, "flashbang-grenade.yml");
		if(o == nullptr){
			DD("couldnt load flashbang-grenade.yml");
			return false;
		}
		fb::affect_room(0,o->uuid);
		return false;
#endif
		return 1;
	}
#undef DD
};
#endif

