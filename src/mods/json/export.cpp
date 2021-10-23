#ifndef __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#define __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#include "npc-export.hpp"
#include "../sql.hpp"
#include "../pq.hpp"
#include "../interpreter.hpp"
#include <forward_list>

namespace mods::json::npc_export {

	/**
	 * Json reader
	 */
	struct json_reader {
		void feed_multi(pqxx::result& r) {
			for(const auto& row : r) {
				content->emplace_front(row[0].c_str());
			}
		}
		json_reader() = delete;
		json_reader(std::forward_list<std::string>* buffer) {
			content = buffer;
		}
		~json_reader() = default;
		std::forward_list<std::string>* content;
	};

	/**
	 * Export all of one single table
	 */
	std::forward_list<std::string> export_all_table(std::string_view table) {
		std::forward_list<std::string> buffer;
		auto reader = std::make_shared<json_reader>(&buffer);
		auto status = select_row_to_json<std::shared_ptr<json_reader>,sql_compositor>(table.data(),reader);
		if(!std::get<0>(status)) {
			std::cerr << "warning: export_all_table(" << table.data() << "): '" << std::get<1>(status) << "'\n";
		}
		return buffer;
	}

	std::forward_list<std::string> export_all_affected_type() {
		return export_all_table("affected_type");
	}
	std::forward_list<std::string> export_all_armor_index() {
		return export_all_table("armor_index");
	}
	std::forward_list<std::string> export_all_camera_feed() {
		return export_all_table("camera_feed");
	}
	std::forward_list<std::string> export_all_class_breacher() {
		return export_all_table("class_breacher");
	}
	std::forward_list<std::string> export_all_class_contagion() {
		return export_all_table("class_contagion");
	}
	std::forward_list<std::string> export_all_class_engineer() {
		return export_all_table("class_engineer");
	}
	std::forward_list<std::string> export_all_class_ghost() {
		return export_all_table("class_ghost");
	}
	std::forward_list<std::string> export_all_class_marine() {
		return export_all_table("class_marine");
	}
	std::forward_list<std::string> export_all_class_medic() {
		return export_all_table("class_medic");
	}
	std::forward_list<std::string> export_all_class_support() {
		return export_all_table("class_support");
	}
	std::forward_list<std::string> export_all_computer_terminal() {
		return export_all_table("computer_terminal");
	}
	std::forward_list<std::string> export_all_contract_step_callback() {
		return export_all_table("contract_step_callback");
	}
	std::forward_list<std::string> export_all_contract_steps() {
		return export_all_table("contract_steps");
	}
	std::forward_list<std::string> export_all_contracts() {
		return export_all_table("contracts");
	}
	std::forward_list<std::string> export_all_event_messages() {
		return export_all_table("event_messages");
	}
	std::forward_list<std::string> export_all_extra_description() {
		return export_all_table("extra_description");
	}
	std::forward_list<std::string> export_all_friendly_reminders() {
		return export_all_table("friendly_reminders");
	}
	std::forward_list<std::string> export_all_hq_locations() {
		return export_all_table("hq_locations");
	}
	std::forward_list<std::string> export_all_integral_object() {
		return export_all_table("integral_object");
	}
	std::forward_list<std::string> export_all_karma() {
		return export_all_table("karma");
	}
	std::forward_list<std::string> export_all_mini_game() {
		return export_all_table("mini_game");
	}
	std::forward_list<std::string> export_all_mini_gunner_sentinel() {
		return export_all_table("mini_gunner_sentinel");
	}
	std::forward_list<std::string> export_all_mob_equipment() {
		return export_all_table("mob_equipment");
	}
	std::forward_list<std::string> export_all_mob_equipment_map() {
		return export_all_table("mob_equipment_map");
	}
	std::forward_list<std::string> export_all_mob_roam() {
		return export_all_table("mob_roam");
	}
	std::forward_list<std::string> export_all_mob_zone() {
		return export_all_table("mob_zone");
	}
	std::forward_list<std::string> export_all_mobile() {
		return export_all_table("mobile");
	}
	std::forward_list<std::string> export_all_npc_dialogue() {
		return export_all_table("npc_dialogue");
	}
	std::forward_list<std::string> export_all_object() {
		return export_all_table("object");
	}
	std::forward_list<std::string> export_all_object_armor() {
		return export_all_table("object_armor");
	}
	std::forward_list<std::string> export_all_object_explosive() {
		return export_all_table("object_explosive");
	}
	std::forward_list<std::string> export_all_object_flags() {
		return export_all_table("object_flags");
	}
	std::forward_list<std::string> export_all_object_gadget() {
		return export_all_table("object_gadget");
	}
	std::forward_list<std::string> export_all_object_rifle() {
		return export_all_table("object_rifle");
	}
	std::forward_list<std::string> export_all_object_weapon() {
		return export_all_table("object_weapon");
	}

