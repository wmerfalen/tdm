#include "base.hpp"

#include "../weapon.hpp"
#include "../affects.hpp"
#include "ghost.hpp"

namespace mods::classes {
	float skill_t::get_increment() {
		return 0.5;
	}
	float skill_t::get_usage_increment() {
		return 1.0;
	}
	std::tuple<bool,std::string> skill_t::load_skill_by_player(std::string_view skill_name, player_ptr_t& player) {
		m_db_id = 0;
		m_name = skill_name;
		m_current_level = 0.0;
		m_loaded_properly = false;
		m_load_status = load(player,skill_name);
		return m_load_status;
	}
	const float& skill_t::get_current_level() const {
		return m_current_level;
	}
	void skill_t::use_skill(player_ptr_t& player) {
		m_current_level += get_usage_increment();
		save(player);
	}
	std::string skill_t::dump() {
		std::string d;
		d = CAT("m_db_id: ",m_db_id,"\r\n",
		        "m_name:'",m_name,"'\r\n",
		        "m_current_level:",m_current_level,"\r\n",
		        "m_loaded_properly:",m_loaded_properly,"\r\n",
		        "m_load_status:",std::get<1>(m_load_status),"\r\n"
		    );
		return d;
	}
	std::tuple<bool,std::string> skill_t::practice(player_ptr_t& player) {
		m_current_level += get_increment();
		auto s = save(player);
		if(std::get<0>(s)) {
			return {1,"You practice for awhile..."};
		} else {
			log(CAT("SYSERR: practicing for player: ",player->name().c_str(),": ",red_str(std::get<1>(s))).c_str());
			return {0,"You can't seem to practice right now."};
		}
	}
	std::map<std::string,std::string>& skill_t::export_class() {
		m_data.clear();
		m_data["skill_name"] = m_name;
		m_data["skill_level"] = std::to_string(m_current_level);
		return m_data;
	}

	bool skill_t::not_learned() const {
		return m_current_level == 0;
	}

	bool skill_t::terrible() const {
		return m_current_level > NOT_LEARNED && m_current_level <= AWFUL;
	}
	bool skill_t::awful() const {
		return m_current_level > AWFUL && m_current_level <= OKAY;
	}
	bool skill_t::okay() const {
		return m_current_level > OKAY && m_current_level <= LEARNED;
	}
	bool skill_t::learned() const {
		return m_current_level > LEARNED && m_current_level <= MASTER;
	}
	bool skill_t::mastered() const {
		return m_current_level > MASTER && m_current_level <= ELITE;
	}
	bool skill_t::elite() const {
		return m_current_level >= ELITE;
	}
	skill_t::proficiency_t skill_t::get_proficiency_enum() const {
		if(not_learned()) {
			return PROF_NOT_LEARNED;
		}
		if(terrible()) {
			return PROF_TERRIBLE;
		}
		if(awful()) {
			return PROF_AWFUL;
		}
		if(okay()) {
			return PROF_OKAY;
		}
		if(learned()) {
			return PROF_LEARNED;
		}
		if(mastered()) {
			return PROF_MASTERED;
		}
		if(elite()) {
			return PROF_ELITE;
		}
		return PROF_NOT_LEARNED;
	}

