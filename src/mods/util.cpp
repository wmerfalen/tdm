#ifndef __MENTOC_UTILS_SOURCE__
#define  __MENTOC_UTILS_SOURCE__

#include "util.hpp"
#include "flags.hpp"


namespace mods {
	namespace util {
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
	obj_data* parse_object(std::shared_ptr<mods::player>& player,std::string_view arg, int start_at, int* last_index) {
		auto ch = player->cd();
		int i, number;
		static constexpr int max_len = MAX_INPUT_LENGTH;
		std::string buffer;
		int ctr = start_at;
		while(isspace(arg[ctr]) && ++ctr < max_len && ctr < arg.length()){}
		while(!isspace(arg[ctr]) && ctr < max_len && ctr < arg.length()){
			buffer += arg[ctr++];
		}
		std::cerr << "object name: '" << buffer << "'\n";
		if(last_index){
			*last_index = ctr;
		}
		char* tmp_name = buffer.data();
		if(!(number = get_number(&tmp_name))) {
			return nullptr;
		}
		for(i = 0; i < NUM_WEARS; i++){
			if(GET_EQ(ch, i) && isname(buffer.data(), GET_EQ(ch, i)->name) && --number == 0) {
				return GET_EQ(ch, i);
			}
		}
		return get_obj_in_list_vis(ch, buffer.data(), &number, ch->carrying);
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
		std::cerr << "direction: '" << buffer << "'\n";
		if(last_index){
			*last_index = ctr;
		}
		return NORTH;
	}

	};
};

namespace mods::util::err {
	std::string get_string(int _errno){
		std::array<char,256> buf;
		std::fill(buf.begin(),buf.end(),0);
		strerror_r(_errno,static_cast<char*>(&buf[0]),255);
		return std::string(static_cast<const char*>(&buf[0]));
	}
};
	objdir_t parse_objdir(std::shared_ptr<mods::player>& player,std::string_view arg){
		int last_index = 0;
		return { mods::util::parse_object(player, arg, 0,&last_index), mods::util::parse_direction(arg, last_index+1, nullptr)};
	}

#endif
