#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#include "../player.hpp"
#include <iostream>
#include <vector>
#include "../weapons/damage-types.hpp"
#include "../damage-event.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../weapons/elemental.hpp"

extern player_ptr_t new_player();
TEST_CASE("armor has stat boosts") {
	SECTION("armor that has +10 strength, adds to player's aff_abils()") {
		auto player = new_player();
		mods::globals::register_player(player);

		player->aff_abils().str = 0;

		auto vest = create_object(ITEM_ARMOR,"falcon-ballistic-vest.yml");
		vest->armor()->attributes->stat_strength = 10;
		player->equip(vest,ITEM_WEAR_BODY);

		REQUIRE(player->aff_abils().str == 10);
		player->unequip(ITEM_WEAR_BODY);

		REQUIRE(player->aff_abils().str == 0);
	}
}// end test case
#endif
