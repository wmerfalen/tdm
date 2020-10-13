#ifndef __MENTOC_MODS_ORM_CLASS_GHOST_HEADER__
#define __MENTOC_MODS_ORM_CLASS_GHOST_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct ghost : public mods::orm::orm_base<ghost,mods::weapon::ghost::primary_choice_t> {
		using primary_choice_t = mods::weapon::ghost::primary_choice_t;
		const std::string PSG1 = "PSG1";
		const std::string L96AW = "L96AW";
		enum drone_scan_levels_t {
				GHOST_DRONE_SCAN_NONE = 0,
				GHOST_DRONE_SCAN_INITIATE,
				GHOST_DRONE_SCAN_FAMILIAR,
				GHOST_DRONE_SCAN_MASTER
		};
		enum stealth_levels_t {
				GHOST_STEALTH_NONE = 0,
				GHOST_STEALTH_INITIATE,
				GHOST_STEALTH_FAMILIAR,
				GHOST_STEALTH_MASTER
		};
		enum xray_shot_levels_t {
				GHOST_XRAY_SHOT_NONE = 0,
				GHOST_XRAY_SHOT_INITIATE,
				GHOST_XRAY_SHOT_FAMILIAR,
				GHOST_XRAY_SHOT_MASTER
		};
		enum feign_death_levels_t {
				GHOST_FEIGN_DEATH_NONE = 0,
				GHOST_FEIGN_DEATH_INITIATE,
				GHOST_FEIGN_DEATH_FAMILIAR,
				GHOST_FEIGN_DEATH_MASTER
		};
		enum summon_extraction_levels_t {
				GHOST_SUMMON_EXTRACTION_NONE = 0,
				GHOST_SUMMON_EXTRACTION_INITIATE,
				GHOST_SUMMON_EXTRACTION_FAMILIAR,
				GHOST_SUMMON_EXTRACTION_MASTER
		};
		std::string table_name(){ return "class_ghost"; }
		std::string column_prefix(){ return "ghost_"; }
		std::string id_column(){ return "ghost_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->id); }

		ghost() : id(0) { this->init(); loaded = 0; }
		~ghost() = default;

		uint64_t initialize_row(player_ptr_t &player, primary_choice_t primary_choice);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		primary_choice_t primary_type(){
			return primary_choice_t::NONE;
		}
		int16_t save(){
			return std::get<0>(this->update<ghost>(this));
		}

		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t ghost_id;
		uint64_t ghost_player_id;
		drone_scan_levels_t ghost_drone_scan_level;
		stealth_levels_t ghost_stealth_level;
		xray_shot_levels_t ghost_xray_shot_level;
		feign_death_levels_t ghost_feign_death_level;
		summon_extraction_levels_t ghost_summon_extraction_level;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
