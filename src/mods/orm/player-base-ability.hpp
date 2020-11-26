#ifndef __MENTOC_MODS_ORM_PLAYER_BASE_ABILITY_HEADER__
#define __MENTOC_MODS_ORM_PLAYER_BASE_ABILITY_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct player_base_ability : public mods::orm::orm_base<player_base_ability,mods::weapon::ghost::primary_choice_t> {
		std::string table_name(){ return "player_base_ability"; }
		std::string column_prefix(){ return "pba_"; }
		std::string id_column(){ return "pba_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->pba_id); }

		player_base_ability() : id(0) { this->init(); loaded = 0; }
		~player_base_ability() = default;

		void feed_player(player_ptr_t& player);
		uint64_t initialize_row(player_ptr_t &player);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		int16_t save(){
			return std::get<0>(this->update<player_base_ability>(this));
		}
		int16_t save_by_player(player_ptr_t& player);
		bool player_row_exists(uint64_t player_id);

		void feed_ability_data(char_ability_data& data);
		void dump();

		int16_t				load_by_player(uint64_t);
		using skill_measurement_t = uint16_t;
		uint64_t id;
		uint64_t pba_id;
		uint64_t pba_player_id;
		skill_measurement_t pba_str;
		skill_measurement_t pba_str_add;
		skill_measurement_t pba_intel;
		skill_measurement_t pba_wis;
		skill_measurement_t pba_dex;
		skill_measurement_t pba_con;
		skill_measurement_t pba_cha;
		skill_measurement_t pba_electronics;
		skill_measurement_t pba_armor;
		skill_measurement_t pba_marksmanship;
		skill_measurement_t pba_sniping;
		skill_measurement_t pba_demolitions;
		skill_measurement_t pba_chemistry;
		skill_measurement_t pba_weapon_handling;
		skill_measurement_t pba_strategy;
		skill_measurement_t pba_medical;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
