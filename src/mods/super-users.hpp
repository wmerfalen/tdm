#ifndef __MENTOC_MODS_SUPER_USERS_HEADER__
#define __MENTOC_MODS_SUPER_USERS_HEADER__


#include <vector>
#include <string>
#include "../globals.hpp"

namespace mods::super_users {
	bool player_name_is(std::string_view);
	bool player_is(player_ptr_t& player);
	bool add(std::string_view);
	bool remove(std::string_view);
	std::string get_list();
};

ACMD(do_add_super_user);
ACMD(do_remove_super_user);
ACMD(do_get_super_user_list);

#endif
