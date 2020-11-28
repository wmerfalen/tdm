#ifndef __MENTOC_MODS_ORM_CLASS_FORGE_HEADER__
#define __MENTOC_MODS_ORM_CLASS_FORGE_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"
#include "../skills.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct forge : public mods::orm::orm_base<forge,mods::weapon::ghost::primary_choice_t> {
		std::string table_name(){ return "class_forge"; }
		std::string column_prefix(){ return "forge_"; }
		std::string id_column(){ return "forge_id"; }
		std::string primary_key_name() { return this->id_column(); }
		std::string primary_key_value() { return std::to_string(this->id); }

		forge() : id(0) { this->init(); loaded = 0; }
		~forge() = default;

		uint64_t initialize_row(player_ptr_t &player);
		int16_t feed(const pqxx::result::reference &);
		void init();	
		strmap_t export_class();
		primary_choice_t primary_type(){
			return primary_choice_t::NONE;
		}
		int16_t save(){
			return std::get<0>(this->update<forge>(this));
		}

		int16_t				load_by_player(uint64_t);
		uint64_t id;
		uint64_t forge_id;
		uint64_t forge_player_id;
		skill_familiarity_t forge_craft_shotgun_underbarrel_level;
		skill_familiarity_t forge_craft_incendiary_underbarrel_level;
		skill_familiarity_t forge_craft_flash_underbarrel_level;
		skill_familiarity_t forge_craft_silencer_level;
		skill_familiarity_t forge_craft_ammunition_level;
		skill_familiarity_t forge_craft_armor_plating_level;
		skill_familiarity_t forge_craft_high_velocity_ammo_level;
		skill_familiarity_t forge_dismantle_weapon_level;
		skill_familiarity_t forge_trade_attribute_level;
		skill_familiarity_t forge_repair_armor_level;
		skill_familiarity_t forge_repair_weapon_level;
		skill_familiarity_t forge_reinforce_weapon_level;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
