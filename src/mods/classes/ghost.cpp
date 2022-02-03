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
#include "../weapons/corrosive-claymore.hpp"
#include "../weapons/shrapnel-claymore.hpp"
#include "../skills.hpp"
#include "../date-time.hpp"
#include "../help.hpp"
#include "../examine.hpp"
#include "../levels.hpp"
#include "../medic.hpp"

namespace mods::rand {
	extern int roll(int num,int size);
};

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t& player);
};
namespace mods::classes {
	struct shorthand {
		static constexpr std::string_view LIGHT_BANDAGE = "lb";
	};
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
		m_abilities = create_abilities({
			{AERIAL_DRONE_SCAN,"ads","Aerial Drone Scan",skillset_t::ELECTRONICS,&m_drone_scan},
			{STEALTH,"stealth","stealth",skillset_t::INTELLIGENCE,&m_stealth},
			{SUMMON_EXTRACTION,"summon","Summon Extraction", skillset_t::STRATEGY,&m_summon_extraction},
			{XRAY_SHOT,"xray","X-Ray Shot",skillset_t::SNIPING,&m_xray_shot},
			{FEIGN_DEATH,"feign","Feign Death",skillset_t::STRATEGY,&m_feign_death},
			{PLANT_CLAYMORE,"claymore","Plant Claymore",skillset_t::DEMOLITIONS,&m_plant_claymore},
			{INTIMIDATION,"intimidation","Intimidation",skillset_t::INTELLIGENCE,&m_intimidation},
			{CRYOGENIC_GRENADE,"cryo","Cryogenic Grenade",skillset_t::DEMOLITIONS,&m_cryogenic_grenade},
			{FLASH_UNDERBARREL,"flash","Flash Underbarrel",skillset_t::WEAPON_HANDLING,&m_flash_underbarrel},
			{TRACKING_SHOT,"ts","Tracking Shot",skillset_t::SNIPING,&m_tracking_shot},
			{LIGHT_BANDAGE,shorthand::LIGHT_BANDAGE.data(),"Light Bandage",skillset_t::MEDICAL,&m_light_bandage},
			{SUTURE,"suture","Suture",skillset_t::MEDICAL,&m_suture},
			{ADRENALINE_SHOT,"as","Adrenaline Shot",skillset_t::MEDICAL,&m_adrenaline_shot},
			{EMP_NADE,"emp","EMP Grenade",skillset_t::DEMOLITIONS,&m_emp_nade},
			{CHAFF_NADE,"chaff","Chaff Grenade",skillset_t::DEMOLITIONS,&m_chaff_nade},
			{SENSOR_NADE,"sensor","Sensor Grenade",skillset_t::INTELLIGENCE,&m_sensor_nade},
			{UB_SHOTGUN,"ubs","Underbarrel Shotgun",skillset_t::DEMOLITIONS,&m_ub_shotgun},
			{UB_FRAG,"ubf","Underbarrel Nade Launcher",skillset_t::DEMOLITIONS,&m_ub_frag},
			{GUIDED_MISSILE,"gm","Guided Missile",skillset_t::DEMOLITIONS,&m_guided_missile},
			{TARGET_LIMB,"limb","Target Limb",skillset_t::SNIPING,&m_target_limb},
			{PLANT_CLAYMORE,"claymore","Plant Claymore",skillset_t::DEMOLITIONS,&m_plant_claymore},
			{SHRAPNEL_CLAYMORE,"smine","Shrapnel Claymore",skillset_t::DEMOLITIONS,&m_plant_shrapnel_claymore},
			{CORROSIVE_CLAYMORE,"cmine","Corrosive Claymore",skillset_t::DEMOLITIONS,&m_plant_corrosive_claymore},
			{REQUEST_RECON,"recon","Request Recon",skillset_t::INTELLIGENCE,&m_request_recon},
		});
		/**
		 * REQUIRED
		 */
		m_adrenaline_shot_charges = 0;
		m_target = 0;
		m_engaged = 0;
		m_xray_shot_charges = 0;
		m_claymore_count = 0;
		m_tracking_shot_charges = 0;
		m_preferences["mute-replenish"] = true;
		m_gauze_count = 0;
		m_medkit_count = 0;
	}
	int16_t ghost::save() {
		return this->m_orm.save();
	}
	void ghost::replenish_notify(std::string_view v) {
		m_player->sendln(v.data());
	}
	ghost::ghost(player_ptr_t p) {
		this->init();
		load_by_player(p);
		m_shotgun_ub.init();
		m_frag_ub.init();
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
		/**
		 * TODO: if the weight of gear and equipment is low, then stealth will last longer
		 */
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
		auto s = roll_skill_success(FEIGN_DEATH);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
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
	/* constructors and destructors */
	ghost::ghost() {
		m_player = nullptr;
	}
	std::tuple<bool,std::string> ghost::dissipate() {
		if(m_dissipate_charges == 0) {
			return {false,"{red}You don't have any dissipate charges{/red}"};
		}
		auto s = roll_skill_success(STEALTH);
		if(!std::get<0>(s)) {
			return s;
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
		mods::weapons::corrosive_claymore_installed(object_uuid);
	}
	void ghost::replenish() {
		static uint16_t call_count = 0;
		++call_count;
		auto tier = tier(m_player);
		if((call_count % GHOST_REPLENISH_PULSE()) == 0) {
			if(m_claymore_count < SNIPER_CLAYMORE_MAX_COUNT() * tier) {
				++m_claymore_count;
			}
			if(m_corrosive_claymore_count < SNIPER_CLAYMORE_MAX_COUNT() * tier) {
				++m_corrosive_claymore_count;
			}
			if(m_shrapnel_claymore_count < SNIPER_CLAYMORE_MAX_COUNT() * tier) {
				++m_shrapnel_claymore_count;
			}
			if(m_xray_shot_charges < SNIPER_XRAY_SHOT_MAX_COUNT() * tier) {
				++m_xray_shot_charges;
			}
			if(m_tracking_shot_charges < SNIPER_TRACKING_SHOT_MAX_COUNT() * tier) {
				++m_tracking_shot_charges;
			}
			if(m_gauze_count < SNIPER_GAUZE_MAX_COUNT() * tier) {
				++m_gauze_count;
			}
			if(m_adrenaline_shot_charges < SNIPER_ADRENALINE_SHOT_MAX_COUNT() * tier) {
				++m_adrenaline_shot_charges;
			}
			if(m_dissipate_charges < GHOST_DISSIPATE_CHARGE_MAX_COUNT() * tier) {
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

	std::tuple<bool,std::string> ghost::inject_adrenaline_shot() {
		if(m_ad_shot.active()) {
			return {0,"You already have an adrenaline shot active!"};
		}
		auto s = roll_skill_success(ADRENALINE_SHOT);
		if(std::get<0>(s)) {
			m_adrenaline_shot.use_skill(m_player);
			return m_ad_shot.inject(m_player);
		}
		return s;
	}
	void ghost::unblock_adrenaline_shot() {
		m_ad_shot.shot_wears_off(m_player);
	}
	obj_ptr_t ghost::underbarrel() {
		if(m_shotgun_ub.is_attached()) {
			return m_shotgun_ub.obj();
		}
		if(m_frag_ub.is_attached()) {
			return m_frag_ub.obj();
		}
		return nullptr;
	}

	std::tuple<bool,std::string> ghost::detach_shotgun_underbarrel() {
		return m_shotgun_ub.detach();
	}

	std::tuple<bool,std::string> ghost::attach_shotgun_underbarrel() {
		if(!m_player) {
			return {0,"No player pointer set!"};
		}
		if(!m_player->primary()) {
			return {0,"You aren't wielding a primary weapon!"};
		}
		if(!m_player->primary()->has_rifle()) {
			return {0,"You aren't wielding a rifle!"};
		}
		if(m_shotgun_ub.is_attached() && m_shotgun_ub.ammo()) {
			return {1,"Already attached. To detach, use 'detach_shotgun_underbarrel'."};
		}
		auto s = roll_skill_success(UB_SHOTGUN);
		if(std::get<0>(s)) {
			m_ub_shotgun.use_skill(m_player);
			return m_shotgun_ub.attach_to(m_player->primary(),tier(m_player));
		}
		return s;
	}
	ghost::shotgun_ub_t& ghost::get_shotgun_underbarrel_wrapper() {
		return m_shotgun_ub;
	}
	ghost::frag_ub_t& ghost::get_frag_underbarrel_wrapper() {
		return m_frag_ub;
	}
	void ghost::unblock_healing() {
		m_player->sendln("Unblock healing");
		if(m_heal_mode == HEAL_MODE_SUTURE) {
			m_suture.use_skill(m_player);
		}
	}
	std::tuple<bool,std::string> ghost::suture() {
		auto s = roll_skill_success(SUTURE);
		if(!std::get<0>(s)) {
			return s;
		}
		static const std::array<uint8_t,3> suture_tier_duration = {
			SUTURE_TIER_ONE_TICKS(),
			SUTURE_TIER_TWO_TICKS(),
			SUTURE_TIER_THREE_TICKS()
		};
		std::size_t tier = mods::levels::player_tier(m_player) - 1;
		std::size_t index = std::clamp(tier,(std::size_t)0,(std::size_t)2);
		m_heal_mode = HEAL_MODE_SUTURE;
		s = mods::medic::set_is_suturing(m_player,suture_tier_duration[index]);
		if(!std::get<0>(s)) {
			return s;
		}
		--m_medkit_count;
		return s;
	}
	void ghost::consume_shotgun_underbarrel_ammo() {
		m_shotgun_ub.consume_ammo();
	}
	std::tuple<bool,std::string,obj_ptr_t> ghost::build_claymore() {
		if(m_claymore_count == 0) {
			return {false,"You don't have any claymore charges!",nullptr};
		}
		--m_claymore_count;
		return {true,"A claymore charge is built",create_object(ITEM_EXPLOSIVE,"claymore-mine.yml")};
	}
	std::tuple<bool,std::string,obj_ptr_t> ghost::build_corrosive_claymore() {
		if(m_corrosive_claymore_count == 0) {
			return {false,"You don't have any claymore charges!",nullptr};
		}
		auto s = roll_skill_success(CORROSIVE_CLAYMORE);
		if(!std::get<0>(s)) {
			return {std::get<0>(s),std::get<1>(s),nullptr};
		}
		--m_corrosive_claymore_count;
		m_plant_corrosive_claymore.use_skill(m_player);
		return {true,"A {grn}corrosive{/grn} claymore charge is built",mods::weapons::corrosive_claymore::create(m_player)};
	}

	std::tuple<bool,std::string,obj_ptr_t> ghost::build_shrapnel_claymore() {
		if(m_shrapnel_claymore_count == 0) {
			return {false,"You don't have any claymore charges!",nullptr};
		}
		auto s = roll_skill_success(SHRAPNEL_CLAYMORE);
		if(!std::get<0>(s)) {
			return {std::get<0>(s),std::get<1>(s),nullptr};
		}
		--m_shrapnel_claymore_count;
		m_plant_shrapnel_claymore.use_skill(m_player);
		return {true,"A {yel}shrapnel{/yel} claymore charge is built",create_object(ITEM_EXPLOSIVE,"shrapnel-claymore-mine.yml")};
	}


	/*
	- Ability: X-Ray Shot (can shoot through multiple layers of walls/doors)
		- Can snipe an enemy within a building
		- Can snipe an enemy through walls or doors
		- If enemy not behind cover, it causes 150% damage to target
		*/
	std::tuple<bool,std::string> ghost::xray_shot() {
		auto weapon = m_player->primary();
		if(!weapon || weapon->has_rifle() == false || weapon->rifle()->attributes->type != mw_rifle::SNIPER) {
			return {0,"You must be wielding a sniper rifle!"};
		}
		if(!m_engaged) {
			return {0,"You must first use the 'engage' command"};
		}
		int distance = weapon->rifle()->attributes->critical_range;
		int direction = NORTH;
		if(m_target == 0) {
			return {0,"You have not marked anyone"};
		}
		auto victim = ptr_by_uuid(m_target);
		if(!victim || victim->visibility() == 0) {
			return {0,"Cannot find your victim!"};
		}
		auto s = roll_skill_success(XRAY_SHOT);
		if(!std::get<0>(s)) {
			return s;
		}
		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
		                    m_player,
		                    weapon,
		                    victim,
		                    distance,
		                    direction
		                );
		m_xray_shot.use_skill(m_player);
		return {1,"stub"};
	}
	void ghost::target_died(uuid_t target) {
		m_target = 0;
		m_engaged = 0;
		m_player->sendln("Your target died.");
	}
	std::tuple<bool,std::string> ghost::mark_target(std::string_view target) {
		m_target = mods::examine::find_player_by_name(m_player, target);
		if(m_target == 0) {
			return {0,"Couldn't find a target that matches that string."};
		}
		//auto s = roll_skill_success(MARK_TARGET);
		//if(!std::get<0>(s)) {
		//	return s;
		//}
		/** TODO: need to add mark target as a skill */
		return {1,"Marked target"};
	}

	std::tuple<bool,std::string> ghost::engage() {
		if(m_target == 0) {
			return {0,"You have not marked a target yet! See ghost:mark"};
		}
		auto ptr = ptr_by_uuid(m_target);
		if(!ptr) {
			m_target = 0;
			return {0,"That target doesn't exist anymore."};
		}
		m_engaged = 1;
		return {1,CAT("Engaged target: '",ptr->name().c_str(),"'")};
	}
	std::tuple<bool,std::string> ghost::disengage() {
		m_target = 0;
		m_engaged = 0;
		return {1,"You disengage with your target."};
	}

	std::tuple<bool,std::string> ghost::tracking_shot(std::string_view target, direction_t direction) {
		if(m_tracking_shot_charges) {
			int depth = 6;
			mods::scan::vec_player_data data;
			mods::scan::los_scan_direction(m_player->cd(),depth,&data,direction);
			for(const auto& result : data) {
				if(mods::util::fuzzy_match(target.data(),result.ch->player.name.c_str())) {
					auto victim = ptr(result.ch);
					if(!victim) {
						return {false,"You could not locate your victim! Are they dead?"};
					}
					auto s = roll_skill_success(TRACKING_SHOT);
					--m_tracking_shot_charges;
					if(!std::get<0>(s)) {
						return s;
					}
					feedback_t feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
					                          m_player,
					                          m_player->primary(),
					                          victim,
					                          result.distance,
					                          direction
					                      );

					if(feedback.hits == 0) {
						return {false,"You missed your target!"};
					}

					mods::affects::affect_player_for(mods::affects::to_affect({"tracked"}),ptr(result.ch),SKILL_SNIPER_TRACKING_SHOT_TICKS());
					m_tracking_shot.use_skill(m_player);
					return {1,"You tag your target!"};
				}
			}
		}
		return {0,"You couldn't tag your target!"};
	}
	std::tuple<bool,std::string> ghost::light_bandage() {
		if(m_gauze_count) {
			auto s = roll_skill_success(LIGHT_BANDAGE);
			--m_gauze_count;
			if(std::get<0>(s)) {
				m_heal_mode = HEAL_MODE_SUTURE;
				auto player_tier = tier(m_player);
				m_player->hp() += player_tier * SNIPER_GAUZE_MULTIPLIER() * (m_player->level() * 0.3178) * (rand_number(1,185) * 0.112);
				m_light_bandage.use_skill(m_player);
				return {1,"You bandage yourself"};
			}
			return {0,std::get<1>(s)};
		}
		return {0,"You don't have any gauze!"};
	}
	std::tuple<bool,std::string> ghost::attach_frag_underbarrel() {
		if(m_frag_ub.is_attached() && m_frag_ub.ammo()) {
			return {1,"Already attached. To detach, use 'detach_frag_underbarrel'."};
		}
		auto s = roll_skill_success(UB_FRAG);
		if(std::get<0>(s)) {
			m_ub_frag.use_skill(m_player);
			return m_frag_ub.attach_to(m_player->primary(),tier(m_player));
		}
		return s;
	}
	std::tuple<bool,std::string> ghost::detach_frag_underbarrel() {
		return m_frag_ub.detach();
	}
	std::tuple<bool,std::string> ghost::fire_frag(const direction_t& direction,const uint8_t& distance) {
		return m_frag_ub.fire(m_player,direction,distance);
	}
};

namespace mods::class_abilities::ghost {
	ACMD(do_light_bandage) {
		PLAYER_CAN("ghost.light_bandage");
		DO_HELP("light_bandage");
		auto status = player->ghost()->light_bandage();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_build_corrosive_claymore) {
		PLAYER_CAN("ghost.build_corrosive_claymore");
		DO_HELP("build_claymore");
		auto status = player->ghost()->build_corrosive_claymore();
		if(std::get<0>(status)) {
			player->carry(std::get<2>(status));
		}
		player->sendln(std::get<1>(status));
	};
	ACMD(do_build_shrapnel_claymore) {
		PLAYER_CAN("ghost.build_shrapnel_claymore");
		DO_HELP("build_claymore");
		auto status = player->ghost()->build_shrapnel_claymore();
		if(std::get<0>(status)) {
			player->carry(std::get<2>(status));
		}
		player->sendln(std::get<1>(status));
	};
	ACMD(do_build_claymore) {
		PLAYER_CAN("ghost.build_claymore");
		DO_HELP("build_claymore");
		auto status = player->ghost()->build_claymore();
		if(std::get<0>(status)) {
			player->carry(std::get<2>(status));
		}
		player->sendln(std::get<1>(status));
	};
	ACMD(do_xray_shot) {
		PLAYER_CAN("ghost.xray_shot");
		DO_HELP("xray_shot");
		auto status = player->ghost()->xray_shot();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_mark_target) {
		PLAYER_CAN("ghost.mark_target");
		DO_HELP("mark_target");
		static constexpr const char* usage = "Usage: mark_target <target>";
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->sendln(usage);
			return;
		}
		auto status = player->ghost()->mark_target(vec_args[0]);
		player->sendln(std::get<1>(status));
	};
	ACMD(do_tracking_shot) {
		PLAYER_CAN("ghost.tracking_shot");
		DO_HELP("tracking_shot");
		static constexpr const char* usage = "Usage: tracking_shot <target> <direction>";
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}
		direction_t dir = mods::util::parse_direction(vec_args[1]);
		auto status = player->ghost()->tracking_shot(vec_args[0],dir);
		player->sendln(std::get<1>(status));
	};
	ACMD(do_engage) {
		PLAYER_CAN("ghost.engage");
		DO_HELP("engage");
		auto status = player->ghost()->engage();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_disengage) {
		PLAYER_CAN("ghost.disengage");
		DO_HELP("disengage");
		auto status = player->ghost()->disengage();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_attach_shotgun_underbarrel) {
		PLAYER_CAN("ghost.attach_shotgun_underbarrel");
		DO_HELP("attach_shotgun_underbarrel");
		if(!player->primary()) {
			player->sendln("You are not wielding a weapon!");
			return;
		}
		if(!player->primary()->has_rifle()) {
			player->sendln("You must be wielding a rifle of some sort!");
			return;
		}
		if(player->ghost()) {
			auto status = player->ghost()->attach_shotgun_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_detach_shotgun_underbarrel) {
		PLAYER_CAN("ghost.detach_shotgun_underbarrel");
		DO_HELP("detach_shotgun_underbarrel");
		if(player->ghost()) {
			auto status = player->ghost()->detach_shotgun_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_attach_frag_underbarrel) {
		PLAYER_CAN("ghost.attach_frag_underbarrel");
		DO_HELP("attach_frag_underbarrel");
		if(player->ghost()) {
			auto status = player->ghost()->attach_frag_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_detach_frag_underbarrel) {
		PLAYER_CAN("ghost.detach_frag_underbarrel");
		DO_HELP("detach_frag_underbarrel");
		if(player->ghost()) {
			auto status = player->ghost()->detach_frag_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_fire) {
		PLAYER_CAN("ghost.fire");
		DO_HELP("fire");
		static constexpr const char* usage = "fire <direction> <distance>\r\n";
		if(argshave()->size_gt(1)->passed() == false) {
			player->sendln(usage);
			return;
		}
		auto dir = mods::util::to_direction(argat(0));
		if(dir < 0) {
			player->sendln("Invalid direction.");
			return;
		}
		auto distance = mods::util::stoi_optional<uint8_t>(argat(1));
		if(distance.has_value() == false) {
			player->sendln("Invalid distance.");
			return;
		}
		if(player->ghost()) {
			player->sendln(std::get<1>(player->ghost()->fire_frag(dir,distance.value())));
			return;
		}
		player->sendln("Your class does not support this.");
	};
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
	ACMD(do_inject_adrenaline_shot) {
		PLAYER_CAN("ghost.adrenaline_shot");
		DO_HELP_WITH_ZERO("ghost.adrenaline_shot");
		auto status = player->ghost()->inject_adrenaline_shot();
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
		player->sendln(std::get<1>(status));
	}

	void init() {
		mods::interpreter::add_command("ghost:adrenaline_shot", POS_RESTING, do_inject_adrenaline_shot, 0,0);
		mods::interpreter::add_command("ghost:dissipate", POS_RESTING, do_dissipate, 0,0);
		mods::interpreter::add_command("ghost:mark", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("ghost:mark_target", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("ghost:tracking_shot", POS_RESTING, do_tracking_shot, 0,0);
		mods::interpreter::add_command("ghost:engage", POS_RESTING, do_engage, 0,0);
		mods::interpreter::add_command("ghost:disengage", POS_RESTING, do_disengage, 0,0);
		mods::interpreter::add_command("ghost:xray_shot", POS_RESTING, do_xray_shot, 0,0);
		mods::interpreter::add_command("ghost:build_claymore", POS_RESTING, do_build_claymore, 0,0);
		mods::interpreter::add_command("ghost:light_bandage", POS_RESTING, do_light_bandage, 0,0);
		mods::interpreter::add_command("ghost:attach_shotgun", POS_RESTING, do_attach_shotgun_underbarrel, 0,0);
		mods::interpreter::add_command("ghost:detach_shotgun", POS_RESTING, do_detach_shotgun_underbarrel, 0,0);
		mods::interpreter::add_command("ghost:attach_frag", POS_RESTING, do_attach_frag_underbarrel, 0,0);
		mods::interpreter::add_command("ghost:detach_frag", POS_RESTING, do_detach_frag_underbarrel, 0,0);
		mods::interpreter::add_command("ghost:fire", POS_RESTING, do_fire, 0,0);
		mods::interpreter::add_command("ghost:build_claymore", POS_RESTING, do_build_claymore, 0,0);
		mods::interpreter::add_command("ghost:build_corrosive_claymore", POS_RESTING, do_build_corrosive_claymore, 0,0);
		mods::interpreter::add_command("build_shrapnel_claymore", POS_RESTING, do_build_shrapnel_claymore, 0,0);
	}
};
