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
#include "../date-time.hpp"

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
		m_claymore_instances.clear();
		m_scanned.clear();
		m_player = nullptr;
		m_dissipate_charges = 10;
		m_dissipated = 0;
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
	bool ghost::is_dissipated() const {
		return m_dissipated;
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
		/** TODO: create catchy name using the deep object parser */
		auto primary = create_object(ITEM_RIFLE,"g36c.yml");
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"ppk.yml"),WEAR_SECONDARY);
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
	std::tuple<bool,std::string> ghost::dissipate(){
		if(m_dissipate_charges == 0){
			return {false,"{red}You don't have any dissipate charges{/red}"};
		}
		--m_dissipate_charges;
		m_player->visibility() = 0;
		uint32_t ticks = GHOST_DISSIPATE_TICKS_DURATION() * tier(m_player);
		mods::globals::defer_queue->push_ticks_event(ticks,m_player->uuid(),mods::deferred::EVENT_PLAYER_GOES_VISIBLE);
#ifdef __MENTOC_SHOW_GHOST_TIME_WHEN_DISSIPATE_WEARS_OFF__
		m_player->send("%s tick count: (%d)\r\n",mods::date_time::irl::now().c_str(),ticks);
#endif
		m_dissipated = true;
		return {true,"{grn}You dissipate into nothing...{/grn}"};
	}
	void ghost::dissipate_wears_off(){
		m_player->visibility() = char_data::STARTING_VISIBILITY;
		m_player->sendln("Your dissipation invisibility wears off...");
#ifdef __MENTOC_SHOW_GHOST_TIME_WHEN_DISSIPATE_WEARS_OFF__
		m_player->send("%s\r\n",mods::date_time::irl::now().c_str());
#endif
		m_dissipated = false;
	}
	void ghost::use_claymore(uuid_t object_uuid){
		auto it = std::find(m_claymore_instances.begin(),m_claymore_instances.end(),object_uuid);
		if(it != m_claymore_instances.end()){
			m_claymore_instances.erase(it);
		}
	}
	void ghost::replenish(){
		static uint16_t call_count = 0;
		++call_count;
		auto tier = tier(m_player);
		if((call_count % GHOST_REPLENISH_PULSE()) == 0){
			if(m_claymore_instances.size() < GHOST_CLAYMORE_MAX_COUNT() * tier){
				auto fatal = create_object(ITEM_EXPLOSIVE,"claymore-mine.yml");
				m_claymore_instances.emplace_back(fatal->uuid);
				m_player->carry(fatal);
				m_player->sendln("{grn}A ghost class claymore mine has been regenerated.{/grn}");
			}
			if(m_dissipate_charges < GHOST_DISSIPATE_CHARGE_MAX_COUNT() * tier){
				m_player->sendln("{grn}A ghost dissipate charge has been regenerated.{/grn}");
				++m_dissipate_charges;
			}
			#ifdef __MENTOC_SEND_GHOST_PLAYER_REPLENISH_DEBUG_MESSAGE__
			m_player->sendln("Replenish");
			#endif
		}
	}
	uint8_t ghost::claymore_count() const{
		return m_claymore_instances.size();
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
namespace mods::class_abilities::ghost {
	static constexpr const char* dissipate_usage = "usage: dissipate";
	ACMD(do_dissipate){
		PLAYER_CAN("ghost.dissipate");
		DO_HELP_WITH_ZERO("ghost.dissipate");
		auto status = player->ghost()->dissipate();
		if(!std::get<0>(status)){
			player->errorln(std::get<1>(status));
			return;
		}
		player->sendln(std::get<1>(status));
	}
	void init(){
		mods::interpreter::add_command("dissipate", POS_RESTING, do_dissipate, 0,0);
	}
};
