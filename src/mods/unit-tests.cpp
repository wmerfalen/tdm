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
#include "damage-event.hpp"
#include "catch2/Catch2/src/catch2/catch_all.hpp"
#include "weapons/elemental.hpp"

extern player_ptr_t new_player();
int calculate_hp(int starting_hp,int requested_damage,int resistance, int incendiary_damage_percent) {
	auto attacker = new_player();
	mods::globals::register_player(attacker);
	auto weapon = create_object(ITEM_RIFLE,"g36c.yml");
	weapon->rifle()->attributes->incendiary_damage = incendiary_damage_percent;

	auto victim = new_player();
	mods::globals::register_player(victim);
	victim->hp() = starting_hp;
	feedback_t feedback;
	feedback.damage = requested_damage;
	feedback.hits = 1;
	feedback.from_direction = NORTH;
	feedback.attacker = attacker->uuid();
	victim->incendiary_resistance_percent() = resistance;
	mods::weapons::elemental::process_elemental_damage(attacker,weapon,victim,feedback);
	return victim->hp();
}
TEST_CASE("calculating player resistances reduces damage properly", "[reduce_elemental_resistance]") {
	int requested_damage = 100;
	int16_t resistance = 50;
	REQUIRE(mods::weapons::elemental::reduce_elemental_resistance(requested_damage, resistance) == 50);
}

TEST_CASE("player resistance member functions should reduce damage", "[reduce_elemental_resistance via player resistance percent]") {
	SECTION("resistance percents reduce damage dealt") {
		int starting_hp = 100;
		int requested_damage = 100;
		int16_t resistance = 50;
		auto player = new_player();
		mods::globals::register_player(player);
		player->hp() = starting_hp;
		player->incendiary_resistance_percent() = resistance;
		mods::weapons::elemental::incendiary_damage(player,requested_damage);
		REQUIRE(player->hp() == (starting_hp - (requested_damage - ((resistance * 0.01) * requested_damage))));
	}
	SECTION("zero resistance percents have no effect") {
		int starting_hp = 100;
		int requested_damage = 100;
		int16_t resistance = 50;
		auto player = new_player();
		mods::globals::register_player(player);
		player->hp() = starting_hp;
		player->incendiary_resistance_percent() = resistance;
		mods::weapons::elemental::incendiary_damage(player,requested_damage);
		REQUIRE(player->hp() == (starting_hp - (requested_damage - ((resistance * 0.01) * requested_damage))));
	}

	SECTION("rifle deals incendiary damage to victim with 50 percent incendiary resistance") {
		REQUIRE(75 == calculate_hp(100,100,50,50));
	}
	SECTION("rifle deals incendiary damage to victim with 0 percent incendiary resistance") {
		REQUIRE(50 == calculate_hp(100,100,0,50));
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
