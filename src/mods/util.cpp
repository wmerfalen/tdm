#ifndef __MENTOC_UTILS_SOURCE__
#define  __MENTOC_UTILS_SOURCE__

#include "util.hpp"
#include "flags.hpp"
#include "filesystem.hpp"
#include "orm/rifle-instance.hpp"
#include "player.hpp"
#include <iostream>
#include <regex>
#include <random>
#include <filesystem>
#include "deep-object-parser.hpp"

#ifdef __MENTOC_MODS_UTIL_DEBUG__
#define mu_debug(A) std::cerr << "[mods::util][debug]:'" << A << "'\n";
#else
#define mu_debug(A) /**-*/
#endif
namespace mods::util {
	std::string admin_section(std::string_view name) {
		std::size_t len = std::min(name.length(),std::size_t(64)) + 4;
		std::string horizontal_line = "{yel}" + std::string(len,'=') + "{/yel}\r\n";

		return CAT(horizontal_line, "{yel}|{/yel}{grn} ", name.data()," {yel}|{/yel}\r\n", horizontal_line);
	}
	std::string overview() {
		return "{yel}Overview{/yel}\r\n";
	}
	/**
	 * @brief parses strings of the form: "#yaml|type/path-to.yml"
	 *
	 * @param reward
	 *
	 * @return
	 */
	std::tuple<bool,int,std::string> extract_yaml_reward(std::string reward) {
		std::string pref = "#yaml|";
		if(reward.length() < pref.length()) {
			return {0,0,""};
		}
		auto extracted = reward.substr(0,pref.length());
		if(extracted.compare(pref.c_str()) != 0) {
			return {0,0,"invalid prefix"};
		}
		auto past = reward.substr(pref.length());
		auto type = mods::util::extract_until(past,'/');

		int i_type = util::yaml_string_to_int(type);
		if(i_type < 0) {
			return {0,0,"invalid type"};
		}
		std::tuple<int,std::string> s = mods::util::extract_yaml_info_from_path(past);
		return {std::get<0>(s) >= 0,std::get<0>(s),std::get<1>(s)};
	}
	/**
	 * @brief extracts deep object string of the form: "#deep|type/deep{...}"
	 *
	 * @param reward
	 *
	 * @return
	 */
	std::tuple<bool,int,std::string,strmap_t> extract_deep_reward(std::string reward) {
		strmap_t extracted;
		std::string pref = "#deep|";
		if(reward.length() < pref.length()) {
			return {0,-1,"invalid prefix",extracted};
		}
		auto prefix = reward.substr(0,pref.length());
		if(prefix.compare(pref.c_str()) != 0) {
			return {0,0,"end of string reached prematurely after prefix",extracted};
		}
		auto past = reward.substr(pref.length());
		auto type = mods::util::extract_until(past,'/');

		int i_type = util::yaml_string_to_int(type);
		if(i_type < 0) {
			return {0,0,"invalid type",extracted};
		}

		mods::deep_object_parser_t parser;
		extracted = parser.extract_line_items(past,mods::util::slot_names_for_type(type));
		return {1,i_type,past,extracted};
	}

	void breakline() {
		for(int i = 0; i < 10; i++) {
			std::cerr << red_str("-----------------------------------------------------") << "\n";
		}
	}
	std::vector<std::string> slot_names_for_type(std::string_view type) {
		if(type.compare("rifle") == 0) {
			return {
				"sight",
				"under_barrel",
				"grip",
				"barrel",
				"muzzle",
				"magazine",
				"stock",
				"strap"
			};
		}
		return {};
	}
	std::string extract_after(std::string_view target, char this_char) {
		std::string buffer;
		buffer.reserve(target.size());
		bool capture = false;
		for(auto ch : target) {
			if(!capture && ch == this_char) {
				capture = true;
				continue;
			}
			if(capture) {
				buffer += ch;
			}
		}
		return buffer;
	}

