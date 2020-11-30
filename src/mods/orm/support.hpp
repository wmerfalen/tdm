#ifndef __MENTOC_MODS_ORM_CLASS_SUPPORT_HEADER__
#define __MENTOC_MODS_ORM_CLASS_SUPPORT_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct support : public mods::orm::orm_base<support,mods::weapon::support::primary_choice_t> {
		using primary_choice_t = mods::weapon::support::primary_choice_t;
		const std::string MK46 = "MK46";
		const std::string HK21 = "HK21";
		const std::string CZP10 = "CZP10";
		std::string table_name() const { return "class_support"; }
		std::string column_prefix() const { return "support_"; }
		std::string id_column() const { return "support_id"; }
		support() : id(0) {
			this->init(); loaded = 0;
		}
		~support() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row(player_ptr_t &player);
		int16_t feed(const pqxx::result::reference &);
		void init();

		primary_choice_t primary_type(){
			return primary_choice_t::MK46;
		}

		strmap_t export_class();
		int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<support>(this));
		}

		uint64_t id;
		uint64_t support_id;
		uint64_t support_player_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
