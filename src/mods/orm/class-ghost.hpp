#ifndef __MENTOC_MODS_ORM_CLASS_GHOST_HEADER__
#define __MENTOC_MODS_ORM_CLASS_GHOST_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct ghost : public mods::orm::orm_base<ghost,mods::weapon::ghost::primary_choice_t> {
		using primary_choice_t = mods::weapon::ghost::primary_choice_t;
		const std::string PSG1 = "PSG1";
		const std::string L96AW = "L96AW";
		std::string table_name(){ return "class_ghost"; }
		std::string column_prefix(){ return "ghost_"; }
		std::string id_column(){ return "ghost_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->id); }

		ghost() : id(0) { this->init(); loaded = 0; }
		~ghost() = default;

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		primary_choice_t primary_type(){
			if(ghost_primary_type.compare(PSG1) == 0){
				return primary_choice_t::PSG1;
			}
			if(ghost_primary_type.compare(L96AW) == 0){
				return primary_choice_t::L96AW;
			}
			return primary_choice_t::NONE;
		}
		int16_t save(){
			return std::get<0>(this->update<ghost>(this));
		}

		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t ghost_id;
		uint64_t ghost_player_id;
		std::string ghost_primary_type;
		uint64_t ghost_primary_weapon_id;
		std::string ghost_secondary_type;
		uint64_t ghost_secondary_weapon_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