	std::string extract_until(std::string_view target, char this_char) {
		std::string buffer;
		buffer.reserve(target.size());
		for(auto ch : target) {
			if(ch == this_char) {
				return buffer;
			}
			buffer += ch;
		}
		return buffer;
	}
	bool regex_match(std::string_view regex_string,std::string_view target_string) {
		using namespace std::regex_constants;
		return std::regex_search(target_string.data(),std::regex(regex_string.data()), std::regex_constants::match_any | std::regex_constants::match_not_null | std::regex_constants::match_continuous);
	}
	std::string trim_view(std::string_view str) {
		int i = 0;
		for(auto ch : str) {
			if(!isspace(ch)) {
				return str.substr(i).data();
			}
			++i;
		}
		return str.data();
	}
	std::string trim(std::string& str) {
		std::string s;
		int i = 0;
		for(auto ch : str) {
			if(!isspace(ch)) {
				return str.substr(i);
			}
			++i;
		}
		return str;
	}
	bool first_alpha_is_any(std::string_view line,std::string_view any) {
		for(auto ch : line) {
			if(isalpha(ch)) {
				for(auto a : any) {
					if(ch == a) {
						return true;
					}
				}
				return false;
			}
		}
		return false;
	}
	void texturize_room(room_rnum room_id, room_data::texture_type_t texture_type) {
		world[room_id].add_texture(texture_type);
	}
	void detexturize_room(room_rnum room_id, room_data::texture_type_t texture_type) {
		world[room_id].remove_texture(texture_type);
	}
	std::string time_string() {
		char outstr[200];
		time_t t;
		struct tm *tmp;

		t = time(NULL);
		tmp = localtime(&t);
		if(tmp == NULL) {
			return "date-unknown";
		}
		std::string format = "%";
		format += "F-%";
		format += "T";
		if(strftime(outstr, sizeof(outstr), format.c_str(), tmp) == 0) {
			return "date-unknown";
		}
		return outstr;
	}

	long pg_timestamp_to_long(std::string timestamp) {
		struct tm time;
		memset(&time,0,sizeof(tm));
		strptime(timestamp.c_str(), "%Y-%m-%d %H:%M:%S",&time);
		return mktime(&time);
	}
	std::string player_class_to_string(player_class_t pc) {
		switch(pc) {
			case player_class_t::CLASS_UNDEFINED:
				return "CLASS_UNDEFINED";
			case player_class_t::SNIPER:
				return "SNIPER";
			case player_class_t::MARINE:
				return "MARINE";
			case player_class_t::SENTINEL:
				return "SENTINEL";
			case player_class_t::CONTAGION:
				return "CONTAGION";
			case player_class_t::ENGINEER:
				return "ENGINEER";
			case player_class_t::MEDIC:
				return "MEDIC";
			case player_class_t::PSYOP:
				return "PSYOP";
			case player_class_t::SUPPORT:
				return "SUPPORT";
			case player_class_t::GHOST:
				return "GHOST";
			case player_class_t::MARKSMAN:
				return "MARKSMAN";
			case player_class_t::BANDIT:
				return "BANDIT";
			case player_class_t::BUTCHER:
				return "BUTCHER";
			case player_class_t::STRIKER:
				return "STRIKER";
			case player_class_t::OBSTRUCTOR:
				return "OBSTRUCTOR";
			case player_class_t::MALADY:
				return "MALADY";
			case player_class_t::PYREXIA:
				return "PYREXIA";
			case player_class_t::DEALER:
				return "DEALER";
			case player_class_t::FORGE:
				return "FORGE";
			case player_class_t::SYNDROME:
				return "SYNDROME";
			case player_class_t::MACHINIST:
				return "MACHINIST";
			default:
				return "UNKNOWN-CLASS";
		}
	}
	std::string proficiency_to_string(int prof) {
		switch(prof) {
			default:
			case 0:
				return "UNLEARNED";
			case 1:
				return "INITIATE";
			case 2:
				return "FAMILIAR";
			case 3:
				return "MASTER";
		}
	}
	bool is_yaml_type(std::string_view type) {
		for(const auto& valid_type : VALID_TYPES) {
			if(type.compare(valid_type.c_str()) == 0) {
				return true;
			}
		}
		return false;
	}
	std::string compile_yaml_path_from_type_and_file(int type, std::string_view file) {
		std::string string_type = mods::util::yaml_int_to_string(type);
		if(string_type.compare(mods::util::UNKNOWN_YAML_FILE) == 0) {
			return mods::util::UNKNOWN_YAML_FILE;
		}
		if(string_type.substr(0,5).compare("ITEM_") == 0) {
			string_type = string_type.substr(5);
			std::string f;
			for(auto c : string_type) {
				f += tolower(c);
			}
			string_type = f;
		}
		std::string path = "";
		path = CAT(MENTOC_CURRENT_WORKING_DIR,"/objects/",string_type,"/",file.data());
		mu_debug("[compiled path]: '" << path << "'");
		if(strstr(file.data(),MENTOC_CURRENT_WORKING_DIR)) {
			std::string f = file.data();
			f = f.substr(f.find_last_of("/"));
			path = CAT(MENTOC_CURRENT_WORKING_DIR,"/objects/",string_type,"/",f);
			mu_debug("[compiled path --{ corrected }--]: '" << path << "'");
		}
		return path;
	}
	std::string&& word_wrap(std::string_view paragraph,int width) {
		std::string buffer;

		if(width <= 0) {
			return std::move(std::string(paragraph));
		}

		int position = 1;

		for(unsigned cint = 0; cint < paragraph.length(); cint++) {
			/* if we run into an open brace then that means it's a color code.
			 * Push past the color code and decrement the position counter as
			 * we go.
			 */
			if(paragraph[cint] == '{') {
				while(paragraph[cint] != '}'
				        && cint < paragraph.length()
				     ) {
					buffer += paragraph[cint++];
					--position;
				}

				buffer += '}';
				continue;
			}

			/* Edge case where the first character in the paragraph is
			 * a color code. The position needs to be 1 since after pushing
			 * past the color code, the position would be a negative value
			 */
			if(position < 1) {
				position = 1;
			}

			if(isspace(paragraph[cint])) {
				/* if the buffer is non-empty and that most recent character
				 * pushed is a space then we would be essentially doubling
				 * up on spaces if we didn't 'continue' here.
				 */
				if(buffer.begin() != buffer.end() &&
				        isspace(*(buffer.end() - 1))) {
					continue;
				} else {
					/* the most recent character was *not* a space so we
					 * can safely append a space without fear of doubling
					 * down on spaces
					 */
					buffer += ' ';
					++position;
				}
			} else {
				buffer += paragraph[cint];
				++position;
			}

			/* we have pushed past the desired screen width. For this,
			 * back track to the last space and insert a newline
			 */
			if(position >= width) {
				if(!isspace(paragraph[cint])) {
					//Perform backtracking
					//====================
					for(int k = buffer.size(); k > 0; k--) {
						if(isspace(buffer[k])) {
							buffer[k] = '\n';
							break;
						}
					}

					/* reset the position back to 1 */
					position = 1;
				}
			}
		}

		return std::move(buffer);
	}
	//uint64_t aff2legacy(mods::flags::aff f){
	//	for(unsigned i=0; i < mods::flags::aff_flags.size();i++){
	//		if(mods::flags::aff_flags[i].first == f){
	//			return mods::flags::aff_flags[i].second;
	//		}
	//	}
	//	return 0;
	//}
	//mods::flags::aff legacy2aff(uint64_t f){
	//	for(unsigned i=0; i < mods::flags::aff_flags.size();i++){
	//		if(mods::flags::aff_flags[i].second == f){
	//			return mods::flags::aff_flags[i].first;
	//		}
	//	}
	//	return mods::flags::aff::__AFF_FIRST;
	//}
	///**
	// * @return returns the legacy PLR_ flag given the modern plr flag
	// */
	//uint64_t plr2legacy(mods::flags::plr f){
	//	for(unsigned i=0; i < mods::flags::plr_flags.size();i++){
	//		if(mods::flags::plr_flags[i].first == f){
	//			return mods::flags::plr_flags[i].second;
	//		}
	//	}
	//	return 0;
	//}
	//mods::flags::plr legacy2plr(uint64_t f){
	//	for(unsigned i=0; i < mods::flags::plr_flags.size();i++){
	//		if(mods::flags::plr_flags[i].second == f){
	//			return mods::flags::plr_flags[i].first;
	//		}
	//	}
	//	return mods::flags::plr::__PLR_FIRST;
	//}
	bool preg_match(std::string_view regex,std::string_view haystack) {
		using namespace std::regex_constants;
		return std::regex_search(haystack.data(), std::regex(regex.data()), match_not_null);
	}

	bool fuzzy_match(const std::string& _needle,const std::string& _haystack) {
		std::string needle = "", haystack = _haystack;

		/* If matches EXACTLY (strcmp) */
		if(_needle.compare(haystack) == 0) {
			return true;
		}
		static const std::vector<char> escape = {
			'-','{','}','*','[',']','!',','
		};
		static const std::vector<char> extras = {
			'_', ':'
		};

		for(auto ch : _needle) {
			auto escape_me = std::find(escape.begin(),escape.end(),ch);
			if(!isalnum(ch) && !isspace(ch) && escape_me != escape.end() && std::find(extras.begin(),extras.end(),ch) != extras.end()) {
				continue;
			}
			if(escape_me != escape.end()) {
				needle += "\\";
				needle += ch;
				continue;
			}
			needle += ch;
		}

		if(needle.length()) {
			/* If the string is composed of spaces, do a match against each word */
			std::vector<std::string> words;
			std::string current = "";

			for(auto character : haystack) {
				if(character == ' ' && current.length()) {
					words.push_back(current);
					current.clear();
					continue;
				}
				current += character;
			}

			if(current.size()) {
				words.push_back(current);
			}

			for(auto str : words) {
				if(std::regex_search(str, std::regex(needle.c_str(),std::regex_constants::ECMAScript | std::regex_constants::icase), std::regex_constants::match_not_null)) {
					return true;
				}
			}
		}

		return false;
	}
	std::optional<unsigned> stoul(std::string_view str) {
		try {
			return std::stoul(str.data());
		} catch(...) {
			return std::nullopt;
		}
	}
	std::optional<int> stoi(std::string_view str) {
		try {
			return std::stoi(str.data());
		} catch(...) {
			return std::nullopt;
		}
	}
	std::string itoa(int number) {
		std::array<char,16> buf;
		std::fill(buf.begin(),buf.end(),0);
		snprintf(&buf[0],16,"%d",number);
		return &buf[0];
	}
	directory_list_t glob(std::string_view path) {
		directory_list_t files;
#ifdef MENTOC_CPP_2017
		std::string path = dir.data();
		for(auto& p : std::filesystem::directory_iterator(path)) {
			files.emplace_back(p);
		}
#else
		DIR *dir;
		struct dirent *ent;
		if((dir = opendir(path.data())) != NULL) {
			/* print all the files and directories within directory */
			while((ent = readdir(dir)) != NULL) {
				files.emplace_back(ent->d_name);
			}
			closedir(dir);
			return files;
		} else {
			return {};
		}
#endif
	}
	std::function<void(const std::string&,std::string&)> sanitize = [](const std::string& command,std::string& sanitized) {
		sanitized = "";
		for(auto ch: command) {
			if(std::isalpha(ch)) {
				sanitized += ch;
			}
		}
	};

	bool dir_exists(const char* dir) {
		auto dir_fp = opendir(dir);
		if(dir_fp == nullptr) {
			return false;
		} else {
			closedir(dir_fp);
			return true;
		}
	}
	obj_ptr_t parse_object_vec(player_ptr_t& player,std::vector<std::string>& vec_args) {
		if(vec_args.size() == 0) {
			return nullptr;
		}
		int last_index = 0;
		return parse_object(player,vec_args[0],0,&last_index);
	}
	obj_ptr_t parse_object(player_ptr_t& player,std::string_view arg, int start_at, int* last_index) {
		int i, number;
		static constexpr int max_len = MAX_INPUT_LENGTH;
		std::string buffer;
		int ctr = start_at;
		while(isspace(arg[ctr]) && ++ctr < max_len && ctr < arg.length()) {}
		while(!isspace(arg[ctr]) && ctr < max_len && ctr < arg.length()) {
			buffer += arg[ctr++];
		}
		if(last_index) {
			*last_index = ctr;
		}
		char* tmp_name = buffer.data();
		if(!(number = get_number(&tmp_name))) {
			return nullptr;
		}
		for(i = 0; i < NUM_WEARS; i++) {
			auto eq = player->equipment(i);
			if(eq && isname(buffer.data(), eq->name.c_str()) && --number == 0) {
				return eq;
			}
		}
		return make_from(get_obj_in_list_vis(player->cd(), buffer.data(), &number, player->carrying()));
	}

	int parse_direction(std::string_view arg) {

		int i = 0;
		while(arg.length() > i && !isalpha(arg[i])) {
			++i;
		}
		if(arg.length() <= i) {
			return -1;
		}
		switch(arg[i]) {
			case 'n':
			case 'N':
				return NORTH;
			case 'e':
			case 'E':
				return EAST;
			case 's':
			case 'S':
				return SOUTH;
			case 'w':
			case 'W':
				return WEST;
			case 'u':
			case 'U':
				return UP;
			case 'd':
			case 'D':
				return DOWN;
			default:
				return -1;
		}
	}
	int parse_direction(std::string_view arg, int start_at, int* last_index) {

		static constexpr int max_len = MAX_INPUT_LENGTH;
		std::string buffer;
		int ctr = start_at;
		static constexpr int max_dir_length = 5;
		int dir_len = 0;
		while(isspace(arg[ctr]) && ++ctr < max_len && ctr < arg.length()) {}
		while(!isspace(arg[ctr]) && ctr < max_len && ctr < arg.length()) {
			buffer += arg[ctr++];
			if(++dir_len >= max_dir_length) {
				break;
			}
		}
		if(last_index) {
			*last_index = ctr;
		}
		return parse_direction(buffer);
	}

	obj_ptr_t parse_object_with_capability(player_ptr_t& player,std::string_view arg, int start_at, int* last_index, mods::weapon::type::type_list type, std::vector<int>& types) {
		auto ch = player->cd();
		int i, number;
		static constexpr int max_len = MAX_INPUT_LENGTH;
		std::string buffer;
		int ctr = start_at;
		while(isspace(arg[ctr]) && ++ctr < max_len && ctr < arg.length()) {}
		while(!isspace(arg[ctr]) && ctr < max_len && ctr < arg.length()) {
			buffer += arg[ctr++];
		}
		if(last_index) {
			*last_index = ctr;
		}
		char* tmp_name = buffer.data();
		if(!(number = get_number(&tmp_name))) {
			return nullptr;
		}
		for(i = 0; i < NUM_WEARS; i++) {
			auto eq = GET_EQ(ch, i);
			if(eq && isname(buffer.data(), eq->name) && --number == 0) {
				if(eq->type == type && std::find(types.begin(),types.end(),eq->obj_flags.type_flag) != types.end()) {
					return make_from(eq);
				}
			}
		}
		return make_from(get_obj_in_list_vis(ch, buffer.data(), &number, ch->carrying));
	}

