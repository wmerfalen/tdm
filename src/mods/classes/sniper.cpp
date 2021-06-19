#include "sniper.hpp"
#include "../orm/inventory.hpp"
#include "../weapons/damage-types.hpp"
#include "../interpreter.hpp"
#include "../help.hpp"
#include "../examine.hpp"

namespace mods::classes {
	sniper::sniper() {
		this->init();
	}
	sniper::sniper(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}
	player_ptr_t 	sniper::player() {
		return m_player;
	}
	void sniper::init() {
		m_target = 0;
		m_engaged = 0;
		m_xray_shot_charges = 0;
		m_player = nullptr;
		m_claymore_charges = 0;
		m_tracking_shot_charges = 0;
		m_preferences["mute-replenish"] = true;
	}
	std::tuple<bool,std::string> sniper::roll_skill_success(std::string_view skill) {
		skill_t* ptr = nullptr;
		if(skill.compare("tracking_shot") == 0) {
			ptr = &m_tracking_shot;
		}
		if(skill.compare("light_bandage") == 0) {
			ptr = &m_light_bandage;
		}
		if(skill.compare("suture") == 0) {
			ptr = &m_suture;
		}
		if(skill.compare("adrenaline_shot") == 0) {
			ptr = &m_adrenaline_shot;
		}
		if(skill.compare("emp_nade") == 0) {
			ptr = &m_emp_nade;
		}
		if(skill.compare("chaff_nade") == 0) {
			ptr = &m_chaff_nade;
		}
		if(skill.compare("sensor_nade") == 0) {
			ptr = &m_sensor_nade;
		}
		if(skill.compare("ub_shotgun") == 0) {
			ptr = &m_ub_shotgun;
		}
		if(skill.compare("ub_frag") == 0) {
			ptr = &m_ub_frag;
		}
		if(skill.compare("guided_missile") == 0) {
			ptr = &m_guided_missile;
		}
		if(skill.compare("target_limb") == 0) {
			ptr = &m_target_limb;
		}
		if(skill.compare("target_limb") == 0) {
			ptr = &m_target_limb;
		}
		if(skill.compare("plant_claymore") == 0) {
			ptr = &m_plant_claymore;
		}
		if(skill.compare("plant_shrapnel_claymore") == 0) {
			ptr = &m_plant_shrapnel_claymore;
		}
		if(skill.compare("plant_corrosive_claymore") == 0) {
			ptr = &m_plant_corrosive_claymore;
		}
		if(skill.compare("xray_shot") == 0) {
			ptr = &m_xray_shot;
		}
		if(skill.compare("request_recon") == 0) {
			ptr = &m_request_recon;
		}
		if(!ptr) {
			return {false,"Which skill?"};
		}
		if(ptr->not_learned()) {
			return {false,"You have no idea how to do that!"};
		}
		if(ptr->terrible()) {
			return {mods::rand::chance(TERRIBLE_SKILL_CHANCE_OF_SUCCESS()),"You try your best"};
		}
		if(ptr->awful()) {
			return {mods::rand::chance(AWFUL_SKILL_CHANCE_OF_SUCCESS()),"You try your best"};
		}
		if(ptr->okay()) {
			return {mods::rand::chance(OKAY_SKILL_CHANCE_OF_SUCCESS()),"You try your best"};
		}
		if(ptr->learned()) {
			return {mods::rand::chance(LEARNED_SKILL_CHANCE_OF_SUCCESS()),"You try your best"};
		}
		if(ptr->mastered()) {
			return {mods::rand::chance(MASTERED_SKILL_CHANCE_OF_SUCCESS()),"You masterfully get to work"};
		}
		if(ptr->elite()) {
			return {mods::rand::chance(ELITE_SKILL_CHANCE_OF_SUCCESS()),"You work with precision"};
		}
		return {false,"You have no idea how to do that. Maybe you should practice!"};
	}
	std::tuple<bool,std::string> sniper::practice(std::string_view skill) {
		if(skill.compare("tracking_shot") == 0) {
			m_tracking_shot.practice(m_player);
		}
		if(skill.compare("light_bandage") == 0) {
			m_light_bandage.practice(m_player);
		}
		if(skill.compare("suture") == 0) {
			m_suture.practice(m_player);
		}
		if(skill.compare("adrenaline_shot") == 0) {
			m_adrenaline_shot.practice(m_player);
		}
		if(skill.compare("emp_nade") == 0) {
			m_emp_nade.practice(m_player);
		}
		if(skill.compare("chaff_nade") == 0) {
			m_chaff_nade.practice(m_player);
		}
		if(skill.compare("sensor_nade") == 0) {
			m_sensor_nade.practice(m_player);
		}
		if(skill.compare("ub_shotgun") == 0) {
			m_ub_shotgun.practice(m_player);
		}
		if(skill.compare("ub_frag") == 0) {
			m_ub_frag.practice(m_player);
		}
		if(skill.compare("guided_missile") == 0) {
			m_guided_missile.practice(m_player);
		}
		if(skill.compare("target_limb") == 0) {
			m_target_limb.practice(m_player);
		}
		if(skill.compare("target_limb") == 0) {
			m_target_limb.practice(m_player);
		}
		if(skill.compare("plant_claymore") == 0) {
			m_plant_claymore.practice(m_player);
		}
		if(skill.compare("plant_shrapnel_claymore") == 0) {
			m_plant_shrapnel_claymore.practice(m_player);
		}
		if(skill.compare("plant_corrosive_claymore") == 0) {
			m_plant_corrosive_claymore.practice(m_player);
		}
		if(skill.compare("xray_shot") == 0) {
			m_xray_shot.practice(m_player);
		}
		if(skill.compare("request_recon") == 0) {
			m_request_recon.practice(m_player);
		}
		return {1,"[stub](sniper)"};
	}
	std::string sniper::tracking_shot_proficiency() {
		return m_tracking_shot.get_proficiency();
	}
	std::string sniper::light_bandage_proficiency() {
		return m_light_bandage.get_proficiency();
	}
	std::string sniper::suture_proficiency() {
		return m_suture.get_proficiency();
	}
	std::string sniper::adrenaline_shot_proficiency() {
		return m_adrenaline_shot.get_proficiency();
	}
	std::string sniper::emp_nade_proficiency() {
		return m_emp_nade.get_proficiency();
	}
	std::string sniper::chaff_nade_proficiency() {
		return m_chaff_nade.get_proficiency();
	}
	std::string sniper::sensor_nade_proficiency() {
		return m_sensor_nade.get_proficiency();
	}
	std::string sniper::ub_shotgun_proficiency() {
		return m_ub_shotgun.get_proficiency();
	}
	std::string sniper::ub_frag_proficiency() {
		return m_ub_frag.get_proficiency();
	}
	std::string sniper::guided_missile_proficiency() {
		return m_guided_missile.get_proficiency();
	}
	std::string sniper::target_limb_proficiency() {
		return m_target_limb.get_proficiency();
	}
	std::string sniper::plant_claymore_proficiency() {
		return m_plant_claymore.get_proficiency();
	}
	std::string sniper::plant_shrapnel_claymore_proficiency() {
		return m_plant_shrapnel_claymore.get_proficiency();
	}
	std::string sniper::plant_corrosive_claymore_proficiency() {
		return m_plant_corrosive_claymore.get_proficiency();
	}
	std::string sniper::xray_shot_proficiency() {
		return m_xray_shot.get_proficiency();
	}
	std::string sniper::request_recon_proficiency() {
		return m_request_recon.get_proficiency();
	}




