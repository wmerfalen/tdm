#include "pyrexia.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a pyrexia player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return 
	 */
	uint64_t pyrexia::initialize_row(player_ptr_t &player){
		init();
		pyrexia_player_id = player->db_id();
		auto status = this->create<pyrexia>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = pyrexia_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t pyrexia::export_class() {
		strmap_t values;
		values["pyrexia_player_id"] = std::to_string(pyrexia_player_id);
		values["pyrexia_fire_nade_level"] = std::to_string(pyrexia_fire_nade_level);
		values["pyrexia_drench_object_in_gasoline_level"] = std::to_string(pyrexia_drench_object_in_gasoline_level);
		values["pyrexia_drench_room_in_gasoline_level"] = std::to_string(pyrexia_drench_room_in_gasoline_level);
		values["pyrexia_convert_to_fire_nade_level"] = std::to_string(pyrexia_convert_to_fire_nade_level);
		values["pyrexia_attach_incendiary_grenade_launcher_level"] = std::to_string(pyrexia_attach_incendiary_grenade_launcher_level);
		values["pyrexia_attach_flamethrower_level"] = std::to_string(pyrexia_attach_flamethrower_level);
		values["pyrexia_create_wall_of_fire_level"] = std::to_string(pyrexia_create_wall_of_fire_level);
		values["pyrexia_seal_off_room_with_wall_of_fire_level"] = std::to_string(pyrexia_seal_off_room_with_wall_of_fire_level);
		values["pyrexia_summon_napalm_strike_level"] = std::to_string(pyrexia_summon_napalm_strike_level);
		values["pyrexia_solar_assistance_level"] = std::to_string(pyrexia_solar_assistance_level);
		values["pyrexia_force_weapon_overheat_level"] = std::to_string(pyrexia_force_weapon_overheat_level);
		return std::move(values);
	}
	int16_t pyrexia::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = pyrexia_id = 0;
		pyrexia_player_id = 0;
		return std::get<0>(this->read<pyrexia>(this,"pyrexia_player_id",std::to_string(pyrexia_player_id)));
	}
	int16_t pyrexia::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["pyrexia_id"].as<uint64_t>();
		pyrexia_id = row["pyrexia_id"].as<uint64_t>();
		pyrexia_player_id = row["pyrexia_player_id"].as<uint64_t>();
		pyrexia_fire_nade_level = (skill_familiarity_t)row["pyrexia_fire_nade_level"].as<int>();
		pyrexia_drench_object_in_gasoline_level = (skill_familiarity_t)row["pyrexia_drench_object_in_gasoline_level"].as<int>();
		pyrexia_drench_room_in_gasoline_level = (skill_familiarity_t)row["pyrexia_drench_room_in_gasoline_level"].as<int>();
		pyrexia_convert_to_fire_nade_level = (skill_familiarity_t)row["pyrexia_convert_to_fire_nade_level"].as<int>();
		pyrexia_attach_incendiary_grenade_launcher_level = (skill_familiarity_t)row["pyrexia_attach_incendiary_grenade_launcher_level"].as<int>();
		pyrexia_attach_flamethrower_level = (skill_familiarity_t)row["pyrexia_attach_flamethrower_level"].as<int>();
		pyrexia_create_wall_of_fire_level = (skill_familiarity_t)row["pyrexia_create_wall_of_fire_level"].as<int>();
		pyrexia_seal_off_room_with_wall_of_fire_level = (skill_familiarity_t)row["pyrexia_seal_off_room_with_wall_of_fire_level"].as<int>();
		pyrexia_summon_napalm_strike_level = (skill_familiarity_t)row["pyrexia_summon_napalm_strike_level"].as<int>();
		pyrexia_solar_assistance_level = (skill_familiarity_t)row["pyrexia_solar_assistance_level"].as<int>();
		pyrexia_force_weapon_overheat_level = (skill_familiarity_t)row["pyrexia_force_weapon_overheat_level"].as<int>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	std::pair<bool,std::string> pyrexia::drench_object_in_gasoline(uuid_t npc){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::drench_room_in_gasoline(uuid_t npc){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::convert_to_fire_nade(obj_ptr_t& object){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::attach_incendiary_grenade_launcher(obj_ptr_t& weapon){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::attach_flamethrower(obj_ptr_t& weapon){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::create_wall_of_fire(uint8_t direction){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::seal_off_room_with_wall_of_fire(room_rnum room_id){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::summon_napalm_strike(room_rnum room_id){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::solar_assistance(room_rnum room_id){
		return {0,"stub"};
	}
	std::pair<bool,std::string> pyrexia::force_weapon_overheat(room_rnum room_id){
		return {0,"stub"};
	}
	void pyrexia::init(){
		id = 0;
		pyrexia_id = 0;
		pyrexia_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

