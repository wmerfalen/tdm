#ifndef __MENTOC_MODS_ORM_CLASS_BREACHER_HEADER__
#define __MENTOC_MODS_ORM_CLASS_BREACHER_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct breacher : public mods::orm::orm_base<breacher,mods::weapon::breacher::primary_choice_t> {
		using primary_choice_t = mods::weapon::breacher::primary_choice_t;
		const std::string MP5 = "MP5";
		const std::string SASG12 = "SASG12";
		std::string table_name() const { return "class_breacher"; }
		std::string column_prefix() const { return "breacher_"; }
		std::string id_column() const { return "breacher_id"; }
		breacher() : id(0) {
			this->init(); loaded = 0;
		}
		~breacher() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row(player_ptr_t &player);
		int16_t feed(const pqxx::result::reference &);
		void init();

		primary_choice_t primary_type(){
			return primary_choice_t::MP5;
		}

		strmap_t export_class();
		int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<breacher>(this));
		}

		uint64_t id;
		uint64_t breacher_id;
		uint64_t breacher_player_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif

