#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_RIFLE_INSTANCE__
#define __MENTOC_UNIT_TESTS_RIFLE_INSTANCE__
#include "../player.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../orm/rifle-instance.hpp"
#include "../yaml.hpp"
#include "../rifle-attachments.hpp"

TEST_CASE("A rifle instance can be saved and loaded to the db") {
	SECTION("rifle instance orm layer saves all properties properly") {
		obj_ptr_t ar = create_object(ITEM_RIFLE,"g36c.yml");
		static constexpr int ammo_max = 59;
		ar->rifle()->attributes->ammo_max = ammo_max;
		mods::orm::rifle_instance inst;
		inst.rifle_ammo_max = ammo_max;
		auto id = inst.initialize_row(ar);
		std::cerr << "id: " << id << "\n";
		inst.get_dump();
		std::cerr << inst.get_dump();
		inst.rifle_ammo_max = 0;
		inst.load_by_id(id);
		std::cerr << inst.get_dump();
		REQUIRE(ammo_max == inst.rifle_ammo_max);
	}

	SECTION("create_object can be retrofitted to load rifle instances from db") {
		obj_ptr_t ar = create_object(ITEM_RIFLE,"g36c.yml");
		static constexpr int ammo_max = 59;
		ar->rifle()->attributes->ammo_max = ammo_max;
		mods::orm::rifle_instance inst;
		inst.rifle_ammo_max = ammo_max;
		auto id = inst.initialize_row(ar);
		auto obj = create_object(ITEM_RIFLE,CAT("rifle|pkid:",id));
		REQUIRE(ammo_max == obj->rifle()->attributes->ammo_max);
	}
	SECTION("deep object parser can be used to load rifle instances from the db") {
		obj_ptr_t ar = create_object(ITEM_RIFLE,"g36c.yml");
		static constexpr int ammo_max = 59;
		ar->rifle()->attributes->ammo_max = ammo_max;
		mods::orm::rifle_instance inst;
		inst.rifle_ammo_max = ammo_max;
		auto id = inst.initialize_row(ar);
		auto obj = create_object(ITEM_RIFLE,CAT("rifle|pkid:",id));
		REQUIRE(ammo_max == obj->rifle()->attributes->ammo_max);
		mods::rifle_attachments_t a(CAT("rifle|pkid:",id,"{sight:acog.yml}"));
		REQUIRE(a.base_object->rifle()->attributes->ammo_max == ammo_max);
	}

	SECTION("when a forge engine object is created, it is saved to the rifle_instance table") {
		REQUIRE(1 == 1);
	}
}
#endif
#endif
