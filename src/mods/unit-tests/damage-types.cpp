#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_DAMAGE_TYPES__
#define __MENTOC_UNIT_TESTS_DAMAGE_TYPES__
#include "../player.hpp"
#include <iostream>
#include <vector>
#include "../weapons/damage-types.hpp"
#include "../damage-event.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"

extern player_ptr_t new_player();
TEST_CASE("user takes 15\% damage when tracked") {
	SECTION("calculations occur when player is tracked") {
		world.emplace_back();
		mods::globals::register_room(1);
		world.back().textures() = {};

		auto target = new_player();
		mods::globals::register_player(target);
		target->affect(AFF_TRACKED);
		target->room() = 0;
		static constexpr int original_damage = 100;
		static constexpr int expected_damage = original_damage + (mods::affects::TRACKED_DAMAGE_BONUS_MULTIPLIER * original_damage);
		REQUIRE(expected_damage == mods::weapons::damage_types::calculate_tracked_damage(target,original_damage));
	}
}
#endif
#endif
