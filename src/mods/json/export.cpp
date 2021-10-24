#ifndef __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#define __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#include "export.hpp"
#include "../sql.hpp"
#include "../interpreter.hpp"

namespace mods::json {
	using list_t = std::forward_list<std::string>;

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
		json_reader(list_t* buffer) {
			content = buffer;
		}
		~json_reader() = default;
		list_t* content;
	};

	/**
	 * Export all of one single table
	 */
	list_t export_all_table(std::string_view table) {
		list_t buffer;
		auto reader = std::make_shared<json_reader>(&buffer);
		auto status = select_row_to_json<std::shared_ptr<json_reader>,sql_compositor>(table.data(),reader);
		if(!std::get<0>(status)) {
			std::cerr << "warning: export_all_table(" << table.data() << "): '" << std::get<1>(status) << "'\n";
		}
		return buffer;
	}

	list_t export_all_affected_type() {
		return export_all_table("affected_type");
	}
	list_t export_all_armor_index() {
		return export_all_table("armor_index");
	}
	list_t export_all_camera_feed() {
		return export_all_table("camera_feed");
	}
	list_t export_all_class_breacher() {
		return export_all_table("class_breacher");
	}
	list_t export_all_class_contagion() {
		return export_all_table("class_contagion");
	}
	list_t export_all_class_engineer() {
		return export_all_table("class_engineer");
	}
	list_t export_all_class_ghost() {
		return export_all_table("class_ghost");
	}
	list_t export_all_class_marine() {
		return export_all_table("class_marine");
	}
	list_t export_all_class_medic() {
		return export_all_table("class_medic");
	}
	list_t export_all_class_support() {
		return export_all_table("class_support");
	}
	list_t export_all_computer_terminal() {
		return export_all_table("computer_terminal");
	}
	list_t export_all_contract_step_callback() {
		return export_all_table("contract_step_callback");
	}
	list_t export_all_contract_steps() {
		return export_all_table("contract_steps");
	}
	list_t export_all_contracts() {
		return export_all_table("contracts");
	}
	list_t export_all_event_messages() {
		return export_all_table("event_messages");
	}
	list_t export_all_extra_description() {
		return export_all_table("extra_description");
	}
	list_t export_all_friendly_reminders() {
		return export_all_table("friendly_reminders");
	}
	list_t export_all_hq_locations() {
		return export_all_table("hq_locations");
	}
	list_t export_all_integral_object() {
		return export_all_table("integral_object");
	}
	list_t export_all_karma() {
		return export_all_table("karma");
	}
	list_t export_all_mini_game() {
		return export_all_table("mini_game");
	}
	list_t export_all_mini_gunner_sentinel() {
		return export_all_table("mini_gunner_sentinel");
	}
	list_t export_all_mob_equipment() {
		return export_all_table("mob_equipment");
	}
	list_t export_all_mob_equipment_map() {
		return export_all_table("mob_equipment_map");
	}
	list_t export_all_mob_roam() {
		return export_all_table("mob_roam");
	}
	list_t export_all_mob_zone() {
		return export_all_table("mob_zone");
	}
	list_t export_all_mobile() {
		return export_all_table("mobile");
	}
	list_t export_all_npc_dialogue() {
		return export_all_table("npc_dialogue");
	}
	list_t export_all_object() {
		return export_all_table("object");
	}
	list_t export_all_object_armor() {
		return export_all_table("object_armor");
	}
	list_t export_all_object_explosive() {
		return export_all_table("object_explosive");
	}
	list_t export_all_object_flags() {
		return export_all_table("object_flags");
	}
	list_t export_all_object_gadget() {
		return export_all_table("object_gadget");
	}
	list_t export_all_object_rifle() {
		return export_all_table("object_rifle");
	}
	list_t export_all_object_weapon() {
		return export_all_table("object_weapon");
	}

	list_t export_all_player() {
		return export_all_table("player");
	}
	list_t export_all_player_base_ability() {
		return export_all_table("player_base_ability");
	}
	list_t export_all_player_classes() {
		return export_all_table("player_classes");
	}
	list_t export_all_player_contract_state() {
		return export_all_table("player_contract_state");
	}
	list_t export_all_player_flags() {
		return export_all_table("player_flags");
	}
	list_t export_all_player_object() {
		return export_all_table("player_object");
	}
	list_t export_all_player_races() {
		return export_all_table("player_races");
	}
	list_t export_all_player_skill_points() {
		return export_all_table("player_skill_points");
	}
	list_t export_all_player_skill_usage() {
		return export_all_table("player_skill_usage");
	}
	list_t export_all_rifle_attachment() {
		return export_all_table("rifle_attachment");
	}
	list_t export_all_rifle_index() {
		return export_all_table("rifle_index");
	}
	list_t export_all_rifle_instance() {
		return export_all_table("rifle_instance");
	}
	list_t export_all_rifle_placements() {
		return export_all_table("rifle_placements");
	}
	list_t export_all_room() {
		return export_all_table("room");
	}
	list_t export_all_room_direction_data() {
		return export_all_table("room_direction_data");
	}
	list_t export_all_room_extra_descriptions() {
		return export_all_table("room_extra_descriptions");
	}
	list_t export_all_scripted_sequences() {
		return export_all_table("scripted_sequences");
	}
	list_t export_all_scripted_steps() {
		return export_all_table("scripted_steps");
	}
	list_t export_all_shop_objects() {
		return export_all_table("shop_objects");
	}
	list_t export_all_shop_rooms() {
		return export_all_table("shop_rooms");
	}
	list_t export_all_shops() {
		return export_all_table("shops");
	}
	list_t export_all_skill_points() {
		return export_all_table("skill_points");
	}
	list_t export_all_skill_trees() {
		return export_all_table("skill_trees");
	}
	list_t export_all_skill_usage() {
		return export_all_table("skill_usage");
	}
	list_t export_all_tasks() {
		return export_all_table("tasks");
	}
	list_t export_all_tasks_notes() {
		return export_all_table("tasks_notes");
	}
	list_t export_all_terminal_choices() {
		return export_all_table("terminal_choices");
	}
	list_t export_all_weapon_locker() {
		return export_all_table("weapon_locker");
	}
	list_t export_all_world_configuration_start_rooms() {
		return export_all_table("world_configuration_start_rooms");
	}
	list_t export_all_zone() {
		return export_all_table("zone");
	}
	list_t export_all_zone_data() {
		return export_all_table("zone_data");
	}
	void dump(player_ptr_t& p,list_t l) {
		for(auto&& row : l) {
			p->raw_send(row.c_str());
		}
	}

	SUPERCMD(do_json_export) {
		dump(player,export_all_affected_type());
		dump(player,export_all_armor_index());
		dump(player,export_all_camera_feed());
		dump(player,export_all_class_breacher());
		dump(player,export_all_class_contagion());
		dump(player,export_all_class_engineer());
		dump(player,export_all_class_ghost());
		dump(player,export_all_class_marine());
		dump(player,export_all_class_medic());
		dump(player,export_all_class_support());
		dump(player,export_all_computer_terminal());
		dump(player,export_all_contract_step_callback());
		dump(player,export_all_contract_steps());
		dump(player,export_all_contracts());
		dump(player,export_all_event_messages());
		dump(player,export_all_extra_description());
		dump(player,export_all_friendly_reminders());
		dump(player,export_all_hq_locations());
		dump(player,export_all_integral_object());
		dump(player,export_all_karma());
		dump(player,export_all_mini_game());
		dump(player,export_all_mini_gunner_sentinel());
		dump(player,export_all_mob_equipment());
		dump(player,export_all_mob_equipment_map());
		dump(player,export_all_mob_roam());
		dump(player,export_all_mob_zone());
		dump(player,export_all_mobile());
		dump(player,export_all_npc_dialogue());
		dump(player,export_all_object());
		dump(player,export_all_object_armor());
		dump(player,export_all_object_explosive());
		dump(player,export_all_object_flags());
		dump(player,export_all_object_gadget());
		dump(player,export_all_object_rifle());
		dump(player,export_all_object_weapon());
		dump(player,export_all_player());
		dump(player,export_all_player_base_ability());
		dump(player,export_all_player_classes());
		dump(player,export_all_player_contract_state());
		dump(player,export_all_player_flags());
		dump(player,export_all_player_object());
		dump(player,export_all_player_races());
		dump(player,export_all_player_skill_points());
		dump(player,export_all_player_skill_usage());
		dump(player,export_all_rifle_attachment());
		dump(player,export_all_rifle_index());
		dump(player,export_all_rifle_instance());
		dump(player,export_all_rifle_placements());
		dump(player,export_all_room());
		dump(player,export_all_room_direction_data());
		dump(player,export_all_room_extra_descriptions());
		dump(player,export_all_scripted_sequences());
		dump(player,export_all_scripted_steps());
		dump(player,export_all_shop_objects());
		dump(player,export_all_shop_rooms());
		dump(player,export_all_shops());
		dump(player,export_all_skill_points());
		dump(player,export_all_skill_trees());
		dump(player,export_all_skill_usage());
		dump(player,export_all_tasks());
		dump(player,export_all_tasks_notes());
		dump(player,export_all_terminal_choices());
		dump(player,export_all_weapon_locker());
		dump(player,export_all_world_configuration_start_rooms());
		dump(player,export_all_zone());
		dump(player,export_all_zone_data());
	}
	void init() {
		ADD_BUILDER_COMMAND("json_export",do_json_export);
	}


};//end namespace
#endif
