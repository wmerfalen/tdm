#ifndef __MENTOC_MODS_ORM_CLASS_MEDIC_HEADER__
#define __MENTOC_MODS_ORM_CLASS_MEDIC_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct integral_object : public mods::orm::orm_base<integral_object,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const { return "integral_object"; }
		std::string column_prefix() const { return "object_"; }
		std::string id_column() const { return "object_id"; }
		integral_object() : id(0) {
			this->init(); loaded = 0;
		}
		~integral_object() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		uint64_t initialize_row();
		int16_t feed(const pqxx::result::reference &);
		void init();

		strmap_t export_class();
		//int16_t				load_by_player(uint64_t);
		int16_t save(){
			return std::get<0>(this->update<integral_object>(this));
		}

		uint64_t id;
		uint64_t object_id;
		uint64_t object_room_vnum;
		std::string object_type;
		uint64_t object_vnum;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
