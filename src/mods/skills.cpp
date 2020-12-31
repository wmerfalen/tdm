#include "skills.hpp"
#include "interpreter.hpp"
#include "super-users.hpp"
#include <functional>
#include "skill-orm-adaptor.hpp"
#include "orm/skill-trees.hpp"
#include "orm/player-skill-points.hpp"

#ifdef __MENTOC_MODS_SKILLS_SHOW_DEBUG_OUTPUT__
#define m_debug(A) std::cerr << "[mods::skills][debug]:'" << A << "'\n";
#else
#define m_debug(A)
#endif

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



void display_user_skills(player_ptr_t& player) {
	mods::orm::skill_trees tree;
	std::string pc = "";
	for(auto ch : player->get_class_string().str()) {
		pc += std::tolower(ch);
	}
	tree.load_by_class(pc);
	mods::orm::player_skill_points ps;
	auto results = ps.get_player_levels(player->db_id(),pc);
	std::map<std::string,std::vector<std::pair<std::string,uint16_t>>> screen;
	for(const auto& row : tree.rows) {
		screen[row.skill_category].emplace_back(std::make_pair<>(row.skill_name,results[row.id]));
	}
	for(const auto& pair : screen) {
		player->send("{grn}%s{/grn}\r\n", pair.first.c_str());
		for(const auto& item : pair.second) {
			player->send("\t%s\t\t| Proficiency: %d\r\n", item.first.c_str(),item.second);
		}
	}
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

ACMD(do_train) {
	DO_HELP("train");
	static constexpr const char* usage = "usage: train <skill> <amount>";
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0) {
		display_user_skills(player);
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

namespace mods::skills {
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
		//mods::interpreter::add_command("allow_skill", POS_RESTING, do_allow_skill, LVL_BUILDER,0);
	}
};


#undef m_debug
