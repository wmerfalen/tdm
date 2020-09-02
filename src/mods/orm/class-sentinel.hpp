#ifndef __MENTOC_MODS_ORM_CLASS_SENTINEL_HEADER__
#define __MENTOC_MODS_ORM_CLASS_SENTINEL_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	/*
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	*/
	using sql_save_map_t = std::map<std::string,std::string>;

	/**
	 * I really wanted to utilize the name of "sentinel" as an homage to
	 * the archetype of the guard. A sentinel is the epitome of a 
	 * bodyguard that is loyal to it's protectee.
	 * - Will, 2020-08-22
	 */
	struct sentinel : public mods::orm::orm_base<sentinel,mods::weapon::sentinel::primary_choice_t> {
		using primary_choice_t = mods::weapon::sentinel::primary_choice_t;
		const std::string MP5 = "MP5";
		const std::string SASG12 = "SASG12";
		const std::string CZP10 = "CZP10";
		std::string table_name() const { return "class_sentinel"; }
		std::string column_prefix() const { return "sentinel_"; }
		std::string id_column() const { return "sentinel_id"; }

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		bool loaded;
		sentinel() : id(0) { this->init(); loaded = 0; }
		~sentinel() = default;
		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		sql_save_map_t export_class();

		int16_t save(){
			return std::get<0>(this->update<sentinel>(this));
		}
		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t sentinel_id;
		uint64_t sentinel_player_id;
		std::string sentinel_primary_type;
		uint64_t sentinel_primary_weapon_id;
		std::string sentinel_secondary_type;
		uint64_t sentinel_secondary_weapon_id;
		uint64_t sentinel_heal_level;
		uint64_t sentinel_intimidate_level;
		uint64_t sentinel_human_shield_level;
		uint64_t sentinel_deny_entry_level;
		uint64_t sentinel_gadget_shield_level;
		uint64_t sentinel_oblique_kick_level;
		long created_at;
		long updated_at;
	};

};

#endif
