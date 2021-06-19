#include "skills.hpp"
#include "interpreter.hpp"
#include "super-users.hpp"
#include <functional>
#include "skill-orm-adaptor.hpp"
#include "orm/skill-trees.hpp"
#include "orm/player-skill-points.hpp"
#include "levels.hpp"
#include "classes/sniper.hpp"
#include "classes/ghost.hpp"

#ifdef __MENTOC_MODS_SKILLS_SHOW_DEBUG_OUTPUT__
#define m_debug(A) std::cerr << "[mods::skills][debug]:'" << A << "'\n";
#else
#define m_debug(A)
#endif

namespace mods::skills {

	std::string pad_string(std::string_view str,std::size_t to, std::string with) {
		std::string padded;
		padded.resize(to);

		for(unsigned i =0; i < to; i++) {
			if(str.length()  > i) {
				padded[i] = str[i];
				continue;
			}
			padded[i] = with[0];
		}
		return padded;
	}

	std::string get_user_skills_page(player_ptr_t& player) {
		if(player->sniper()) {
			return player->sniper()->skills_page();
		}
		if(player->ghost()) {
			return player->ghost()->skills_page();
		}
		return "[stub]";
	}

	std::string get_user_stats_page(player_ptr_t& player) {
		return CAT("{hr}\r\n",
		           "Stats:\r\n",
		           "Armor:            ",player->real_armor(),"\r\n",
		           "Chemistry:        ",player->chemistry(),"\r\n",
		           "Constitution:     ",player->constitution(),"\r\n",
		           "Demolitions:      ",player->demolitions(),"\r\n",
		           "Dexterity:        ",player->dexterity(),"\r\n",
		           "Electronics:      ",player->electronics(),"\r\n",
		           "Intelligence:     ",player->intelligence(),"\r\n",
		           "Marksmanship:     ",player->marksmanship(),"\r\n",
		           "Medical:          ",player->medical(),"\r\n",
		           "Sniping:          ",player->sniping(),"\r\n",
		           "Strength:         ",player->strength(),"\r\n",
		           "Weapon Handling:  ",player->weapon_handling(),"\r\n"
		          );
	}
	std::string get_user_unlocks_page(player_ptr_t& player) {
		return CAT("{hr}\r\n",
		           "Unlocks:\r\n"
		          );
	}



	ACMD(do_score) {
		if(IS_NPC(ch)) {
			return;
		}

		std::string screen = "";
		std::string header = CAT("You have scored ",player->exp()," exp, and have ",player->gold()," mission points.\r\n");

		if(GET_LEVEL(ch) < LVL_IMMORT) {
			header += CAT("You need ", mods::levels::level_exp(player->level() + 1) - player->exp()," exp to reach your next level.\r\n");
		}

		screen += CAT("This ranks you as ",player->name()," ", GET_TITLE(ch).c_str()," (level ", player->level(),")\r\n");
		screen += "{hr}";

		switch(GET_POS(ch)) {
			case POS_DEAD:
				screen += CAT("You are DEAD!\r\n");
				break;

			case POS_MORTALLYW:
				screen += CAT("You are mortally wounded!  You should seek help!\r\n");
				break;

			case POS_INCAP:
				screen += CAT("You are incapacitated, slowly fading away...\r\n");
				break;

			case POS_STUNNED:
				screen += CAT("You are stunned!  You can't move!\r\n");
				break;

			case POS_SLEEPING:
				screen += CAT("You are sleeping.\r\n");
				break;

			case POS_RESTING:
				screen += CAT("You are resting.\r\n");
				break;

			case POS_SITTING:
				screen += CAT("You are sitting.\r\n");
				break;

			case POS_FIGHTING:
				screen += CAT("You are fighting ", FIGHTING(ch) ? std::string(PERS(FIGHTING(ch), ch)).c_str() : "thin air","\r\n");
				break;

			case POS_STANDING:
				screen += CAT("You are standing.\r\n");
				break;

			default:
				screen += CAT("You are floating.\r\n");
				break;
		}

		if(GET_COND(ch, DRUNK) > 10) {
			screen += CAT("You are intoxicated.\r\n");
		}

		if(GET_COND(ch, FULL) == 0) {
			screen += CAT("You are hungry.\r\n");
		}

		if(GET_COND(ch, THIRST) == 0) {
			screen += CAT("You are thirsty.\r\n");
		}

		if(AFF_FLAGGED(ch, AFF_BLIND)) {
			screen += CAT("You have been blinded!\r\n");
		}

		if(AFF_FLAGGED(ch, AFF_INVISIBLE)) {
			screen += CAT("You are invisible.\r\n");
		}

		screen += get_user_skills_page(player);
		screen += get_user_stats_page(player);
		screen += get_user_unlocks_page(player);
		player->sendln(mods::util::mail_format("Your stats",header,screen,player->screen_width()));
	}

