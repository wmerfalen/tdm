#ifndef __MENTOC_MODS_ORM_CLASS_PSYOP_HEADER__
#define __MENTOC_MODS_ORM_CLASS_PSYOP_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct psyop : public mods::orm::orm_base<psyop,mods::weapon::psyop::primary_choice_t> {
		using primary_choice_t = mods::weapon::psyop::primary_choice_t;
		const std::string SCARH = "SCARH";
		const std::string UMP45 = "UMP45";
		const std::string CZP10 = "CZP10";
		std::string table_name() const { return "class_psyop"; }
		std::string column_prefix() const { return "psyop_"; }
		std::string id_column() const { return "psyop_id"; }
		psyop() : id(0) {
			this->init(); loaded = 0;
		}
		~psyop() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();

		primary_choice_t primary_type(){
			if(psyop_primary_type.compare(SCARH) == 0){
				return primary_choice_t::SCARH;
			}
			if(psyop_primary_type.compare(UMP45) == 0){
				return primary_choice_t::UMP45;
			}
			return primary_choice_t::NONE;
		}

		strmap_t export_class();
		int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<psyop>(this));
		}

		uint64_t id;
		uint64_t psyop_id;
		uint64_t psyop_player_id;
		std::string psyop_primary_type;
		uint64_t psyop_primary_weapon_id;
		std::string psyop_secondary_type;
		uint64_t psyop_secondary_weapon_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
