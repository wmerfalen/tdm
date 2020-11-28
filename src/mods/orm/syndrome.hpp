#ifndef __MENTOC_MODS_ORM_CLASS_SYNDROME_HEADER__
#define __MENTOC_MODS_ORM_CLASS_SYNDROME_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

/** STUB */
namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct syndrome : public mods::orm::orm_base<syndrome,mods::weapon::ghost::primary_choice_t> {
		std::string table_name(){ return "class_syndrome"; }
		std::string column_prefix(){ return "syndrome_"; }
		std::string id_column(){ return "syndrome_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->id); }

		syndrome() : id(0) { this->init(); loaded = 0; }
		~syndrome() = default;

		uint64_t initialize_row(player_ptr_t &player);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		primary_choice_t primary_type(){
			return primary_choice_t::NONE;
		}
		int16_t save(){
			return std::get<0>(this->update<syndrome>(this));
		}

		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t syndrome_id;
		uint64_t syndrome_player_id;
		/** TODO */
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