	void train_user_skill(player_ptr_t& player,uint32_t skill_id,int16_t amount) {
		mods::orm::skill_trees tree;
		std::string pc = "";
		for(auto ch : player->get_class_string().str()) {
			pc += std::tolower(ch);
		}
		tree.load_by_class(pc);

		mods::orm::player_skill_points ps;
		auto results = ps.get_player_levels(player->db_id(),pc);
		results[skill_id] += amount;
		ps.delete_by_player(player->db_id());
		ps.rows.clear();
		ps.populate(player->db_id(),results);
		ps.save();
	}

	std::optional<uint32_t> from_string_to_skill_id(
	    const std::string&  player_class,
	    std::string_view skill_name) {
		mods::orm::skill_trees tree;
		std::string pc = "";
		for(auto ch : player_class) {
			pc += std::tolower(ch);
		}
		tree.load_by_class(pc);
		for(const auto& row : tree.rows) {
			if(row.skill_name.compare(skill_name.data()) == 0) {
				return row.id;
			}
		}
		return std::nullopt;
	}


	void train_skill(player_ptr_t& player,std::string_view skill_name, int16_t amount) {
		auto id = from_string_to_skill_id(player->get_class_string().str(),skill_name);
		if(!id.has_value()) {
			player->send("That skill doesn't seem to exist: '%s'\r\n",skill_name.data());
			return;
		}
		train_user_skill(player,id.value(),amount);
		player->sendln("You practice for awhile...\r\n");
	}

	uint8_t calculate_available_practice_sessions(uint8_t level) {
		uint8_t sessions = 0;
		for(uint8_t current_level = 1; current_level <= level; ++current_level) {
			sessions += mods::levels::award_practice_sessions(current_level);
		}
		return sessions;
	}
	std::string get_practice_dump() {
		std::string report = "[practice sessions per level]\r\n";
		for(uint8_t i = 1; i <= mods::levels::MAX_PLAYER_LEVEL; ++i) {
			report += CAT("Player level: ",i,": ", calculate_available_practice_sessions(i), "\r\n");
		}
		return report;
	}
	ACMD(do_practice_sessions) {
		player->sendln(get_practice_dump());
	}
	ACMD(do_train) {
		DO_HELP("train");
		static constexpr const char* usage = "usage: train <skill> <amount>";
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->sendln(get_user_skills_page(player));
			return;
		}

		if(vec_args.size() >= 2) {
			auto i = mods::util::stoi_optional<int16_t>(vec_args[1]);
			if(!i.has_value()) {
				player->errorln(CAT("Invalid amount.\r\n",usage,"\r\n").c_str());
				return;
			}
			train_skill(player,vec_args[0],i.value());
		}
		return;
	}

	bool npc_can(player_ptr_t& player,int e_name) {
		return true;/** TODO */
	}

	bool player_can(player_ptr_t& player,std::string_view skill) {
		return true; /** TODO */
	}
	/** called by game initialization sequence */
	void init() {
		mods::interpreter::add_command("skills", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("skill", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("train", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("practice", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("prac", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("practice_sessions", POS_RESTING, do_practice_sessions, 0,0);
		mods::interpreter::add_command("score", POS_RESTING, do_score, 0,0);
		//mods::interpreter::add_command("allow_skill", POS_RESTING, do_allow_skill, LVL_BUILDER,0);
	}
};


#undef m_debug
