#ifndef __MENTOC_MODS_ORM_CLASS_PYREXIA_HEADER__
#define __MENTOC_MODS_ORM_CLASS_PYREXIA_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"
#include "../skills.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct pyrexia : public mods::orm::orm_base<pyrexia,int> {
		using primary_choice_t = int;
		const std::string PSG1 = "PSG1";
		const std::string L96AW = "L96AW";
		std::pair<bool,std::string> drench_object_in_gasoline(uuid_t npc);
		std::pair<bool,std::string> drench_room_in_gasoline(uuid_t npc);
		std::pair<bool,std::string> convert_to_fire_nade(obj_ptr_t& object);
		std::pair<bool,std::string> attach_incendiary_grenade_launcher(obj_ptr_t& weapon);
		std::pair<bool,std::string> attach_flamethrower(obj_ptr_t& weapon);
		std::pair<bool,std::string> create_wall_of_fire(uint8_t direction);
		std::pair<bool,std::string> seal_off_room_with_wall_of_fire(room_rnum room_id);

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
		skill_familiarity_t pyrexia_fire_nade_level;
		skill_familiarity_t pyrexia_drench_object_in_gasoline_level;
		skill_familiarity_t pyrexia_drench_room_in_gasoline_level;
		skill_familiarity_t pyrexia_convert_to_fire_nade_level;
		skill_familiarity_t pyrexia_attach_incendiary_grenade_launcher_level;
		skill_familiarity_t pyrexia_attach_flamethrower_level;
		skill_familiarity_t pyrexia_create_wall_of_fire_level;
		skill_familiarity_t pyrexia_seal_off_room_with_wall_of_fire_level;
		skill_familiarity_t pyrexia_summon_napalm_strike_level;
		skill_familiarity_t pyrexia_solar_assistance_level;
		skill_familiarity_t pyrexia_force_weapon_overheat_level;
		long created_at;
		long updated_at;
		bool loaded;
	};

};

#endif
