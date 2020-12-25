#ifdef __MENTOC_DONT_COMPILE_UNIT_TESTS__
namepace mods::unit_tests {
	int run() {
		return 0;
	}
};
#else
#if 0
#undef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#include "catch2.hpp"
#endif
#include "player.hpp"
#include <iostream>
#include <vector>
#include "weapons/damage-types.hpp"
#include "catch2/Catch2/src/catch2/catch_all.hpp"

extern player_ptr_t new_player();
TEST_CASE("calculating player resistances reduces damage properly", "[reduce_elemental_resistance]") {
	int requested_damage = 100;
	int16_t resistance = 50;
	REQUIRE(mods::weapons::damage_types::reduce_elemental_resistance(requested_damage, resistance) == 50);
}

TEST_CASE("player resistance member functions should reduce damage", "[reduce_elemental_resistance via player resistance percent]") {
	int starting_hp = 100;
	int requested_damage = 100;
	int16_t resistance = 50;
	auto player = new_player();
	mods::globals::register_player(player);
	SECTION("resistance percents reduce damage dealt") {
		player->hp() = starting_hp;
		player->incendiary_resistance_percent() = resistance;
		mods::weapons::damage_types::incendiary_damage(player,requested_damage);
		REQUIRE(player->hp() == (starting_hp - (requested_damage - ((resistance * 0.01) * requested_damage))));
	}
	SECTION("zero resistance percents have no effect") {
		starting_hp = 100;
		requested_damage = 100;
		resistance = 0;
		player->hp() = starting_hp;
		player->incendiary_resistance_percent() = resistance;
		mods::weapons::damage_types::incendiary_damage(player,requested_damage);
		REQUIRE(player->hp() == (starting_hp - (requested_damage - ((resistance * 0.01) * requested_damage))));
	}
}
namespace mods::unit_tests {
	int run() {
		Catch::Session().run();
		sleep(-1);
		exit(0);
		return 0;
	}
};
#endif
