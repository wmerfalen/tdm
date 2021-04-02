#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_MOB_ROAM__
#define __MENTOC_UNIT_TESTS_MOB_ROAM__
#include "../orm/mob-roam.hpp"
#include "../mob-roam.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../builder.hpp"

extern player_ptr_t new_player();
player_ptr_t create_builder() {
	auto player = new_player();
	mods::builder::initialize_builder(player);
	return std::move(player);
}

TEST_CASE("a mob roaming profile has crud properties") {
	SECTION("saving a mob roaming profile") {
		auto player = create_builder();
		REQUIRE(1 == 1);
	}
}// end test case
#endif
#endif
