#ifndef __MENTOC_MODS_SKILL_ORM_ADAPTOR_HEADER__
#define __MENTOC_MODS_SKILL_ORM_ADAPTOR_HEADER__

#include "../globals.hpp"
#include "string.hpp"
#include <unordered_map>
namespace mods::skill_orm_adaptor {
	int get_player_map(player_ptr_t& player, std::vector<std::string,std::string>& skills);
	void put_player_map(player_ptr_t& player,const std::map<std::string,std::string>& map);
};

#endif
