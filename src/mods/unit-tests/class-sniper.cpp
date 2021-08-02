#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_CLASS_SNIPER__
#define __MENTOC_UNIT_TESTS_CLASS_SNIPER__
#include "../player.hpp"
#include <iostream>
#include <vector>
#include "../levels.hpp"
#include "../super-users.hpp"
#include "../injure.hpp"
#include "../weapons/damage-types.hpp"
#include "../weapons/damage-calculator.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../object-utils.hpp"
#include "../projectile.hpp"
using de = damage_event_t;
#include "../damage-event.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"

extern player_ptr_t new_player();
TEST_CASE("a sniper class can deploy several skills") {
	SECTION("") {
		world.emplace_back();
		mods::globals::register_room(0);
		world[0].textures() = {};

		auto attacker = new_player();
		mods::globals::register_player(attacker);
		attacker->room() = 0;

		auto victim = new_player();
		mods::globals::register_player(victim);
		victim->room() = 0;

		auto weapon = create_object(ITEM_RIFLE,"g36c.yml");

		attacker->level() = 5;	//tier 1
		victim->level() = 5;	//tier 1

		victim->set_damage_nerf(0);
		mods::weapons::damage_calculator::set_static_roll_weapon_amount(50);
		auto base_damage = mods::weapons::damage_calculator::calculate(attacker,weapon,victim);

		victim->set_damage_nerf(10);
		int16_t nerfed_damage = mods::weapons::damage_calculator::calculate(attacker,weapon,victim);

		REQUIRE(nerfed_damage == (int16_t)(base_damage - (base_damage * 0.10)));
		mods::weapons::damage_calculator::clear_static_roll_weapon_amount();
	}
}


#endif
#endif
