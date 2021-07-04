#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_DAMAGE_TYPES__
#define __MENTOC_UNIT_TESTS_DAMAGE_TYPES__
#include "../player.hpp"
#include <iostream>
#include <vector>
#include "../levels.hpp"
#include "../super-users.hpp"
#include "../injure.hpp"
#include "../weapons/damage-types.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../object-utils.hpp"
#include "../projectile.hpp"
using de = damage_event_t;
#include "../damage-event.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"

namespace mods::weapons::damage_types {
	extern std::tuple<bool,feedback_t> can_continue(player_ptr_t& player,obj_ptr_t weapon,std::variant<player_ptr_t,obj_ptr_t,int8_t> victim,direction_t direction, const uint8_t& item_type);
};
extern player_ptr_t new_player();
TEST_CASE("user takes 15\% damage when tracked") {
	SECTION("calculations occur when player is tracked") {
		world.emplace_back();
		mods::globals::register_room(1);
		world[0].textures() = {};

		auto victim = new_player();
		mods::globals::register_player(victim);
		victim->affect(AFF_TRACKED);
		victim->room() = 0;
		static constexpr int original_damage = 100;
		static constexpr int expected_damage = original_damage + (mods::affects::TRACKED_DAMAGE_BONUS_MULTIPLIER * original_damage);
		REQUIRE(expected_damage == mods::weapons::damage_types::calculate_tracked_damage(victim,original_damage));
	}
}
TEST_CASE("user cannot attack within peaceful rooms") {
	SECTION("melee weapons") {
		world.emplace_back();
		mods::globals::register_room(0);
		world[0].room_flags = ROOM_PEACEFUL;
		world[0].textures() = {};

		auto attacker = new_player();
		mods::globals::register_player(attacker);
		attacker->room() = 0;

		auto victim = new_player();
		mods::globals::register_player(victim);
		victim->room() = 0;

		auto weapon = create_object(ITEM_MELEE,"crowbar.yml");

		REQUIRE(mods::rooms::is_peaceful(attacker->room()) == true);

		std::tuple<bool,feedback_t> result = mods::weapons::damage_types::can_continue(attacker,
		                                                                               weapon,
		                                                                               victim,
		                                                                               NORTH,
		                                                                               ITEM_MELEE
		                                                                              );
		REQUIRE(std::get<0>(result) == false);
		REQUIRE(std::get<1>(result).damage_event == de::YOURE_IN_PEACEFUL_ROOM);

		auto feedback = mods::weapons::damage_types::melee_damage_with_feedback(
		                    attacker,
		                    weapon,
		                    victim);
		REQUIRE(feedback.damage_event == de::YOURE_IN_PEACEFUL_ROOM);

	}

	SECTION("ranged weapons") {
		world.emplace_back();
		mods::globals::register_room(0);
		world[0].room_flags = ROOM_PEACEFUL;
		world[0].textures() = {};

		auto attacker = new_player();
		mods::globals::register_player(attacker);
		attacker->room() = 0;

		auto victim = new_player();
		mods::globals::register_player(victim);
		victim->room() = 0;

		auto weapon = create_object(ITEM_RIFLE,"g36c.yml");

		REQUIRE(mods::rooms::is_peaceful(attacker->room()) == true);

		std::tuple<bool,feedback_t> result = mods::weapons::damage_types::can_continue(attacker,
		                                                                               weapon,
		                                                                               victim,
		                                                                               NORTH,
		                                                                               ITEM_RIFLE
		                                                                              );
		REQUIRE(std::get<0>(result) == false);
		REQUIRE(std::get<1>(result).damage_event == de::YOURE_IN_PEACEFUL_ROOM);


		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
		                    attacker,
		                    weapon,
		                    victim,
		                    0,
		                    NORTH
		                );
		REQUIRE(feedback.damage_event == de::YOURE_IN_PEACEFUL_ROOM);
	}


	SECTION("explosives") {

		world.emplace_back();
		mods::globals::register_room(0);
		world[0].room_flags = ROOM_PEACEFUL;
		world[0].textures() = {};

		auto attacker = new_player();
		mods::globals::register_player(attacker);
		attacker->room() = 0;

		auto victim = new_player();
		mods::globals::register_player(victim);
		victim->room() = 0;

		auto weapon = create_object(ITEM_EXPLOSIVE,"frag-grenade.yml");
		auto object_uuid = weapon->uuid;

		REQUIRE(mods::rooms::is_peaceful(attacker->room()) == true);

		REQUIRE(mods::projectile::explode(0,object_uuid,attacker->uuid()) == -2);
	}
}
#endif
#endif
