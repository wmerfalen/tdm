#ifndef __MENTOC_MODS_ORM_CLASS_SENTINEL_HEADER__
#define __MENTOC_MODS_ORM_CLASS_SENTINEL_HEADER__

#include <iostream>
#include <vector>
#include "../player.hpp"
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include "base.hpp"
#include <string_view>
#include "../weapon.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	struct sentinel : public base {
		using primary_choice_t = mods::weapon::sentinel::primary_choice_t;
		sentinel() : base(), id(0) { this->init(); loaded = 0; }
		~sentinel() = default;
		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		static constexpr const char* table_name = "class_sentinel";
		uint64_t id;
		uint64_t sentinel_id; // SERIAL,
		uint64_t sentinel_player_id; // INTEGER NOT NULL,
		std::string sentinel_primary_type;// VARCHAR(16) NOT NULL DEFAULT 'mp5',
		uint64_t sentinel_primary_weapon_id;// INTEGER NOT NULL DEFAULT 0,
		std::string sentinel_secondary_type;// VARCHAR(16) NOT NULL DEFAULT 'czp10',
		uint64_t sentinel_secondary_weapon_id;// INTEGER NOT NULL DEFAULT 0,
		uint64_t sentinel_heal_level;// INTEGER NOT NULL DEFAULT 0,
		uint64_t sentinel_intimidate_level;// INTEGER NOT NULL DEFAULT 0,
		long created_at;// TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		long updated_at;// TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	};

};

#endif
