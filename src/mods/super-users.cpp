#include "super-users.hpp"
#include "help.hpp"

namespace mods::super_users {
	static std::vector<std::string> super_users;
	static bool super_users_initialized = 0;
	bool player_name_is(std::string_view name){
		if(super_users_initialized){
			return invec(name.data(),super_users);
		}
		if(!super_users_initialized){
			super_users = EXPLODE(SUPER_USERS_LIST(),'|');
			super_users_initialized = 1;
			return player_name_is(name);
		}
		return invec(name.data(),super_users);
	}
	bool player_is(player_ptr_t& player){
		return player_name_is(player->name());
	}
	bool add(std::string_view name){
		if(!player_name_is(name)){
			super_users.emplace_back(name);
			return true;
		}
		return false;
	}
	bool remove(std::string_view name){
		auto it = std::find(super_users.begin(),super_users.end(),name.data());
		if(it != super_users.end()){
			super_users.erase(it);
			return true;
		}
		return false;
	}
	std::string get_list(){
		return IMPLODE_PAD("{grn}'",super_users,"'{/grn}\r\n",true);
	}
};

ACMD(do_add_super_user){
	DO_HELP("add_super_user");
	ADMIN_REJECT();

	auto vec_args = PARSE_ARGS();
	if(vec_args.size() > 0){
		for(auto name : vec_args){
			mods::super_users::add(name);
		}
		ADMIN_DONE();
		return;
	}
	ADMIN_FAIL();
}

ACMD(do_get_super_user_list){
	DO_HELP("get_super_user_list");
	ADMIN_REJECT();
	player->send(mods::super_users::get_list().c_str());
}
ACMD(do_remove_super_user){
	DO_HELP("remove_super_user");
	ADMIN_REJECT();
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() > 0){
		for(auto name : vec_args){
			mods::super_users::remove(name);
		}
		ADMIN_DONE();
		return;
	}
	ADMIN_FAIL();
}
