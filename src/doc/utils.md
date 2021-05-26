# Utility functions inside mods::utils

There are a few functions in the mods::util namespace that can make your life as a coder a bit easier.

# Parsing arguments
The legacy way of parsing arguments can be seen anywhere in the codebase where you see `PARSE_ARGS()` being called.
This is deprecated and will be phased out. The preferred method of parsing arguments is using the `arg*` helpers.

Examples:
```
ACMD(do_something){
	if(argshave()->int_at({1})->passed()){
		player->sendln("You passed in an integer at the second position!");
	}
}
```

DEPRECATED way to do it:
```
ACMD(do_something){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() > 1 && mods::util::stoi(vec_args[1]).value_or(-1) != -1){
		player->sendln("You passed in an integer at the second position!");
	}
}
```

# args() and argshave() helper
All the various helpers essentially call the same functions but for the most part have different implications. 
The `args()` helper is usually used for checking one or two different conditions. The `argshave()` helper
is used when you want to *chain* together assertions.

Let's say you want to check if the size of the arguments is greater than 2 and the first parameter is "create".
In addition, you'd like to save the second parameter as an integer. To do this using the `args()` helper
would look like this:
```
	if(args()->save_integer(1)->first_is("create") && args()->size > 2){
		player->sendln(CAT("You specified a create operation with an integer of: ",args()->fetch_integer(1)));
	}
```

Since the `args()` helper returns a `parsed_args` instance, once you call `first_is` you cannot chain any other
assertion because `first_is` returns a boolean.

To chain together assertions it would be easier to use `argshave()`:
```
	if(argshave()->int_at(1)->first_is("create")->size_gt(2)->passed()){
		...
	}
```

Notice here that you have to use `passed()`. This is because the `argshave()` macro returns a point to
`check_parsed_args` for every function. The `passed()` function will check all your assertions and return
true if all of them passed.

# argat() helper
If all you want is a specific argument at a specific index, simply call `argat(N)` where N is the index.

Example:
```
ACMD(do_something){
	player->sendln(CAT("Argument at 0:'",argat(0),"'"));
}
```

# The following helpers all exist within mods::util 

# string matching helpers
	bool match_any(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);
	bool is_lower_match(std::string_view str1,std::string_view str2);
	bool match_any_lower(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);
	bool nth_is_any(std::size_t n,const std::vector<T> vec_args,const std::vector<T>& list_string);
	bool nth_is(std::size_t n,const std::vector<T> vec_args,std::string_view search);
	std::string extract_after(std::string_view target, char this_char);
	std::string extract_until(std::string_view target, char this_char);
	bool regex_match(std::string_view regex_string,std::string_view target_string);
	std::string trim_view(std::string_view str);
	std::string trim(std::string& str);
	bool first_alpha_is_any(std::string_view line,std::string_view any);
	directory_list_t glob(std::string_view);
	bool fuzzy_match(const std::string& _needle,const std::string& _haystack);

	bool parse_help(std::string_view argument);

	bool match_any(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);
	bool is_lower_match(std::string_view str1,std::string_view str2);
	bool match_any_lower(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);
	player_class_t to_player_class(std::string_view str);
	bool preg_match(std::string_view regex,std::string_view haystack);
	std::vector<std::string> explode(char delim,std::string& haystack);
	std::vector<std::string> explode_view(char delim,std::string_view haystack);

# int to string conversions
	std::optional<unsigned> stoul(std::string_view str);
	T stoi(const std::string& i);
	std::optional<T> stoi_optional(std::string_view str)
	std::string itoa(int number);

# linked list
	void linked_list_remove(T item, T head)

# container operations
	bool exists(const ContainerType * container_ptr,
	            const FindType& find_me,
	            typename ContainerType::const_iterator& found_at)

# parsing commandline objects
	obj_ptr_t parse_object_vec(player_ptr_t& player,std::vector<std::string>& vec_args);
	obj_ptr_t parse_object(player_ptr_t& player,std::string_view arg, int start_at, int* last_index);
	objdir_t parse_objdir(player_ptr_t& player,std::string_view arg);

# objects
	obj_ptr_t make_from(obj_data* o);
	obj_ptr_t create_pkid_object(std::string_view schema);

# directional
	int to_direction(TStringObject str);
	std::tuple<bool,direction_t,uint8_t> parse_direction_count(std::vector<std::string>& vec_args);
	std::optional<std::pair<direction_t,uint8_t>> parse_direction_count_optional(std::vector<std::string>& vec_args);
	int parse_direction(std::string_view arg, int start_at, int* last_index);
	int parse_direction(std::string_view arg);

# file system
	bool dir_exists(const char*);

# map operations
	std::string map2str(std::map<TKey,TVal> in_map);

# vector operations
	void vector_erase(std::vector<T>& vec,T element) {
	void vector_add_if_not_exists(std::vector<T>& vec,T element) {
	bool in_array(std::string f,std::vector<T> strings) {
	void shuffle(T& vec);

# time functions
	std::string time_string();


# player proficiencies
	std::string proficiency_to_string(int prof);

# player classes
	std::string player_class_to_string(player_class_t pc);

# affects
	uint64_t aff2legacy(mods::flags::aff f);
	mods::flags::aff legacy2aff(uint64_t f);
	uint64_t plr2legacy(mods::flags::plr f);
	mods::flags::plr legacy2plr(uint64_t f);

# postgres helpers
	long pg_timestamp_to_long(std::string timestamp);

# yaml helpers
	std::string compile_yaml_path_from_type_and_file(int type, std::string_view file);
	std::tuple<bool,int,std::string> extract_yaml_reward(std::string reward);
	std::tuple<bool,int,std::string,strmap_t> extract_deep_reward(std::string reward);
	bool yaml_file_path_is_sane(std::string path);
	std::string yaml_int_to_string(int type);
	int yaml_string_to_int(std::string type);
	std::string yaml_caps_to_lower(std::string type);
	std::tuple<int,std::string> extract_yaml_info_from_path(std::string_view path);
	bool yaml_file_exists(std::string path);
	bool is_yaml_type(std::string_view type);
