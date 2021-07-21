#include "sniper.hpp"
#include "../orm/inventory.hpp"
#include "../weapons/damage-types.hpp"
#include "../interpreter.hpp"
#include "../help.hpp"
#include "../examine.hpp"
#include "../medic.hpp"
#include "../levels.hpp"

namespace mods::rand {
	extern int roll(int num,int size);
};

namespace mods::classes {
	struct shorthand {
		static constexpr std::string_view LIGHT_BANDAGE = "lb";
	};
	sniper::sniper() {
		this->init();
	}
	sniper::sniper(player_ptr_t p) {
		this->init();
		load_by_player(p);
		m_shotgun_ub.init();
		m_frag_ub.init();
	}
	void sniper::init() {
		m_target = 0;
		m_engaged = 0;
		m_xray_shot_charges = 0;
		m_player = nullptr;
		m_claymore_charges = 0;
		m_tracking_shot_charges = 0;
		m_preferences["mute-replenish"] = true;
		m_gauze_count = 0;
		m_medkit_count = 0;
		using s = ability_data_t::skillset_t;
		m_abilities = {
			{TRACKING_SHOT,"ts","Tracking Shot",s::SNIPING,&m_tracking_shot},
			{LIGHT_BANDAGE,shorthand::LIGHT_BANDAGE.data(),"Light Bandage",s::MEDICAL,&m_light_bandage},
			{SUTURE,"suture","Suture",s::MEDICAL,&m_suture},
			{ADRENALINE_SHOT,"as","Adrenaline Shot",s::MEDICAL,&m_adrenaline_shot},
			{EMP_NADE,"emp","EMP Grenade",s::DEMOLITIONS,&m_emp_nade},
			{CHAFF_NADE,"chaff","Chaff Grenade",s::DEMOLITIONS,&m_chaff_nade},
			{SENSOR_NADE,"sensor","Sensor Grenade",s::INTELLIGENCE,&m_sensor_nade},
			{UB_SHOTGUN,"ubs","Underbarrel Shotgun",s::DEMOLITIONS,&m_ub_shotgun},
			{UB_FRAG,"ubf","Underbarrel Nade Launcher",s::DEMOLITIONS,&m_ub_frag},
			{GUIDED_MISSILE,"gm","Guided Missile",s::DEMOLITIONS,&m_guided_missile},
			{TARGET_LIMB,"limb","Target Limb",s::SNIPING,&m_target_limb},
			{PLANT_CLAYMORE,"claymore","Plant Claymore",s::DEMOLITIONS,&m_plant_claymore},
			{SHRAPNEL_CLAYMORE,"smine","Shrapnel Claymore",s::DEMOLITIONS,&m_plant_shrapnel_claymore},
			{CORROSIVE_CLAYMORE,"cmine","Corrosive Claymore",s::DEMOLITIONS,&m_plant_corrosive_claymore},
			{XRAY_SHOT,"xray","X-Ray Shot",s::SNIPING,&m_xray_shot},
			{REQUEST_RECON,"recon","Request Recon",s::INTELLIGENCE,&m_request_recon},
		};
	}
	obj_ptr_t sniper::underbarrel() {
		if(m_shotgun_ub.is_attached()) {
			return m_shotgun_ub.obj();
		}
		if(m_frag_ub.is_attached()) {
			return m_frag_ub.obj();
		}
		return nullptr;
	}

	std::tuple<bool,std::string> sniper::detach_shotgun_underbarrel() {
		return m_shotgun_ub.detach();
	}

	std::tuple<bool,std::string> sniper::attach_shotgun_underbarrel() {
		if(m_shotgun_ub.is_attached() && m_shotgun_ub.ammo()) {
			return {1,"Already attached. To detach, use 'detach_shotgun_underbarrel'."};
		}
		auto s = roll_skill_success(UB_SHOTGUN);
		if(std::get<0>(s)) {
			return m_shotgun_ub.attach_to(m_player->primary(),tier(m_player));
		}
		return s;
	}
	void sniper::unblock_healing() {
		m_player->sendln("Unblock healing");
		if(m_heal_mode == HEAL_MODE_SUTURE) {
			m_suture.use_skill(m_player);
		}
	}
	void sniper::use_claymore(uuid_t object_uuid) {

	}
	std::tuple<bool,std::string> sniper::suture() {
		static const std::array<uint8_t,3> suture_tier_duration = {
			SUTURE_TIER_ONE_TICKS(),
			SUTURE_TIER_TWO_TICKS(),
			SUTURE_TIER_THREE_TICKS()
		};
		std::size_t tier = mods::levels::player_tier(m_player) - 1;
		std::size_t index = std::clamp(tier,(std::size_t)0,(std::size_t)2);
		m_heal_mode = HEAL_MODE_SUTURE;
		auto s = roll_skill_success(SUTURE);
		if(std::get<0>(s)) {
			s = mods::medic::set_is_suturing(m_player,suture_tier_duration[index]);
			if(!std::get<0>(s)) {
				return s;
			}
			--m_medkit_count;
			return s;
		}
		return s;
	}
	void sniper::consume_shotgun_underbarrel_ammo() {
		m_shotgun_ub.consume_ammo();
	}
	void sniper::replenish_notify(std::string_view msg) {
		if(m_preferences["mute-replenish"]) {
			return;
		}
		m_player->sendln(msg.data());
	}
	std::tuple<bool,std::string,obj_ptr_t> sniper::build_claymore() {
		if(m_claymore_charges == 0) {
			return {false,"You don't have any claymore charges!",nullptr};
		}
		--m_claymore_charges;
		return {true,"A claymore charge is built",create_object(ITEM_EXPLOSIVE,"claymore-mine.yml")};
	}

