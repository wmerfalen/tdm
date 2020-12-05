#ifndef __MENTOC_UTILS_HEADER__
#define  __MENTOC_UTILS_HEADER__

#include <iostream>
#include <string>
#include <regex>
#include "../globals.hpp"
#include <optional>
#include <stdlib.h>
#include <sys/types.h>	/** for opendir() */
#include <dirent.h>			/** for opendir() */
#include <sys/stat.h>		/** for mkdir() */
#include "flags.hpp"
#ifdef MENTOC_CPP_2017
#include <filesystem>
#else
#include <dirent.h>
#endif
#include <ctime>
#include <random>
#include <regex>
	
#ifndef __MENTOC_STRING_LIT__
#define __MENTOC_STRING_LIT__
static inline std::string operator "" _s(const char* s,uint64_t i) {
	return std::string(s);
}
#endif
#include "weapon-types.hpp"

extern std::deque<room_data> world;
extern int get_number(char **name);
extern int isname(const char *str, const char *namelist);
extern struct obj_data *get_obj_in_list_vis(char_data *ch, char *name, int *number, struct obj_data *list);
extern std::deque<std::shared_ptr<obj_data>> obj_list;

namespace mods::util {
		/** !!*****************!! */
		/** !!UPDATE_ITEM_TYPES!! */
		/** !!*****************!! */
		static std::vector<std::string> VALID_TYPES = {
			"rifle","explosive","drone","attachment","gadget",
			"armor","trap","consumable","container"
		};
	enum yaml_type_t {
		UNRECOGNIZED = 0,
		CATCHY_YAML_FILE = 1,
		YAML_FILE_WITH_PREFIX = 2,
		DEEP_OBJECT = 3
	};

	static inline std::string extract_until(std::string_view target, char this_char){
		std::string buffer;
		buffer.reserve(target.size());
		for(auto ch : target){
			if(ch == this_char){
				return buffer;
			}
			buffer += ch;
		}
		return buffer;
	}
	static inline bool regex_match(std::string_view regex_string,std::string_view target_string){
    using namespace std::regex_constants;
  	return std::regex_search(target_string.data(),std::regex(regex_string.data()), match_any | match_not_null | match_continuous);
	}
	static inline std::string trim_view(std::string_view str){
		int i = 0;
		for(auto ch : str){
			if(!isspace(ch)){
				return str.substr(i).data();
			}
			++i;
		}
		return str.data();
	}
	static inline std::string trim(std::string& str){
		std::string s;
		int i = 0;
		for(auto ch : str){
			if(!isspace(ch)){
				return str.substr(i);
			}
			++i;
		}
		return str;
	}
	static inline bool first_alpha_is_any(std::string_view line,std::string_view any){
		for(auto ch : line){
			if(isalpha(ch)){
				for(auto a : any){
					if(ch == a){
						return true;
					}
				}
				return false;
			}
		}
		return false;
	}
	template <typename T>
	static inline void shuffle(T& vec){
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(vec.begin(),vec.end(),g);
	}
		static inline long pg_timestamp_to_long(std::string timestamp){
			struct tm time;
			memset(&time,0,sizeof(tm));
			strptime(timestamp.c_str(), "%Y-%m-%d %H:%M:%S",&time);
			return mktime(&time);
		}
	constexpr static const char* UNKNOWN_YAML_FILE = "unknown-yits";
	template <typename TKey,typename TVal>
	std::string map2str(std::map<TKey,TVal> in_map){
		std::string d = "";
		for(auto && it : in_map){
			d += it.first + ": ";
			d += it.second + "\r\n";
		}
		return d;
	}

	std::string&& word_wrap(std::string_view paragraph,int width);
	using directory_list_t = std::vector<std::string>;
	using obj_ptr_t = std::shared_ptr<obj_data>;
	struct objdir_struct {
		obj_ptr_t obj;
		int dir;
	};
	using objdir_t = objdir_struct;

	static inline void texturize_room(room_rnum room_id, room_data::texture_type_t texture_type){
		world[room_id].add_texture(texture_type);
	}
	static inline void detexturize_room(room_rnum room_id, room_data::texture_type_t texture_type){
		world[room_id].remove_texture(texture_type);
	}
	static inline std::string time_string(){
		 char outstr[200];
		 time_t t;
		 struct tm *tmp;

		 t = time(NULL);
		 tmp = localtime(&t);
		 if (tmp == NULL) {
			 return "date-unknown";
		 }
		 std::string format = "%";
		 format += "F-%";
		 format += "T";
		 if (strftime(outstr, sizeof(outstr), format.c_str(), tmp) == 0) {
			 return "date-unknown";
		 }
		 return outstr;
	}



namespace detail{ 
template<typename... Args>
std::ostream& log(std::ostream& out, Args... args);/*{
    (out << ... << args) << "\n";
    return out;
}*/
};

template<typename... Args>
std::ostream& log(Args... args); /*{
    return detail::log(std::cout, args...);
}*/

