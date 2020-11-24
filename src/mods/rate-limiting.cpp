#include "rate-limiting.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "help.hpp"

#ifdef __MENTOC_MODS_RATE_LIMITING_DEBUG__
#define mo_debug(A) std::cerr << "[mods::rate_limiting][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

namespace mods::rate_limiting {
	/**
	 * static database asset
	 */
	static std::vector<std::string> action_strings;
	/**
	 * static database asset
	 */
	static const std::vector<action_t> valid_action_types = {
		action_t::PLAYER_SAVE
	};
	/**
	 * static database asset
	 */
	static std::map<action_t,std::pair<int,int>> limits;
	
	/** 
	 * enum<->string converters
	 */
	std::string action_to_string(action_t action){
#define MENTOC_ACT(A) case action_t::A: return #A;
		switch(action){
			MENTOC_ACT(PLAYER_SAVE);
			default:
				return std::to_string(action);
		}
#undef MENTOC_ACT
	}
	action_t string_to_action(std::string_view action){
#define MENTOC_ACT(A) if(ICMP(action.data(),#A)){ return action_t::A; }
		MENTOC_ACT(PLAYER_SAVE);
		return action_t::UNKNOWN_ACTION;
	}

	/**
	 * ACMD support functions
	 */

	void set_rate_limit(action_t action,int times,int seconds){
		mods::rate_limiting::limits[action] = std::make_pair<>(times,seconds);
	}

	/**
	 * Passive utility function
	 */
	void trim_rate_limit_data(action_t action,mods::player::rate_limit_data_t& data,int times,int seconds){
		auto now = ::time(nullptr);
		mods::player::rate_limit_data_t trimmed;
		for(auto & timestamp : data){
			if(timestamp < (now - seconds)){
				continue;
			}
			trimmed.emplace_back(timestamp);
		}
		data = trimmed;
	}

	/**
	 * Called by ACMD client code
	 */
	bool should_throttle(action_t action, player_ptr_t& player){
		auto & rate_limit_data = player->get_rate_limit_data(action);
		int times = limits[action].first;
		int seconds = limits[action].second;
		trim_rate_limit_data(action,rate_limit_data,times,seconds);
		return rate_limit_data.size() >= times;
	}
	void rate_limit_hit(action_t action,player_ptr_t& player){
		player->rate_limit_hit(action);
	}

static const char* set_rate_limit_help = "usage: set_rate_limit <action> <times> <seconds>";
static const char* get_rate_limit_types_help = "usage: get_rate_limit_types";
static const char* list_rate_limits_help = "usage: list_rate_limits [search]";
	/**
	 * admin utility code
	 */
ACMD(do_get_rate_limit_types){
	ADMIN_REJECT();
	DO_HELP("get_rate_limit_types");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size()){
		player->sendln("Starting list...");
		int limit = 128;
		mods::search_screen(player,mods::rate_limiting::action_strings, vec_args, limit);
		player->sendln("Done listing.");
		return;
	}
	player->sendln("Starting list...");
	player->send(mods::rate_limiting::action_strings);
	player->sendln("Done listing.");
	player->sendln(CAN_BE_SEARCHED());
	ADMIN_DONE();
}
	
	/**
	 * admin utility code
	 */
ACMD(do_set_rate_limit){
	ADMIN_REJECT();
	DO_HELP("set_rate_limit");
	static const char* usage = set_rate_limit_help;
	/** code here */
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() != 3){
		player->sendln(usage);
		return;
	}
	mods::rate_limiting::action_t action = action_t::UNKNOWN_ACTION;
	action = mods::rate_limiting::string_to_action(vec_args[0]);
	if(action == action_t::UNKNOWN_ACTION){
		player->errorln("Invalid action type. For a list of valid actions, use get_rate_limit_types");
		return;
	}
	set_rate_limit(
		action,
		mods::util::stoi<int>(vec_args[1]),
		mods::util::stoi<int>(vec_args[2])
	);
	ADMIN_DONE();
}
	/**
	 * admin utility code
	 */
ACMD(do_list_rate_limits){
	ADMIN_REJECT();
	DO_HELP("list_rate_limits");
	/** code here */
	auto vec_args = PARSE_ARGS();
	player->sendln("Starting list...");
	std::vector<std::string> screen;
	static constexpr std::size_t limit = 64;
	for(const auto & data : mods::rate_limiting::limits){
		screen.emplace_back(CAT("type: ",mods::rate_limiting::action_to_string(data.first)," allow ",data.second.first," times over a span of ",data.second.second," seconds"));
	}
	if(vec_args.size()){
		mods::search_screen(player,screen, vec_args, limit);
	}else{
		player->send(screen);
	}
	player->sendln("Done listing.");
	player->sendln(CAN_BE_SEARCHED());
	ADMIN_DONE();
}

	void init(){
		for(auto & type : valid_action_types){
			action_strings.emplace_back(action_to_string(type));
		}
		mods::interpreter::add_command("set_rate_limit", POS_RESTING, do_set_rate_limit, LVL_BUILDER,0);
		mods::interpreter::add_command("get_rate_limit_types", POS_RESTING, do_get_rate_limit_types, LVL_BUILDER,0);
		mods::interpreter::add_command("list_rate_limits", POS_RESTING, do_list_rate_limits, LVL_BUILDER,0);
		mods::interpreter::add_command("list_rate_limit", POS_RESTING, do_list_rate_limits, LVL_BUILDER,0);
		mods::help::register_help_command_with_permission("set_rate_limit",set_rate_limit_help,LVL_BUILDER);
		mods::help::register_help_command_with_permission("get_rate_limit_types",get_rate_limit_types_help,LVL_BUILDER);
		mods::help::register_help_command_with_permission("list_rate_limits",list_rate_limits_help,LVL_BUILDER);
	}
};
#undef mo_debug
