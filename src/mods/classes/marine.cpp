#include "marine.hpp"
#include "../orm/inventory.hpp"
#include "../query-objects.hpp"
#include "../interpreter.hpp"
#include "../object-utils.hpp"
#include "../weapon.hpp"
#include "../combat-composer/spray-target.hpp"
#include "../pc-ability.hpp"

namespace mods::classes {
	using cmd_report_t = mods::classes::base::cmd_report_t;
	static constexpr std::string_view DEPLOY_EXPLOSIVE_DRONE_USAGE = "marine:deploy_explosive_drone <direction> <distance>";
	marine::marine() {
		this->init();
	}
	bool marine::is_surpressing(const uuid_t& victim_uuid) {
		return ACT_SURPRESSING_FIRE == m_current_activity
		    && m_surpressing.contains(victim_uuid) &&
		    m_surpressing[victim_uuid] >= CURRENT_TICK() - 30;
	}
	void marine::set_surpressing(const uuid_t& victim_uuid) {
		m_surpressing[victim_uuid] = CURRENT_TICK() + 30; // FIXME: use values system
	}
	void marine::remove_surpressing(const uuid_t& victim_uuid) {
		m_surpressing.erase(victim_uuid);
	}
	void marine::init() {
		using s = base::ability_data_t::skillset_t;
		uint16_t M = 0;
		m_acquired_target.first = false;
		m_call_count =0;
		m_explosive_drone_count = 0;
		m_pindown_count = 0;
		m_abilities = create_abilities({
			{LOAD_TRACER_ROUNDS,"ltr","Load Tracer Rounds",s::WEAPON_HANDLING,&m_load_tracer_rounds,M},
			//{FIELD_STRIP_WEAPON,"fsw","Field Strip Weapon",s::INTELLIGENCE,&m_field_strip_weapon,M},
			{DEPLOY_EXPLOSIVE_DRONE,"dedrone","Deploy Explosive Drone",s::ELECTRONICS,&m_deploy_explosive_drone,M},
			{ATTACH_M203,"m2","Attach m203",s::DEMOLITIONS,&m_attach_m203,M},
			{PIN_DOWN,"pin","Pin Down",s::WEAPON_HANDLING,&m_pin_down,M},
			//{ATTACH_M203_X12,"m2x","Attach m203-x12",s::DEMOLITIONS,&m_attach_m203_x12,M},
			//{ATTACH_FLASH_SURPRESSOR,"afs","Attach Flash Surpressor",s::WEAPON_HANDLING,&m_attach_flash_surpressor,M},
			//{TEEP_KICK,"teep","Teep Kick",s::STRENGTH,&m_teep_kick,M},
			//{SINGLE_LEG_TAKEDOWN,"slt","Single Leg Takedown [ Grapple 1]",s::DEXTERITY,&m_single_leg_takedown,M},
			//{DOUBLE_LEG_TAKEDOWN,"tlt","Double Leg Takedown [ Grapple 2]",s::DEXTERITY,&m_double_leg_takedown,M},
			//{UCHI_MATA,"uch","Uchi Mata [ Throw 1 ]",s::DEXTERITY,&m_uchi_mata,M},
			//{MOROTE_SEOI_NAGE,"msn","Morote Seoi Nage [ Throw 2 ]",s::DEXTERITY,&m_morote_seoi_nage,M},
			//{TOMOE_NAGE,"tnage","Tomoe Nage [ Throw 3 ]",s::DEXTERITY,&m_tomoe_nage,M},
			//{THERMAL_VISION,"therm","Thermal Vision",s::ELECTRONICS,&m_thermal_vision,M},
			//{NIGHT_VISION,"nvg","Night Vision",s::ELECTRONICS,&m_night_vision,M},
			//{DEPLOY_SMOKE,"ds","Deploy Smoke",s::INTELLIGENCE,&m_deploy_smoke,M},
			//{SPECIALIZE_IN,"spec","Specialize In",s::INTELLIGENCE,&m_specialize_in,M},
			//{BENZO_STIMULI_SHOT,"bss","Benzo Stimuli Shot",s::MEDICAL,&m_benzo_stimuli_shot,M},
			//{LIGHT_BANDAGE,"lb","Light Bandage",s::MEDICAL,&m_light_bandage,M},
			//{SUTURE,"str","Suture",s::MEDICAL,&m_suture,M},
			//{ADRENALINE_SHOT,"ads","Adrenaline Shot",s::MEDICAL,&m_adrenaline_shot,M},
			//{TARGET_LIMB,"tlimb","Target Limb",s::MEDICAL,&m_target_limb,M},
			//{PLANT_CLAYMORE,"clay","Plant Claymore",s::DEMOLITIONS,&m_plant_claymore,M},
			//{SHRAPNEL_CLAYMORE,"sclay","Shrapnel Claymore",s::DEMOLITIONS,&m_shrapnel_claymore,M},
			//{CORROSIVE_CLAYMORE,"corclay","Corrosive Claymore",s::DEMOLITIONS,&m_corrosive_claymore,M},
			//{REQUEST_RECON,"reqrecon","Request Recon",s::INTELLIGENCE,&m_request_recon,M},
			//{REQUEST_SMOKE_SCREEN,"reqs","Request Smoke Screen",s::INTELLIGENCE,&m_request_smoke_screen,M},
			//{REQUEST_DRONE_STRIKE,"reqds","Request Drone Strike",s::INTELLIGENCE,&m_request_drone_strike,M},
			//{REQUEST_DRONE_RECON,"reqdr","Request Drone Recon",s::INTELLIGENCE,&m_request_drone_recon,M},
		});
	}
	cmd_report_t marine::giveme_m16() {
		if(!m_m16) {
			m_m16 = create_object(ITEM_RIFLE,marine::M16A_RIFLE_OBJECT.data());
		}
		if(m_player->primary()) {
			m_player->unequip_into_inventory(WEAR_PRIMARY);
		}
		m_player->equip(m_m16,WEAR_PRIMARY);
		return {1,"You equip your M16A into your primary weapon slot."};
	}
	bool marine::is_core_ability(const ability_t& ability) const {
		return std::find(core_abilities.cbegin(),core_abilities.cend(),ability) != core_abilities.cend();
	}
	skill_t* marine::m_get_skill(const ability_t& ab) {
		for(const auto& ability : m_abilities) {
			if(ability.ability_value == ab) {
				return ability.skill_ptr;
			}
		}
		return nullptr;
	}

