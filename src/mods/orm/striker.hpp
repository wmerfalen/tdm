#ifndef __MENTOC_MODS_ORM_CLASS_STRIKER_HEADER__
#define __MENTOC_MODS_ORM_CLASS_STRIKER_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct striker : public mods::orm::orm_base<striker,int> {
		enum teep_levels_t {
				STRIKER_TEEP_NONE = 0,
				STRIKER_TEEP_INITIATE,
				STRIKER_TEEP_FAMILIAR,
				STRIKER_TEEP_MASTER
		};
		enum stealth_levels_t {
				STRIKER_STEALTH_NONE = 0,
				STRIKER_STEALTH_INITIATE,
				STRIKER_STEALTH_FAMILIAR,
				STRIKER_STEALTH_MASTER
		};
		enum xray_shot_levels_t {
				STRIKER_XRAY_SHOT_NONE = 0,
				STRIKER_XRAY_SHOT_INITIATE,
				STRIKER_XRAY_SHOT_FAMILIAR,
				STRIKER_XRAY_SHOT_MASTER
		};
		enum feign_death_levels_t {
				STRIKER_FEIGN_DEATH_NONE = 0,
				STRIKER_FEIGN_DEATH_INITIATE,
				STRIKER_FEIGN_DEATH_FAMILIAR,
				STRIKER_FEIGN_DEATH_MASTER
		};
		enum summon_extraction_levels_t {
				STRIKER_SUMMON_EXTRACTION_NONE = 0,
				STRIKER_SUMMON_EXTRACTION_INITIATE,
				STRIKER_SUMMON_EXTRACTION_FAMILIAR,
				STRIKER_SUMMON_EXTRACTION_MASTER
		};
		std::string table_name(){ return "class_striker"; }
		std::string column_prefix(){ return "striker_"; }
		std::string id_column(){ return "striker_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->id); }

		striker() : id(0) { this->init(); loaded = 0; }
		~striker() = default;

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		int16_t save(){
			return std::get<0>(this->update<striker>(this));
		}

		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t striker_id;
		uint64_t striker_player_id;
		teep_levels_t striker_teep_level;
		stealth_levels_t striker_stealth_level;
		xray_shot_levels_t striker_xray_shot_level;
		feign_death_levels_t striker_feign_death_level;
		summon_extraction_levels_t striker_summon_extraction_level;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
