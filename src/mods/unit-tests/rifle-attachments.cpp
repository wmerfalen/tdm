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
#endif
#endif
