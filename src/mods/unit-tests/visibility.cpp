#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_VISIBILITY__
#define __MENTOC_UNIT_TESTS_VISIBILITY__
#include "../player.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../calc-visibility.hpp"

extern player_ptr_t new_player();
TEST_CASE("Any target that is being tracked will be visible") {
	SECTION("target attempts to go invisible") {
		auto observer = new_player();
		mods::globals::register_player(observer);
		auto target = new_player();
		mods::globals::register_player(target);
		target->affect(AFF_TRACKED);
		REQUIRE(mods::calc_visibility::is_visible(observer,target) == true);
	}
}
#endif
#endif
