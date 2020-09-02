#ifndef __MENTOC_MODS_ORM_CLASS_CONTAGION_HEADER__
#define __MENTOC_MODS_ORM_CLASS_CONTAGION_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct contagion : public mods::orm::orm_base<contagion,mods::weapon::contagion::primary_choice_t> {
		using primary_choice_t = mods::weapon::contagion::primary_choice_t;
		const std::string M3 = "M3";
		const std::string FAMAS = "FAMAS";
		const std::string CZP10 = "CZP10";
		std::string table_name() const { return "class_contagion"; }
		std::string column_prefix() const { return "contagion_"; }
		std::string id_column() const { return "contagion_id"; }
		contagion() : id(0) {
			this->init(); loaded = 0;
		}
		~contagion() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();

		primary_choice_t primary_type(){
			if(contagion_primary_type.compare(M3) == 0){
				return primary_choice_t::M3;
			}
			if(contagion_primary_type.compare(FAMAS) == 0){
				return primary_choice_t::FAMAS;
			}
			return primary_choice_t::NONE;
		}

		strmap_t export_class();
		int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<contagion>(this));
		}

		uint64_t id;
		uint64_t contagion_id;
		uint64_t contagion_player_id;
		std::string contagion_primary_type;
		uint64_t contagion_primary_weapon_id;
		std::string contagion_secondary_type;
		uint64_t contagion_secondary_weapon_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif

