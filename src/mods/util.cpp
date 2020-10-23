#ifndef __MENTOC_UTILS_SOURCE__
#define  __MENTOC_UTILS_SOURCE__

#include "util.hpp"
#include "flags.hpp"
#include "filesystem.hpp"


namespace mods::util {
	std::string compile_yaml_path_from_type_and_file(int type, std::string_view file){
		std::string string_type = mods::util::yaml_int_to_string(type);
		if(string_type.compare(mods::util::UNKNOWN_YAML_FILE) == 0){
			return mods::util::UNKNOWN_YAML_FILE;
		}
		if(string_type.substr(0,5).compare("ITEM_") == 0){
			string_type = string_type.substr(5);
			std::string f;
			for(auto c : string_type){
				f += tolower(c);
			}
			string_type = f;
		}
		std::string path = "";
		path = CAT(MENTOC_CURRENT_WORKING_DIR,"/objects/",string_type,"/",file.data());
		std::cerr << "[compiled path]: '" << path << "'\n";
		if(strstr(file.data(),MENTOC_CURRENT_WORKING_DIR)){
			std::string f = file.data();
			f = f.substr(f.find_last_of("/"));
			path = CAT(MENTOC_CURRENT_WORKING_DIR,"/objects/",string_type,"/",f);
			std::cerr << "[compiled path --{ corrected }--]: '" << path << "'\n";
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

		bool fuzzy_match(const std::string& _needle,const std::string & _haystack) {
			std::string needle = _needle,haystack = _haystack;

			/* If matches EXACTLY (strcmp) */
			if(needle.compare(haystack) == 0) {
				return true;
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

				if(current.size()){
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
		directory_list_t glob(std::string_view path){
			directory_list_t files;
#ifdef MENTOC_CPP_2017
			std::string path = dir.data();
			for (auto & p : std::filesystem::directory_iterator(path))
				files.emplace_back(p);
#else
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir (path.data())) != NULL) {
			  /* print all the files and directories within directory */
			  while ((ent = readdir (dir)) != NULL) {
				files.emplace_back(ent->d_name);
			  }
			  closedir (dir);
			  return files;
			} else {
				return {};
			}
#endif
		}
		std::function<void(const std::string&,std::string&)> sanitize = [](const std::string & command,std::string& sanitized){
		sanitized = "";
		for(auto ch: command){
			if(std::isalpha(ch)){
				sanitized += ch;
			}
		}
	};

	bool dir_exists(const char* dir){
		auto dir_fp = opendir(dir);
		if(dir_fp == nullptr){
			return false;
		}
		else{
		closedir(dir_fp);
		return true;
		}
	}
	obj_ptr_t parse_object_vec(player_ptr_t& player,std::vector<std::string>& vec_args){
		if(vec_args.size() == 0){
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
			auto eq = player->equipment(i);
			if(eq && isname(buffer.data(), eq->name.c_str()) && --number == 0) {
				return eq;
			}
		}
		return make_from(get_obj_in_list_vis(player->cd(), buffer.data(), &number, player->carrying()));
	}

	int parse_direction(std::string_view arg, int start_at, int* last_index) {
		static constexpr int max_len = MAX_INPUT_LENGTH;
		std::string buffer;
		int ctr = start_at;
		static constexpr int max_dir_length = 5;
		int dir_len = 0;
		while(isspace(arg[ctr]) && ++ctr < max_len && ctr < arg.length()){}
		while(!isspace(arg[ctr]) && ctr < max_len && ctr < arg.length()){
			buffer += arg[ctr++];
			if(++dir_len >= max_dir_length){ break; }
		}
		if(last_index){
			*last_index = ctr;
		}
		return NORTH;
	}

	obj_ptr_t parse_object_with_capability(player_ptr_t& player,std::string_view arg, int start_at, int* last_index, mods::weapon::type::type_list type, std::vector<int>& types) {
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

	objdir_t parse_objdir(player_ptr_t& player,std::string_view arg){
		int last_index = 0;
		return { mods::util::parse_object(player, arg, 0,&last_index), mods::util::parse_direction(arg, last_index+1, nullptr)};
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

	obj_ptr_t make_from(obj_data* o){
		if(!o){ return nullptr; }
		for(auto & obj : obj_list){
			if(o == obj.get()){
				return obj;
			}
		}
		obj_list.push_back(std::make_shared<obj_data>(*o));
		return obj_list.back();
	}
	bool parse_help(std::string_view argument){
		static constexpr int max_len = 64;
		std::string buffer;
		int ctr = 0;
		while(isspace(argument[ctr]) && ++ctr < max_len && ctr < argument.length()){}
		while(!isspace(argument[ctr]) && ctr < max_len && ctr < argument.length()){
			buffer += tolower(argument[ctr++]);
		}
		if(buffer.compare("usage") == 0 || buffer.compare("help") == 0 || buffer.compare("-h") == 0 || buffer.compare("--help") == 0){
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
	std::vector<std::string> explode(char delim,std::string& haystack){
		std::vector<std::string> results;
		std::string current = "";
		for(auto ch : haystack){
			if(ch == delim){
				if(current.length()){
					results.emplace_back(current);
				}
				current = "";
				continue;
			}
			if(ch == '\0'){
				break;
			}
			current += ch;
		}
		if(current.length()){
#ifdef __MENTOC_SHOW_FINAL_EXPLODE_STRING__
			for(auto c : current){
				std::cerr << "[mods::utils::explode][c]'" << (int)c << "'\n";
			}
			std::cerr << "[mods::util::explode] trailing current:'" << current << "', delim:'" << delim << "', haystack: '" << haystack << "'\n";
#endif
			results.emplace_back(current);
		}
		return results;
	}
	bool yaml_file_path_is_sane(std::string path){
		return preg_match("^objects/[a-z]+/[a-z0-9\\-]+\\.yml$",path);
	}
	std::string yaml_int_to_string(int type){
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
#undef MENTOC_LAZY_ME
		return mods::util::UNKNOWN_YAML_FILE;
	}
	int yaml_string_to_int(std::string type){
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
		MENTOC_LAZY_ME(conatiner,ITEM_CONTAINER);
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
#undef MENTOC_LAZY_ME
		return -1;
	}
	std::string yaml_caps_to_lower(std::string type){
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
#undef MENTOC_LAZY_ME
		return mods::util::UNKNOWN_YAML_FILE;
	}
	std::tuple<int,std::string> extract_yaml_info_from_path(std::string_view path){
		std::string yaml_file = "",buffer = "", current = "";
		std::vector<std::string> parts;
		for(auto ch: path){
			if(ch == '/' && current.length()){
				parts.emplace_back(current);
				current = "";
				continue;
			}
			current += ch;
		}
		if(current.length()){
			parts.emplace_back(current);
			current = "";
		}
		/**
		 * parts:
		 * [0] => objects
		 * [1] => rifle
		 * [2] => psg1.yml
		 */
		if(parts.size() != 3){
			return {-1,""};
		}
#define MENTOC_ITEM_PARSE_IMPL(r,data,CLASS_TYPE)\
		if(ICMP(parts[1],BOOST_PP_STRINGIZE(CLASS_TYPE))){\
			return {BOOST_PP_CAT(ITEM_,CLASS_TYPE),parts[2]};\
		}
#define MENTOC_ITEM_PARSE \
BOOST_PP_SEQ_FOR_EACH(MENTOC_ITEM_PARSE_IMPL, ~, MENTOC_ITEM_TYPES_CAPS_SEQ)

		MENTOC_ITEM_PARSE
		return {-2,""};
	}
	bool yaml_file_exists(std::string path){
		std::string f = path;
		std::cerr << "[yaml_file_exists test] '" << f << "'\n";
		if(!strstr(path.data(),MENTOC_CURRENT_WORKING_DIR)){
			f = CAT(MENTOC_CURRENT_WORKING_DIR,"/",path);
		}
		return mods::filesystem::file_exists(f);
	}

};/** end mods::util */

namespace mods::util::err {
	std::string get_string(int _errno){
		std::array<char,256> buf;
		std::fill(buf.begin(),buf.end(),0);
		strerror_r(_errno,static_cast<char*>(&buf[0]),255);
		return std::string(static_cast<const char*>(&buf[0]));
	}
};

#endif