	/**========================================*/
	/** affected/plr flag conversion utilities */
	/**========================================*/
	uint64_t aff2legacy(mods::flags::aff f);
	mods::flags::aff legacy2aff(uint64_t f);
	uint64_t plr2legacy(mods::flags::plr f);
	mods::flags::plr legacy2plr(uint64_t f);
	/**========================================*/
	/**========================================*/
	/**========================================*/

	directory_list_t glob(std::string_view);
	bool fuzzy_match(const std::string& _needle,const std::string& _haystack);
	template <typename T>
		T arglist(std::string in_arglist_argument) {
			T our_arglist;
			bool escaped = false;
			std::string current = "";
			int string_length = in_arglist_argument.length();

			for(auto c : in_arglist_argument) {
				--string_length;

				if(c == '"') {
					if(escaped) {
						our_arglist.emplace_back(current);
						escaped = false;
						current = "";
					} else {
						escaped = true;
					}

					continue;
				}

				if(c == ' ' && !escaped) {
					if(current.length()) {
						our_arglist.emplace_back(current);
						current = "";
					}

					continue;
				}

				if(c == ' ' && escaped) {
					current += c;
					continue;
				}

				current += c;

				if(string_length == 0 && current.length()) {
					our_arglist.emplace_back(current);
					break;
				}
			}

			return std::move(our_arglist);
		}
	std::optional<unsigned> stoul(std::string_view str);
	template <typename T>
		T stoi(const std::string& i){
			T m;
			std::stringstream ss;
			ss.str(i);
			ss >> m;
			return m;
		}
/*
	template <typename T>
		T stoi(const pqxx::tuple::reference & i){
			T m;
			std::stringstream ss;
			ss.str(pqxx::to_string(i));
			ss >> m;
			return m;
		}
		*/