	std::forward_list<std::string> export_all_player() {
		return export_all_table("player");
	}
	std::forward_list<std::string> export_all_player_base_ability() {
		return export_all_table("player_base_ability");
	}
	std::forward_list<std::string> export_all_player_classes() {
		return export_all_table("player_classes");
	}
	std::forward_list<std::string> export_all_player_contract_state() {
		return export_all_table("player_contract_state");
	}
	std::forward_list<std::string> export_all_player_flags() {
		return export_all_table("player_flags");
	}
	std::forward_list<std::string> export_all_player_object() {
		return export_all_table("player_object");
	}
	std::forward_list<std::string> export_all_player_races() {
		return export_all_table("player_races");
	}
	std::forward_list<std::string> export_all_player_skill_points() {
		return export_all_table("player_skill_points");
	}
	std::forward_list<std::string> export_all_player_skill_usage() {
		return export_all_table("player_skill_usage");
	}
	std::forward_list<std::string> export_all_rifle_attachment() {
		return export_all_table("rifle_attachment");
	}
	std::forward_list<std::string> export_all_rifle_index() {
		return export_all_table("rifle_index");
	}
	std::forward_list<std::string> export_all_rifle_instance() {
		return export_all_table("rifle_instance");
	}
	std::forward_list<std::string> export_all_rifle_placements() {
		return export_all_table("rifle_placements");
	}
	std::forward_list<std::string> export_all_room() {
		return export_all_table("room");
	}
	std::forward_list<std::string> export_all_room_direction_data() {
		return export_all_table("room_direction_data");
	}
	std::forward_list<std::string> export_all_room_extra_descriptions() {
		return export_all_table("room_extra_descriptions");
	}
	std::forward_list<std::string> export_all_scripted_sequences() {
		return export_all_table("scripted_sequences");
	}
	std::forward_list<std::string> export_all_scripted_steps() {
		return export_all_table("scripted_steps");
	}
	std::forward_list<std::string> export_all_shop_objects() {
		return export_all_table("shop_objects");
	}
	std::forward_list<std::string> export_all_shop_rooms() {
		return export_all_table("shop_rooms");
	}
	std::forward_list<std::string> export_all_shops() {
		return export_all_table("shops");
	}
	std::forward_list<std::string> export_all_skill_points() {
		return export_all_table("skill_points");
	}
	std::forward_list<std::string> export_all_skill_trees() {
		return export_all_table("skill_trees");
	}
	std::forward_list<std::string> export_all_skill_usage() {
		return export_all_table("skill_usage");
	}
	std::forward_list<std::string> export_all_tasks() {
		return export_all_table("tasks");
	}
	std::forward_list<std::string> export_all_tasks_notes() {
		return export_all_table("tasks_notes");
	}
	std::forward_list<std::string> export_all_terminal_choices() {
		return export_all_table("terminal_choices");
	}
	std::forward_list<std::string> export_all_weapon_locker() {
		return export_all_table("weapon_locker");
	}
	std::forward_list<std::string> export_all_world_configuration_start_rooms() {
		return export_all_table("world_configuration_start_rooms");
	}
	std::forward_list<std::string> export_all_zone() {
		return export_all_table("zone");
	}
	std::forward_list<std::string> export_all_zone_data() {
		return export_all_table("zone_data");
	}
	SUPERCMD(do_json_export) {
		export_all_affected_type() ;
		export_all_armor_index() ;
		export_all_camera_feed() ;
		export_all_class_breacher() ;
		export_all_class_contagion() ;
		export_all_class_engineer() ;
		export_all_class_ghost() ;
		export_all_class_marine() ;
		export_all_class_medic() ;
		export_all_class_support() ;
		export_all_computer_terminal() ;
		export_all_contract_step_callback() ;
		export_all_contract_steps() ;
		export_all_contracts() ;
		export_all_event_messages() ;
		export_all_extra_description() ;
		export_all_friendly_reminders() ;
		export_all_hq_locations() ;
		export_all_integral_object() ;
		export_all_karma() ;
		export_all_mini_game() ;
		export_all_mini_gunner_sentinel() ;
		export_all_mob_equipment() ;
		export_all_mob_equipment_map() ;
		export_all_mob_roam() ;
		export_all_mob_zone() ;
		export_all_mobile() ;
		export_all_npc_dialogue() ;
		export_all_object() ;
		export_all_object_armor() ;
		export_all_object_explosive() ;
		export_all_object_flags() ;
		export_all_object_gadget() ;
		export_all_object_rifle() ;
		export_all_object_weapon() ;
		export_all_player() ;
		export_all_player_base_ability() ;
		export_all_player_classes() ;
		export_all_player_contract_state() ;
		export_all_player_flags() ;
		export_all_player_object() ;
		export_all_player_races() ;
		export_all_player_skill_points() ;
		export_all_player_skill_usage() ;
		export_all_rifle_attachment() ;
		export_all_rifle_index() ;
		export_all_rifle_instance() ;
		export_all_rifle_placements() ;
		export_all_room() ;
		export_all_room_direction_data() ;
		export_all_room_extra_descriptions() ;
		export_all_scripted_sequences() ;
		export_all_scripted_steps() ;
		export_all_shop_objects() ;
		export_all_shop_rooms() ;
		export_all_shops() ;
		export_all_skill_points() ;
		export_all_skill_trees() ;
		export_all_skill_usage() ;
		export_all_tasks() ;
		export_all_tasks_notes() ;
		export_all_terminal_choices() ;
		export_all_weapon_locker() ;
		export_all_world_configuration_start_rooms() ;
		export_all_zone() ;
		export_all_zone_data() ;
	}
	void init() {
		ADD_BUILDER_COMMAND("json_export",do_json_export);
	}


};//end namespace
#endif