	uint8_t sniper::claymore_count() const {
		return m_claymore_charges;
	}
	void sniper::replenish() {
		static uint16_t call_count = 0;
		++call_count;
		auto tier = tier(m_player);
		if((call_count % SNIPER_REPLENISH_PULSE() / tier) == 0) {
			if(m_claymore_charges < SNIPER_CLAYMORE_MAX_COUNT() * tier) {
				++m_claymore_charges;
				replenish_notify("{grn}A sniper class claymore mine has been regenerated.{/grn}");
			}
			if(m_xray_shot_charges < SNIPER_XRAY_SHOT_MAX_COUNT() * tier) {
				replenish_notify("{grn}A sniper class X-Ray shot charge has been regenerated.{/grn}");
				++m_xray_shot_charges;
			}
			if(m_tracking_shot_charges < SNIPER_TRACKING_SHOT_MAX_COUNT() * tier) {
				replenish_notify("{grn}A sniper class Tracking Shot charge has been regenerated.{/grn}");
				++m_tracking_shot_charges;
			}
			if(m_gauze_count < SNIPER_GAUZE_MAX_COUNT() * tier) {
				replenish_notify("{grn}A gauze has been regenerated.{/grn}");
				++m_gauze_count;
			}
		}
	}
	int16_t sniper::new_player(player_ptr_t& player) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			report(CAT("unable to initialize row for player (sniper::new_player) ",(player->db_id()),".. player's database id is zero!"));
			return -2;
		}
		player->set_db_id(db_id);
		load_by_player(player);
		return 0;
	}
	int16_t sniper::load_by_player(player_ptr_t& player) {
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("unable to load sniper class by player id: ", player->db_id(), ".. return status: ", result));
			return -100 - result;
		}
		initialize_skills_for_player(m_player);
		return result;
	}
	int16_t sniper::save() {
		return m_orm.save();
	}
	/*
	- Ability: X-Ray Shot (can shoot through multiple layers of walls/doors)
		- Can snipe an enemy within a building
		- Can snipe an enemy through walls or doors
		- If enemy not behind cover, it causes 150% damage to target
		*/
	std::tuple<bool,std::string> sniper::xray_shot() {
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
		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
		                    m_player,
		                    weapon,
		                    victim,
		                    distance,
		                    direction
		                );
		return {1,"stub"};
	}
	void sniper::target_died(uuid_t target) {
		m_target = 0;
		m_engaged = 0;
		m_player->sendln("Your target died.");
	}
	std::tuple<bool,std::string> sniper::mark_target(std::string_view target) {
		m_target = mods::examine::find_player_by_name(m_player, target);
		if(m_target == 0) {
			return {0,"Couldn't find a target that matches that string."};
		}
		return {1,"Marked target"};
	}

	std::tuple<bool,std::string> sniper::engage() {
		if(m_target == 0) {
			return {0,"You have not marked a target yet!"};
		}
		auto ptr = ptr_by_uuid(m_target);
		if(!ptr) {
			m_target = 0;
			return {0,"That target doesn't exist anymore."};
		}
		m_engaged = 1;
		return {1,CAT("Engaged target: '",ptr->name().c_str(),"'")};
	}
	std::tuple<bool,std::string> sniper::disengage() {
		m_target = 0;
		m_engaged = 0;
		return {1,"You disengage with your target."};
	}

	std::tuple<bool,std::string> sniper::tracking_shot(std::string_view target, direction_t direction) {
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

					--m_tracking_shot_charges;
					mods::affects::affect_player_for(mods::affects::to_affect({"tracked"}),ptr(result.ch),SKILL_SNIPER_TRACKING_SHOT_TICKS());
					return {1,"You tag your target!"};
				}
			}
		}
		return {0,"You couldn't tag your target!"};
	}
	std::shared_ptr<sniper> create_sniper(player_ptr_t& in_player) {
		return std::move(std::make_shared<sniper>(in_player));
	}
	std::tuple<bool,std::string> sniper::light_bandage() {
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
	std::tuple<bool,std::string> sniper::attach_frag_underbarrel() {
		if(m_frag_ub.is_attached() && m_frag_ub.ammo()) {
			return {1,"Already attached. To detach, use 'detach_frag_underbarrel'."};
		}
		auto s = roll_skill_success(UB_FRAG);
		if(std::get<0>(s)) {
			return m_frag_ub.attach_to(m_player->primary(),tier(m_player));
		}
		return s;
	}
	std::tuple<bool,std::string> sniper::detach_frag_underbarrel() {
		return m_frag_ub.detach();
	}
	std::tuple<bool,std::string> sniper::fire_frag(const direction_t& direction,const uint8_t& distance) {
		return m_frag_ub.fire(m_player,direction,distance);
	}
};