	std::tuple<bool,direction_t,uint8_t> parse_direction_count(std::vector<std::string>& vec_args) {
		if(vec_args.size() < 2) {
			return {0,0,0};
		}
		direction_t dir = NORTH;
		dir = (direction_t)parse_direction(vec_args[0]);
		uint8_t count = mods::util::stoi(vec_args[1]).value_or(0);
		return {1,dir,count};
	}
	std::optional<std::pair<direction_t,uint8_t>> parse_direction_count_optional(std::vector<std::string>& vec_args) {
		if(vec_args.size() < 2) {
			return std::nullopt;
		}
		direction_t dir = NORTH;
		dir = (direction_t)parse_direction(vec_args[0]);
		uint8_t count = mods::util::stoi(vec_args[1]).value_or(0);
		return std::make_pair<>(dir,count);
	}
	objdir_t parse_objdir(player_ptr_t& player,std::string_view arg) {
		int last_index = 0;
		auto obj = mods::util::parse_object(player, arg, 0,&last_index);
		auto dir = mods::util::parse_direction(arg, last_index+1, nullptr);
		return { obj, dir };
	}
	/*
		objdir_t expect_explosive_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_explosive>& types){

		}
		objdir_t expect_rifle_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_rifle>& types){

		}
		objdir_t expect_gadget_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_gadget>& types){

		}
		objdir_t expect_drone_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_drone>& types){

		}
		*/

	obj_ptr_t make_from(obj_data* o) {
		if(!o) {
			return nullptr;
		}
		for(auto& obj : obj_list) {
			if(o == obj.get()) {
				return obj;
			}
		}
		obj_list.push_back(std::make_shared<obj_data>(*o));
		return obj_list.back();
	}
	bool parse_help(std::string_view argument) {
		static constexpr int max_len = 64;
		std::string buffer;
		int ctr = 0;
		while(isspace(argument[ctr]) && ++ctr < max_len && ctr < argument.length()) {}
		while(!isspace(argument[ctr]) && ctr < max_len && ctr < argument.length()) {
			buffer += tolower(argument[ctr++]);
		}
		if(buffer.compare("usage") == 0 || buffer.compare("help") == 0 || buffer.compare("-h") == 0 || buffer.compare("--help") == 0) {
			return true;
		}
		return false;
	}
	/*
	objdir_t parse_objdir_capable(player_ptr_t& player,std::string_view arg, uint8_t query_type, cap_list_t& capabilities){
		auto ch = player->cd();
		int i, number;
		static constexpr int max_len = MAX_INPUT_LENGTH;
		std::string buffer;
		int ctr = start_at;
		while(isspace(arg[ctr]) && ++ctr < max_len && ctr < arg.length()){}
		while(!isspace(arg[ctr]) && ctr < max_len && ctr < arg.length()){
			buffer += arg[ctr++];
		}
		if(last_index){
			*last_index = ctr;
		}
		char* tmp_name = buffer.data();
		if(!(number = get_number(&tmp_name))) {
			return nullptr;
		}
		for(i = 0; i < NUM_WEARS; i++){
			auto eq = GET_EQ(ch, i);
			if(eq && isname(buffer.data(), eq->name) && --number == 0) {
				if(eq->type == type && std::find(types.begin(),types.end(),eq->obj_flags.type_flag) != types.end()){
					return make_from(eq);
				}
			}
		}
		return make_from(get_obj_in_list_vis(ch, buffer.data(), &number, ch->carrying));
	}


	objdir_t parse_objdir_cap_single(player_ptr_t& player,std::string_view arg, cap_list_t& capabilities){
		return parse_objdir_capable(player,arg, CAP_SINGLE, capabilities);
	}
	objdir_t parse_objdir_cap_any(player_ptr_t& player,std::string_view arg, cap_list_t& capabilities){
		return parse_objdir_capable(player,arg, CAP_ANY, capabilities);
	}
	objdir_t parse_objdir_cap_all(player_ptr_t& player,std::string_view arg, cap_list_t& capabilities){
		return parse_objdir_capable(player,arg, CAP_ALL, capabilities);
	}
	*/
	// --
	bool match_any(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch) {
		for(auto& str : any_of_these) {
			if(strncmp(src.data(),str.c_str(),max_ch) == 0) {
				return true;
			}
		}
		return false;
	}

