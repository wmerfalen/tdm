#ifndef __MENTOC_MODS_SUPER_USERS_HEADER__
#define __MENTOC_MODS_SUPER_USERS_HEADER__


#include <vector>
#include <string>
#include "../globals.hpp"
#include "classes/super-user-fiddler.hpp"
#include "../interpreter.h"

namespace mods::super_users {
	bool player_name_is(std::string_view);
	bool player_is(player_ptr_t& player);
	bool is(const uuid_t& player_uuid);
	bool add(std::string_view);
	bool remove(std::string_view);
	bool invisible(player_ptr_t& player);
	std::string get_list();
	std::string who_line(player_ptr_t& player);
	void set_who_line(std::string_view player_name,std::string_view line);
	void clear_who_line(std::string_view player_name);
	static std::shared_ptr<mods::classes::super_user_fiddler> fiddler;
};

ACMD(do_set_who_line);
ACMD(do_add_super_user);
ACMD(do_remove_super_user);
ACMD(do_get_super_user_list);

#endif
