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

#define FOREACH_SKILLSET_AS(PROF_NAME) for(auto & skillset : mods::skills::proficiencies::list) for(auto & PROF_NAME : std::get<1>(skillset))
ACMD(do_allow_skill) {
	ADMIN_REJECT();
	DO_HELP("allow_skill");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2) {
		player->sendln("Invalid argument count");
		return;
	}
	auto found = mods::globals::player_name_map.find(vec_args[0]);
	if(found == mods::globals::player_name_map.end()) {
		player->sendln("Couldn't find player by that name.");
		return;
	}

	if(ICMP(vec_args[1],"all")) {
		player->send("Setting all skills...");
		mods::skills::allow_player_to_do_everything(found->second);
		player->sendln("done");
		return;
	}
	skill_t s = mods::skills::to_skill_from_user_friendly_string(vec_args[1]);
	player->send("Setting skill...");
	mods::skills::set_player_can_override(found->second, {s});
	player->sendln("done");
	ADMIN_DONE();
}
namespace mods::skills {
	constexpr static const char* DB_PREFIX = "skills";
	static std::vector<std::string> skill_name_list;
	static std::map<player_class_t,std::vector<std::string>> registered_class_skillsets;

	void refresh_skill_name_list() {
		skill_name_list.clear();
		FOREACH_SKILLSET_AS(prof) {
			skill_name_list.emplace_back(prof.name);
		}
	}

	/** PLAYER BOOTSTRAP */
	void init_player_levels(player_ptr_t& player) {
		strmap_t m;
		FOREACH_SKILLSET_AS(prof) {
			m[prof.name.str()] = "0";
		}

		mods::skill_orm_adaptor::put_player_map(player,m);
	}
	using just_proficiency_name_t = mods::skills::proficiencies::proficiency_name_t;
	static std::map<std::string,int> mappings;
	static std::map<int,std::string> to_string_mappings;
	static std::map<int,proficiencies::proficiency_t> e_name_to_proficiency;
	static constexpr const std::pair<uint8_t,uint8_t> tier_one = {1,9};
	static constexpr const std::pair<uint8_t,uint8_t> tier_two = {10,19};
	static constexpr const std::pair<uint8_t,uint8_t> tier_three = {20,31};
	static std::vector<just_proficiency_name_t> tier_one_proficiencies;
	static std::vector<just_proficiency_name_t> tier_two_proficiencies;
	static std::vector<just_proficiency_name_t> tier_three_proficiencies;
	static std::vector<just_proficiency_name_t> up_to_tier_two;
	static std::vector<just_proficiency_name_t> up_to_tier_three;

	std::vector<just_proficiency_name_t> get_player_tier_skills(player_ptr_t& player) {
		auto level = player->level();
		if(tier_one.first <= level && level <= tier_one.second) {
			return tier_one_proficiencies;
		}
		if(tier_two.first <= level && level <= tier_two.second) {
			return up_to_tier_two;
		}
		if(tier_three.first <= level && level <= tier_three.second) {
			return up_to_tier_three;
		}
		return tier_one_proficiencies;
	}
	std::string to_string(int e_name) {
		return to_string_mappings[e_name];
	}

	uint16_t get_minimum_proficiency(int e_name) {
		return e_name_to_proficiency[e_name].minimum_proficiency;
	}

	void load_player_levels(player_ptr_t& player) {
#if 0
		strmap_t skills;
		if(0 == get_player_map(player->name(),DB_PREFIX,skills)) {
			init_player_levels(player);
			get_player_map(player->name(),DB_PREFIX,skills);
		}
		for(auto& pair : skills) {
			int e_name = get_enum_by_name(pair.first);
			if(e_name == -1) {
				continue;
			}
			auto level_opt = mods::util::stoi_optional<int>(pair.second.c_str());
			int level = 0;
			if(level_opt.has_value() == false) {
				level = 0;
			}
			player->set_skill(e_name,level);
		}
#endif
	}

	uint16_t get_player_level(player_ptr_t& player,int skill) {
		return player->skill(skill);
	}

#if 0
	uint16_t get_player_level(player_ptr_t& player,std::string_view skill) {
		//std::cerr << "player's skill: '" << skill.data() << "', level:" << player->skill(get_enum_by_name(skill)) << "\n";
		return player->skill(get_enum_by_name(skill));
	}
#endif

	bool npc_can(player_ptr_t& player,int e_name) {
		return true;/** TODO */
	}

	bool player_has_override_for(player_ptr_t& player,int e_name) {
		auto it = player_can_overrides.find(player->name().c_str());
		if(it != player_can_overrides.end()) {
			m_debug("[player can by override]: " << green_str(player->name().c_str()) << "->" << e_name);
			return std::find(it->second.begin(),it->second.end(),e_name) != it->second.end();
		}
		return false;
	}

	bool player_can(player_ptr_t& player,int e_name) {
		if(player->is_npc()) {
			return npc_can(player,e_name);
		}
		if(player_has_override_for(player,e_name)) {
			return true;
		}
		return player->skill(e_name) >= get_minimum_proficiency(e_name);
	}

	void allow_player_to_do_everything(player_ptr_t& player) {
		std::vector<skill_t> skills;
		for(auto& skill : passive_skills) {
			skills.emplace_back(skill);
		}
		for(auto& skill : castable_skills) {
			skills.emplace_back(skill);
		}
		set_player_can_override(player,skills);
	}


	void set_player_can_override(player_ptr_t& player,std::vector<skill_t> in_skill_list) {
		player_can_overrides[player->name().c_str()] = in_skill_list;
	}


	void clear_player_can_override(player_ptr_t& player) {
		player_can_overrides.erase(player->name().c_str());
	}

	skill_t to_skill_from_user_friendly_string(std::string_view s) {
		return static_cast<skill_t>(mappings[s.data()]);
	}


	std::string_view to_user_friendly_string_from_skill(skill_t s) {
		return e_name_to_proficiency[s].name.c_str();
	}
	/**
	 * Deletes and inserts skills at zero
	 */
	void init_user_skills(player_ptr_t& player) {
		mods::orm::skill_trees tree;
		std::string pc = "";
		for(auto ch : player->get_class_string().str()) {
			pc += std::tolower(ch);
		}
		tree.load_by_class(pc);
		std::vector<uint32_t> skill_ids;
		for(const auto& row : tree.rows) {
			skill_ids.emplace_back(row.id);
		}
		mods::orm::player_skill_points ps;
		auto results = ps.get_player_levels(player->db_id(),pc);
		ps.delete_by_player(player->db_id());
		ps.populate(player->db_id(),results);
		ps.save();
	}


	ACMD(do_init_user_skills) {
		init_user_skills(player);
	};
	/** called by game initialization sequence */
	void init() {
		mods::interpreter::add_command("init_user_skills", POS_RESTING, do_init_user_skills, 0,0);
		mods::interpreter::add_command("skills", POS_RESTING, do_skills, 0,0);
		mods::interpreter::add_command("skill", POS_RESTING, do_skills, 0,0);
		mods::interpreter::add_command("train", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("practice", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("prac", POS_RESTING, do_train, 0,0);
		mods::interpreter::add_command("allow_skill", POS_RESTING, do_allow_skill, LVL_BUILDER,0);
	}

	void register_class_skillset(player_class_t class_type,std::vector<std::string>& skillset) {
		mods::skills::registered_class_skillsets[class_type] = skillset;
	}
};



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
	auto skills = mods::skills::get_player_tier_skills(player);
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

ACMD(do_skills) {
	DO_HELP("skills");
	auto vec_args = PARSE_ARGS();
	display_user_skills(player);
}
#undef m_debug
