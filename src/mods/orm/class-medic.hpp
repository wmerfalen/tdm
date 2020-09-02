#ifndef __MENTOC_MODS_ORM_CLASS_MEDIC_HEADER__
#define __MENTOC_MODS_ORM_CLASS_MEDIC_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct medic : public mods::orm::orm_base<medic,mods::weapon::medic::primary_choice_t> {
		using primary_choice_t = mods::weapon::medic::primary_choice_t;
		const std::string AUGPARA = "AUGPARA";
		const std::string TAR21 = "TAR21";
		const std::string CZP10 = "CZP10";
		std::string table_name() const { return "class_medic"; }
		std::string column_prefix() const { return "medic_"; }
		std::string id_column() const { return "medic_id"; }
		medic() : id(0) {
			this->init(); loaded = 0;
		}
		~medic() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();

		primary_choice_t primary_type(){
			if(medic_primary_type.compare(AUGPARA) == 0){
				return primary_choice_t::AUGPARA;
			}
			if(medic_primary_type.compare(TAR21) == 0){
				return primary_choice_t::TAR21;
			}
			return primary_choice_t::NONE;
		}

		strmap_t export_class();
		int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<medic>(this));
		}

		uint64_t id;
		uint64_t medic_id;
		uint64_t medic_player_id;
		std::string medic_primary_type;
		uint64_t medic_primary_weapon_id;
		std::string medic_secondary_type;
		uint64_t medic_secondary_weapon_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