	template <typename T>
#ifdef __MENTOC_USE_PQXX_RESULT__
		T stoi(const pqxx::result::reference & i){
#else
		T stoi(const pqxx::row::reference & i){
#endif
			T m;
			std::stringstream ss;
			ss.str(pqxx::to_string(i));
			ss >> m;
			return m;
		}
	template <typename T>
		std::optional<T> stoi_optional(std::string_view str){
			T i;
			std::stringstream ss;
			ss << str;
			ss >> i;
			if(ss.fail()){
				return std::nullopt;
			}else{
				return i;
			}
		}
	std::optional<int> stoi(std::string_view str);

	template<int max_char,typename Container>
		inline std::optional<Container> subcmd_args(std::string argument,const char* subcmd) {
			std::array<char,max_char> command;
			std::fill(command.begin(),command.end(),0);
			one_argument(const_cast<char*>(argument.c_str()),&command[0],max_char);

			if(std::string(&command[0]).compare(subcmd) == 0) {
				std::string arg = argument.data();
				return mods::util::arglist<Container>(arg);
			}

			return {};
		}
	std::string itoa(int number);

	template<typename T>
		inline void linked_list_remove(T item, T head) {
			if((item) == (head)) {
				head = (item)->next;
			} else {
				T temp = head;

				while(temp && (temp->next != (item))) {
					temp = temp->next;
				}

				if(temp) {
					temp->next = (item)->next;
				}
			}
		}

	template <typename ContainerType,typename FindType>
		bool exists(const ContainerType * container_ptr,
				const FindType & find_me,
				typename ContainerType::const_iterator & found_at ){
			return (
					found_at =
					std::find(
						container_ptr->begin(),
						container_ptr->end(),
						find_me
						)
					)!= container_ptr->end();
		}
	extern std::function<void(const std::string&,std::string&)> sanitize;
	bool dir_exists(const char*);

	template <typename TStringObject>
		int to_direction(TStringObject str){
			if(str.length() == 0){
				return -1;
			}
			switch(std::tolower(str[0])){
				case 'n': return NORTH;
				case 'e': return EAST;
				case 's': return SOUTH;
				case 'w': return WEST;
				case 'u': return UP;
				case 'd': return DOWN;
				default: return -1;
			}
			return -1;
		}
	std::tuple<bool,direction_t,uint8_t> parse_direction_count(std::vector<std::string>& vec_args);
	std::optional<std::pair<direction_t,uint8_t>> parse_direction_count_optional(std::vector<std::string>& vec_args);
	obj_ptr_t parse_object_vec(player_ptr_t& player,std::vector<std::string>& vec_args);
	obj_ptr_t parse_object(player_ptr_t& player,std::string_view arg, int start_at, int* last_index); 
	int parse_direction(std::string_view arg, int start_at, int* last_index); 
	int parse_direction(std::string_view arg);
	objdir_t parse_objdir(player_ptr_t& player,std::string_view arg);
	static constexpr uint8_t CAP_SINGLE = 0;
	static constexpr uint8_t CAP_ANY = 0;
	static constexpr uint8_t CAP_ALL = 0;
	/*
	objdir_t parse_objdir_capable(player_ptr_t& player,std::string_view arg, uint8_t query_type, cap_list_t& capabilities);
	objdir_t parse_objdir_cap_single(player_ptr_t& player,std::string_view arg, cap_list_t& capabilities);
	objdir_t parse_objdir_cap_any(player_ptr_t& player,std::string_view arg, cap_list_t& capabilities);
	objdir_t parse_objdir_cap_all(player_ptr_t& player,std::string_view arg, cap_list_t& capabilities);
	objdir_t expect_explosive_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_explosive>& types);
	objdir_t expect_rifle_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_rifle>& types);
	objdir_t expect_gadget_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_gadget>& types);
	objdir_t expect_drone_objdir(player_ptr_t& player,std::string_view arg, const std::vector<mw_drone>& types);
	*/
	obj_ptr_t make_from(obj_data* o);
	bool parse_help(std::string_view argument);

	static inline bool match_any(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch){
		for(auto & str : any_of_these){
			if(strncmp(src.data(),str.c_str(),max_ch) == 0){
				return true;
			}
		}
		return false;
	}

	static inline bool is_lower_match(std::string_view str1,std::string_view str2){
		unsigned int sz = str1.size();
		if (str2.size() != sz){
			return false;
		}
	
		for (unsigned int i = 0; i < sz; ++i){
			if (std::tolower(str1[i]) != std::tolower(str2[i])){
				return false;
			}
		}
		return true;
	}
	static inline bool match_any_lower(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch){
		for(auto & str : any_of_these){
			if(is_lower_match(str,src)){
				return true;
			}
		}
		return false;
	}
	static inline player_class_t to_player_class(std::string_view str){
			if(is_lower_match(str,"SNIPER")){ return player_class_t::SNIPER; }
			if(is_lower_match(str,"MARINE")){ return player_class_t::MARINE; }
			if(is_lower_match(str,"BREACHER")){ return player_class_t::BREACHER; }
			if(is_lower_match(str,"ENGINEER")){ return player_class_t::ENGINEER; }
			if(is_lower_match(str,"GHOST")){ return player_class_t::GHOST; }
			if(is_lower_match(str,"MEDIC")){ return player_class_t::MEDIC; }
			if(is_lower_match(str,"SUPPORT")){ return player_class_t::SUPPORT; }
		 	if(is_lower_match(str,"MARKSMAN")){ return player_class_t::CLASS_MARKSMAN; }
		 	if(is_lower_match(str,"BANDIT")){ return player_class_t::CLASS_BANDIT; }
		 	if(is_lower_match(str,"BUTCHER")){ return player_class_t::CLASS_BUTCHER; }
		 	if(is_lower_match(str,"STRIKER")){ return player_class_t::CLASS_STRIKER; }
		 	if(is_lower_match(str,"OBSTRUCTOR")){ return player_class_t::CLASS_OBSTRUCTOR; }
		 	if(is_lower_match(str,"MALADY")){ return player_class_t::CLASS_MALADY; }
		 	if(is_lower_match(str,"PYREXIA")){ return player_class_t::CLASS_PYREXIA; }
		 	if(is_lower_match(str,"DEALER")){ return player_class_t::CLASS_DEALER; }
		 	if(is_lower_match(str,"FORGE")){ return player_class_t::CLASS_FORGE; }
		 	if(is_lower_match(str,"SYNDROME")){ return player_class_t::CLASS_SYNDROME; }
		 	if(is_lower_match(str,"MACHINIST")){ return player_class_t::CLASS_MACHINIST; }
			return player_class_t::CLASS_UNDEFINED;
	}
	/*
	template <typename T,typename TT>
	bool icompare(T str1,TT str2){
		unsigned int sz = a.size();
		if (b.size() != sz){
			return false;
		}
	
		for (unsigned int i = 0; i < sz; ++i){
			if (std::tolower(a[i]) != std::tolower(b[i])){
				return false;
			}
		}
		return true;
	}
	*/

	bool preg_match(std::string_view regex,std::string_view haystack);
	std::vector<std::string> explode(char delim,std::string& haystack);

	bool yaml_file_path_is_sane(std::string path);
	std::string yaml_int_to_string(int type);
	int yaml_string_to_int(std::string type);
	std::string yaml_caps_to_lower(std::string type);
	std::tuple<int,std::string> extract_yaml_info_from_path(std::string_view path);
	bool yaml_file_exists(std::string path);
	bool is_yaml_type(std::string_view type);

	template <typename T>
	void vector_erase(std::vector<T>& vec,T element){
		vec.erase(std::remove(vec.begin(), vec.end(),element), vec.end());
	}
	template <typename T>
	void vector_add_if_not_exists(std::vector<T>& vec,T element){
		auto it = std::find(vec.begin(), vec.end(),element);
		if(it != vec.end()){
			vec.emplace_back(element);
		}
	}
	template <typename T>
	bool in_array(std::string f,std::vector<T> strings){
		return std::find(strings.begin(),strings.end(),f) != strings.end();
	}
	std::string compile_yaml_path_from_type_and_file(int type, std::string_view file);
	std::string proficiency_to_string(int prof);
	std::string player_class_to_string(player_class_t pc);
};

namespace mods::util::err {
	std::string get_string(int);
};

#endif
