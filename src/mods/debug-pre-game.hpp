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
#define __MENTOC_RUN_PBA_CODE__
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
#define DD(a){ std::cerr << "[debug::pre_game][line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"; }
	namespace fb = ::mods::flashbang;
	void mini_game_test() {
		mods::mini_games::line_up game;
		std::cerr << game.get_body().data() << "\n";
		std::cerr << game.rotate_right(1).data() << "\n";
		std::cerr << game.rotate_left(1).data() << "\n";
	}
	void mini_game_orm_test() {
		mods::orm::mini_game db;
		db.load_by_room_vnum(128);
	}
	void wire_mini_game_test() {
		mods::mini_games::wires w;
		std::cerr << w.get_body() << "\n";
	}
	void deep_object_parser_test() {
#ifdef __MENTOC_RUN_DEEP_OBJECT_PARSER_TEST__
		mods::rifle_attachments_t a(CAT("rifle|pkid:",__MENTOC_PREDEFINED_RIFLE_PKID__,"{sight:acog.yml,under_barrel:gmtgrenadelauncher.yml,grip:vertical-grip.yml,barrel:extended-barrel.yml,muzzle:brake.yml,magazine:extended-mag.yml,stock:default-stock.yml,strap:shoulder-strap.yml}"));
#endif
	}
	void migrations_test() {
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
		                                     "The realtime project has been the source of many of the innovations that have found their way into the core kernel in the last fifteen years or so. There is more to it than that, though; the wider realtime community is also doing interesting work in a number of areas that go beyond ensuring deterministic response.\r\n\r\n"
		                                     "One example is Daniel Bristot de Oliveira's runtime verification patch set, which can monitor the kernel to ensure that it is behaving the way one thinks it should. Realtime development in the kernel community is a pragmatic effort to add determinism to a production system, but there is also an active academic community focused on realtime work.\r\nAcademic developers often struggle to collaborate effectively with projects like the kernel, where concerns about performance, regressions, and maintainability have been the downfall of many a bright idea. As a result, there is a lot of good academic work that takes a long time to make it into a production system, if it ever does.\r\n\r\n"
		                                     "Imagine, for a moment, a project to create a realtime system that absolutely cannot be allowed to fail; examples might include a controller for a nuclear reactor, a jetliner's flight-control system, or the image processor in a television set showing that important football game. In such a setting, it is nice to know that the system will always respond to events within the budgeted time.\r\n\r\n"
		                                     "Simply observing that it seems to do so tends to be considered inadequate for these systems. One way to get to a higher level of assurance is to create a formal model of the system, prove mathematically that the model produces the desired results, then run that model with every scenario that can be imagined. This approach can work, but it has its difficulties: ensuring that the model properly matches the real system is a challenge in its own right and, even if the model is perfect, it is almost certain to be far too slow for any sort of exhaustive testing.\r\nThe complexity of real-world systems makes this approach impractical, at best. Runtime verification. The runtime verification patches take a different approach.\r\n\r\n"
		                                     "Developers can create a high-level model describing the states that the system can be in and the transitions that occur in response to events. The verification code will then watch the kernel to ensure that the system does, indeed, behave as expected. If a discrepancy is found in a running system, then there is either an error in the model or a bug in the system; either way, fixing it will improve confidence that the system is behaving correctly. The use of this mechanism is described in this documentation patch helpfully included with the code; the following example is shamelessly stolen from there.\r\n\r\n"
		                                     "The first step is to express one's model of an aspect of the system's behavior; the example given is whether a CPU can be preempted or not a question that realtime researchers tend to be interested in. This model is described in the DOT language.\r\n\r\n",

		                                     80
		                                    );
		sleep(60);
		exit(1);

	}
#endif

	bool run() {
#ifdef __MENTOC_RUN_WRAPPER_PREGAME__
		do_wrapper();
#endif
		deep_object_parser_test();
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