	void marine::replenish() {
		auto tier = tier(m_player);
		if((m_call_count % MARINE_REPLENISH_PULSE()) == 0) {
			if(m_explosive_drone_count < MARINE_EXPLOSIVE_DRONE_COUNT_PER_TIER() * tier) {
				++m_explosive_drone_count;
			}
			if(m_pindown_count < MARINE_PIN_DOWN_COUNT_PER_TIER() * tier) {
				++m_pindown_count;
			}
#if 0
			// TODO FIXME
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
				if(m_gauze_count < SNIPER_GAUZE_MAX_COUNT() * tier) {
					++m_gauze_count;
				}
				if(m_adrenaline_shot_charges < SNIPER_ADRENALINE_SHOT_MAX_COUNT() * tier) {
					++m_adrenaline_shot_charges;
				}
				if(m_debugging_report_replenish) {
					m_player->sendln("Replenished.");
				}
			}
#endif
		}
	}

	marine::marine(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}
	int16_t marine::new_player(player_ptr_t& player, primary_choice_t primary_choice) {
		if(primary_choice == primary_choice_t::NONE) {
			report("marine::new_player. got primary_choice of zero. defaulting to M16A4");
			primary_choice = primary_choice_t::M16A4;
		}
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t marine::load_by_player(player_ptr_t& player) {
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("unable to load marine class by player id: ",(player->db_id()),".. return status: ",(result),"player:",player->name().c_str()));
			return -100 - result;
		}
		initialize_skills_for_player(player);
		return result;
	}
	int16_t marine::save() {
		return m_orm.save();
	}
	std::shared_ptr<marine> create_marine(player_ptr_t& in_player) {
		return std::move(std::make_shared<marine>(in_player));
	}
	cmd_report_t marine::attach_frag_underbarrel() {
		if(!m_player->primary() || !m_player->primary()->has_rifle()) {
			return {0,"Must be wielding a rifle"};
		}
		if(m_frag_ub.is_attached() && m_frag_ub.ammo()) {
			return {1,"Already attached. To detach, use 'marine:detach_m203'."};
		}
		auto s = roll_skill_success(ATTACH_M203);
		if(std::get<0>(s)) {
			uint16_t frag_ammo = 2 * (tier(m_player) + 1);
			m_ub_frag.use_skill(m_player);
			m_frag_ub.set_yaml(M203_UNDERBARREL);
			auto f = m_frag_ub.attach_to(m_player->primary(),tier(m_player));
			m_frag_ub.set_ammo(frag_ammo);
			return f;
		}
		return s;
	}
	cmd_report_t marine::detach_frag_underbarrel() {
		return m_frag_ub.detach();
	}
	cmd_report_t marine::fire_frag(const direction_t& direction,const uint8_t& distance) {
		return m_frag_ub.fire(m_player,direction,distance);
	}
	cmd_report_t marine::deploy_explosive_drone() {
		auto s = roll_skill_success(DEPLOY_EXPLOSIVE_DRONE);
		if(std::get<0>(s)) {
			uint16_t frag_ammo = 2 * (tier(m_player) + 1);
			m_ub_frag.use_skill(m_player);
			m_frag_ub.set_yaml(M203_UNDERBARREL);
			auto f = m_frag_ub.attach_to(m_player->primary(),tier(m_player));
			m_frag_ub.set_ammo(frag_ammo);
			return f;
		}
		return {true,"You attach an M203."};
	}
	cmd_report_t marine::engage() {

		if(m_current_activity == ACT_SURPRESSING_FIRE) {
			if(m_acquired_target.first == false) {
				m_current_activity = ACT_NON_COMBATIVE;
				return {false,"It doesn't look like you can find your target."};
			}
			if(m_player->position() != POS_PRONE) {
				m_player->sendln("You get on your stomach");
				m_player->set_position(POS_PRONE);
			}
			m_player->sendln("You aim...");
			m_player->sendln("You pull the trigger...");

			m_current_activity = ACT_SURPRESSING_FIRE;
			auto weapon = m_player->primary();
			mods::combat_composer::spray_target(m_player,m_acquired_target.second.direction,weapon);
			m_pin_down.use_skill(m_player);
			return {true,"You engage your target!"};
		}
		return {false,"Engage with what? You aren't doing anything!"};
	}
	cmd_report_t marine::disengage() {
		if(m_current_activity == ACT_SURPRESSING_FIRE) {
			if(m_player->position() == POS_PRONE) {
				m_player->sendln("You pick yourself up off the ground");
				m_player->set_position(POS_STANDING);
				m_acquired_target.first = false;
			}
			m_current_activity = ACT_NON_COMBATIVE;
			m_surpressing.clear();
		}

		return {false,"Disengage from what? You aren't doing anything!"};
	}
	cmd_report_t marine::pin_down(const acquired_target_t& input,pin_down_mode_t pin_mode) {
		auto s = roll_skill_success(PIN_DOWN);
		if(!std::get<0>(s)) {
			return s;
		}
		if(!mods::object_utils::player_primary_is_lmg(m_player)) {
			return {0,"You must be wielding an {grn}LMG{/grn}"};
		}
		m_pin_down.use_skill(m_player);
		/**
		 * Brainstorm:
		 * ----------------
		 *  Possible implementations:
		 *  	- flagging a target as being "pinned down"
		 *  		- an AFFECT
		 *  		- a type of effect similar to bleed/blind/etc
		 *  		- a free_to_move_on_tick variable stored on target
		 *
		 * "Movement penalty":
		 * - whenever the target wants to move, we first do:
		 * ```
		 * if(target->attempt_to_move(direction)){
		 *  	target->move(direction);
		 * }else{
		 * 		target->sendln("You are pinned down!");
		 * }
		 *
		 * bool player::attempt_to_move(direction_t dir) {
		 *		if(m_target_movement_penalty > 0){
		 *   		m_target_movement_penalty -= dexterity * 0.5 * tier;
		 *		}
		 *		if(m_target_movement_penalty < 0){
		 *			m_target_movement_penalty = 0;
		 *		}
		 *		return m_target_movement_penalty == 0;
		 * }
		 */
		if(m_player->position() != POS_PRONE) {
			m_player->sendln("You get on your stomach");
			m_player->set_position(POS_PRONE);
		}
		m_player->sendln("You aim...");
		m_player->sendln("You pull the trigger...");

		m_current_activity = ACT_SURPRESSING_FIRE;
		auto weapon = m_player->primary();
		mods::combat_composer::spray_target(m_player,input.direction,weapon);
		m_acquired_target.first = true;
		m_acquired_target.second = input;
		set_surpressing(m_acquired_target.second.target->uuid());
		return {true,
		        std::string("HINT: {yel}Type {grn}marine:engage{/grn}{yel} to spray again!{/yel}\r\n") +
		        std::string("HINT: {yel}Type {grn}marine:disengage{/grn}{yel} to stop spraying{/yel}")};
	}
	cmd_report_t marine::load_tracer_rounds(const uuid_t& wpn) {
		// TEST ME
		auto ptr = optr_by_uuid(wpn);
		if(!ptr) {
			return {0,"Couldn't find that item"};
		}
		if(!ptr->has_rifle() || ptr->rifle()->attributes->type != weapon_rifle_t::ASSAULT_RIFLE) {
			return {0,"Item is not an assault rifle"};
		}
		auto r = mods::rifle_attachments_t::first_or_create(ptr);
		r->detach_into_inventory(m_player,slot_t::SLOT_MAGAZINE);
		m_tracer_rounds = create_object(ITEM_ATTACHMENT,TRACER_ROUNDS.data());
		if(!m_tracer_rounds->attachment()) {
			return {false,"Couldn't load object!"};
		}
		auto status = r->attach_from_inventory(m_player,slot_t::SLOT_MAGAZINE,m_tracer_rounds);
		if(status < 0) {
			return {false,"Failed to load!"};
		}

		return {true,"Loaded."};
	}
};

