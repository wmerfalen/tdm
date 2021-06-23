#include "base.hpp"

#include "../weapon.hpp"
#include "../affects.hpp"
#include "ghost.hpp"
#include "sniper.hpp"

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

	skill_t* base::skill_ptr(std::string_view skill) {
		for(const auto& s : get_abilities()) {
			std::cerr << s.dump() << "\n" <<
			          "skill.data:'" << skill.data() << "'\n";
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

	std::tuple<bool,std::string> base::roll_skill_success(std::string_view skill) {
		auto ptr = skill_ptr(skill);
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
		for(const auto& s : get_abilities()) {
			org[s.category].emplace_back(CAT(s.pretty,"{->}(",s.shortened,") [ ",s.skill_ptr->get_proficiency()," ]\r\n"));
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
		return mods::util::mail_format("Your skills","Proficiencies",page,get_player_ptr()->screen_width());
	}

	void base::report(std::vector<std::string> msgs) {
		mods::bugs::fixtures(mods::classes::to_string(this->kind()), IMPLODE(msgs,""));
	}
	void base::report(std::string_view msg) {
		mods::bugs::fixtures(mods::classes::to_string(this->kind()), msg.data());
	}
	void unblock_event(uint32_t unblock_event,uuid_t player_uuid) {
		auto player = ptr_by_uuid(player_uuid);
		switch(unblock_event) {
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
