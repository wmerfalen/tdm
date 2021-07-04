#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#include "../player.hpp"
#include <iostream>
#include <vector>
#include "../weapons/damage-types.hpp"
#include "../damage-event.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../weapons/elemental.hpp"

extern player_ptr_t new_player();
TEST_CASE("armor with elemental resistance reduces damage", "[reduce_elemental_resistance]") {
	SECTION("armor that has 50 percent incendiary damage reduces incendiary damage by 50 percent") {
		int starting_hp = 100;
		int requested_damage = 100;
		auto attacker = new_player();
		auto victim= new_player();
		mods::globals::register_player(attacker);
		mods::globals::register_player(victim);
		attacker->position() = POS_STANDING;
		victim->position() = POS_STANDING;
		auto vest = create_object(ITEM_ARMOR,"falcon-ballistic-vest.yml");
		vest->armor()->attributes->incendiary_resistance_percent = 50;
		victim->equip(vest,ITEM_WEAR_BODY);
		REQUIRE(victim->incendiary_resistance_percent() == vest->armor()->attributes->incendiary_resistance_percent);
		victim->hp() = starting_hp;
		mods::weapons::elemental::incendiary_damage(attacker,victim,requested_damage);
		REQUIRE(victim->hp() == 50);

	}
	SECTION("removing armor removes resistances") {
		int starting_hp = 100;
		int requested_damage = 100;
		auto player = new_player();
		mods::globals::register_player(player);
		auto vest = create_object(ITEM_ARMOR,"falcon-ballistic-vest.yml");
		vest->armor()->attributes->incendiary_resistance_percent = 50;
		player->equip(vest,ITEM_WEAR_BODY);
		REQUIRE(player->incendiary_resistance_percent() == 50);
		player->hp() = starting_hp;
		mods::weapons::elemental::incendiary_damage(player,player,requested_damage);
		REQUIRE(player->hp() == 50);

		player->unequip(ITEM_WEAR_BODY);
		player->hp() = starting_hp;
		mods::weapons::elemental::incendiary_damage(player,player,requested_damage);
		REQUIRE(player->hp() == 0);
	}//end section
}// end test case
#endif
