#ifndef __MENTOC_MODS_ORM_INVENTORY_HEADER__
#define __MENTOC_MODS_ORM_INVENTORY_HEADER__

#include <iostream>
#include "../player.hpp"
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include "base.hpp"

namespace mods::orm::inventory {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	int16_t feed_player_by_uuid(uuid_t);
	int16_t feed_player(player_ptr_t& player);
	std::tuple<bool,std::string> unequip(const uint64_t& player_db_id,obj_ptr_t& object,const std::size_t& position);
	std::tuple<bool,std::string> equip(const uint64_t& player_db_id,const std::size_t& position,obj_ptr_t& object);
	std::tuple<bool,std::string> carry(const uint64_t& player_db_id,obj_ptr_t& object);
	std::tuple<bool,std::string> uncarry(const uint64_t& player_db_id,obj_ptr_t& object);
};

#endif
