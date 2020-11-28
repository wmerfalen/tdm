#include "ghost.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../player-utils.hpp"
#include "../object-utils.hpp"
#include "../replenish.hpp"
#include "../interpreter.hpp"
#include "../demolitions.hpp"
#include "../date-time.hpp"
#include "../weapons/damage-types.hpp"
#include "../skills.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
	void ghost_advance_level(player_ptr_t& player){
		/** TODO fill me in */
		player->send("[stub] file:%s line:%d\r\n",__FILE__,__LINE__);

	}
	void ghost::init(){
		m_claymore_count = 0;
		m_cryogenic_grenade_count = 0;
		m_flash_underbarrel_charges = 0;
		m_scanned.clear();
		m_player = nullptr;

		m_drone_scan_level = (decltype(m_drone_scan_level))0;
		m_stealth_level = (decltype(m_stealth_level))0;
		m_summon_extraction_level = (decltype(m_summon_extraction_level))0;
		m_xray_shot_level = (decltype(m_xray_shot_level))0;
		m_feign_death_level = (decltype(m_feign_death_level))0;
		m_plant_claymore_level = (decltype(m_plant_claymore_level))0;
		m_penetrating_shot_level = (decltype(m_penetrating_shot_level))0;
		m_intimidation_level = (decltype(m_intimidation_level))0;
		m_cryogenic_grenade_level = (decltype(m_cryogenic_grenade_level))0;
		m_flash_underbarrel_level = (decltype(m_flash_underbarrel_level))0;
		m_claymores.clear();
	}
	int16_t ghost::save(){
		return this->m_orm.save();
	}
	ghost::ghost(player_ptr_t p){
		this->init();
		load_by_player(p);
	}
	player_ptr_t 	ghost::player(){
		return m_player;
	}
	void ghost::set_player(player_ptr_t p){
		m_player = p;
	}
	int16_t ghost::new_player(player_ptr_t &player){
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0){
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t ghost::load_by_player(player_ptr_t & player){
		set_player(player);
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			report(CAT("ghost::load_by_player unable to load ghost class by player id: ",(player->db_id()),".. return status: ",(result)));
			return result;
		}
		m_drone_scan_level = (decltype(m_drone_scan_level))(m_orm.ghost_drone_scan_level);
		m_stealth_level = (decltype(m_stealth_level))(m_orm.ghost_stealth_level);
		m_summon_extraction_level = (decltype(m_summon_extraction_level))(m_orm.ghost_summon_extraction_level);
		m_xray_shot_level = (decltype(m_xray_shot_level))(m_orm.ghost_xray_shot_level);
		m_feign_death_level = (decltype(m_feign_death_level))(m_orm.ghost_feign_death_level);
		m_plant_claymore_level = (decltype(m_plant_claymore_level))(m_orm.ghost_plant_claymore_level);
		m_penetrating_shot_level = (decltype(m_penetrating_shot_level))(m_orm.ghost_penetrating_shot_level);
		m_intimidation_level = (decltype(m_intimidation_level))m_orm.ghost_intimidation_level;
		m_cryogenic_grenade_level = (decltype(m_cryogenic_grenade_level))m_orm.ghost_cryogenic_grenade_level;
		m_flash_underbarrel_level = (decltype(m_flash_underbarrel_level))m_orm.ghost_flash_underbarrel_level;
#ifdef MENTOC_GHOST_EQUIPS
		player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
#endif
		return result;
	}
	void ghost::apply_stealth_to(obj_ptr_t& target){
		int stealth = 0;
		switch(m_stealth_level) {
			default:
			case skill_familiarity_t::NONE:
				m_player->sendln("It looks like you still need to train that skill");
				return;
			case skill_familiarity_t::INITIATE:
				stealth = dice(1, 8) + 1 + (m_player->level() / 4);
				break;

			case skill_familiarity_t::FAMILIAR:
				stealth = dice(3, 8) + 3 + (m_player->level() / 4);
				break;

			case skill_familiarity_t::MASTER:
				stealth = 100 + dice(3, 8);
				break;
		}
		mods::object_utils::change_visibility(target,- stealth);
	}
	void ghost::apply_stealth_to_player(player_ptr_t& target){
		int stealth = 0;
		switch(m_stealth_level) {
			default:
			case skill_familiarity_t::NONE:
				m_player->sendln("It looks like you still need to train that skill");
				return;
			case skill_familiarity_t::INITIATE:
				stealth = dice(1, 8) + 1 + (m_player->level() / 4);
				break;

			case skill_familiarity_t::FAMILIAR:
				stealth = dice(3, 8) + 3 + (m_player->level() / 4);
				break;

			case skill_familiarity_t::MASTER:
				stealth = 100 + dice(3, 8);
				break;
		}
		mods::player_utils::change_visibility(target,- stealth);
	}
	void ghost::feign_death_done(){
		m_player->set_position(POS_STANDING);
		m_player->sendln("You stand up and dust yourself off.");
	}
	std::pair<int16_t,std::string> ghost::feign_death(){
		uint16_t duration = 0;
		switch(m_feign_death_level){
			default:
			case skill_familiarity_t::NONE:
			case skill_familiarity_t::INITIATE:
				duration = GHOST_FEIGN_DEATH_INITIATE_DURATION() + dice(3,6);
				break;
			case skill_familiarity_t::FAMILIAR:
				duration = GHOST_FEIGN_DEATH_FAMILIAR_DURATION() + dice(3,6);
				break;
			case skill_familiarity_t::MASTER:
				duration =  GHOST_FEIGN_DEATH_MASTER_DURATION() + dice(3,6);
				break;
		}
		m_player->position() = POS_DEAD;
		player_ptr_t attacker = m_player->fighting();
		if(attacker){
			feedback_t f;
			f.damage_event = damage_event_t::TARGET_DEAD_EVENT;
			f.damage = 1;
			f.hits = 1;
			f.injured = {};
			f.damage_info.emplace_back(m_player->uuid(),0,0);
			f.from_direction = NORTH;
			f.attacker = m_player->uuid();
			attacker->damage_event(f);
			stop_fighting(m_player->cd());
			stop_fighting(attacker->cd());
		}
		m_player->block_for(duration, mods::deferred::EVENT_PLAYER_FINISHES_FEIGN_DEATH, 0);
		m_player->set_position(POS_DEAD);
		return {1, MSG_YOU_FEIGN_DEATH()};
	}
	std::pair<int16_t,std::string> ghost::summon_extraction(room_rnum room){
		return {0,""};
	}
	/** requires drone assisted sniping mode */
	std::pair<int16_t,std::string> ghost::xray_shot(){

		return {0,""};
	}
	/* constructors and destructors */
	ghost::ghost(){
		m_player = nullptr;
	}
	void ghost::replenish(){
		static uint8_t call_count = 0;
		++call_count;
		bool increment_claymore = false;
		switch(m_plant_claymore_level){
			default:
			case skill_familiarity_t::NONE:
				increment_claymore = 0;
				break;
			case skill_familiarity_t::INITIATE:
				if(0 == (call_count % GHOST_PLANT_CLAYMORE_INITIATE_MOD_CALL_COUNT())){
					increment_claymore = 1;
				}
				break;
			case skill_familiarity_t::FAMILIAR:
				if(0 == (call_count % GHOST_PLANT_CLAYMORE_FAMILIAR_MOD_CALL_COUNT())){
					increment_claymore = 1;
				}
				break;
			case skill_familiarity_t::MASTER:
				if(0 == (call_count % GHOST_PLANT_CLAYMORE_MASTER_MOD_CALL_COUNT())){
					increment_claymore = 1;
				}
				break;
		}
		if(increment_claymore && m_claymore_count < GHOST_CLAYMORE_MAX_COUNT()){
			m_player->sendln("A ghost class claymore mine has been regenerated.");
			++m_claymore_count;
		}
	}
	uint8_t ghost::claymore_count() const{
		return m_claymore_count;
	}
	std::tuple<bool,std::string> ghost::plant_claymore(int direction,room_rnum room){
		if(m_claymore_count == 0){
			return {false, "You don't have any GHOST claymores!"};
		}
		--m_claymore_count;
		m_claymores.emplace_back(std::move(create_object(ITEM_EXPLOSIVE, "claymore.yml")));
		mods::demolitions::plant_claymore(m_player,direction,m_claymores.back());
		return {1,CAT("You begin planting a {grn}",m_claymores.back()->name,"{/grn}...")};
	}
	std::vector<uuid_t> ghost::get_targets_scanned_by_drone(){
		std::vector<uuid_t> scanned;

		return scanned;
	}
	std::shared_ptr<ghost> create_ghost(player_ptr_t &in_player){
		return std::move(std::make_shared<ghost>(in_player));
	}
	std::vector<uuid_t> ghost::get_scanned() const {
		return m_scanned;
	}
	void ghost::set_scanned(std::vector<uuid_t> s){
		m_scanned = s;
	}
	std::tuple<uint32_t,std::string> ghost::fire_penetrating_shot_at(uuid_t npc_uuid){
		uint32_t damage = 0;
		std::string msg = "";
		return {damage,msg};
	}
	std::tuple<bool,std::string> ghost::intimidate_target(uuid_t npc_uuid){
		bool worked = false;
		std::string msg = "";
		return {worked,msg};
	}
	uint8_t ghost::cryogenic_grenade_count() const {
		return m_cryogenic_grenade_count;
	}
	std::tuple<bool,std::string> ghost::toss_cryogenic_grenade_towards(uint8_t direction, uint8_t rooms){
		bool has_nades = !!m_cryogenic_grenade_count;
		std::string msg = "";
		return {has_nades,msg};
	}
	/** applies it to the entire room. every will get flashed */
	std::tuple<bool,std::string> ghost::use_flash_underbarrel(){
		bool has_charge = !!m_flash_underbarrel_charges;
		std::string msg = "";
		return {has_charge,msg};
	}
	std::string ghost::get_proficiency_by_name(std::string_view prof) const {
		if(!prof.compare("drone_scan")){ return mods::util::proficiency_to_string((int)m_drone_scan_level); }
		if(!prof.compare("stealth")){ return mods::util::proficiency_to_string((int)m_stealth_level); }
		if(!prof.compare("summon_extraction")){ return mods::util::proficiency_to_string((int)m_summon_extraction_level); }
		if(!prof.compare("xray_shot")){ return mods::util::proficiency_to_string((int)m_xray_shot_level); }
		if(!prof.compare("feign_death")){ return mods::util::proficiency_to_string((int)m_feign_death_level); }
		if(!prof.compare("plant_claymore")){ return mods::util::proficiency_to_string((int)m_plant_claymore_level); }
		if(!prof.compare("penetrating_shot")){ return mods::util::proficiency_to_string((int)m_penetrating_shot_level); }
		if(!prof.compare("intimidation")){ return mods::util::proficiency_to_string((int)m_intimidation_level); }
		if(!prof.compare("cryogenic_grenade")){ return mods::util::proficiency_to_string((int)m_cryogenic_grenade_level); }
		if(!prof.compare("flash_underbarrel")){ return mods::util::proficiency_to_string((int)m_flash_underbarrel_level); }
		return "UNKNOWN SKILL";
	}
	std::string ghost::skill_screen() const {
		std::string screen;
		for(auto str : {"drone_scan", "stealth", "summon_extraction",
		"xray_shot", "feign_death", "plant_claymore",
		"penetrating_shot", "intimidation", "cryogenic_grenade",
		"flash_underbarrel"}){
			screen += CAT("{yel}",str,"{/yel}\t->\t",get_proficiency_by_name(str),"\r\n");
		}
		return screen;
	}
};
ACMD(do_penetrating_shot){
	PLAYER_CAN("ghost.penetrating_shot");
	DO_HELP("penetrating_shot");
}
ACMD(do_intimidate) {
	PLAYER_CAN("ghost.intimidate");
	DO_HELP("intimidate");
}
ACMD(do_toss_cryogenic_grenade) {
	PLAYER_CAN("ghost.toss_cryogenic_grenade");
	DO_HELP("toss_cryogenic_grenade");
}
ACMD(do_use_flash_underbarrel) {
	PLAYER_CAN("ghost.use_flash_underbarrel");
	DO_HELP("use_flash_underbarrel");
}
ACMD(do_go_dark){
	PLAYER_CAN("ghost.go_dark");
	DO_HELP("go_dark");
}
ACMD(do_conceal){
	PLAYER_CAN("ghost.conceal");
	DO_HELP("conceal");

}
ACMD(do_feign_death){
	PLAYER_CAN("ghost.feign_death");
	DO_HELP("feign_death");
	auto msg = std::get<1>(player->ghost()->feign_death());
	player->sendln(msg);
}
ACMD(do_summon_extraction){
	PLAYER_CAN("ghost.summon_extraction");
	DO_HELP("summon_extraction");

}
ACMD(do_xray_shot){
	PLAYER_CAN("ghost.xray_shot");
	DO_HELP("xray_shot");
	auto weapon = player->primary();
	if(!weapon || weapon->rifle()->attributes->type != mw_rifle::SNIPER){
		player->sendln("You must be wielding a sniper rifle!");
		return;
	}
	int distance = weapon->rifle()->attributes->critical_range;
	int direction = NORTH;
	for(auto scanned : player->ghost()->get_targets_scanned_by_drone()){
		auto victim = ptr_by_uuid(scanned);
		if(!victim){
			continue;
		}
		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
			player,
			weapon,
			victim,
			distance,
			direction
		);
	}
}
namespace mods::class_abilities {
	static constexpr const char* plant_claymore_usage = "usage: plant_claymore <direction>";
	ACMD(do_plant_claymore){
		PLAYER_CAN("ghost.plant_claymore");
		DO_HELP_WITH_ZERO("ghost.plant_claymore");
		auto vec_args = PARSE_ARGS();
		int dir = mods::util::parse_direction(vec_args[0]);
		if(dir < 0){
			player->sendln("Use a valid direction");
			return;
		}
		auto status = player->ghost()->plant_claymore(dir,player->room());
		if(!std::get<0>(status)){
			player->errorln(std::get<1>(status));
			return;
		}
		player->sendln(std::get<1>(status));
	}
	void init(){
		mods::interpreter::add_command("plant_claymore", POS_RESTING, do_plant_claymore, 0,0);
		mods::interpreter::add_command("penetrating_shot", POS_RESTING, do_penetrating_shot, 0,0);
		mods::interpreter::add_command("intimidate", POS_RESTING, do_intimidate, 0,0);
		mods::interpreter::add_command("toss_cryogenic_grenade", POS_RESTING, do_toss_cryogenic_grenade, 0,0);
		mods::interpreter::add_command("use_flash_underbarrel", POS_RESTING, do_use_flash_underbarrel, 0,0);
		mods::help::register_help_command_with_permission("ghost.plant_claymore",plant_claymore_usage,LVL_MORTAL);
	}
};
