#ifndef __MENTOC_MODS_ORM_CLASS_MARINE_HEADER__
#define __MENTOC_MODS_ORM_CLASS_MARINE_HEADER__

#include <iostream>
#include <vector>
#include "../../globals.hpp"
#include "../pq.hpp"
#include "../sql.hpp"


namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct marine {
		using primary_choice_t = mods::weapon::marine::primary_choice_t;
		marine() : id(0) { this->init(); loaded = 0; }
		~marine() = default;
		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		static constexpr const char* table_name = "class_marine";

		int16_t save();
		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t marine_id;
		uint64_t marine_player_id;
		std::string marine_primary_type;
		uint64_t marine_primary_weapon_id;
		std::string marine_secondary_type;
		uint64_t marine_secondary_weapon_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
