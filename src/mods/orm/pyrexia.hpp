#ifndef __MENTOC_MODS_ORM_CLASS_PYREXIA_HEADER__
#define __MENTOC_MODS_ORM_CLASS_PYREXIA_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct pyrexia : public mods::orm::orm_base<pyrexia,int> {
		using primary_choice_t = int;
		const std::string PSG1 = "PSG1";
		const std::string L96AW = "L96AW";
		enum drone_scan_levels_t {
				PYREXIA_DRONE_SCAN_NONE = 0,
				PYREXIA_DRONE_SCAN_INITIATE,
				PYREXIA_DRONE_SCAN_FAMILIAR,
				PYREXIA_DRONE_SCAN_MASTER
		};
		enum stealth_levels_t {
				PYREXIA_STEALTH_NONE = 0,
				PYREXIA_STEALTH_INITIATE,
				PYREXIA_STEALTH_FAMILIAR,
				PYREXIA_STEALTH_MASTER
		};
		enum xray_shot_levels_t {
				PYREXIA_XRAY_SHOT_NONE = 0,
				PYREXIA_XRAY_SHOT_INITIATE,
				PYREXIA_XRAY_SHOT_FAMILIAR,
				PYREXIA_XRAY_SHOT_MASTER
		};
		enum feign_death_levels_t {
				PYREXIA_FEIGN_DEATH_NONE = 0,
				PYREXIA_FEIGN_DEATH_INITIATE,
				PYREXIA_FEIGN_DEATH_FAMILIAR,
				PYREXIA_FEIGN_DEATH_MASTER
		};
		enum summon_extraction_levels_t {
				PYREXIA_SUMMON_EXTRACTION_NONE = 0,
				PYREXIA_SUMMON_EXTRACTION_INITIATE,
				PYREXIA_SUMMON_EXTRACTION_FAMILIAR,
				PYREXIA_SUMMON_EXTRACTION_MASTER
		};
		std::string table_name(){ return "class_pyrexia"; }
		std::string column_prefix(){ return "pyrexia_"; }
		std::string id_column(){ return "pyrexia_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->id); }

		pyrexia() : id(0) { this->init(); loaded = 0; }
		~pyrexia() = default;

		uint64_t initialize_row(player_ptr_t &player);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		int16_t save(){
			return std::get<0>(this->update<pyrexia>(this));
		}

		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t pyrexia_id;
		uint64_t pyrexia_player_id;
		drone_scan_levels_t pyrexia_drone_scan_level;
		stealth_levels_t pyrexia_stealth_level;
		xray_shot_levels_t pyrexia_xray_shot_level;
		feign_death_levels_t pyrexia_feign_death_level;
		summon_extraction_levels_t pyrexia_summon_extraction_level;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
