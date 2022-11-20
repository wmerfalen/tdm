#include "breacher.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../rooms.hpp"
#include "../interpreter.hpp"

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
	std::tuple<bool,std::string> breacher::inject_adrenaline_shot() {
		if(m_ad_shot.active()) {
			return {0,"You already have an adrenaline shot active!"};
		}
		if(!has_mana_for_skill(ADRENALINE_SHOT)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(ADRENALINE_SHOT);
		auto s = roll_skill_success(ADRENALINE_SHOT);
		if(std::get<0>(s)) {
			m_adrenaline_shot.use_skill(m_player);
			return m_ad_shot.inject(m_player);
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
		m_push_count.clear();
		m_orm.init();
		m_player = nullptr;
		m_explosive_shot_charges = 0;

		m_teep_level = 0;

		using SK = ability_data_t::skillset_t;
		m_abilities = create_abilities({
			{ADRENALINE_SHOT,"as","Adrenaline Shot",SK::MEDICAL,&m_adrenaline_shot,GHOST_ADRENALSHOT_MANA_COST()},
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
ACMD(do_inject_adrenaline_shot) {
	PLAYER_CAN("breacher.adrenaline_shot");
	DO_HELP_WITH_ZERO("breacher.adrenaline_shot");
	auto status = player->breacher()->inject_adrenaline_shot();
	if(!std::get<0>(status)) {
		player->errorln(std::get<1>(status));
		return;
	}
	player->sendln(std::get<1>(status));
}
ACMD(do_teep) {
	player->sendln("Command not implemented.");
	return;
#if 0
	PLAYER_CAN("breacher.teep");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 1) {
		player->sendln("Which direction?!");
		return;
	}
	auto s = to_direction(vec_args[0]);
	if(!s.first) {
		player->sendln("Which direction?!");
		return;
	}
	auto msg = std::get<1>(player->breacher()->teep(s.second));
	player->sendln(msg);
#endif
}
namespace mods::class_abilities::breacher {
	void init() {
		mods::interpreter::add_command("breacher:adrenaline_shot", POS_RESTING, do_inject_adrenaline_shot, 0,0);

	}
};
