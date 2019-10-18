#ifndef __MENTOC_MODS_ACL_LIST_HEADER__
#define __MENTOC_MODS_ACL_LIST_HEADER__

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "player.hpp"

namespace mods::acl_list {
		void set_access_rights(player_ptr_t,const std::string& group,bool status);
};


#endif
