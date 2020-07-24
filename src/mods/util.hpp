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
			std::cerr << "arglist! '" << in_arglist_argument << "'\n";
			T our_arglist;
			bool escaped = false;
			std::string current = "";
			int string_length = in_arglist_argument.length();
			std::cerr << "string_length: " << string_length << "\n";

			for(auto c : in_arglist_argument) {
				--string_length;

				if(c == '"') {
					if(escaped) {
					std::cerr << "emplacing current: '" << current << "'\n";
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
					std::cerr << "emplacing current: '" << current << "'\n";
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
					std::cerr << "emplacing current: '" << current << "'\n";
					our_arglist.emplace_back(current);
					break;
				}
			}
			std::cerr << "moving...\n";

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
	obj_ptr_t parse_object_vec(player_ptr_t& player,std::vector<std::string>& vec_args);
	obj_ptr_t parse_object(player_ptr_t& player,std::string_view arg, int start_at, int* last_index); 
	int parse_direction(std::string_view arg, int start_at, int* last_index); 
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
};

namespace mods::util::err {
	std::string get_string(int);
};

#endif
