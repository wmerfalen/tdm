#ifndef __MENTOC_MODS_DEBUG_PRE_GAME_HEADER__
#define __MENTOC_MODS_DEBUG_PRE_GAME_HEADER__
#include "../globals.hpp"
#include "flashbang.hpp"
#include "orm/room.hpp"
#include "db.hpp"
#ifdef __MENTOC_RUN_PREGAME_SKILL_CODE__
#include "skills.hpp"
#include "rand.hpp"
#endif
#include "orm/medic.hpp"
#include "orm/sniper.hpp"
#include "classes/medic.hpp"
#include "builder/object-placement.hpp"
#include "mini-games.hpp"
#include "orm/mini-game.hpp"
#include "mini-games/wires.hpp"
#include "migrations.hpp"
#include "rifle-attachments.hpp"
#include "levels.hpp"
#include "deep-object-parser.hpp"
//#define __MENTOC_RUN_ARMOR_CALCULATOR__
#include "weapons/armor-calculator.hpp"

//#define __MENTOC_RUN_WRAPPER_PREGAME__

#ifdef __MENTOC_RUN_PBA_CODE__
#include "orm/player-base-ability.hpp"
#endif

#ifdef __MENTOC_RUN_MOB_ROAM_ORM_PREGAME__
#include "orm/mob-roam.hpp"
#endif

#ifdef __MENTOC_RUN_RIFLE_INDEX_ORM_PREGAME__
#include "orm/rifle-index.hpp"
#endif

extern bool login(std::string_view user_name,std::string_view password);
extern int16_t save_char_data(player_ptr_t& player,std::map<std::string,std::string> values);
namespace mods::debug::pre_game {
#ifdef __MENTOC_SHOW_PREGAME_DD_OUTPUT__
#define DD(a){ std::cerr << "[debug::pre_game][line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"; }
#else
#define DD(a)
#endif

	namespace fb = ::mods::flashbang;
	void mini_game_test() {
#ifdef __MENTOC_DO_MINIGAME_PREGAME_CODE__
		mods::mini_games::line_up game;
		std::cerr << game.get_body().data() << "\n";
		std::cerr << game.rotate_right(1).data() << "\n";
		std::cerr << game.rotate_left(1).data() << "\n";
#endif
	}
	void mini_game_orm_test() {
#ifdef __MENTOC_DO_MINIGAME_PREGAME_CODE__
		mods::orm::mini_game db;
		db.load_by_room_vnum(128);
#endif
	}
	void wire_mini_game_test() {
#ifdef __MENTOC_DO_MINIGAME_PREGAME_CODE__
		mods::mini_games::wires w;
		std::cerr << w.get_body() << "\n";
#endif
	}
	void deep_object_parser_test() {
#ifdef __MENTOC_RUN_DEEP_OBJECT_PARSER_TEST__
		mods::rifle_attachments_t a(CAT("rifle|pkid:",__MENTOC_PREDEFINED_RIFLE_PKID__,"{sight:acog.yml,under_barrel:gmtgrenadelauncher.yml,grip:vertical-grip.yml,barrel:extended-barrel.yml,muzzle:brake.yml,magazine:extended-mag.yml,stock:default-stock.yml,strap:shoulder-strap.yml}"));
#endif
	}
	void migrations_test() {
#ifdef __MENTOC_DO_MIGRATIONS_PREGAME__
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
#endif
	}
#ifdef __MENTOC_RUN_WRAPPER_PREGAME__
	void do_wrapper() {
		std::string text = "Greetings, soldier.\r\n"
		                   "\r\n"
		                   "Doctor Land has recently been notified that our {blu}group of TRITON Labs\r\n"
		                   "scientists have made astounding breakthroughs in ballistics; particularly in\r\n"
		                   "the realm of high velocity experimental magazines. The type of ammo\r\n"
		                   "generated is a combination of radioactive isotypes and aerodynamic shell\r\n"
		                   "design. The resulting ammunition causes a great deal of damage to the target\r\n"
		                   "while continuously doing radioactive damage even if the rounds did not \r\n"
		                   "penetrate the flesh.\r\n"
		                   "\r\n"
		                   "This magazine is just one element of the recipe for what Doctor Land\r\n"
		                   "affectionately calls the DXC DemonLance XIV.\r\n"
		                   ;
		std::cerr << mods::util::mail_format("{yel}High Velocity{/yel}\r \t Magazines Contract [1]",
		                                     "Gather the necessary items",
		                                     text,
		                                     80
		                                    );
		std::cerr << mods::util::mail_format("a",
		                                     "Gather the necessary items",
		                                     text,
		                                     80
		                                    );
		std::cerr << mods::util::mail_format("The runtime verification subsystem",
		                                     "[LWN subscriber-only content]",
		                                     "The realtime{->}[ not-learned ]\r\nproject has been the source of many of the innovations that have found their way into the core kernel in the last fifteen years or so. There is more to it than that, though; the wider realtime community is also doing interesting work in a number of areas that go beyond ensuring deterministic response.\r\n\r\n",
		                                     80
		                                    );
		sleep(60);
		exit(1);

	}
#endif

