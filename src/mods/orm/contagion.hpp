#ifndef __MENTOC_MODS_ORM_CLASS_CONTAGION_HEADER__
#define __MENTOC_MODS_ORM_CLASS_CONTAGION_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"
#include "../skills.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct contagion : public mods::orm::orm_base<contagion,mods::weapon::contagion::primary_choice_t> {
		std::string table_name() {
			return "class_contagion";
		}
		std::string column_prefix() {
			return "contagion_";
		}
		std::string id_column() {
			return "contagion_id";
		}
		std::string primary_key_name() {
			return this->id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		contagion() : id(0) {
			this->init();
			loaded = 0;
		}
		~contagion() = default;

		uint64_t initialize_row(player_ptr_t& player);
		int16_t new_player(player_ptr_t& player);
		int16_t feed(const pqxx::result::reference&);
		void init();
		strmap_t export_class();
		primary_choice_t primary_type() {
			return primary_choice_t::NONE;
		}
		int16_t save() {
			return std::get<0>(this->update<contagion>(this));
		}

		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t contagion_id;
		uint64_t contagion_player_id;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
