#ifndef __MENTOC_MODS_ORM_rifle_instance_HEADER__
#define __MENTOC_MODS_ORM_rifle_instance_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct rifle_instance : public mods::orm::orm_base<rifle_instance,bool> {
		std::string table_name() {
			return "rifle_instance";
		}
		std::string column_prefix() {
			return "rifle_";
		}
		std::string id_column() {
			return "rifle_id";
		}
		std::string primary_key_name() {
			return this->id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->rifle_id);
		}

		rifle_instance() : rifle_id(0) {
			this->init();
			loaded = 0;
		}
		~rifle_instance() = default;

		void import_object(obj_ptr_t& obj);
		uint64_t initialize_row(obj_ptr_t& obj);
		int16_t feed(const pqxx::result::reference&);
		void init();
		strmap_t export_class();

		void populate_object(obj_ptr_t& obj);
		int16_t save() {
			return std::get<0>(this->update<rifle_instance>(this));
		}
		std::string& get_dump();

		int16_t				load_by_id(uint64_t);
		uint64_t rifle_id;
		double rifle_accuracy_map_0;
		double rifle_accuracy_map_1;
		double rifle_accuracy_map_2;
		double rifle_accuracy_map_3;
		double rifle_damage_map_0;
		double rifle_damage_map_1;
		double rifle_damage_map_2;
		double rifle_damage_map_3;
		std::string rifle_rarity;
		std::string rifle_file;
		std::string rifle_str_type;
		std::string rifle_type;
		std::string rifle_manufacturer;
		std::string rifle_name;
		int16_t rifle_vnum;
		int16_t rifle_ammo_max;
		std::string rifle_ammo_type;
		float rifle_chance_to_injure;
		int16_t rifle_clip_size;
		int16_t rifle_cooldown_between_shots;
		int16_t rifle_critical_chance;
		int16_t rifle_critical_range;
		float rifle_damage_per_second;
		float rifle_disorient_amount;
		float rifle_headshot_bonus;
		int16_t rifle_max_range;
		float rifle_range_multiplier;
		int16_t rifle_reload_time;
		int16_t rifle_rounds_per_minute;
		int16_t rifle_muzzle_velocity;
		int16_t rifle_effective_firing_range;
		int16_t rifle_damage_dice_count;
		int16_t rifle_damage_dice_sides;
		int16_t rifle_incendiary_damage;
		int16_t rifle_explosive_damage;
		int16_t rifle_shrapnel_damage;
		int16_t rifle_corrosive_damage;
		int16_t rifle_cryogenic_damage;
		int16_t rifle_radioactive_damage;
		int16_t rifle_emp_damage;
		int16_t rifle_shock_damage;
		int16_t rifle_anti_matter_damage;
		int16_t rifle_stat_strength;
		int16_t rifle_stat_intelligence;
		int16_t rifle_stat_wisdom;
		int16_t rifle_stat_dexterity;
		int16_t rifle_stat_constitution;
		int16_t rifle_stat_electronics;
		int16_t rifle_stat_armor;
		int16_t rifle_stat_marksmanship;
		int16_t rifle_stat_sniping;
		int16_t rifle_stat_demolitions;
		int16_t rifle_stat_chemistry;
		int16_t rifle_stat_weapon_handling;
		int16_t rifle_stat_strategy;
		int16_t rifle_stat_medical;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
