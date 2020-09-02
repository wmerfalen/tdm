#ifndef __MENTOC_MODS_ORM_CLASS_SNIPER_HEADER__
#define __MENTOC_MODS_ORM_CLASS_SNIPER_HEADER__

#include <iostream>
#include <vector>
#include "../../globals.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct sniper {
		constexpr static const char* PSG1 = "PSG1";
		constexpr static const char* L96AW = "L96AW";
		using primary_choice_t = mods::weapon::sniper::primary_choice_t;

		sniper() : id(0) { this->init(); loaded = 0; }
		~sniper() = default;

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		static constexpr const char* table_name = "class_sniper";
		primary_choice_t primary_type(){
			if(sniper_primary_type.compare(PSG1) == 0){
				return primary_choice_t::PSG1;
			}
			if(sniper_primary_type.compare(L96AW) == 0){
				return primary_choice_t::L96AW;
			}
			return primary_choice_t::NONE;
		}

		int16_t save();
		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t sniper_id;
		uint64_t sniper_player_id;
		std::string sniper_primary_type;
		uint64_t sniper_primary_weapon_id;
		std::string sniper_secondary_type;
		uint64_t sniper_secondary_weapon_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
