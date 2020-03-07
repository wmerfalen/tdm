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
	int16_t flush_player(player_ptr_t & player);
	int16_t feed_player(player_ptr_t & player);
	namespace lmdb {
		void add_player_wear(uint64_t player_db_id, uint64_t object_db_id, uint8_t obj_type, uint8_t position);
		void remove_player_wear(uint64_t player_db_id, uint8_t position);
		void add_player_inventory(uint64_t player_db_id, uint64_t object_db_id, uint16_t obj_type);
		void remove_player_inventory(uint64_t player_db_id, uint64_t object_db_id);
	};
};

#endif
