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
#include "orm/class-medic.hpp"
#include "orm/class-sniper.hpp"
#include "classes/medic.hpp"
#include "builder/object-placement.hpp"
#include "mini-games.hpp"
#include "orm/mini-game.hpp"
#include "mini-games/wires.hpp"
#include "migrations.hpp"
#include "rifle-attachments.hpp"
#include "levels.hpp"

extern bool login(std::string_view user_name,std::string_view password);
extern int16_t save_char_data(player_ptr_t& player,std::map<std::string,std::string> values);
namespace mods::debug::pre_game {
#define DD(a){ std::cerr << "[debug::pre_game][line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"; }
	namespace fb = ::mods::flashbang;
	void mini_game_test(){
		mods::mini_games::line_up game;
		std::cerr << game.get_body().data() << "\n";
		std::cerr << game.rotate_right(1).data() << "\n";
		std::cerr << game.rotate_left(1).data() << "\n";
	}
	void mini_game_orm_test(){
		mods::orm::mini_game db;
		db.load_by_room_vnum(128);
	}
	void wire_mini_game_test(){
		mods::mini_games::wires w;
		std::cerr << w.get_body() << "\n";
	}
	void migrations_test(){
		std::string identifier = "create-karma-table";
		std::string purpose = "to create the karma table";

		auto i = mods::migrations::run_migration(identifier,purpose,"up");
		std::cout << "[" << std::get<0>(i) << ",";
		std::cout << std::get<1>(i) << "]\n";

		identifier = "purposely broken";
		purpose = "purposely run a broken migration";
		i = mods::migrations::run_migration(identifier,purpose,"down");
		std::cout << "[" << std::get<0>(i) << ",";
		std::cout << std::get<1>(i) << "]\n";

		identifier = "create-karma-table";
		purpose = "down the create karma table";
		i = mods::migrations::run_migration(identifier,purpose,"down");
		std::cout << "[" << std::get<0>(i) << ",";
		std::cout << std::get<1>(i) << "]\n";

		identifier = "create-karma-table";
		purpose = "recreate karma table";
		i = mods::migrations::run_migration(identifier,purpose,"up");
		std::cout << "[" << std::get<0>(i) << ",";
		std::cout << std::get<1>(i) << "]\n";
	}
	bool run(){
#define __MENTOC_RUN_LEVELS_PREGAME__
#ifdef __MENTOC_RUN_LEVELS_PREGAME__
		for(unsigned player_level = 1; player_level <= LVL_IMMORT;++player_level){
			for(auto & cl_type : {
				player_class_t::GHOST,
				player_class_t::MARKSMAN,
				player_class_t::BANDIT,
				player_class_t::BUTCHER,
				player_class_t::STRIKER,
				player_class_t::OBSTRUCTOR,
				player_class_t::MALADY,
				player_class_t::PYREXIA,
				player_class_t::DEALER,
				player_class_t::FORGE,
				player_class_t::SYNDROME,
				player_class_t::MACHINIST
				}){
				std::cerr << "[player_class(" << player_level << ")]\n\t"
					<< mods::util::player_class_to_string(cl_type) << "\n";
				auto triads = mods::levels::get_triads_by_class(cl_type);
				auto stats = mods::levels::calculate_based_on_triads(triads,player_level);
				mods::levels::report_advances(stats);
				std::cerr << "\n\n";
			}
		}
		sleep(1200);
		exit(0);
#endif
		wire_mini_game_test();

		mini_game_test();

		mini_game_orm_test();

		std::cerr << "RUN\n";
		mods::rifle_attachments_t a("g36c.yml{sight:acog.yml,under_barrel:gmtgrenadelauncher.yml,grip:vertical-grip.yml,barrel:extended-barrel.yml,muzzle:brake.yml,magazine:extended-mag.yml,stock:default-stock.yml,strap:shoulder-strap.yml}");

#define __MENTOC_RUN_MEDIC_INHERITANCE_SANITY_CHECKS__
#ifdef __MENTOC_RUN_MEDIC_INHERITANCE_SANITY_CHECKS__
		{
			//mods::classes::medic m;
			//m.report({"foobar message goes here. lot's of things to report about..","bar"});
		}
#endif
#define __MENTOC_RUN_SNIPER_PREGAME_TESTS__
#ifdef __MENTOC_RUN_SNIPER_PREGAME_TESTS__
		//mods::orm::sniper sniper;
		//auto p = std::make_shared<mods::player>();
		//p->set_db_id(69);
		//std::cerr << "initialize_row:'" << std::to_string(sniper.initialize_row(p,mods::orm::sniper::primary_choice_t::PSG1)) << "'\n";
		//std::cerr << "pk name:'" << sniper.primary_key_name() << "'\n";
		//sniper.save();
		//sniper.load_by_player(69);
#endif

#define __MENTOC_RUN_MEDIC_PREGAME_TESTS__
#ifdef __MENTOC_RUN_MEDIC_PREGAME_TESTS__
		mods::orm::medic m;
		auto mp = std::make_shared<mods::player>();
		mp->set_db_id(69);
		std::cerr << "initialize_row:'" << m.initialize_row(mp,mods::orm::medic::primary_choice_t::AUGPARA) << "'\n";
		std::cerr << "pk name:'" << m.primary_key_name() << "'\n";
		m.save();
		m.load_by_player(69);
#endif
		if(login("zim", "zoo")){
			DD(green_str("Logged in properly with 'zim': 'zoo'"));
		}else{
			DD(red_str("Login test failed for 'zim': 'zoo'"));
		}

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