	bool is_lower_match(std::string_view str1,std::string_view str2) {
		unsigned int sz = str1.size();
		if(str2.size() != sz) {
			return false;
		}

		for(unsigned int i = 0; i < sz; ++i) {
			if(std::tolower(str1[i]) != std::tolower(str2[i])) {
				return false;
			}
		}
		return true;
	}
	bool match_any_lower(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch) {
		for(auto& str : any_of_these) {
			if(is_lower_match(str,src)) {
				return true;
			}
		}
		return false;
	}
	player_class_t to_player_class(std::string_view str) {
		if(is_lower_match(str,"SNIPER")) {
			return player_class_t::SNIPER;
		}
		if(is_lower_match(str,"MARINE")) {
			return player_class_t::MARINE;
		}
		if(is_lower_match(str,"BREACHER")) {
			return player_class_t::BREACHER;
		}
		if(is_lower_match(str,"ENGINEER")) {
			return player_class_t::ENGINEER;
		}
		if(is_lower_match(str,"GHOST")) {
			return player_class_t::GHOST;
		}
		if(is_lower_match(str,"MEDIC")) {
			return player_class_t::MEDIC;
		}
		if(is_lower_match(str,"SUPPORT")) {
			return player_class_t::SUPPORT;
		}
		if(is_lower_match(str,"MARKSMAN")) {
			return player_class_t::CLASS_MARKSMAN;
		}
		if(is_lower_match(str,"BANDIT")) {
			return player_class_t::CLASS_BANDIT;
		}
		if(is_lower_match(str,"BUTCHER")) {
			return player_class_t::CLASS_BUTCHER;
		}
		if(is_lower_match(str,"STRIKER")) {
			return player_class_t::CLASS_STRIKER;
		}
		if(is_lower_match(str,"OBSTRUCTOR")) {
			return player_class_t::CLASS_OBSTRUCTOR;
		}
		if(is_lower_match(str,"MALADY")) {
			return player_class_t::CLASS_MALADY;
		}
		if(is_lower_match(str,"PYREXIA")) {
			return player_class_t::CLASS_PYREXIA;
		}
		if(is_lower_match(str,"DEALER")) {
			return player_class_t::CLASS_DEALER;
		}
		if(is_lower_match(str,"FORGE")) {
			return player_class_t::CLASS_FORGE;
		}
		if(is_lower_match(str,"SYNDROME")) {
			return player_class_t::CLASS_SYNDROME;
		}
		if(is_lower_match(str,"MACHINIST")) {
			return player_class_t::CLASS_MACHINIST;
		}
		return player_class_t::CLASS_UNDEFINED;
	}
	// --/
	std::vector<std::string> explode(char delim,std::string& haystack) {
		std::vector<std::string> results;
		std::string current = "";
		for(auto ch : haystack) {
			if(ch == delim) {
				if(current.length()) {
					results.emplace_back(current);
				}
				current = "";
				continue;
			}
			if(ch == '\0') {
				break;
			}
			current += ch;
		}
		if(current.length()) {
#ifdef __MENTOC_SHOW_FINAL_EXPLODE_STRING__
			for(auto c : current) {
				mu_debug("[mods::utils::explode][c]'" << (int)c << "'");
			}
			mu_debug("[mods::util::explode] trailing current:'" << current << "', delim:'" << delim << "', haystack: '" << haystack << "'");
#endif
			results.emplace_back(current);
		}
		return results;
	}
	bool yaml_file_path_is_sane(std::string path) {
		return preg_match("^[a-z]+/[a-z0-9\\-]+\\.yml$",path);
	}
	std::string yaml_int_to_string(int type) {
		/** !!*****************!! */
		/** !!UPDATE_ITEM_TYPES!! */
		/** !!*****************!! */
#define MENTOC_LAZY_ME(mtype,B) if(type == B){ return #B; }
		MENTOC_LAZY_ME(rifle,ITEM_RIFLE);
		MENTOC_LAZY_ME(explosive,ITEM_EXPLOSIVE);
		MENTOC_LAZY_ME(drone,ITEM_DRONE);
		MENTOC_LAZY_ME(gadget,ITEM_GADGET);
		MENTOC_LAZY_ME(attachment,ITEM_ATTACHMENT);
		MENTOC_LAZY_ME(armor,ITEM_ARMOR);
		MENTOC_LAZY_ME(consumable,ITEM_CONSUMABLE);
		MENTOC_LAZY_ME(trap,ITEM_TRAP);
		MENTOC_LAZY_ME(container,ITEM_CONTAINER);
		MENTOC_LAZY_ME(melee,ITEM_MELEE);
		MENTOC_LAZY_ME(vehicle,ITEM_VEHICLE);
#undef MENTOC_LAZY_ME
		return mods::util::UNKNOWN_YAML_FILE;
	}
	int yaml_string_to_int(std::string type) {
		/** !!*****************!! */
		/** !!UPDATE_ITEM_TYPES!! */
		/** !!*****************!! */
#define MENTOC_LAZY_ME(mtype,B) if(is_lower_match(#mtype,type)){ return B; }
		MENTOC_LAZY_ME(rifle,ITEM_RIFLE);
		MENTOC_LAZY_ME(explosive,ITEM_EXPLOSIVE);
		MENTOC_LAZY_ME(drone,ITEM_DRONE);
		MENTOC_LAZY_ME(gadget,ITEM_GADGET);
		MENTOC_LAZY_ME(attachment,ITEM_ATTACHMENT);
		MENTOC_LAZY_ME(armor,ITEM_ARMOR);
		MENTOC_LAZY_ME(consumable,ITEM_CONSUMABLE);
		MENTOC_LAZY_ME(trap,ITEM_TRAP);
		MENTOC_LAZY_ME(container,ITEM_CONTAINER);
		MENTOC_LAZY_ME(melee,ITEM_MELEE);
		MENTOC_LAZY_ME(vehicle,ITEM_VEHICLE);
#undef MENTOC_LAZY_ME
#define MENTOC_LAZY_ME(mtype) if(is_lower_match(#mtype,type)){ return mtype; }
		MENTOC_LAZY_ME(ITEM_RIFLE);
		MENTOC_LAZY_ME(ITEM_EXPLOSIVE);
		MENTOC_LAZY_ME(ITEM_DRONE);
		MENTOC_LAZY_ME(ITEM_GADGET);
		MENTOC_LAZY_ME(ITEM_ATTACHMENT);
		MENTOC_LAZY_ME(ITEM_ARMOR);
		MENTOC_LAZY_ME(ITEM_CONSUMABLE);
		MENTOC_LAZY_ME(ITEM_TRAP);
		MENTOC_LAZY_ME(ITEM_CONTAINER);
		MENTOC_LAZY_ME(ITEM_MELEE);
		MENTOC_LAZY_ME(ITEM_VEHICLE);
#undef MENTOC_LAZY_ME
		return -1;
	}
	std::string yaml_caps_to_lower(std::string type) {
		/** !!*****************!! */
		/** !!UPDATE_ITEM_TYPES!! */
		/** !!*****************!! */
#define MENTOC_LAZY_ME(mtype,B) if(is_lower_match(#B,type)){ return #mtype; }
		MENTOC_LAZY_ME(rifle,ITEM_RIFLE);
		MENTOC_LAZY_ME(explosive,ITEM_EXPLOSIVE);
		MENTOC_LAZY_ME(drone,ITEM_DRONE);
		MENTOC_LAZY_ME(gadget,ITEM_GADGET);
		MENTOC_LAZY_ME(attachment,ITEM_ATTACHMENT);
		MENTOC_LAZY_ME(armor,ITEM_ARMOR);
		MENTOC_LAZY_ME(consumable,ITEM_CONSUMABLE);
		MENTOC_LAZY_ME(trap,ITEM_TRAP);
		MENTOC_LAZY_ME(container,ITEM_CONTAINER);
		MENTOC_LAZY_ME(melee,ITEM_MELEE);
		MENTOC_LAZY_ME(vehicle,ITEM_VEHICLE);
#undef MENTOC_LAZY_ME
		return mods::util::UNKNOWN_YAML_FILE;
	}
	/**
	 * @brief returns {ITEM_*,"pathtoyaml"} on success. returns {negative,""} on fail
	 *
	 * @param path
	 *
	 * @return
	 */
	std::tuple<int,std::string> extract_yaml_info_from_path(std::string_view path) {
		std::string yaml_file = "",buffer = "", current = "";
		std::vector<std::string> parts;
		for(auto ch: path) {
			if(ch == '/' && current.length()) {
				parts.emplace_back(current);
				current = "";
				continue;
			}
			current += ch;
		}
		if(current.length()) {
			parts.emplace_back(current);
			current = "";
		}
		if(parts.size() == 2) {
			/** parts:
			 * [0] => rifle
			 * [1] => psg1.yml
			 */
#define MENTOC_ITEM_PARSE_IMPL(r,data,CLASS_TYPE)\
		if(ICMP(parts[0],BOOST_PP_STRINGIZE(CLASS_TYPE))){\
			return {BOOST_PP_CAT(ITEM_,CLASS_TYPE),parts[1]};\
		}
#define MENTOC_ITEM_PARSE \
BOOST_PP_SEQ_FOR_EACH(MENTOC_ITEM_PARSE_IMPL, ~, MENTOC_ITEM_TYPES_CAPS_SEQ)

			MENTOC_ITEM_PARSE

#undef MENTOC_ITEM_PARSE_IMPL
#undef MENTOC_ITEM_PARSE
		}

#define MENTOC_ITEM_PARSE_IMPL(r,data,CLASS_TYPE)\
		if(ICMP(parts[1],BOOST_PP_STRINGIZE(CLASS_TYPE))){\
			return {BOOST_PP_CAT(ITEM_,CLASS_TYPE),parts[2]};\
		}
#define MENTOC_ITEM_PARSE \
BOOST_PP_SEQ_FOR_EACH(MENTOC_ITEM_PARSE_IMPL, ~, MENTOC_ITEM_TYPES_CAPS_SEQ)

		/**
		 * parts:
		 * [0] => objects
		 * [1] => rifle
		 * [2] => psg1.yml
		 */
		if(parts.size() == 3) {
			MENTOC_ITEM_PARSE
		}
		return {-2,""};
	}
	bool yaml_file_exists(std::string path) {
		std::string f = path;
		mu_debug("[yaml_file_exists test] '" << f << "'");
		if(!strstr(path.data(),MENTOC_CURRENT_WORKING_DIR)) {
			f = CAT(MENTOC_CURRENT_WORKING_DIR,"/",path);
		}
		return mods::filesystem::file_exists(f);
	}
	obj_ptr_t create_pkid_object(std::string_view in_schema) {
		std::string object_type = extract_until(in_schema,'|');
		if(std::string::npos == in_schema.find_first_of(':')) {
			std::cerr << red_str("WARNING: create_pkid_object couldnt find pkid colon: '") << in_schema << "'\n";
			return nullptr;
		}
		if(object_type.compare("rifle") == 0) {
			//mods::orm::rifle_instance r;
			std::string schema = in_schema.data();
			std::string pkid = extract_after(schema,':');
			mods::orm::rifle_instance inst;
			uint64_t id = mods::util::stoi<uint64_t>(pkid);
			inst.load_by_id(id);
			obj_list.push_back(std::make_shared<obj_data>(ITEM_RIFLE,inst.rifle_file));
			mods::globals::register_object(obj_list.back());
			auto obj = obj_list.back();
			inst.populate_object(obj);
			return obj;
		}
		return nullptr;
	}

};/** end mods::util */