	std::string skill_t::get_proficiency() const {
		if(not_learned()) {
			return "not-learned";
		}
		if(terrible()) {
			return "terrible";
		}
		if(awful()) {
			return "awful";
		}
		if(okay()) {
			return "okay";
		}
		if(learned()) {
			return "learned";
		}
		if(mastered()) {
			return "mastered";
		}
		if(elite()) {
			return "elite";
		}
		return "not-learned";
	}
	int16_t skill_t::feed(const pqxx::result::reference& row) {
		m_name = row["skill_name"].c_str();
		m_current_level = row["skill_level"].as<float>();
		return 0;
	}
	std::tuple<bool,std::string> skill_t::load(player_ptr_t& player,std::string_view skill_name) {
		try {
			auto check_txn = txn();
			auto check_sql = sql_compositor(table_name,&check_txn)
			    .select("*")
			    .from(table_name)
			    .where("player_id",player->db_id())
			    .op_and("skill_name",skill_name)
			    .sql();
			auto check_result = mods::pq::exec(check_txn,check_sql);
			mods::pq::commit(check_txn);
			if(check_result.size()) {
				m_current_level = check_result[0]["skill_level"].as<float>();
				m_db_id = check_result[0]["id"].as<uint64_t>();
				m_loaded_properly = true;
			} else {
				/* insert the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				    .insert()
				    .into(table_name)
				.values({
					{"skill_name",skill_name.data()},
					{"skill_level","0.0"},
					{"player_id",std::to_string(player->db_id())}
				})
				.returning("id")
				.sql();
				auto row = mods::pq::exec(t,sql);
				mods::pq::commit(t);
				if(row.size()) {
					m_db_id = row[0]["id"].as<uint64_t>();
					m_loaded_properly = true;
				} else {
					m_db_id = 0;
					m_loaded_properly = false;
					return {false,"pkid not returned!"};
				}
			}
		} catch(const std::exception& e) {
			return {false,e.what()};
		}
		return {true,"success"};
	}
	std::tuple<bool,std::string> skill_t::save(player_ptr_t& player) {
		try {
			auto check_txn = txn();
			auto check_sql = sql_compositor(table_name,&check_txn)
			    .select("id")
			    .from(table_name)
			    .where("player_id",player->db_id())
			    .op_and("skill_name",m_name)
			    .sql();
			auto check_result = mods::pq::exec(check_txn,check_sql);
			mods::pq::commit(check_txn);
			if(check_result.size()) {
				/* update the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				    .update(table_name)
				.set({{"skill_level",std::to_string(m_current_level)}})
				.where("id",check_result[0]["id"].c_str())
				.sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			} else {
				/* insert the record */
				auto t = txn();
				auto sql = sql_compositor(table_name,&t)
				    .insert()
				    .into(table_name)
				.values({
					{"skill_name",m_name},
					{"skill_level",std::to_string(m_current_level)},
					{"player_id",std::to_string(player->db_id())}
				})
				.sql();
				mods::pq::exec(t,sql);
				mods::pq::commit(t);
			}
		} catch(const std::exception& e) {
			return {false,e.what()};
		}
		return {true,"success"};
	}

	std::string base::ability_data_t::dump() const {
		return CAT("ability_value:",ability_value,"\r\n",
		        "shortened:'",shortened,"'\r\n",
		        "pretty:'",pretty,"'\r\n",
		        "category:",category,"\r\n"
		    );
	}

	skill_t* base::skill_ptr(uint8_t skill) {
		for(const auto& s : get_abilities()) {
			if(s.ability_value == skill) {
				return s.skill_ptr;
			}
		}
		return nullptr;
	}
	skill_t* base::skill_ptr(std::string_view skill) {
		for(const auto& s : get_abilities()) {
			if(s.shortened.compare(skill.data()) == 0) {
				return s.skill_ptr;
			}
		}
		return nullptr;
	}

	std::tuple<bool,std::string> base::practice(std::string_view skill) {
		auto s = skill_ptr(skill.data());
		if(!s) {
			return {false,"Which skill?"};
		}
		auto player_ptr = get_player_ptr();
		return s->practice(player_ptr);
	}

	std::tuple<bool,std::string> base::roll_skill_success(uint8_t skill) {
		auto ptr = skill_ptr(skill);
		if(!ptr) {
			return {false,"Which skill?"};
		}
		if(ptr->not_learned()) {
			return {false,"You have no idea how to do that!"};
		}
		if(ptr->terrible()) {
			return {mods::rand::chance(TERRIBLE_SKILL_CHANCE_OF_SUCCESS()),"You attempt to, but fail miserably..."};
		}
		if(ptr->awful()) {
			return {mods::rand::chance(AWFUL_SKILL_CHANCE_OF_SUCCESS()),"You try, but fail..."};
		}
		if(ptr->okay()) {
			return {mods::rand::chance(OKAY_SKILL_CHANCE_OF_SUCCESS()),"You almost did it..."};
		}
		if(ptr->learned()) {
			return {mods::rand::chance(LEARNED_SKILL_CHANCE_OF_SUCCESS()),"You try your best, but fail miserably..."};
		}
		if(ptr->mastered()) {
			return {mods::rand::chance(MASTERED_SKILL_CHANCE_OF_SUCCESS()),"You masterfully get to work"};
		}
		if(ptr->elite()) {
			return {mods::rand::chance(ELITE_SKILL_CHANCE_OF_SUCCESS()),"You work with precision"};
		}
		return {false,"You have no idea how to do that. Maybe you should practice!"};
	}
	void base::initialize_skills_for_player(player_ptr_t& player) {
		for(auto& s : get_abilities()) {
			s.skill_ptr->load_skill_by_player(s.shortened,player);
		}
	}
	std::string base::shorthand_page() {
		std::string how_to = "How to practice each skill:\r\n";
		for(const auto& s : get_abilities()) {
			how_to += CAT("To practice ",s.pretty," use:\r\npractice ",s.shortened,"\r\n");
		}
		return mods::util::mail_format("Your skills","How to practice each one of your skills",how_to,get_player_ptr()->screen_width());
	}

