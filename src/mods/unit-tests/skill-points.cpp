#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_SKILL_POINTS__
#define __MENTOC_UNIT_TESTS_SKILL_POINTS__
#include "../player.hpp"
#include <iostream>
#include <vector>
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../orm/skill-points.hpp"
#include "../orm/player-skill-points.hpp"

extern player_ptr_t new_player();
#if 0
TEST_CASE("skill points are loaded from sql") {
	SECTION("skill points for a specific level are calculated correctly") {
		uint16_t sp_counter = 0;
		for(int i=1; i <= 5; i++) {
			sp_counter += i;
		}
		mods::orm::skill_points sp;

		REQUIRE(sp_counter == sp.calculate_skill_points_by_level(5));
	}
}// end test case
#endif
#endif
#endif
