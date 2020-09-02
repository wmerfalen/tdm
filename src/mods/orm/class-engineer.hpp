#ifndef __MENTOC_MODS_ORM_CLASS_ENGINEER_HEADER__
#define __MENTOC_MODS_ORM_CLASS_ENGINEER_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct engineer : public mods::orm::orm_base<engineer,mods::weapon::engineer::primary_choice_t> {
		using primary_choice_t = mods::weapon::engineer::primary_choice_t;
		const std::string FMG9 = "FMG9";
		const std::string P90 = "P90";
		const std::string CZP10 = "CZP10";
		std::string table_name() const { return "class_engineer"; }
		std::string column_prefix() const { return "engineer_"; }
		std::string id_column() const { return "engineer_id"; }
		engineer() : id(0) {
			this->init(); loaded = 0;
		}
		~engineer() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();

		primary_choice_t primary_type(){
			if(engineer_primary_type.compare(FMG9) == 0){
				return primary_choice_t::FMG9;
			}
			if(engineer_primary_type.compare(P90) == 0){
				return primary_choice_t::P90;
			}
			return primary_choice_t::NONE;
		}

		strmap_t export_class();
		int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<engineer>(this));
		}

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
