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
	extern int16_t flush_player(player_ptr_t& player);
};
namespace mods::classes {
	void ghost_advance_level(player_ptr_t& player) {
		/** TODO fill me in */
		player->send("[stub] file:%s line:%d\r\n",__FILE__,__LINE__);
	}
	void ghost::init() {
		m_scanned.clear();
		m_player = nullptr;
		m_dissipate_charges = 10;
		m_dissipated = 0;
		using skillset_t = ability_data_t::skillset_t;
		m_abilities = {
			{AERIAL_DRONE_SCAN,"ads","Aerial Drone Scan",skillset_t::ELECTRONICS,&m_drone_scan},
			{STEALTH,"stealth","stealth",skillset_t::INTELLIGENCE,&m_stealth},
			{SUMMON_EXTRACTION,"summon","Summon Extraction", skillset_t::STRATEGY,&m_summon_extraction},
			{XRAY_SHOT,"xray","X-Ray Shot",skillset_t::SNIPING,&m_xray_shot},
			{FEIGN_DEATH,"feign","Feign Death",skillset_t::STRATEGY,&m_feign_death},
			{PLANT_CLAYMORE,"claymore","Plant Claymore",skillset_t::DEMOLITIONS,&m_plant_claymore},
			{INTIMIDATION,"intimidation","Intimidation",skillset_t::INTELLIGENCE,&m_intimidation},
			{CRYOGENIC_GRENADE,"cryo","Cryogenic Grenade",skillset_t::DEMOLITIONS,&m_cryogenic_grenade},
			{FLASH_UNDERBARREL,"flash","Flash Underbarrel",skillset_t::WEAPON_HANDLING,&m_flash_underbarrel},
		};
	}
	int16_t ghost::save() {
		return this->m_orm.save();
	}
	ghost::ghost(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}
	bool ghost::is_dissipated() const {
		return m_dissipated;
	}
	void ghost::set_player(player_ptr_t p) {
		m_player = p;
	}
	int16_t ghost::new_player(player_ptr_t& player) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t ghost::load_by_player(player_ptr_t& player) {
		set_player(player);
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("ghost::load_by_player unable to load ghost class by player id: ",(player->db_id()),".. return status: ",(result)));
			return result;
		}
		initialize_skills_for_player(m_player);
		return result;
	}
	void ghost::apply_stealth_to(obj_ptr_t& target) {
		int stealth = 0;
		if(m_stealth.not_learned()) {
			m_player->sendln("It looks like you still need to train that skill");
			return;
		}
		if(m_stealth.awful() || m_stealth.terrible() || m_stealth.okay()) {
			stealth = dice(1, 8) + 1 + (m_player->level() / 4);
		}
		if(m_stealth.learned()) {
			stealth = dice(3, 8) + 3 + (m_player->level() / 4);
		}

		if(m_stealth.mastered() || m_stealth.elite()) {
			stealth = 100 + dice(3, 8);
		}
		mods::object_utils::change_visibility(target,- stealth);
	}
	void ghost::apply_stealth_to_player(player_ptr_t& target) {
		int stealth = 0;
		if(m_stealth.not_learned()) {
			m_player->sendln("It looks like you still need to train that skill");
			return;
		}
		if(m_stealth.awful() || m_stealth.terrible() || m_stealth.okay()) {
			stealth = dice(1, 8) + 1 + (m_player->level() / 4);
		}

		if(m_stealth.learned()) {
			stealth = dice(3, 8) + 3 + (m_player->level() / 4);
		}

		if(m_stealth.mastered() || m_stealth.elite()) {
			stealth = 100 + dice(3, 8);
		}
		mods::player_utils::change_visibility(target,- stealth);
	}
	void ghost::feign_death_done() {
		m_player->set_position(POS_STANDING);
		m_player->sendln("You stand up and dust yourself off.");
	}
	std::pair<int16_t,std::string> ghost::feign_death() {
		uint16_t duration = 0;
		if(m_feign_death.not_learned()) {
			m_player->sendln("It looks like you still need to train that skill");
			return {0, "It looks like you still need to train that skill"};
		}
		if(m_feign_death.awful() || m_feign_death.terrible() || m_feign_death.okay()) {
			duration = GHOST_FEIGN_DEATH_INITIATE_DURATION() + dice(3,6);
		}
		if(m_feign_death.learned()) {
			duration = GHOST_FEIGN_DEATH_FAMILIAR_DURATION() + dice(3,6);
		}
		if(m_feign_death.mastered() || m_feign_death.elite()) {
			duration =  GHOST_FEIGN_DEATH_MASTER_DURATION() + dice(3,6);
		}
		m_player->position() = POS_DEAD;
		player_ptr_t attacker = m_player->fighting();
		if(attacker) {
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
	std::pair<int16_t,std::string> ghost::summon_extraction(room_rnum room) {
		return {0,""};
	}
	/** requires drone assisted sniping mode */
	std::pair<int16_t,std::string> ghost::xray_shot() {

		return {0,""};
	}
	/* constructors and destructors */
	ghost::ghost() {
		m_player = nullptr;
	}
	std::tuple<bool,std::string> ghost::dissipate() {
		if(m_dissipate_charges == 0) {
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
	void ghost::dissipate_wears_off() {
		m_player->visibility() = char_data::STARTING_VISIBILITY;
		m_player->sendln("Your dissipation invisibility wears off...");
#ifdef __MENTOC_SHOW_GHOST_TIME_WHEN_DISSIPATE_WEARS_OFF__
		m_player->send("%s\r\n",mods::date_time::irl::now().c_str());
#endif
		m_dissipated = false;
	}
	void ghost::use_claymore(uuid_t object_uuid) {
	}
	void ghost::replenish() {
		static uint16_t call_count = 0;
		++call_count;
		auto tier = tier(m_player);
		if((call_count % GHOST_REPLENISH_PULSE()) == 0) {
			if(m_claymore_count < GHOST_CLAYMORE_MAX_COUNT() * tier) {
				++m_claymore_count;
				m_player->sendln("{grn}A ghost class claymore mine has been regenerated.{/grn}");
			}
			if(m_dissipate_charges < GHOST_DISSIPATE_CHARGE_MAX_COUNT() * tier) {
				m_player->sendln("{grn}A ghost dissipate charge has been regenerated.{/grn}");
				++m_dissipate_charges;
			}
#ifdef __MENTOC_SEND_GHOST_PLAYER_REPLENISH_DEBUG_MESSAGE__
			m_player->sendln("Replenish");
#endif
		}
	}
	uint8_t ghost::claymore_count() const {
		return m_claymore_count;
	}
	std::vector<uuid_t> ghost::get_targets_scanned_by_drone() {
		std::vector<uuid_t> scanned;

		return scanned;
	}
	std::shared_ptr<ghost> create_ghost(player_ptr_t& in_player) {
		return std::move(std::make_shared<ghost>(in_player));
	}
	std::vector<uuid_t> ghost::get_scanned() const {
		return m_scanned;
	}
	void ghost::set_scanned(std::vector<uuid_t> s) {
		m_scanned = s;
	}
	std::tuple<uint32_t,std::string> ghost::fire_penetrating_shot_at(uuid_t npc_uuid) {
		uint32_t damage = 0;
		std::string msg = "";
		return {damage,msg};
	}
	std::tuple<bool,std::string> ghost::intimidate_target(uuid_t npc_uuid) {
		bool worked = false;
		std::string msg = "";
		return {worked,msg};
	}
	uint8_t ghost::cryogenic_grenade_count() const {
		return m_cryogenic_grenade_count;
	}
	std::tuple<bool,std::string> ghost::toss_cryogenic_grenade_towards(uint8_t direction, uint8_t rooms) {
		bool has_nades = !!m_cryogenic_grenade_count;
		std::string msg = "";
		return {has_nades,msg};
	}
	/** applies it to the entire room. every will get flashed */
	std::tuple<bool,std::string> ghost::use_flash_underbarrel() {
		bool has_charge = !!m_flash_underbarrel_charges;
		std::string msg = "";
		return {has_charge,msg};
	}
	/*
	std::string ghost::skills_page() {
		std::string how_to = "How to practice each skill:\r\n";
		for(const auto& pair : user_friendly_abilites_map()) {
			how_to += CAT("example: practice ",pair.first,"\r\n");
		}
		std::string page = CAT("Sniping:{hr}",
		                       "Penetrating Shot{->}[ ",penetrating_shot_proficiency()," ]\r\n",

		                       "{hr}\r\n",
		                       "Demolitions:{hr}",
		                       "Cryogenic Grenade{->}[ ",cryogenic_grenade_proficiency()," ]\r\n",
		                       "Plant Claymore{->}[ ",plant_claymore_proficiency()," ]\r\n",

		                       "{hr}\r\n",
		                       "Technology:{hr}",
		                       "Drone Scan:{->}[ ",drone_scan_proficiency(), " ]\r\n",
		                       "Stealth:{->}[ ",stealth_proficiency(), " ]\r\n",
		                       "Summon Extraction{->}[ ",summon_extraction_proficiency()," ]\r\n",
		                       "X-Ray Shot{->}[ ",xray_shot_proficiency(), " ]\r\n",

		                       "{hr}\r\n",
		                       "Strategy:{hr}",
		                       "Feign Death:{->}[ ",feign_death_proficiency(), " ]\r\n",
		                       "Intimidation:{->}[ ",intimidation_proficiency(), " ]\r\n",
		                       "{hr}\r\n",
		                       how_to
		                      );
		return mods::util::mail_format("Your skills","Proficiencies",page,m_player->screen_width());
	}
	*/

};
namespace mods::class_abilities::ghost {
	static constexpr const char* dissipate_usage = "usage: dissipate";
	ACMD(do_dissipate) {
		PLAYER_CAN("ghost.dissipate");
		DO_HELP_WITH_ZERO("ghost.dissipate");
		auto status = player->ghost()->dissipate();
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
		player->sendln(std::get<1>(status));
	}
	void init() {
		mods::interpreter::add_command("dissipate", POS_RESTING, do_dissipate, 0,0);
	}
};
