#ifndef __MENTOC_MODS_ORM_SNIPER_HEADER__
#define __MENTOC_MODS_ORM_SNIPER_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct sniper : public mods::orm::orm_base<sniper,mods::weapon::sniper::primary_choice_t> {
		using primary_choice_t = mods::weapon::sniper::primary_choice_t;
		const std::string PSG1 = "PSG1";
		const std::string L96AW = "L96AW";
		std::string table_name(){ return "class_sniper"; }
		std::string column_prefix(){ return "sniper_"; }
		std::string id_column(){ return "sniper_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->id); }

		sniper() : id(0) { this->init(); loaded = 0; }
		~sniper() = default;

		uint64_t initialize_row(player_ptr_t &player);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		primary_choice_t primary_type(){
				return primary_choice_t::PSG1;
		}
		int16_t save(){
			return std::get<0>(this->update<sniper>(this));
		}

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
