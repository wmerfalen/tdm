#ifndef __MENTOC_UTILS_HEADER__
#define  __MENTOC_UTILS_HEADER__

#include "../globals.hpp"
#include "flags.hpp"
#include <random>
//#include <ctime>
#include "../interpreter.h"

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
		"armor","trap","consumable","container","vehicle"
	};
	bool match_any(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);

	bool is_lower_match(std::string_view str1,std::string_view str2);
	bool match_any_lower(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);

	template <typename T>
	static inline bool nth_is_any(std::size_t n,const std::vector<T> vec_args,const std::vector<T>& list_string) {
		if(vec_args.size() <= n) {
			return false;
		}
		return std::any_of(list_string.cbegin(),list_string.cend(),[&](const auto & token) -> bool {
			return is_lower_match(token,vec_args[n]);
		});
	}
	template <typename T>
	static inline bool nth_is(std::size_t n,const std::vector<T> vec_args,std::string_view search) {
		if(vec_args.size() <= n) {
			return false;
		}
		return is_lower_match(search.data(),vec_args[n]);
	}
	enum yaml_type_t {
		UNRECOGNIZED = 0,
		CATCHY_YAML_FILE = 1,
		YAML_FILE_WITH_PREFIX = 2,
		DEEP_OBJECT = 3
	};
	std::string admin_section(std::string_view name);
	std::string overview();
	using strmap_t = std::map<std::string,std::string>;
	std::tuple<bool,int,std::string> extract_yaml_reward(std::string reward);
	std::tuple<bool,int,std::string,strmap_t> extract_deep_reward(std::string reward);
	void wipe();
	void breakline();
	std::vector<std::string> slot_names_for_type(std::string_view type);
	std::string extract_after(std::string_view target, char this_char);
	std::string extract_until(std::string_view target, char this_char);
	bool regex_match(std::string_view regex_string,std::string_view target_string);
	std::string trim_view(std::string_view str);
	std::string trim(std::string& str);
	bool first_alpha_is_any(std::string_view line,std::string_view any);
	long pg_timestamp_to_long(std::string timestamp);

	template <typename T>
	static inline void shuffle(T& vec) {
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(vec.begin(),vec.end(),g);
	}

	constexpr static const char* UNKNOWN_YAML_FILE = "unknown-yits";
	template <typename TKey,typename TVal>
	std::string map2str(std::map<TKey,TVal> in_map) {
		std::string d = "";
		for(auto&& it : in_map) {
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

	void texturize_room(room_rnum room_id, room_data::texture_type_t texture_type);
	void detexturize_room(room_rnum room_id, room_data::texture_type_t texture_type);
	std::string time_string();

	namespace detail {
		template<typename... Args>
		std::ostream& log(std::ostream& out, Args... args);
	};

	template<typename... Args>
	std::ostream& log(Args... args);

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
	T stoi(const std::string& i) {
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
	T stoi(const pqxx::result::reference& i) {
#else
	T stoi(const pqxx::row::reference& i) {
#endif
		T m;
		std::stringstream ss;
		ss.str(pqxx::to_string(i));
		ss >> m;
		return m;
	}
	template <typename T>
	std::optional<T> stoi_optional(std::string_view str) {
		T i;
		std::stringstream ss;
		ss << str;
		ss >> i;
		if(ss.fail()) {
			return std::nullopt;
		} else {
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
	            const FindType& find_me,
	            typename ContainerType::const_iterator& found_at) {
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
	int to_direction(TStringObject str) {
		if(str.length() == 0) {
			return -1;
		}
		switch(std::tolower(str[0])) {
			case 'n':
				return NORTH;
			case 'e':
				return EAST;
			case 's':
				return SOUTH;
			case 'w':
				return WEST;
			case 'u':
				return UP;
			case 'd':
				return DOWN;
			default:
				return -1;
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
	obj_ptr_t make_from(obj_data* o);
	bool parse_help(std::string_view argument);

	bool match_any(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);
	bool is_lower_match(std::string_view str1,std::string_view str2);
	bool match_any_lower(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);
	player_class_t to_player_class(std::string_view str);
	bool preg_match(std::string_view regex,std::string_view haystack);
	std::vector<std::string> explode(char delim,std::string& haystack);
	std::vector<std::string> explode_view(char delim,std::string_view haystack);

	bool yaml_file_path_is_sane(std::string path);
	std::string yaml_int_to_string(int type);
	int yaml_string_to_int(std::string type);
	std::string yaml_caps_to_lower(std::string type);
	std::tuple<int,std::string> extract_yaml_info_from_path(std::string_view path);
	bool yaml_file_exists(std::string path);
	bool is_yaml_type(std::string_view type);

	template <typename T>
	void vector_erase(std::vector<T>& vec,T element) {
		vec.erase(std::remove(vec.begin(), vec.end(),element), vec.end());
	}
	template <typename T>
	void vector_add_if_not_exists(std::vector<T>& vec,T element) {
		auto it = std::find(vec.begin(), vec.end(),element);
		if(it != vec.end()) {
			vec.emplace_back(element);
		}
	}

	template <typename T>
	bool in_array(std::string f,std::vector<T> strings) {
		return std::find(strings.begin(),strings.end(),f) != strings.end();
	}
	std::string compile_yaml_path_from_type_and_file(int type, std::string_view file);
	std::string proficiency_to_string(int prof);
	std::string player_class_to_string(player_class_t pc);
	obj_ptr_t create_pkid_object(std::string_view schema);
};

namespace mods::util::err {
	std::string get_string(int);
};

namespace mods::util::args {
	bool imatch(std::string_view needle, const std::vector<std::string>& arg_list);
	bool match(std::string_view needle, const std::vector<std::string>& arg_list);
	struct parsed_args {
		std::size_t size;
		std::vector<std::string> vec_args;
		std::vector<int> i_storage;

		int fetch_integer(std::size_t index);
		int fetch_parsed_integer(std::size_t index);
		void assign(const std::string& argument);
		bool first_is(std::string_view list_string);
		bool first_is_any(std::vector<std::string> list_string);
		bool nth_is_any(std::size_t index,std::vector<std::string> list_string);
		bool if_nth_has_either(std::size_t index,std::vector<std::string> list_string);
		bool nth_has_integer(std::size_t index);
		bool nth_has_integer(std::vector<std::size_t> index);
		parsed_args* save_integer(std::size_t index);
		parsed_args* save_integer(const std::vector<std::size_t>& index);
		std::vector<int> gather_integers_starting_at(std::size_t index);
		std::string gather_strings_starting_at(std::size_t index);

		std::string at(std::size_t index);
		int int_at(std::size_t index);
		std::vector<std::string> multi(std::vector<std::size_t> indexes);
		std::map<std::size_t,int> int_map(std::vector<std::size_t> indexes);
	};
	struct check_parsed_args {
			parsed_args* p;
			void feed(parsed_args* _p) {
				p = _p;
			}
			std::map<std::string,bool> passed_map;
			std::vector<std::string> where_failed;
			void clear() {
				passed_map.clear();
				where_failed.clear();
			}
			check_parsed_args* first_is(std::string_view list_string);
			check_parsed_args* first_is_any(std::vector<std::string> list_string);
			check_parsed_args* nth_is_any(std::size_t index,std::vector<std::string> list_string);
			check_parsed_args* if_nth_has_either(std::size_t index,std::vector<std::string> list_string);
			check_parsed_args* nth_has_integer(std::size_t index);
			check_parsed_args* nth_has_integer(std::vector<std::size_t> index);
			check_parsed_args* int_at(std::size_t index);
			check_parsed_args* int_at(std::vector<std::size_t> index);
			check_parsed_args* size_gt(std::size_t size);
			check_parsed_args* size_eq(std::size_t size);
			std::string errors();
			bool passed();
		private:
			void result(std::string f,std::string value,bool passed);
			void result(std::string f,std::size_t value,bool passed);
	};
	struct arglist_parser {
		parsed_args parsed;
		parsed_args* use(const std::string& argument) {
			static std::string static_argument;
			if(static_argument.compare(argument.c_str()) == 0) {
				/** already parsed */
				return &parsed;
			}
			static_argument = argument;
			parsed.assign(argument);
			return &parsed;
		}
		check_parsed_args check;
		check_parsed_args* have(const std::string& argument) {
			this->use(argument);
			check.clear();
			check.feed(&parsed);
			return &check;
		}
	};
	static std::map<uuid_t,arglist_parser> player_parsers;
};
#define args() mods::util::args::player_parsers[player->uuid()].use(argument)
#define argat(A) mods::util::args::player_parsers[player->uuid()].use(argument)->at(A)
#define intat(A) mods::util::args::player_parsers[player->uuid()].use(argument)->int_at(A)
#define argsat(A) mods::util::args::player_parsers[player->uuid()].use(argument)->multi(A)
#define argshave() mods::util::args::player_parsers[player->uuid()].have(argument)

#endif
