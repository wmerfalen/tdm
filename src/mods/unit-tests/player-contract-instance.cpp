#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_CONTRACT_TRACKING__
#define __MENTOC_UNIT_TESTS_CONTRACT_TRACKING__
#include "../orm/contracts.hpp"
#include "../orm/contract-steps.hpp"
#include "../orm/player-contract-state.hpp"
#include "../player-contract-instance.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"
#include "../player.hpp"
#include "../contracts.hpp"

extern player_ptr_t new_player();
namespace mods::globals {
	extern std::map<uint64_t,uuid_t> db_id_to_uuid_map;
	extern void register_authenticated_player(player_ptr_t player);
	extern void register_player(player_ptr_t player);
};

TEST_CASE("contracts are tracked by player instance wrappers") {
	SECTION("a contract is tracked by finding items") {
		mods::orm::contracts orm;
		orm.delete_by_contract_vnum(1);
		orm.c_vnum = 1;
		orm.c_title = "Find and eliminate HVT";
		orm.c_description = "Intelligence tells us that our HVT's current location is somewhere "
		                    "near the Gasaraki border checkpoint. Find and eliminate doctor Friedman.";
		orm.save();

		mods::orm::contract_steps cs;

		{
			cs.rows.emplace_back();
			auto& step = cs.rows.back();
			step.s_contract_vnum = orm.c_vnum;
			step.s_task_type = mods::contracts::contract_step::task_type_t::GOAL_FIND;
			step.s_task_target = mods::contracts::contract_step::task_target_t::TARGET_ITEM;
			step.s_description = "Find the G36c rifle at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "g36c.yml";
			step.s_room_vnum = 0;
			step.s_quota = 0;
			step.s_is_optional = 0;
			step.s_order = 0;
		}
		{
			cs.rows.emplace_back();
			auto& step = cs.rows.back();
			step.s_contract_vnum = orm.c_vnum;
			step.s_task_type = mods::contracts::contract_step::task_type_t::GOAL_FIND;
			step.s_task_target = mods::contracts::contract_step::task_target_t::TARGET_ITEM;
			step.s_description = "Find the mp5 smg at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "mp5.yml";
			step.s_room_vnum = 0;
			step.s_quota = 0;
			step.s_is_optional = 0;
			step.s_order = 1;
		}
		cs.save();

		mods::contracts::load_all_contracts();

		auto player = new_player();
		player->set_db_id(1);
		player->cd()->player.name.assign("unittest");
		player->name() = "unittest";
		mods::globals::register_player(player);
		mods::globals::register_authenticated_player(player);
		mods::contracts::start_contract(player,1);
		auto c = player->contracts().front();
		REQUIRE(c->is_find_item() == true);
		REQUIRE(c->step_counter() == 0);
		auto rifle = create_object(ITEM_RIFLE,"g36c.yml");
		player->contract_find_item(rifle);
		REQUIRE(c->step_counter() == 1);
	}
}

#endif
#endif
