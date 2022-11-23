#include "breacher.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../rooms.hpp"
#include "../interpreter.hpp"
#include "../query-objects.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t& player);
};
namespace mods::classes {
	breacher::breacher(player_ptr_t p) {
		m_explosive_shot_charges = 0;
		init();
		load_by_player(p);
	}
	std::tuple<bool,std::string> breacher::deconstruct_weapon(obj_ptr_t& obj) {
		if(m_deconstruct_weapon_charges == 0) {
			return {0,"You can't do that yet!"};
		}
		if(obj->has_rifle() == false) {
			return {0,"You can only deconstruct weapons!"};
		}
		auto s = roll_skill_success(DECONSTRUCT_WEAPON);
		if(std::get<0>(s)) {
			m_deconstruct_weapon.use_skill(m_player);
			--m_deconstruct_weapon_charges;
		}
		return s;
	}
	void breacher::replenish() {
		static uint32_t call_count = 0;
		uint8_t tier = tier(m_player);
		if(++call_count >= BREACHER_REPLENISH_CALL_COUNT()) {
			call_count = 0;
			if(m_explosive_shot_charges + 1 < BREACHER_EXPLOSIVE_SHOT_MAX_COUNT() * tier) {
				++m_explosive_shot_charges;
			}
			if(m_deconstruct_weapon_charges + 1 < BREACHER_DECONSTRUCT_WEAPON_MAX_COUNT() * tier) {
				++m_deconstruct_weapon_charges;
			}
		}
	}
	player_ptr_t 	breacher::player() {
		return m_player;
	}
	void breacher::set_player(player_ptr_t p) {
		m_player = p;
	}
	int16_t breacher::new_player(player_ptr_t& player) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t breacher::load_by_player(player_ptr_t& player) {
		m_explosive_shot_charges = 0;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("breacher::load_by_player unable to load breacher class by player id: ",(player->db_id()),".. return status: ",(result)));
			return result;
		}
		set_player(player);
		return result;
	}
	/* constructors and destructors */
	void breacher::init() {
		m_explosive_shot_charges = 0;
		m_deconstruct_weapon_charges = 0;
		m_push_count.clear();
		m_orm.init();
		m_player = nullptr;
		m_explosive_shot_charges = 0;

		m_teep_level = 0;

		using SK = ability_data_t::skillset_t;
		m_abilities = create_abilities({
			{DECONSTRUCT_WEAPON,"dw","Deconstruct Weapon",SK::DEMOLITIONS,&m_deconstruct_weapon,0},
			//{UB_SHOTGUN,"ubs","Underbarrel Shotgun",SK::DEMOLITIONS,&m_ub_shotgun,GHOST_SHOTUB_MANA_COST()},
			//{UB_FRAG,"ubf","Underbarrel Nade Launcher",SK::DEMOLITIONS,&m_ub_frag,GHOST_UBFRAG_MANA_COST()},
			//{PLANT_CLAYMORE,"claymore","Plant Claymore",SK::DEMOLITIONS,&m_plant_claymore,GHOST_CLAYMORE_MANA_COST()},
			//{SHRAPNEL_CLAYMORE,"smine","Shrapnel Claymore",SK::DEMOLITIONS,&m_plant_shrapnel_claymore,GHOST_SHRAPCLAY_MANA_COST()},
			//{CORROSIVE_CLAYMORE,"cmine","Corrosive Claymore",SK::DEMOLITIONS,&m_plant_corrosive_claymore,GHOST_CORCLAY_MANA_COST()},
		});
	}

	std::tuple<bool,std::string> breacher::explosive_shot(direction_t direction) {
		if(0 == m_explosive_shot_charges) {
			return {false,"You must wait until you can use your {grn}Explosive shot{/grn} again."};
		}
		auto& room = world[m_player->room()];
		if(!room.dir_option[direction]) {
			mods::rooms::pave_once(m_player->room(),direction);
			--m_explosive_shot_charges;
			return {true,"{grn}Your explosive shot makes an entrance!{/grn}"};
		}
		return {true,"You enter the pre-existing entrance."};
	}
	void breacher::attempt_direction(direction_t dir) {
		auto p = std::make_pair<>(m_player->room(),dir);
		m_push_count[p]++;
		if(m_push_count[p] >= 2) {
			if(m_explosive_shot_charges == 0) {
				m_player->sendln("{red}You attempt to charge an explosive shot, but you are all out!{/red}");
				return;
			}
			auto status = explosive_shot(dir);
			m_player->sendln(std::get<1>(status));
			if(std::get<0>(status)) {
				m_push_count.erase(p);
			}
			return;
		}
		if(m_push_count[p] < 2 && m_explosive_shot_charges) {
			m_player->sendln(
			    CAT(
			        "{grn}You ready an explosive charge toward the ",dirstr(dir).c_str(),
			        " door...{/grn}"
			    )
			);
		}
	}
	std::shared_ptr<breacher> create_breacher(player_ptr_t& in_player) {
		return std::move(std::make_shared<breacher>(in_player));
	}
};
ACMD(do_deconstruct_weapon) {
	PLAYER_CAN("breacher.deconstruct");
	DO_HELP_WITH_ZERO("breacher.deconstruct");
	static constexpr std::string_view usage = "Usage: breacher:deconstruct_weapon <item>\r\n"
	    "Please note that the item in question must be in your inventory.\r\n";
	const auto vec_args = PARSE_ARGS();
	if(!argshave()->size_gt(0)->passed()) {
		player->sendln(usage);
		return;
	}
	std::vector<uuid_t> items = mods::query_objects::query_inventory_for_object(player,vec_args[0]);
	if(items.size() == 0) {
		player->sendln("Couldn't find anything in your inventory matching that.");
		return;
	}
	auto obj = optr_by_uuid(items[0]);
	if(!obj) {
		player->sendln("Couldn't find anything in your inventory matching that.");
		return;
	}

	auto status = player->breacher()->deconstruct_weapon(obj);
	if(!std::get<0>(status)) {
		player->errorln(std::get<1>(status));
		return;
	}
	player->sendln(std::get<1>(status));
}
namespace mods::class_abilities::breacher {
	void init() {
		mods::interpreter::add_command("breacher:deconstruct", POS_RESTING, do_deconstruct_weapon, 0,0);

	}
};
