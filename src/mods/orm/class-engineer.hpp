#ifndef __MENTOC_MODS_ORM_CLASS_ENGINEER_HEADER__
#define __MENTOC_MODS_ORM_CLASS_ENGINEER_HEADER__

#include <iostream>
#include <vector>
#include "../../globals.hpp"
#include "../pq.hpp"
#include "../sql.hpp"


namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct engineer {
		using primary_choice_t = mods::weapon::engineer::primary_choice_t;
		engineer() : id(0) { this->init(); loaded = 0; }
		~engineer() = default;
		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		static constexpr const char* table_name = "class_engineer";

		int16_t save();
		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t engineer_id;
		uint64_t engineer_player_id;
		std::string engineer_primary_type;
		uint64_t engineer_primary_weapon_id;
		std::string engineer_secondary_type;
		uint64_t engineer_secondary_weapon_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