namespace mods::class_abilities::sniper {
	ACMD(do_light_bandage) {
		PLAYER_CAN("sniper.light_bandage");
		DO_HELP("light_bandage");
		auto status = player->sniper()->light_bandage();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_build_claymore) {
		PLAYER_CAN("sniper.build_claymore");
		DO_HELP("build_claymore");
		auto status = player->sniper()->build_claymore();
		if(std::get<0>(status)) {
			player->carry(std::get<2>(status));
		}
		player->sendln(std::get<1>(status));
	};
	ACMD(do_xray_shot) {
		PLAYER_CAN("sniper.xray_shot");
		DO_HELP("xray_shot");
		auto status = player->sniper()->xray_shot();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_mark_target) {
		PLAYER_CAN("sniper.mark_target");
		DO_HELP("mark_target");
		static constexpr const char* usage = "Usage: mark_target <target>";
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->sendln(usage);
			return;
		}
		auto status = player->sniper()->mark_target(vec_args[0]);
		player->sendln(std::get<1>(status));
	};
	ACMD(do_tracking_shot) {
		PLAYER_CAN("sniper.tracking_shot");
		DO_HELP("tracking_shot");
		static constexpr const char* usage = "Usage: tracking_shot <target> <direction>";
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}
		direction_t dir = mods::util::parse_direction(vec_args[1]);
		auto status = player->sniper()->tracking_shot(vec_args[0],dir);
		player->sendln(std::get<1>(status));
	};
	ACMD(do_engage) {
		PLAYER_CAN("sniper.engage");
		DO_HELP("engage");
		auto status = player->sniper()->engage();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_disengage) {
		PLAYER_CAN("sniper.disengage");
		DO_HELP("disengage");
		auto status = player->sniper()->disengage();
		player->sendln(std::get<1>(status));
	};
	ACMD(do_attach_shotgun_underbarrel) {
		PLAYER_CAN("sniper.attach_shotgun_underbarrel");
		DO_HELP("attach_shotgun_underbarrel");
		if(player->sniper()) {
			auto status = player->sniper()->attach_shotgun_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_detach_shotgun_underbarrel) {
		PLAYER_CAN("sniper.detach_shotgun_underbarrel");
		DO_HELP("detach_shotgun_underbarrel");
		if(player->sniper()) {
			auto status = player->sniper()->detach_shotgun_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_attach_frag_underbarrel) {
		PLAYER_CAN("sniper.attach_frag_underbarrel");
		DO_HELP("attach_frag_underbarrel");
		if(player->sniper()) {
			auto status = player->sniper()->attach_frag_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_detach_frag_underbarrel) {
		PLAYER_CAN("sniper.detach_frag_underbarrel");
		DO_HELP("detach_frag_underbarrel");
		if(player->sniper()) {
			auto status = player->sniper()->detach_frag_underbarrel();
			player->sendln(std::get<1>(status));
			return;
		}
		player->sendln("Your class does not support this.");
	};
	ACMD(do_fire_frag) {
		PLAYER_CAN("sniper.fire_frag");
		DO_HELP("fire_frag");
		static constexpr const char* usage = "fire_frag <direction> <distance>\r\n";
		if(argshave()->size_gt(2)->passed() == false) {
			player->sendln(usage);
			return;
		}
		auto dir = mods::util::to_direction(argat(1));
		if(dir < 0) {
			player->sendln("Invalid direction.");
			return;
		}
		auto distance = mods::util::stoi_optional<uint8_t>(argat(2));
		if(distance.has_value() == false) {
			player->sendln("Invalid distance.");
			return;
		}
		if(player->sniper()) {
			player->sendln(std::get<1>(player->sniper()->fire_frag(dir,distance.value())));
			return;
		}
		player->sendln("Your class does not support this.");
	};

	void init() {
		mods::interpreter::add_command("mark", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("mark_target", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("tracking_shot", POS_RESTING, do_tracking_shot, 0,0);
		mods::interpreter::add_command("engage", POS_RESTING, do_engage, 0,0);
		mods::interpreter::add_command("disengage", POS_RESTING, do_disengage, 0,0);
		mods::interpreter::add_command("xray_shot", POS_RESTING, do_xray_shot, 0,0);
		mods::interpreter::add_command("build_claymore", POS_RESTING, do_build_claymore, 0,0);
		mods::interpreter::add_command("light_bandage", POS_RESTING, do_light_bandage, 0,0);
		mods::interpreter::add_command("attach_shotgun_underbarrel", POS_RESTING, do_attach_shotgun_underbarrel, 0,0);
		mods::interpreter::add_command("attach_frag_underbarrel", POS_RESTING, do_attach_frag_underbarrel, 0,0);
		mods::interpreter::add_command("fire_frag", POS_RESTING, do_fire_frag, 0,0);
	}
};