	player_ptr_t new_player() {
		return mods::globals::player_list.emplace_back(std::make_shared<mods::player>());
	}
	void do_armor_calculator() {
		player_ptr_t attacker = new_player();
		player_ptr_t victim = new_player();
		obj_ptr_t weapon = create_object(ITEM_RIFLE,"g36c.yml");
		attacker->level() = 10;
		attacker->armor() = 80;
		victim->level() = 5;
		int16_t dam = 450;
		float res =  mods::weapons::armor_calculator::roll_critical_damage_reduction(
		                 attacker,
		                 weapon,
		                 victim,
		                 dam
		             );
		std::cerr << "damage: " << dam << "\n"
		          << "dampener: " << res << "\n";
		sleep(60);

	}

	bool run() {
#ifdef __MENTOC_RUN_ARMOR_CALCULATOR__
		do_armor_calculator();
#endif
#ifdef __MENTOC_RUN_WRAPPER_PREGAME__
		do_wrapper();
#endif
#ifdef __MENTOC_DO_PARSER_PREGAME__
		deep_object_parser_test();
#endif
#ifdef __MENTOC_RUN_RIFLE_INDEX_ORM_PREGAME__
		{
			mods::orm::rifle_index r;
			std::vector<std::string> ar_list;
			std::cout << "loading all..\n";
			r.load_all();
			for(const auto& record : r.rows) {
				std::cout << green_str("AR: '") << record.rifle_filename << "', of type: '" << record.rifle_type << "'\n";
			}
		}
#endif
#ifdef __MENTOC_RUN_MOB_ROAM_ORM_PREGAME__
		{
			mods::orm::mob_roam mr;
			mr.get_compound_list();
			mr.delete_foobar_profile();
			mr.get_compound_list();
			std::vector<room_vnum> room_list;
			uint16_t max_loops = 10;
			for(auto& room : world) {
				if(--max_loops == 0) {
					break;
				}
				room_list.emplace_back(room.number);
			}
			mr.populate(410, room_list,"foobar_profile");
			mr.save();
		}
#endif


#ifdef __MENTOC_RUN_PBA_CODE__
		mods::orm::player_base_ability p;

#endif


#ifdef __MENTOC_RUN_LEVELS_PREGAME__
		mods::levels::csv_export_report();
		exit(0);
#endif

#ifdef __MENTOC_DO_MINIGAME_PREGAME_CODE__
		wire_mini_game_test();

		mini_game_test();

		mini_game_orm_test();

		std::cerr << "RUN\n";
#endif

#ifdef __MENTOC_DO_RIFLE_ATTACHMENTS_PREGAME__
		mods::rifle_attachments_t a("g36c.yml{sight:acog.yml,under_barrel:gmtgrenadelauncher.yml,grip:vertical-grip.yml,barrel:extended-barrel.yml,muzzle:brake.yml,magazine:extended-mag.yml,stock:default-stock.yml,strap:shoulder-strap.yml}");
#endif

#ifdef __MENTOC_RUN_MEDIC_INHERITANCE_SANITY_CHECKS__
		{
			//mods::classes::medic m;
			//m.report({"foobar message goes here. lot's of things to report about..","bar"});
		}
#endif
#ifdef __MENTOC_RUN_SNIPER_PREGAME_TESTS__
		//mods::orm::sniper sniper;
		//auto p = std::make_shared<mods::player>();
		//p->set_db_id(69);
		//std::cerr << "initialize_row:'" << std::to_string(sniper.initialize_row(p,mods::orm::sniper::primary_choice_t::PSG1)) << "'\n";
		//std::cerr << "pk name:'" << sniper.primary_key_name() << "'\n";
		//sniper.save();
		//sniper.load_by_player(69);
#endif

#ifdef __MENTOC_RUN_MEDIC_PREGAME_TESTS__
		mods::orm::medic m;
		auto mp = std::make_shared<mods::player>();
		mp->set_db_id(69);
		std::cerr << "initialize_row:'" << m.initialize_row(mp) << "'\n";
		std::cerr << "pk name:'" << m.primary_key_name() << "'\n";
		m.save();
		m.load_by_player(69);
#endif
		if(login("zim", "zoo")) {
			DD(green_str("Logged in properly with 'zim': 'zoo'"));
		} else {
			DD(red_str("Login test failed for 'zim': 'zoo'"));
		}

#ifdef __MENTOC_RUN_PREGAME_RANDOM_MINIGUNNER_YELLING_CODE__
		mods::rand::init();
		for(int i=0; i < 50; i++) {
			std::cerr << "random_string:" << random_key_string(MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS()) << "\n";
		}
#endif



#ifdef __MENTOC_RUN_PREGAME_EXTRACT_YAML_INFO_CODE__
		std::tuple<int,std::string> e = mods::util::extract_yaml_info_from_path("objects/armor/ballistic-helmet.yml");
		std::cerr << "[type]:'" << std::get<0>(e) << "', yaml_file:'" << std::get<1>(e) << "'\n";
#endif



#ifndef __MENTOC_RUN_PREGAME__
		return 1;
#endif
#if 0
		mods::orm::foobar();

		auto o = create_object(ITEM_EXPLOSIVE, "flashbang-grenade.yml");
		if(o == nullptr) {
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

