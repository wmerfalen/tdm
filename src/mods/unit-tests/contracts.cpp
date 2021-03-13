#ifndef __MENTOC_DONT_COMPILE_UNIT_TESTS__
#ifndef __MENTOC_UNIT_TESTS_CONTRACTS__
#define __MENTOC_UNIT_TESTS_CONTRACTS__
#include "../orm/contracts.hpp"
#include "../orm/contract-steps.hpp"
#include "../orm/player-contract-state.hpp"
#include "../catch2/Catch2/src/catch2/catch_all.hpp"

extern player_ptr_t new_player();
int count_contracts() {
	std::deque<std::shared_ptr<mods::contracts::contract>> list;
	auto s = mods::orm::load_all_non_orm_contracts(&list);
	std::size_t contract_count = 0;
	for(auto row : list) {
		if(row->vnum == 1) {
			++contract_count;
		}
	}
	return contract_count;
}

TEST_CASE("contracts are saved to orm layer") {
	SECTION("a contract record can be saved and retrieved") {
		mods::orm::contracts orm;
		orm.c_vnum = 1;
		orm.c_title = "Find and eliminate HVT";
		orm.c_description = "Intelligence tells us that our HVT's current location is somewhere "
		                    "near the Gasaraki border checkpoint. Find and eliminate doctor Friedman.";
		orm.save();

		REQUIRE(count_contracts() == 1);

		orm.delete_by_contract_vnum(1);
		REQUIRE(count_contracts() == 0);
	}
	SECTION("a contract's steps can be saved and loaded") {
		mods::orm::contracts orm;
		orm.delete_by_contract_vnum(1);

		mods::orm::contract_steps cs;
		cs.delete_by_contract_vnum(1);
		cs.rows.clear();
		auto result = db_get_all("contract_steps");
		REQUIRE(result.size() == 0);

		orm.c_vnum = 1;
		orm.c_title = "Find and eliminate HVT";
		orm.c_description = "Intelligence tells us that our HVT's current location is somewhere "
		                    "near the Gasaraki border checkpoint. Find and eliminate doctor Friedman.";
		orm.save();


		{
			cs.rows.emplace_back();
			auto& step = cs.rows.back();
			step.s_contract_vnum = orm.c_vnum;
			step.s_task_type = mods::contracts::contract_step::task_type_t::GOAL_FIND;
			step.s_task_target = mods::contracts::contract_step::task_target_t::TARGET_ITEM;
			step.s_description = "Find the G36c rifle at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "rifle/g36c.yml";
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
			step.s_description = "Find the MP-5 sub-machine gun at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "rifle/mp5.yml";
			step.s_room_vnum = 0;
			step.s_quota = 0;
			step.s_is_optional = 0;
			step.s_order = 1;
		}

		{
			cs.rows.emplace_back();
			auto& step = cs.rows.back();
			step.s_contract_vnum = orm.c_vnum;
			step.s_task_type = mods::contracts::contract_step::task_type_t::GOAL_FIND;
			step.s_task_target = mods::contracts::contract_step::task_target_t::TARGET_ITEM;
			step.s_description = "Find the CZP-10 pistol at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "rifle/czp10.yml";
			step.s_room_vnum = 0;
			step.s_quota = 0;
			step.s_is_optional = 0;
			step.s_order = 2;
		}
		cs.save();

		{
			std::deque<std::shared_ptr<mods::contracts::contract>> list;
			auto s = mods::orm::load_all_non_orm_contracts(&list);
			REQUIRE(list[0]->steps.size() == 3);
			REQUIRE(list[0]->steps[0].object_yaml.compare("rifle/g36c.yml") == 0);
			REQUIRE(list[0]->steps[1].object_yaml.compare("rifle/mp5.yml") == 0);
			REQUIRE(list[0]->steps[2].object_yaml.compare("rifle/czp10.yml") == 0);
		}
		cs.delete_by_contract_vnum(1);

		{
			orm.delete_by_contract_vnum(1);
			REQUIRE(count_contracts() == 0);
		}
	}
	SECTION("player state can be saved and deleted") {
		mods::orm::contracts orm;
		orm.delete_by_contract_vnum(1);

		mods::orm::contract_steps cs;
		cs.delete_by_contract_vnum(1);
		cs.rows.clear();
		auto result = db_get_all("contract_steps");
		REQUIRE(result.size() == 0);

		orm.c_vnum = 1;
		orm.c_title = "Find and eliminate HVT";
		orm.c_description = "Intelligence tells us that our HVT's current location is somewhere "
		                    "near the Gasaraki border checkpoint. Find and eliminate doctor Friedman.";
		orm.save();


		{
			cs.rows.emplace_back();
			auto& step = cs.rows.back();
			step.s_contract_vnum = orm.c_vnum;
			step.s_task_type = mods::contracts::contract_step::task_type_t::GOAL_FIND;
			step.s_task_target = mods::contracts::contract_step::task_target_t::TARGET_ITEM;
			step.s_description = "Find the G36c rifle at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "rifle/g36c.yml";
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
			step.s_description = "Find the MP-5 sub-machine gun at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "rifle/mp5.yml";
			step.s_room_vnum = 0;
			step.s_quota = 0;
			step.s_is_optional = 0;
			step.s_order = 1;
		}

		{
			cs.rows.emplace_back();
			auto& step = cs.rows.back();
			step.s_contract_vnum = orm.c_vnum;
			step.s_task_type = mods::contracts::contract_step::task_type_t::GOAL_FIND;
			step.s_task_target = mods::contracts::contract_step::task_target_t::TARGET_ITEM;
			step.s_description = "Find the CZP-10 pistol at the entrance of the border compound.";
			step.s_mob_vnum = 0;
			step.s_object_yaml = "rifle/czp10.yml";
			step.s_room_vnum = 0;
			step.s_quota = 0;
			step.s_is_optional = 0;
			step.s_order = 2;
		}
		cs.save();

		{
			std::deque<std::shared_ptr<mods::contracts::contract>> list;
			auto s = mods::orm::load_all_non_orm_contracts(&list);
			REQUIRE(list[0]->steps.size() == 3);
			REQUIRE(list[0]->steps[0].object_yaml.compare("rifle/g36c.yml") == 0);
			REQUIRE(list[0]->steps[1].object_yaml.compare("rifle/mp5.yml") == 0);
			REQUIRE(list[0]->steps[2].object_yaml.compare("rifle/czp10.yml") == 0);
		}

		{
			mods::orm::player_contract_state state_orm;
			state_orm.pc_contract_vnum = 1;
			state_orm.pc_state_data = "foobarstate";
			state_orm.pc_player_id = 1;
			state_orm.save();
			std::string in_buffer;
			auto res = mods::orm::load_player_contract_state(1,1,in_buffer);
			REQUIRE(in_buffer.compare("foobarstate") == 0);

			static constexpr std::string_view UPDATED_STR = "updated|foobar|state";
			state_orm.update_player_data(1,1,UPDATED_STR.data());

			in_buffer.clear();
			res = mods::orm::load_player_contract_state(1,1,in_buffer);
			REQUIRE(in_buffer.compare(UPDATED_STR.data()) == 0);

			state_orm.delete_by_player_id_contract_vnum(1,1);
			res = mods::orm::load_player_contract_state(1,1,in_buffer);
			REQUIRE(std::get<0>(res) == 0);
			REQUIRE(std::get<1>(res).compare("no results") == 0);

		}
		cs.delete_by_contract_vnum(1);

		{
			orm.delete_by_contract_vnum(1);
			REQUIRE(count_contracts() == 0);
		}
	}
}// end test case
#endif
#endif