namespace mods::class_abilities::marine {
	using pin_down_mode_t = ::mods::classes::marine::pin_down_mode_t;
	ACMD(do_load_tracer_rounds) {
		static constexpr std::string_view usage = "Usage: marine:load_tracer_rounds <weapon>";
		PLAYER_CAN("marine.load_tracer_rounds");
		DO_HELP_WITH_ZERO("marine.load_tracer_rounds");
		auto vec_args = PARSE_ARGS();
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		auto opt = mods::query_objects::inv::query_for_single_object(player,vec_args[0]);
		if(!opt.has_value()) {
			player->errorln("Couldn't find anything in your inventory matching that!");
			return;
		}

		CMDREPORT(player->marine()->load_tracer_rounds(opt.value()));
	}
	ACMD(do_deploy_explosive_drone) {
		PLAYER_CAN("marine.deploy_explosive_drone");
		CMDREPORT(player->marine()->deploy_explosive_drone());
	}
	ACMD(do_attach_m203) { // TEST ME
		PLAYER_CAN("marine.attach_m203");
		DO_HELP_WITH_ZERO("marine.attach_m203");
		CMDREPORT(player->marine()->attach_frag_underbarrel());
	}
	ACMD(do_detach_m203) { // TEST ME
		PLAYER_CAN("marine.detach_m203");
		CMDREPORT(player->marine()->detach_frag_underbarrel());
	}
	ACMD(do_marine_fire) { // TEST ME
		PLAYER_CAN("marine.fire");
		DO_HELP("marine.fire");
		static constexpr const char* usage = "marine:fire <direction> <distance>\r\n";
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
		if(player->marine()) {
			CMDREPORT(player->marine()->fire_frag(dir,distance.value()));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_marine_giveme_m16) { // TEST ME
		PLAYER_CAN("marine.giveme_m16");
		DO_HELP("marine.giveme_m16");
		if(!player->marine()) {
			player->sendln("Your class does not support this.");
			return;
		}
		CMDREPORT(player->marine()->giveme_m16());
	};
	ACMD(do_marine_pin_down_change_target) { // TEST ME
		using namespace mods::target_acquisition;
		PLAYER_CAN("marine.pin_down");
		DO_HELP("marine.pin_down");
		if(!player->marine()) {
			player->sendln("Your class does not support this.");
			return;
		}
		static constexpr std::string_view usage = "marine:pin_down:change_target <target> <direction> <distance>\r\n"
		    "Example:\r\n"
		    "marine:pin_down:change_target Guard west 2\r\n";

		auto vec_args = PARSE_ARGS();
		if(argshave()->size_gt(2)->passed()) {
			player->sendln(usage);
			return;
		}
		auto opt_tdd = mods::util::parse_target_direction_distance(player,vec_args);
		if(!opt_tdd.has_value()) {
			player->sendln("Couldn't find target!");
			return;
		}
		acquired_target_t t(opt_tdd);
		CMDREPORT(player->marine()->pin_down(t,pin_down_mode_t::PD_CHANGE_TARGET));
	};
	ACMD(do_marine_pin_down) { // TEST ME
		using namespace mods::target_acquisition;
		PLAYER_CAN("marine.pin_down");
		DO_HELP("marine.pin_down");
		if(!player->marine()) {
			player->sendln("Your class does not support this.");
			return;
		}
		static constexpr std::string_view usage = "marine:pin_down <target> <direction> <distance>\r\n"
		    "Example:\r\n"
		    "marine:pin_down Guard west 2\r\n";

		auto vec_args = PARSE_ARGS();
		if(!argshave()->size_gt(2)->passed()) {
			player->sendln(usage);
			return;
		}
		auto opt_tdd = mods::util::parse_target_direction_distance(player,vec_args);
		if(!opt_tdd.has_value()) {
			player->sendln("Couldn't find target!");
			return;
		}
		acquired_target_t t(opt_tdd);
		CMDREPORT(player->marine()->pin_down(t,pin_down_mode_t::PD_START_SURPRESSING));
	};
	ACMD(do_marine_engage) { // TEST ME
		using namespace mods::target_acquisition;
		PLAYER_CAN("marine.engage");
		DO_HELP("marine.engage");
		if(!player->marine()) {
			player->sendln("Your class does not support this.");
			return;
		}
		CMDREPORT(player->marine()->engage());
	};
	ACMD(do_marine_disengage) { // TEST ME
		PLAYER_CAN("marine.disengage");
		DO_HELP("marine.disengage");
		if(!player->marine()) {
			player->sendln("Your class does not support this.");
			return;
		}
		CMDREPORT(player->marine()->disengage());
	};
	void init() {
		mods::interpreter::add_command("marine:giveme_m16", POS_RESTING, do_marine_giveme_m16, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:load_tracer_rounds", POS_RESTING, do_load_tracer_rounds, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:deploy_explosive_drone", POS_RESTING, do_deploy_explosive_drone, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:attach_m203", POS_RESTING, do_attach_m203, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:detach_m203", POS_RESTING, do_detach_m203, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:fire", POS_RESTING, do_marine_fire, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:pin_down", POS_RESTING, do_marine_pin_down, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:pin_down:change_target", POS_RESTING, do_marine_pin_down_change_target, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:engage", POS_PRONE, do_marine_engage, 0,0); // TESTME FIXME
		mods::interpreter::add_command("marine:disengage", POS_PRONE, do_marine_disengage, 0,0); // TESTME FIXME

	}
};