	std::string base::request_page_for(std::string_view page) {
		using namespace mods::util;
		if(is_lower_match(page,"skills") || is_lower_match(page,"skill")) {
			return skills_page();
		}
		if(is_lower_match(page,"shorthand")) {
			return shorthand_page();
		}
		return "Unknown page requested";
	}


	std::string base::skills_page() {
		std::map<ability_data_t::skillset_t,std::vector<std::string>> org;
		std::string proficiency_string;
		for(const auto& s : get_abilities()) {
			proficiency_string.clear();
			using PROF = skill_t::proficiency_t;
			switch(s.skill_ptr->get_proficiency_enum()) {
				default:
				case PROF::PROF_NOT_LEARNED:
					proficiency_string = "not-learned";
					break;
				case PROF::PROF_TERRIBLE:
					proficiency_string = "terrible";
					break;
				case PROF::PROF_AWFUL:
					proficiency_string = "awful";
					break;
				case PROF::PROF_OKAY:
					proficiency_string = "okay";
					break;
				case PROF::PROF_LEARNED:
					proficiency_string= "learned";
					break;
				case PROF::PROF_MASTERED:
					proficiency_string = "mastered";
					break;
				case PROF::PROF_ELITE :
					proficiency_string = "ELITE";
					break;
			}
			org[s.category].emplace_back(CAT(s.pretty,"{->}(",s.shortened,") [ ",proficiency_string," ]\r\n"));
		}
		std::string page;

		using sk = ability_data_t::skillset_t;
		for(const auto& pair : org) {
			switch(pair.first) {
				case sk::SNIPING:
					page += "Sniping:{hr}";
					break;
				case sk::STRENGTH:
					page += "Strength:{hr}";
					break;
				case sk::INTELLIGENCE:
					page += "Intelligence:{hr}";
					break;
				case sk::DEXTERITY:
					page += "Dexterity:{hr}";
					break;
				case sk::CONSTITUTION:
					page += "Constitution:{hr}";
					break;
				case sk::ELECTRONICS:
					page += "Electronics:{hr}";
					break;
				case sk::ARMOR:
					page += "Armor:{hr}";
					break;
				case sk::MARKSMANSHIP:
					page += "Marksmanship:{hr}";
					break;
				case sk::DEMOLITIONS:
					page += "Demolitions:{hr}";
					break;
				case sk::CHEMISTRY:
					page += "Chemistry:{hr}";
					break;
				case sk::WEAPON_HANDLING:
					page += "Weapon Handling:{hr}";
					break;
				case sk::STRATEGY:
					page += "Strategy:{hr}";
					break;
				case sk::MEDICAL:
					page += "Medical:{hr}";
					break;
				default:
					page += "Other:{hr}";
					break;
			}
			for(const auto& str : pair.second) {
				page += str;
			}
			page += "\r\n{hr}";
		}
		page += CAT("{hr}\r\n",
		        "The strings in parenthesis are what you would pass to the practice or train command. ",
		        "{hr}\r\nFor example, to train ",get_abilities().begin()->pretty," you would type: practice ",get_abilities().begin()->shortened,"\r\n"
		    );
		return page;
	}
	base::ability_list_t& base::weapon_abilities() {
		using skillset_t = ability_data_t::skillset_t;
		m_weapon_abilities = {
			{ASSAULT_RIFLES,"wpn-ar","Assault Rifles",skillset_t::WEAPON_HANDLING,&m_wpn_assault_rifles,0},
			{SNIPER_RIFLES,"wpn-sniper","Sniper Rifles",skillset_t::WEAPON_HANDLING,&m_wpn_sniper_rifles,0},
			{SUB_MACHINE_GUNS,"wpn-smg","Sub Machine Guns",skillset_t::WEAPON_HANDLING,&m_wpn_sub_machine_guns,0},
			{SHOTGUNS,"wpn-shotguns","Shotguns",skillset_t::WEAPON_HANDLING,&m_wpn_shotguns,0},
			{PISTOLS,"wpn-pistols","Pistols",skillset_t::WEAPON_HANDLING,&m_wpn_pistols,0},
			{MACHINE_PISTOLS,"wpn-mp","Machine Pistols",skillset_t::WEAPON_HANDLING,&m_wpn_machine_pistols,0},
			{LIGHT_MACHINE_GUNS,"wpn-lmg","Light Machine Guns",skillset_t::WEAPON_HANDLING,&m_wpn_light_machine_guns,0},
		};
		return m_weapon_abilities;
	}
	base::ability_list_t& base::cqc_abilities() {
		using skillset_t = ability_data_t::skillset_t;
		m_cqc_abilities = {
			{JAB_TO_HEAD,"cqc-jab-to-head","Jab to head",skillset_t::STRENGTH,&m_cqc_jab_to_head,0},
			{JAB_TO_BODY,"cqc-jab-to-body","Jab to body",skillset_t::STRENGTH,&m_cqc_jab_to_body,0},
			{CROSS_TO_HEAD,"cqc-cross-to-head","Cross to head",skillset_t::STRENGTH,&m_cqc_cross_to_head,0},
			{CROSS_TO_BODY,"cqc-cross-to-body","Cross to body",skillset_t::STRENGTH,&m_cqc_cross_to_body,0},
			{LEFT_HOOK_TO_HEAD,"cqc-left-hook-to-head","Left hook to head",skillset_t::STRENGTH,&m_cqc_left_hook_to_head,0},
			{RIGHT_HOOK_TO_HEAD,"cqc-right-hook-to-head","Right hook to head",skillset_t::STRENGTH,&m_cqc_right_hook_to_head,0},
			{LEFT_HOOK_TO_BODY,"cqc-left-hook-to-body","Left hook to body",skillset_t::STRENGTH,&m_cqc_left_hook_to_body,0},
			{RIGHT_HOOK_TO_BODY,"cqc-right-hook-to-body","Right hook to body",skillset_t::STRENGTH,&m_cqc_right_hook_to_body,0},
			{LEFT_UPPERCUT,"cqc-left-uppercut","Left uppercut",skillset_t::STRENGTH,&m_cqc_left_uppercut,0},
			{RIGHT_UPPERCUT,"cqc-right-uppercut","Right uppercut",skillset_t::STRENGTH,&m_cqc_right_uppercut,0},
			{LEFT_ELBOW,"cqc-left-elbow","Left elbow",skillset_t::STRENGTH,&m_cqc_left_elbow,0},
			{RIGHT_ELBOW,"cqc-right-elbow","Right elbow",skillset_t::STRENGTH,&m_cqc_right_elbow,0},
			{RIGHT_UPWARD_ELBOW,"cqc-right-upward-elbow","Right upward elbow",skillset_t::STRENGTH,&m_cqc_right_upward_elbow,0},
			{LEFT_UPWARD_ELBOW,"cqc-left-upward-elbow","Left upward elbow",skillset_t::STRENGTH,&m_cqc_left_upward_elbow,0},
			{RIGHT_OBLIQUE,"cqc-right-oblique","Right oblique",skillset_t::STRENGTH,&m_cqc_right_oblique,0},
			{LEFT_OBLIQUE,"cqc-left-oblique","Left oblique",skillset_t::STRENGTH,&m_cqc_left_oblique,0},
			{LEFT_TEEP,"cqc-left-teep","Left teep",skillset_t::STRENGTH,&m_cqc_left_teep,0},
			{RIGHT_TEEP,"cqc-right-teep","Right teep",skillset_t::STRENGTH,&m_cqc_right_teep,0},
			{LEFT_FRONT_KICK,"cqc-left-front-kick","Left front kick",skillset_t::STRENGTH,&m_cqc_left_front_kick,0},
			{RIGHT_FRONT_KICK,"cqc-right-front-kick","Right front kick",skillset_t::STRENGTH,&m_cqc_right_front_kick,0},
			{LEFT_KNEE_TO_HEAD,"cqc-left-knee-to-head","Left knee to head",skillset_t::STRENGTH,&m_cqc_left_knee_to_head,0},
			{RIGHT_KNEE_TO_HEAD,"cqc-right-knee-to-head","Right knee to head",skillset_t::STRENGTH,&m_cqc_right_knee_to_head,0},
			{LEFT_KNEE_TO_BODY,"cqc-left-knee-to-body","Left knee to body",skillset_t::STRENGTH,&m_cqc_left_knee_to_body,0},
			{RIGHT_KNEE_TO_BODY,"cqc-right-knee-to-body","Right knee to body",skillset_t::STRENGTH,&m_cqc_right_knee_to_body,0},
			{KNIFE_DISARM,"cqc-knife-disarm","Knife disarm",skillset_t::STRENGTH,&m_cqc_knife_disarm,0},
			{PISTOL_DISARM,"cqc-pistol-disarm","Pistol disarm",skillset_t::STRENGTH,&m_cqc_pistol_disarm,0},
			{RIGHT_LEG_KICK,"cqc-right-leg-kick","Right leg kick",skillset_t::STRENGTH,&m_cqc_right_leg_kick,0},
			{LEFT_LEG_KICK,"cqc-left-leg-kick","Left leg kick",skillset_t::STRENGTH,&m_cqc_left_leg_kick,0},
			{RIGHT_KICK_TO_HEAD,"cqc-right-kick-to-head","Right kick to head",skillset_t::STRENGTH,&m_cqc_right_kick_to_head,0},
			{LEFT_KICK_TO_HEAD,"cqc-left-kick-to-head","Left kick to head",skillset_t::STRENGTH,&m_cqc_left_kick_to_head,0},
			{RIGHT_STOMP_TO_HEAD,"cqc-right-stomp-to-head","Right stomp to head",skillset_t::STRENGTH,&m_cqc_right_stomp_to_head,0},
			{LEFT_STOMP_TO_HEAD,"cqc-left-stomp-to-head","Left stomp to head",skillset_t::STRENGTH,&m_cqc_left_stomp_to_head,0},
		};
		return m_cqc_abilities;
	}
	base::ability_list_t base::create_abilities(ability_list_t additional_list) {
		additional_list.insert(additional_list.end(),weapon_abilities().begin(),weapon_abilities().end());
		additional_list.insert(additional_list.end(),cqc_abilities().begin(),cqc_abilities().end());
		return additional_list;
	}

