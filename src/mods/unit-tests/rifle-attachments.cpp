#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_RIFLE_ATTACHMENTS__
#define __MENTOC_UNIT_TESTS_RIFLE_ATTACHMENTS__
#include "../player.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../orm/rifle-attachment.hpp"
#include "../yaml.hpp"
#include "../rifle-attachments.hpp"

extern player_ptr_t new_player();
TEST_CASE("A rifle attachment can be syncd to and from orm") {
	SECTION("rifle attachment orm layer saves all properties properly") {
		auto player = new_player();
		const std::string schema = "rifle/g36c.yml{barrel:silencer.yml,grip:hexon-grip.yml,magazine:extended-ar-mag.yml,muzzle:flash-hider.yml,sight:reflex-sight.yml,stock:titanium-stock.yml,strap:tactical-strap.yml,under_barrel:frag-underbarrel.yml}#level:32";
		mods::rifle_attachments_t rifle(schema);
		REQUIRE(schema.compare(rifle.export_objects()) == 0);
	}
}
TEST_CASE("An attachment that is added or removed affects the ranged combat totals") {
	SECTION("adding a stabilizer grip reflects the added bonuses of that item to the RCT totals") {

		/**
		 * @@@@@ ATTENTION
		 * @@@@@
		 * @@@@@ This value is hard-coded and must stay in-synch with the values in
		 * @@@@@ objects/attachment/stabilizer-grip.yml!!!
		 * @@@@@ This value HAS to be the same as armor_penetration_amount in said yml file!
		 * @@@@@
		 * @@@@@ ATTENTION
		 */
		static constexpr std::size_t ARMOR_PENETRATION_AMOUNT = 13;
		auto player = new_player();
		mods::globals::register_player(player);
		const std::string schema = "rifle/g36c.yml{sight:reflex-sight.yml,under_barrel:frag-underbarrel.yml}#level:1";
		auto rifle = mods::rifle_attachments::make(schema);
		player->equip(rifle->base_object,WEAR_PRIMARY,false);
		auto before = player->calculate_ranged_combat_totals();
		const auto before_armor_penetration = before->armor_penetration;
		std::cout << "before_armor_penetration: '" << before_armor_penetration << "'\n";

		auto grip = create_object(ITEM_ATTACHMENT,"stabilizer-grip.yml");
		REQUIRE(grip != nullptr);
		player->carry(grip);

		player->unequip_into_inventory(WEAR_PRIMARY);
		auto status = rifle->attach_from_inventory(player,grip->attachment()->attributes->str_type,grip);
		REQUIRE(status == 0);
		player->equip(rifle->base_object,WEAR_PRIMARY,false);

		auto after = player->calculate_ranged_combat_totals();
		const auto after_armor_penetration = after->armor_penetration;
		std::cout << "after_armor_penetration: '" << after_armor_penetration << "'\n";

		REQUIRE(after_armor_penetration - before_armor_penetration == ARMOR_PENETRATION_AMOUNT);
	}
}
#endif
#endif
