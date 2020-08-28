#ifndef __MENTOC_MODS_ORM_CLASS_SENTINEL_HEADER__
#define __MENTOC_MODS_ORM_CLASS_SENTINEL_HEADER__

#include "../pq.hpp"
#include "../sql.hpp"
#include <iostream>
#include <vector>
//#include "../player.hpp"
#include "../../structs.h"
//#include "base.hpp"
#include <string_view>
#include "../weapon.hpp"
#include <map>
#include <string>

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
	struct sentinel {
		using primary_choice_t = mods::weapon::sentinel::primary_choice_t;
		bool loaded;
		sentinel() : id(0) { this->init(); loaded = 0; }
		~sentinel() = default;
		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		sql_save_map_t export_class();

		static constexpr const char* table_name = "class_sentinel";
		int16_t save(void* s);
#if 0
		template <typename TObject>
			int16_t save(TObject* s){
				try{
					auto up_txn = txn();
					sql_compositor comp(sentinel::table_name,&up_txn);
					auto up_sql = comp
						.update(sentinel::table_name)
						.set(export_class())
						.where("sentinel_id","=",std::to_string(this->id))
						.sql();
					mods::pq::exec(up_txn,up_sql);
					mods::pq::commit(up_txn);
					return 0;
				}catch(std::exception& e){
					std::cerr << __FILE__ << ": " << __LINE__ << ": error updating sentinel by pkid: '" << e.what() << "'\n";
					return -1;
				}
			}
#endif
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