	void base::report(std::vector<std::string> msgs) {
		mods::bugs::fixtures(mods::classes::to_string(this->kind()), IMPLODE(msgs,""));
	}
	void base::report(std::string_view msg) {
		mods::bugs::fixtures(mods::classes::to_string(this->kind()), msg.data());
	}
	void unblock_event(uint32_t unblock_event,uuid_t player_uuid) {
		auto player = ptr_by_uuid(player_uuid);
		if(!player) {
			return;
		}
		switch(unblock_event) {
			case mods::deferred::EVENT_PLAYER_AERIAL_DRONE_SCAN_OVER:
				if(player->ghost()) {
					player->ghost()->set_aerial_drone_scan(false);
				}
				break;
			case mods::deferred::EVENT_PLAYER_UNBLOCK_ADRENALINE_SHOT_SMALL:
			case mods::deferred::EVENT_PLAYER_UNBLOCK_ADRENALINE_SHOT_MEDIUM:
			case mods::deferred::EVENT_PLAYER_UNBLOCK_ADRENALINE_SHOT_LARGE: {
					if(player->ghost()) {
						player->ghost()->unblock_adrenaline_shot();
					}
					break;
				}
			case mods::deferred::EVENT_PLAYER_UNBLOCK_HEALING:
				if(player->ghost()) {
					player->ghost()->unblock_healing();
				}
				break;
			case mods::deferred::EVENT_PLAYER_FINISHES_FEIGN_DEATH:
				player->ghost()->feign_death_done();
				break;
			case mods::deferred::EVENT_PLAYER_GOES_VISIBLE:
				player->ghost()->dissipate_wears_off();
				break;
			default:
				break;
		}
	}
};
