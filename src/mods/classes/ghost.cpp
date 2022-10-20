#include "ghost.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../projectile.hpp"
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
#include "../combat-composer/shared.hpp"
#include "../npc.hpp"
#include "../blind.hpp"
#include "../terrify.hpp"

/**
 * Resource costs via mana
 * -----------------------
 *  Skill: intimidation
 *  -------------------
 *  50 Mana
 */

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);
/**
 * This enables us to have unlimited cryogenic grenades for testing
 * delete or comment out once go to prod
 */
//#define GHOST_DEBUG_CRYO
//#define GHOST_DEBUG_PENSHOT
//#define GHOST_DEBUG_ADS
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
		player->sendln(
		    CAT(
		        "[stub] file:",__FILE__,
		        "line:",__LINE__
		    )
		);
	}
	bool ghost::is_penetrating_shot() {
		return m_is_penetrating_shot;
	}
	bool ghost::can_toss_grenade_towards(const direction_t& direction) {
		auto& room = m_player->room();
		if(room >= world.size()) {
			return false;
		}
		for(const auto& dir : world[room].directions()) {
			if(dir == direction) {
				return true;
			}
		}
		return false;
	}
	void ghost::set_aerial_drone_scan(bool on_off) {
		m_has_aerial_drone_scan = on_off;
		if(m_has_aerial_drone_scan) {
			if(!(m_overhead_lense & LENSE_AERIAL_DRONE_SCAN)) {
				m_overhead_lense = m_overhead_lense | LENSE_AERIAL_DRONE_SCAN;
			}
		} else {
			if((m_overhead_lense & LENSE_AERIAL_DRONE_SCAN)) {
				m_overhead_lense = m_overhead_lense ^ LENSE_AERIAL_DRONE_SCAN;
			}
		}
	}
	ghost::ghost(const ghost& copy) {
		std::cerr << "[ghost::ghost copy constructor]\n";
		m_overhead_lense = copy.m_overhead_lense;
		m_has_aerial_drone_scan = copy.m_has_aerial_drone_scan;
		m_target = copy.m_target;
		m_engaged = copy.m_engaged;
		m_xray_shot_charges = copy.m_xray_shot_charges;
		m_gauze_count = copy.m_gauze_count;
		m_medkit_count = copy.m_medkit_count;
		m_adrenaline_shot_charges = copy.m_adrenaline_shot_charges;

		m_heal_mode = copy.m_heal_mode;

		m_player = copy.m_player;
		m_scanned = copy.m_scanned;
		m_tracking_shot_charges = copy.m_tracking_shot_charges;
		m_preferences = copy.m_preferences;

		m_mark_target = copy.m_mark_target;
		m_tracking_shot = copy.m_tracking_shot;
		m_light_bandage = copy.m_light_bandage;
		m_suture = copy.m_suture;
		m_adrenaline_shot = copy.m_adrenaline_shot;
		m_emp_nade = copy.m_emp_nade;
		m_chaff_nade = copy.m_chaff_nade;
		m_sensor_nade = copy.m_sensor_nade;
		m_ub_shotgun = copy.m_ub_shotgun;
		m_ub_frag = copy.m_ub_frag;
		m_guided_missile = copy.m_guided_missile;
		m_target_limb = copy.m_target_limb;
		m_plant_claymore = copy.m_plant_claymore;
		m_plant_shrapnel_claymore = copy.m_plant_shrapnel_claymore;
		m_plant_corrosive_claymore = copy.m_plant_corrosive_claymore;
		m_xray_shot = copy.m_xray_shot;
		m_request_recon = copy.m_request_recon;
		m_abilities = copy.m_abilities;
		m_shotgun_ub = copy.m_shotgun_ub;
		m_frag_ub = copy.m_frag_ub;
		m_ad_shot = copy.m_ad_shot;

		m_dissipate_charges = copy.m_dissipate_charges;

		m_claymore_count = copy.m_claymore_count;
		m_corrosive_claymore_count = copy.m_corrosive_claymore_count;
		m_shrapnel_claymore_count = copy.m_shrapnel_claymore_count;

		m_cryogenic_grenade_count = copy.m_cryogenic_grenade_count;
		m_penetrating_shot_count = copy.m_penetrating_shot_count;
		m_flash_underbarrel_charges = copy.m_flash_underbarrel_charges;
		m_orm = copy.m_orm;

		m_cryogenic_grenade = copy.m_cryogenic_grenade;
		m_aerial_drone_scan = copy.m_aerial_drone_scan;
		m_feign_death = copy.m_feign_death;
		m_flash_underbarrel = copy.m_flash_underbarrel;
		m_intimidation = copy.m_intimidation;
		m_penetrating_shot = copy.m_penetrating_shot;
		m_dissipate = copy.m_dissipate;
		m_dissipated = copy.m_dissipated;
		m_call_count = copy.m_call_count;
		m_is_penetrating_shot = copy.m_is_penetrating_shot;
		m_fantom = copy.m_fantom;
	}
	std::vector<base::ability_data_t>& ghost::get_abilities() {
		return m_abilities;
	}
	player_ptr_t ghost::get_player_ptr() {
		return m_player;
	}
	bool ghost::has_mana_for_skill(uint16_t skill) {
		switch((ability_t)skill) {
			case AERIAL_DRONE_SCAN:
			case DISSIPATE:
			case FEIGN_DEATH:
			case INTIMIDATION:
			case LIGHT_BANDAGE:
			case SUTURE:
			case ADRENALINE_SHOT:
			case SENSOR_NADE:
			case GUIDED_MISSILE:
			case TARGET_LIMB:
			case REQUEST_RECON:
				break;
			default:
			case PENETRATING_SHOT:
			case CRYOGENIC_GRENADE:
			case FLASH_UNDERBARREL:
			case TRACKING_SHOT:
			case MARK_TARGET:
			case PLANT_CLAYMORE:
			case SHRAPNEL_CLAYMORE:
			case CORROSIVE_CLAYMORE:
			case UB_SHOTGUN:
			case UB_FRAG:
			case XRAY_SHOT:
				return true;
		}
		for(const auto& ability : m_abilities) {
			if(ability.ability_value == skill) {
				return m_player->mana() >= ability.mana_cost;
			}
		}
		return false;
	}
	void ghost::use_mana_for_skill(uint16_t skill) {
		switch((ability_t)skill) {
			case AERIAL_DRONE_SCAN:
			case DISSIPATE:
			case FEIGN_DEATH:
			case INTIMIDATION:
			case LIGHT_BANDAGE:
			case SUTURE:
			case ADRENALINE_SHOT:
			case SENSOR_NADE:
			case GUIDED_MISSILE:
			case TARGET_LIMB:
			case REQUEST_RECON:
				break;
			default:
			case PENETRATING_SHOT:
			case CRYOGENIC_GRENADE:
			case FLASH_UNDERBARREL:
			case TRACKING_SHOT:
			case MARK_TARGET:
			case PLANT_CLAYMORE:
			case SHRAPNEL_CLAYMORE:
			case CORROSIVE_CLAYMORE:
			case UB_SHOTGUN:
			case UB_FRAG:
			case XRAY_SHOT:
				return;
		}
		for(const auto& ability : m_abilities) {
			if(ability.ability_value == skill) {
				m_player->mana() -= ability.mana_cost;
				return;
			}
		}
	}
	void ghost::init() {
		m_target = 0;
		m_overhead_lense = 0;
		m_has_aerial_drone_scan = false;
		m_is_penetrating_shot = false;
		m_call_count = 0;
		m_scanned.clear();
		m_player = nullptr;
		m_dissipate_charges = 10;
		m_dissipated = 0;
		using SK = ability_data_t::skillset_t;
		m_abilities = create_abilities({
			{AERIAL_DRONE_SCAN,"ads","Aerial Drone Scan",SK::ELECTRONICS,&m_aerial_drone_scan,GHOST_ADS_MANA_COST()},
			{DISSIPATE,"dissipate","Dissipate",SK::INTELLIGENCE,&m_dissipate,GHOST_DISSIPATE_MANA_COST()},
			{XRAY_SHOT,"xray","X-Ray Shot",SK::SNIPING,&m_xray_shot,GHOST_XRAY_SHOT_MANA_COST()},
			{PENETRATING_SHOT,"penshot","Penetrating Shot",SK::SNIPING,&m_penetrating_shot,GHOST_PENSHOT_MANA_COST()},
			{FEIGN_DEATH,"feign","Feign Death",SK::STRATEGY,&m_feign_death,GHOST_FEIGN_DEATH_MANA_COST()},
			{INTIMIDATION,"intimidation","Intimidation",SK::INTELLIGENCE,&m_intimidation,GHOST_INTIMIDATION_MANA_COST()},
			{CRYOGENIC_GRENADE,"cryo","Cryogenic Grenade",SK::DEMOLITIONS,&m_cryogenic_grenade,GHOST_CRYONADE_MANA_COST()},
			{FLASH_UNDERBARREL,"flash","Flash Underbarrel",SK::WEAPON_HANDLING,&m_flash_underbarrel,GHOST_FLASHUB_MANA_COST()},
			{TRACKING_SHOT,"ts","Tracking Shot",SK::SNIPING,&m_tracking_shot,GHOST_TRACKINGSHOT_MANA_COST()},
			{LIGHT_BANDAGE,shorthand::LIGHT_BANDAGE.data(),"Light Bandage",SK::MEDICAL,&m_light_bandage,GHOST_LIGHTBANDAGE_MANA_COST()},
			{SUTURE,"suture","Suture",SK::MEDICAL,&m_suture,GHOST_SUTURE_MANA_COST()},
			{ADRENALINE_SHOT,"as","Adrenaline Shot",SK::MEDICAL,&m_adrenaline_shot,GHOST_ADRENALSHOT_MANA_COST()},
			{SENSOR_NADE,"sensor","Sensor Grenade",SK::INTELLIGENCE,&m_sensor_nade,GHOST_SENSORNADE_MANA_COST()},
			{UB_SHOTGUN,"ubs","Underbarrel Shotgun",SK::DEMOLITIONS,&m_ub_shotgun,GHOST_SHOTUB_MANA_COST()},
			{UB_FRAG,"ubf","Underbarrel Nade Launcher",SK::DEMOLITIONS,&m_ub_frag,GHOST_UBFRAG_MANA_COST()},
			{GUIDED_MISSILE,"gm","Guided Missile",SK::DEMOLITIONS,&m_guided_missile,GHOST_GUIDEDMIS_MANA_COST()},
			{TARGET_LIMB,"limb","Target Limb",SK::SNIPING,&m_target_limb,GHOST_TARGETLIMB_MANA_COST()},
			{MARK_TARGET,"mark","Mark Target",SK::SNIPING,&m_mark_target,GHOST_MARKTARGET_MANA_COST()},
			{PLANT_CLAYMORE,"claymore","Plant Claymore",SK::DEMOLITIONS,&m_plant_claymore,GHOST_CLAYMORE_MANA_COST()},
			{SHRAPNEL_CLAYMORE,"smine","Shrapnel Claymore",SK::DEMOLITIONS,&m_plant_shrapnel_claymore,GHOST_SHRAPCLAY_MANA_COST()},
			{CORROSIVE_CLAYMORE,"cmine","Corrosive Claymore",SK::DEMOLITIONS,&m_plant_corrosive_claymore,GHOST_CORCLAY_MANA_COST()},
			{REQUEST_RECON,"recon","Request Recon",SK::INTELLIGENCE,&m_request_recon,GHOST_REQRECON_MANA_COST()},
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
		m_claymore_count = 0;
		m_corrosive_claymore_count = 0;
		m_shrapnel_claymore_count = 0;
		m_xray_shot_charges = 0;
		m_tracking_shot_charges = 0;
		m_gauze_count = 0;
		m_adrenaline_shot_charges = 0;
		m_dissipate_charges = 0;
		m_penetrating_shot_count = 0;
		m_fantom = nullptr;
	}
	int16_t ghost::save() {
		return this->m_orm.save();
	}
	std::tuple<bool,std::string> ghost::aerial_drone_scan() {
		uint16_t ticks = 0;
#ifdef GHOST_DEBUG_ADS
		ticks = GHOST_AERIAL_DRONE_SCAN_TIER_ONE_DURATION();
#else
		if(!has_mana_for_skill(AERIAL_DRONE_SCAN)) {
			return {0,"You don't have enough mana!"};
		}

		if(m_aerial_drone_scan.not_learned()) {
			set_aerial_drone_scan(false);
			return {0, "It looks like you still need to train that skill"};
		}
		use_mana_for_skill(AERIAL_DRONE_SCAN);
		auto s = roll_skill_success(AERIAL_DRONE_SCAN);
		if(!std::get<0>(s)) {
			set_aerial_drone_scan(false);
			return {0,std::get<1>(s)};
		}
		switch(tier(m_player)) {
			case 1:
			default:
				ticks = GHOST_AERIAL_DRONE_SCAN_TIER_ONE_DURATION();
				break;
			case 2:
				ticks = GHOST_AERIAL_DRONE_SCAN_TIER_TWO_DURATION();
				break;
			case 3:
				ticks = GHOST_AERIAL_DRONE_SCAN_TIER_THREE_DURATION();
				break;
		}
		if(m_aerial_drone_scan.awful() || m_aerial_drone_scan.terrible() || m_aerial_drone_scan.okay()) {
			ticks += dice(5,5);
		}
		if(m_aerial_drone_scan.learned()) {
			ticks += dice(5,10);
		}
		if(m_aerial_drone_scan.mastered() || m_aerial_drone_scan.elite()) {
			ticks += dice(10,10);
		}
#endif
		set_aerial_drone_scan(true);
		mods::globals::defer_queue->push_ticks_event(ticks, m_player->uuid(),mods::deferred::EVENT_PLAYER_AERIAL_DRONE_SCAN_OVER);
		return {1, "Your drone launches into the air and begins scanning the area..."};
	}

	const uint8_t& ghost::overhead_lense() const {
		return m_overhead_lense;
	}
	const bool& ghost::has_aerial_drone_scan() const {
		return m_has_aerial_drone_scan;
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
		if(m_dissipate.not_learned()) {
			m_player->sendln("It looks like you still need to train that skill");
			return;
		}
		if(!has_mana_for_skill(DISSIPATE)) {
			m_player->sendln("You don't have enough mana!");
			return;
		}
		use_mana_for_skill(DISSIPATE);
		/**
		 * TODO: if the weight of gear and equipment is low, then stealth will last longer
		 */
		if(m_dissipate.awful() || m_dissipate.terrible() || m_dissipate.okay()) {
			stealth = dice(1, 8) + 1 + (m_player->level() / 4);
		}
		if(m_dissipate.learned()) {
			stealth = dice(3, 8) + 3 + (m_player->level() / 4);
		}

		if(m_dissipate.mastered() || m_dissipate.elite()) {
			stealth = 100 + dice(3, 8);
		}
		mods::object_utils::change_visibility(target,- stealth);
	}
	void ghost::apply_stealth_to_player(player_ptr_t& target) {
		int stealth = 0;
		if(m_dissipate.not_learned()) {
			m_player->sendln("It looks like you still need to train that skill");
			return;
		}
		if(!has_mana_for_skill(DISSIPATE)) {
			m_player->sendln("You don't have enough mana!");
			return;
		}
		use_mana_for_skill(DISSIPATE);
		uint8_t weight_index = (uint8_t)target->effective_weight_index();
		if(m_dissipate.awful() || m_dissipate.terrible() || m_dissipate.okay()) {
			stealth = dice(1, 8) + 1 + (m_player->level() / 4) + dice(1,weight_index);
		}

		if(m_dissipate.learned()) {
			stealth = dice(3, 8) + 3 + (m_player->level() / 4) + dice(weight_index,10);
		}

		if(m_dissipate.mastered()) {
			/**
			 * Maximum: 2.15 minutes
			 * values: 50,3,40,10,8
			 */
			stealth = 50 + dice(3, 40) + dice(weight_index * 10, 8);
		}
		if(m_dissipate.elite()) {
			/**
			 * Maximum 3.66 minutes
			 * values: 64, 5, 40, 12, 12
			 */
			stealth = 64 + dice(5, 40) + dice(weight_index * 12,12);
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
		if(!has_mana_for_skill(FEIGN_DEATH)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(FEIGN_DEATH);
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
	/* constructors and destructors */
	ghost::ghost() {
		m_player = nullptr;
	}
	std::tuple<bool,std::string> ghost::dissipate() {
		if(m_dissipate_charges == 0) {
			return {false,"{red}You don't have any dissipate charges{/red}"};
		}
		if(!has_mana_for_skill(DISSIPATE)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(DISSIPATE);
		auto s = roll_skill_success(DISSIPATE);
		if(!std::get<0>(s)) {
			return s;
		}
		--m_dissipate_charges;
		m_player->visibility() = 0;
		uint32_t ticks = GHOST_DISSIPATE_TICKS_DURATION() * tier(m_player);
		mods::globals::defer_queue->push_ticks_event(ticks,m_player->uuid(),mods::deferred::EVENT_PLAYER_GOES_VISIBLE);
#ifdef __MENTOC_SHOW_GHOST_TIME_WHEN_DISSIPATE_WEARS_OFF__
		m_player->sendln(
		    CAT(
		        mods::date_time::irl::now().c_str(),
		        " tick count: (",ticks,")"
		    )
		);
#endif
		m_dissipated = true;
		return {true,"{grn}You dissipate into nothing...{/grn}"};
	}
	void ghost::dissipate_wears_off() {
		m_player->visibility() = char_data::STARTING_VISIBILITY;
		m_player->sendln("Your dissipation invisibility wears off...");
#ifdef __MENTOC_SHOW_GHOST_TIME_WHEN_DISSIPATE_WEARS_OFF__
		m_player->sendln(mods::date_time::irl::now().c_str());
#endif
		m_dissipated = false;
	}
	void ghost::use_claymore(uuid_t object_uuid) {
		mods::weapons::corrosive_claymore_installed(object_uuid);
	}
	void ghost::replenish() {
		auto tier = tier(m_player);
		if((m_call_count % GHOST_REPLENISH_PULSE()) == 0) {
			if(m_penetrating_shot_count < GHOST_CRYOGENIC_GRENADE_COUNT() * tier) {
				++m_penetrating_shot_count;
			}
			if(m_claymore_count < SNIPER_CLAYMORE_MAX_COUNT() * tier) {
				if(m_cryogenic_grenade_count < GHOST_CRYOGENIC_GRENADE_COUNT() * tier) {
					++m_cryogenic_grenade_count;
				}
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
	void ghost::apply_penetrating_shot_mods(mods::combat_composer::phases::calculated_damage_t& damage) {
		if(m_penetrating_shot.awful() || m_penetrating_shot.terrible() || m_penetrating_shot.okay()) {
			damage.damage += 50;
		}
	}
	std::tuple<bool,std::string> ghost::fire_penetrating_shot_at(const direction_t& direction) {
		m_is_penetrating_shot = true;
		if(!has_mana_for_skill(PENETRATING_SHOT)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(PENETRATING_SHOT);
		auto s = roll_skill_success(PENETRATING_SHOT);
		uint8_t extra_shots = 1;
		if(!std::get<0>(s)) {
			m_is_penetrating_shot = false;
			return {0,std::get<1>(s)};
		}
#ifdef GHOST_DEBUG_PENSHOT
		m_penetrating_shot_count = 10;
#endif
		if(m_penetrating_shot_count == 0) {
			m_is_penetrating_shot = false;
			return {false,"You don't have any shots left!"};
		}
		if(m_penetrating_shot.awful() || m_penetrating_shot.terrible() || m_penetrating_shot.okay()) {
			extra_shots = 0;
			if(dice(3,6) < 14) {
				--m_penetrating_shot_count;
				m_is_penetrating_shot = false;
				return {0,"Your lack of experience causes you to misfire!"};
			}
		}
		if(m_penetrating_shot.learned()) {
			extra_shots = 1;
			if(dice(3,6) < 11) {
				--m_penetrating_shot_count;
				m_is_penetrating_shot = false;
				return {0,"Your lack of experience causes you to misfire!"};
			}
		}
		//auto ammo = create_object(ghost::PENETRATING_SHOT_SLUG_YAML.data());
		bool free_shot = false;
		if(m_penetrating_shot.mastered()) {
			extra_shots = 3;
		}
		if(m_penetrating_shot.elite()) {
			extra_shots = 5;
		}
		if(m_penetrating_shot.mastered() || m_penetrating_shot.elite()) {
			if(dice(3,6) > 16) {
				free_shot = true;
				m_player->sendln("Your experience causes you to gain one extra penetrating shot charge!");
			}
		}

		if(free_shot) {
			++m_penetrating_shot_count;
		}
		if(!m_fantom) {
			m_fantom = create_object(PENETRATING_SHOT_RIFLE_YAML.data());
		}

		auto primary = m_player->primary();
		m_player->equip(m_fantom,WEAR_PRIMARY);

		int i = 0;
		auto occupants = mods::globals::get_room_list_from_position(m_player,direction);
		for(auto& target : occupants) {
			mods::combat_composer::snipe_target(
			    m_player,//attacker,
			    target, // victim
			    NORTH, // direction
			    2, // distance
			    m_fantom
			);
			++i;
			if(i > extra_shots) {
				break;
			}
		}
		--m_penetrating_shot_count;
		m_is_penetrating_shot = false;
		m_player->unequip(WEAR_PRIMARY);
		if(primary != nullptr) {
			m_player->equip(primary,WEAR_PRIMARY);
		}
		return {true,"You fire off a penetrating shot!"};
	}
	ghost::~ghost() {
		if(m_fantom) {
			mods::globals::dispose_object(m_fantom->uuid);
			m_fantom = nullptr;
		}
	}
	std::tuple<bool,std::string> ghost::intimidate_target(uuid_t npc_uuid) {
		auto npc = npc_by_uuid(npc_uuid);
		if(!npc) {
			return {0,"You cannot find your target anywhere!"};
		}
		uint16_t ticks = 0;
		if(!has_mana_for_skill(INTIMIDATION)) {
			return {0,"You're all out of mana!"};
		}
		use_mana_for_skill(INTIMIDATION);
		auto s = roll_skill_success(INTIMIDATION);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		std::string str;
		if(m_intimidation.awful() || m_intimidation.terrible() || m_intimidation.okay()) {
			ticks = GHOST_INTIMIDATION_TICKS_INITIATE();
			str = "$n drains the confidence of $N";
		}
		if(m_intimidation.learned()) {
			ticks = GHOST_INTIMIDATION_TICKS_LEARNED();
			str = "$n effectively kills the morale of $N";
		}
		if(m_intimidation.mastered()) {
			ticks = GHOST_INTIMIDATION_TICKS_MASTERED();
			str = "$n's {red}bright red{/red} eyes read the soul of $N";
		}
		if(m_intimidation.elite()) {
			ticks = GHOST_INTIMIDATION_TICKS_ELITE();
			str = "$n {red}kills{/red} any semblance of sanity in $N";
		}
		act(str,true, m_player->cd(), nullptr, npc->cd(), TO_ROOM);
		mods::terrify::terrify_for(npc, ticks);
		return {true,""};
	}

	uint8_t ghost::cryogenic_grenade_count() const {
		return m_cryogenic_grenade_count;
	}
	std::tuple<bool,std::string> ghost::toss_cryogenic_grenade_towards(const direction_t& direction, uint8_t rooms) {
		auto s = roll_skill_success(CRYOGENIC_GRENADE);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		if(m_cryogenic_grenade.awful() || m_cryogenic_grenade.terrible() || m_cryogenic_grenade.okay()) {
			if(dice(3,6) < 14) {
				return {0,"Your lack of experience causes you to fumble the grenade"};
			}
		}
		if(m_cryogenic_grenade.learned()) {
			if(dice(3,6) < 11) {
				return {0,"Your lack of experience causes you to fumble the grenade"};
			}
		}
		//m_player->block_for(duration, mods::deferred::EVENT_PLAYER_FINISHES_FEIGN_DEATH, 0);
#ifdef GHOST_DEBUG_CRYO
		m_cryogenic_grenade_count = 10;
#endif
		if(m_cryogenic_grenade_count == 0) {
			return {false,"You don't have any cryogenic grenades"};
		}
		if(!can_toss_grenade_towards(direction)) {
			return {false,"You can't seem to throw anything in that direction!"};
		}
		auto grenade = create_object(ghost::CRYOGENIC_GRENADE_YAML.data());
		mods::projectile::throw_object(m_player, direction, rooms, grenade, "toss");
		if(m_cryogenic_grenade.mastered() || m_cryogenic_grenade.elite()) {
			if(dice(3,6) > 16) {
				++m_cryogenic_grenade_count;
				m_player->sendln("Your experience causes you to gain one extra cryogenic grenade!");
			}
		} else {
			--m_cryogenic_grenade_count;
		}
		return {true,""};
	}
	/** applies it to the entire room. every will get flashed */
	std::tuple<bool,std::string> ghost::use_flash_underbarrel(const uuid_t& npc_uuid) {
		if(m_flash_underbarrel_charges == 0) {
			return {0,"You don't have any flash underbarrel charges!"};
		}
		auto npc = ptr_by_uuid(npc_uuid);
		if(!npc) {
			return {0,"You cannot find your target anywhere!"};
		}
		uint16_t ticks = 0;
		if(!has_mana_for_skill(FLASH_UNDERBARREL)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(FLASH_UNDERBARREL);
		auto s = roll_skill_success(FLASH_UNDERBARREL);
		if(!std::get<0>(s)) {
			return {0,std::get<1>(s)};
		}
		std::string str;
		if(m_flash_underbarrel.awful() || m_flash_underbarrel.terrible() || m_flash_underbarrel.okay()) {
			ticks = GHOST_FLASH_UNDERBARREL_TICKS_INITIATE();
			str = "A high-powered flash appears from $n's underbarrel attachment";
		}
		if(m_flash_underbarrel.learned()) {
			ticks = GHOST_FLASH_UNDERBARREL_TICKS_LEARNED();
			str = "A high-powered flash appears from $n's underbarrel attachment";
		}
		if(m_flash_underbarrel.mastered()) {
			ticks = GHOST_FLASH_UNDERBARREL_TICKS_MASTERED();
			str = "An unyielding amount of light pours from $n's underbarrel attachment blinding $N!";
		}
		if(m_flash_underbarrel.elite()) {
			ticks = GHOST_FLASH_UNDERBARREL_TICKS_ELITE();
			str = "$N's vision is utterly {red}DECIMATED{/red} from $n's underbarrel attachment";
		}
		act(str,true, m_player->cd(), nullptr, npc->cd(), TO_ROOM);
		mods::blind::blind_for(npc,ticks);
		return {true,""};
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
		if(!has_mana_for_skill(SUTURE)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(SUTURE);
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
		if(!has_mana_for_skill(XRAY_SHOT)) {
			return {0,"You don't have enough mana!"};
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
		if(!has_mana_for_skill(MARK_TARGET)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(MARK_TARGET);
		auto s = roll_skill_success(MARK_TARGET);
		if(!std::get<0>(s)) {
			return s;
		}
		m_target = mods::examine::find_player_by_name(m_player, target);
		if(m_target == 0) {
			return {0,"Couldn't find a target that matches that string."};
		}
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
		if(!has_mana_for_skill(TRACKING_SHOT)) {
			return {0,"You don't have enough mana!"};
		}
		if(m_tracking_shot_charges) {
			use_mana_for_skill(TRACKING_SHOT);
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
			if(!has_mana_for_skill(LIGHT_BANDAGE)) {
				return {0,"You don't have enough mana!"};
			}
			use_mana_for_skill(LIGHT_BANDAGE);
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
		if(!has_mana_for_skill(UB_FRAG)) {
			return {0,"You don't have enough mana!"};
		}
		use_mana_for_skill(UB_FRAG);
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
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
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

	ACMD(do_toss_cryogenic_grenade) {
		static constexpr std::string_view usage = "Usage: ghost:toss_cryogenic_grenade <direction> <depth>";
		PLAYER_CAN("ghost.toss_cryogenic_grenade");
		DO_HELP_WITH_ZERO("ghost.toss_cryogenic_grenade");
		auto vec_args = PARSE_ARGS();
		if(argshave()->size_gt(1)->passed() == false) {
			player->sendln(usage);
			return;
		}
		int opt_direction = mods::util::to_direction(vec_args[0]);
		if(opt_direction < 0) {
			player->errorln("Please provide a valid direction");
			player->sendln(usage);
			return;
		}
		auto opt_depth = mods::util::stoi_optional<uint8_t>(vec_args[1]);
		if(opt_depth.has_value() == false) {
			player->errorln("Please provide a valid integer for depth");
			player->sendln(usage);
			return;
		}
		direction_t direction = (direction_t)opt_direction;
		uint8_t depth = opt_depth.value();

		auto status = player->ghost()->toss_cryogenic_grenade_towards(direction,depth);
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
	}
	ACMD(do_fire_penetrating_shot) {
		static constexpr std::string_view usage = "Usage: ghost:fire_penetrating_shot <direction>";
		PLAYER_CAN("ghost.fire_penetrating_shot");
		DO_HELP_WITH_ZERO("ghost.fire_penetrating_shot");
		auto vec_args = PARSE_ARGS();
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		int direction = mods::util::to_direction(vec_args[0]);
		if(direction < 0) {
			player->errorln("Must use a valid direction");
			player->sendln(usage);
			return;
		}
		auto status = player->ghost()->fire_penetrating_shot_at(direction);
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
		player->sendln(std::get<1>(status));
	}
	ACMD(do_aerial_drone_scan) {
		PLAYER_CAN("ghost.aerial_drone_scan");
		if(player->ghost()->has_aerial_drone_scan()) {
			player->errorln("You already have a drone scan active");
			return;
		}
		auto status = player->ghost()->aerial_drone_scan();
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
		player->sendln(std::get<1>(status));
	}
	ACMD(do_intimidate) {
		static constexpr std::string_view usage = "Usage: ghost:intimidate <npc>";
		PLAYER_CAN("ghost.intimidate");
		DO_HELP_WITH_ZERO("ghost.intimidate");
		auto vec_args = PARSE_ARGS();
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		uuid_t npc =  mods::examine::find_player_by_name(player, vec_args[0]);
		if(npc == 0) {
			player->errorln("Couldn't find any npc in the same room that matches that name!");
			player->sendln(usage);
			return;
		}

		auto status = player->ghost()->intimidate_target(npc);
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
		/** On success, the function doesn't send a string to send back to the user */
	}
	ACMD(do_flash_underbarrel) {
		static constexpr std::string_view usage = "Usage: ghost:flash_underbarrel <npc>";
		PLAYER_CAN("ghost.flash_underbarrel");
		DO_HELP_WITH_ZERO("ghost.flash_underbarrel");
		auto vec_args = PARSE_ARGS();
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		uuid_t npc =  mods::examine::find_player_by_name(player, vec_args[0]);
		if(npc == 0) {
			player->errorln("Couldn't find any npc in the same room that matches that name!");
			player->sendln(usage);
			return;
		}

		auto status = player->ghost()->use_flash_underbarrel(npc);
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
		/** On success, the function doesn't send a string to send back to the user */
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
		mods::interpreter::add_command("ghost:toss_cryogenic_grenade", POS_RESTING, do_toss_cryogenic_grenade, 0,0);
		mods::interpreter::add_command("ghost:fire_penetrating_shot", POS_RESTING, do_fire_penetrating_shot, 0,0);
		mods::interpreter::add_command("ghost:aerial_drone_scan", POS_RESTING, do_aerial_drone_scan, 0,0);
		mods::interpreter::add_command("ghost:intimidate", POS_RESTING, do_intimidate, 0,0);
		mods::interpreter::add_command("ghost:flash_underbarrel", POS_RESTING, do_flash_underbarrel, 0,0);
	}
};