namespace mods::util::err {
	std::string get_string(int _errno) {
		std::array<char,256> buf;
		std::fill(buf.begin(),buf.end(),0);
		strerror_r(_errno,static_cast<char*>(&buf[0]),255);
		return std::string(static_cast<const char*>(&buf[0]));
	}
};
namespace mods::util::args {
	bool match(std::string_view needle, const std::vector<std::string>& arg_list) {
		for(const auto& str : arg_list) {
			if(std::regex_search(str, std::regex(needle.data(),std::regex_constants::ECMAScript), std::regex_constants::match_not_null)) {
				return true;
			}
		}
		return false;
	}
	bool imatch(std::string_view needle, const std::vector<std::string>& arg_list) {
		for(const auto& str : arg_list) {
			if(std::regex_search(str, std::regex(needle.data(),std::regex_constants::ECMAScript | std::regex_constants::icase), std::regex_constants::match_not_null)) {
				return true;
			}
		}
		return false;
	}
};
namespace mods::util::args {
	int parsed_args::fetch_parsed_integer(int index) {
		return i_storage[index];
		if(i_storage.size() > index) {
			std::cerr << "returning i_storage index :)\n";
			return i_storage[index];
		}
		return -1;
	}
	void parsed_args::assign(const std::string& argument) {
		static std::string static_argument;
		if(static_argument.compare(argument.c_str()) == 0) {
			/** already parsed */
			return;
		}
		static_argument = argument;
		vec_args = arglist<std::vector<std::string>>(argument);
		size = vec_args.size();
	}
	bool parsed_args::first_is(std::string_view list_string) {
		if(vec_args.size() == 0) {
			return false;
		}
		return is_lower_match(list_string,vec_args[0]);
	}
	bool parsed_args::first_is_any(std::vector<const char*> list_string) {
		if(vec_args.size() == 0) {
			return false;
		}
		for(const auto& token : list_string) {
			if(is_lower_match(token,vec_args[0])) {
				return true;
			}
		}
		return false;
	}
	bool parsed_args::nth_is_any(int index,std::vector<const char*> list_string) {
		if(vec_args.size() <= index) {
			return false;
		}
		for(const auto& token : list_string) {
			if(is_lower_match(token,vec_args[index])) {
				return true;
			}
		}
		return false;
	}
	parsed_args* parsed_args::save_integer(int index) {
		int n = 0;
		for(auto f : vec_args) {
			std::cerr << "f[" << n++ << "](asking for:" << index << "): '" << f << "'\n";
		}
		while(i_storage.size() < index) {
			i_storage.emplace_back(-1);
		}
		i_storage[index] = -1;
		if(vec_args.size() > index) {
			std::cerr << "setting index for: " << index << " vec_args[index]:'" << vec_args[index] << "'\n";
			i_storage[index] = mods::util::stoi(vec_args[index]).value_or(-1);
			std::cerr << "index for: " << index << " i_storage[index]'" << i_storage[index] << "'\n";
		}
		return this;
	}

	bool parsed_args::if_nth_has_either(int index,std::vector<const char*> list_string) {
		if(vec_args.size() <= index) {
			return false;
		}
		for(const auto& token : list_string) {
			if(is_lower_match(token,vec_args[index])) {
				return true;
			}
		}
		return false;
	}
};
#undef mu_debug
#endif
