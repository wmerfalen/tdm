#if 0
#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_FRAG_UNDERBARREL__
#define __MENTOC_UNIT_TESTS_FRAG_UNDERBARREL__
#include "../player.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../weapons/attachment-frag-underbarrel.hpp"
#include "../classes/sniper.hpp"
#include "../fluxkraft.hpp"

extern player_ptr_t new_player();
TEST_CASE("A frag grenade launches in a direction") {

	SECTION("Generating a horizontal line of rooms") {
		auto attacker = new_player();
		attacker->name().assign("xisop");
		mods::globals::register_player(attacker);
		mods::globals::register_authenticated_player(attacker);
		mods::fluxkraft::generate_horizonal_line_at(attacker,0,0,0,8);
		auto room = attacker->room();
		for(int i=0; i < 8; i++) {
			command_interpreter(attacker,"e");
		}
		REQUIRE(room != attacker->room());
	}

	SECTION("A frag grenade stops at closed doors and blows up in that room") {
		obj_ptr_t frag = create_object(ITEM_ATTACHMENT,"sniper-class-frag-ub.yml");
		auto attacker = new_player();
		attacker->name().assign("xisop");
		REQUIRE(true);
	}
}
#endif
#endif
#endif