	float sniper::tracking_shot_level() {
		return m_tracking_shot.get_current_level();
	}
	float sniper::light_bandage_level() {
		return m_light_bandage.get_current_level();
	}
	float sniper::suture_level() {
		return m_suture.get_current_level();
	}
	float sniper::adrenaline_shot_level() {
		return m_adrenaline_shot.get_current_level();
	}
	float sniper::emp_nade_level() {
		return m_emp_nade.get_current_level();
	}
	float sniper::chaff_nade_level() {
		return m_chaff_nade.get_current_level();
	}
	float sniper::sensor_nade_level() {
		return m_sensor_nade.get_current_level();
	}
	float sniper::ub_shotgun_level() {
		return m_ub_shotgun.get_current_level();
	}
	float sniper::ub_frag_level() {
		return m_ub_frag.get_current_level();
	}
	float sniper::guided_missile_level() {
		return m_guided_missile.get_current_level();
	}
	float sniper::target_limb_level() {
		return m_target_limb.get_current_level();
	}
	float sniper::plant_claymore_level() {
		return m_plant_claymore.get_current_level();
	}
	float sniper::plant_shrapnel_claymore_level() {
		return m_plant_shrapnel_claymore.get_current_level();
	}
	float sniper::plant_corrosive_claymore_level() {
		return m_plant_corrosive_claymore.get_current_level();
	}
	float sniper::xray_shot_level() {
		return m_xray_shot.get_current_level();
	}
	float sniper::request_recon_level() {
		return m_request_recon.get_current_level();
	}
	std::string sniper::skills_page() {
		std::string page = CAT("Sniping:{hr}",
		                       "Underbarrel Shotgun{->}[ ",ub_shotgun_proficiency()," ]\r\n",
		                       "Underbarrel Frag Launcher{->}[ ",ub_frag_proficiency()," ]\r\n",
		                       "Target Limb{->}[ ",target_limb_proficiency()," ]\r\n",

		                       "{hr}\r\n",
		                       "Demolitions:{hr}",
		                       "Plant Claymore{->}[ ",plant_claymore_proficiency()," ]\r\n",
		                       "Shrapnel Claymore{->}[ ",plant_shrapnel_claymore_proficiency()," ]\r\n",
		                       "Corrosive Claymore{->}[ ",plant_corrosive_claymore_proficiency()," ]\r\n",
		                       "Guided Missile{->}[ ",guided_missile_proficiency()," ]\r\n",

		                       "{hr}\r\n",
		                       "Technology:{hr}",
		                       "Tracking Shot:{->}[ ",tracking_shot_proficiency(), " ]\r\n",
		                       "Sensor Grenade:{->}[ ",sensor_nade_proficiency(), " ]\r\n",
		                       "X-Ray Shot{->}[ ",xray_shot_proficiency(), " ]\r\n",
		                       "Request Recon{->}[ ",request_recon_proficiency()," ]\r\n",
		                       "Chaff Grenade{->}[ ",chaff_nade_proficiency()," ]\r\n",
		                       "EMP Grenade{->}[ ",emp_nade_proficiency()," ]\r\n",

		                       "{hr}\r\n",
		                       "Medical:{hr}",
		                       "Light Bandage:{->}[ ",light_bandage_proficiency(), " ]\r\n",
		                       "Suture:{->}[ ",suture_proficiency(), " ]\r\n",
		                       "Adrenaline Shot:{->}[ ",adrenaline_shot_proficiency(), " ]\r\n",
		                       "{hr}\r\n",
		                       "How to practice each skill:\r\n"
		                       "example: practice tracking_shot\r\n"
		                       "example: practice light_bandage\r\n"
		                       "example: practice suture\r\n"
		                       "example: practice adrenaline_shot\r\n"
		                       "example: practice emp_nade\r\n"
		                       "example: practice chaff_nade\r\n"
		                       "example: practice sensor_nade\r\n"
		                       "example: practice ub_shotgun\r\n"
		                       "example: practice ub_frag\r\n"
		                       "example: practice guided_missile\r\n"
		                       "example: practice target_limb\r\n"
		                       "example: practice target_limb\r\n"
		                       "example: practice plant_claymore\r\n"
		                       "example: practice plant_shrapnel_claymore\r\n"
		                       "example: practice plant_corrosive_claymore\r\n"
		                       "example: practice xray_shot\r\n"
		                       "example: practice request_recon\r\n"
		                      );
		return mods::util::mail_format("Your skills","Proficiencies",page,m_player->screen_width());
	}
	void sniper::use_claymore(uuid_t object_uuid) {
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
		obj_ptr_t primary = nullptr;
		/** TODO: create catchy name using the deep object parser */
		//primary = create_object(ITEM_RIFLE,"psg1.yml");
		//player->equip(primary,WEAR_PRIMARY);
		//player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);

		m_tracking_shot.load_skill_by_player("tracking_shot",m_player);
		m_light_bandage.load_skill_by_player("light_bandage",m_player);
		m_suture.load_skill_by_player("suture",m_player);
		m_adrenaline_shot.load_skill_by_player("adrenaline_shot",m_player);
		m_emp_nade.load_skill_by_player("emp_nade",m_player);
		m_chaff_nade.load_skill_by_player("chaff_nade",m_player);
		m_sensor_nade.load_skill_by_player("sensor_nade",m_player);
		m_ub_shotgun.load_skill_by_player("ub_shotgun",m_player);
		m_ub_frag.load_skill_by_player("ub_frag",m_player);
		m_guided_missile.load_skill_by_player("guided_missile",m_player);
		m_target_limb.load_skill_by_player("target_limb",m_player);
		m_plant_claymore.load_skill_by_player("plant_claymore",m_player);
		m_plant_shrapnel_claymore.load_skill_by_player("plant_shrapnel_claymore",m_player);
		m_plant_corrosive_claymore.load_skill_by_player("plant_corrosive_claymore",m_player);
		m_xray_shot.load_skill_by_player("xray_shot",m_player);
		m_request_recon.load_skill_by_player("request_recon",m_player);
		return result;
	}
	int16_t sniper::save() {
		m_player->sendln("Saving from sniper class");
		//mods::orm::inventory::flush_player(m_player);
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
		if(!weapon || weapon->rifle()->attributes->type != mw_rifle::SNIPER) {
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
			/*
			 * vec_player_data {
				*	char_data* ch;
				*	obj_data* obj;
				*	uint16_t distance;
				*	uuid_t uuid;
				*	uint8_t direction;
				*	uint32_t room_rnum;
				* }
			*/
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
};

namespace mods::class_abilities::sniper {
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
	void init() {
		mods::interpreter::add_command("mark", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("mark_target", POS_RESTING, do_mark_target, 0,0);
		mods::interpreter::add_command("tracking_shot", POS_RESTING, do_tracking_shot, 0,0);
		mods::interpreter::add_command("engage", POS_RESTING, do_engage, 0,0);
		mods::interpreter::add_command("disengage", POS_RESTING, do_disengage, 0,0);
		mods::interpreter::add_command("xray_shot", POS_RESTING, do_xray_shot, 0,0);
		mods::interpreter::add_command("build_claymore", POS_RESTING, do_build_claymore, 0,0);
	}
};
