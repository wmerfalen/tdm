#ifndef __MENTOC_MODS_ORM_CLASS_MARINE_HEADER__
#define __MENTOC_MODS_ORM_CLASS_MARINE_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct marine : public mods::orm::orm_base<marine,mods::weapon::marine::primary_choice_t> {
		using primary_choice_t = mods::weapon::marine::primary_choice_t;
		const std::string M16A4 = "M16A4";
		const std::string M4 = "M4";
		const std::string CZP10 = "CZP10";
		std::string table_name() const { return "class_marine"; }
		std::string column_prefix() const { return "marine_"; }
		std::string id_column() const { return "marine_id"; }
		marine() : id(0) {
			this->init(); loaded = 0;
		}
		~marine() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();

		primary_choice_t primary_type(){
			if(marine_primary_type.compare(M16A4) == 0){
				return primary_choice_t::M16A4;
			}
			if(marine_primary_type.compare(M4) == 0){
				return primary_choice_t::M4;
			}
			return primary_choice_t::NONE;
		}

		strmap_t export_class();
		int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<marine>(this));
		}

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

