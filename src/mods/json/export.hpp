#ifndef __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#define __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#include "../pq.hpp"
#include <string_view>
#include <forward_list>

namespace mods::json {
	using list_t = std::forward_list<std::string>;
	list_t export_all_table(std::string_view table);
	list_t export_all_affected_type();
	list_t export_all_armor_index();
	list_t export_all_camera_feed();
	list_t export_all_class_breacher();
	list_t export_all_class_contagion();
	list_t export_all_class_engineer();
	list_t export_all_class_ghost();
	list_t export_all_class_marine();
	list_t export_all_class_medic();
	list_t export_all_class_support();
	list_t export_all_computer_terminal();
	list_t export_all_contract_step_callback();
	list_t export_all_contract_steps();
	list_t export_all_contracts();
	list_t export_all_event_messages();
	list_t export_all_extra_description();
	list_t export_all_friendly_reminders();
	list_t export_all_hq_locations();
	list_t export_all_integral_object();
	list_t export_all_karma();
	list_t export_all_mini_game();
	list_t export_all_mini_gunner_sentinel();
	list_t export_all_mob_equipment();
	list_t export_all_mob_equipment_map();
	list_t export_all_mob_roam();
	list_t export_all_mob_zone();
	list_t export_all_mobile();
	list_t export_all_npc_dialogue();
	list_t export_all_object();
	list_t export_all_object_armor();
	list_t export_all_object_explosive();
	list_t export_all_object_flags();
	list_t export_all_object_gadget();
	list_t export_all_object_rifle();
	list_t export_all_object_weapon();
	list_t export_all_player();
	list_t export_all_player_base_ability();
	list_t export_all_player_classes();
	list_t export_all_player_contract_state();
	list_t export_all_player_flags();
	list_t export_all_player_object();
	list_t export_all_player_races();
	list_t export_all_player_skill_points();
	list_t export_all_player_skill_usage();
	list_t export_all_rifle_attachment();
	list_t export_all_rifle_index();
	list_t export_all_rifle_instance();
	list_t export_all_rifle_placements();
	list_t export_all_room();
	list_t export_all_room_direction_data();
	list_t export_all_room_extra_descriptions();
	list_t export_all_scripted_sequences();
	list_t export_all_scripted_steps();
	list_t export_all_shop_objects();
	list_t export_all_shop_rooms();
	list_t export_all_shops();
	list_t export_all_skill_points();
	list_t export_all_skill_trees();
	list_t export_all_skill_usage();
	list_t export_all_tasks();
	list_t export_all_tasks_notes();
	list_t export_all_terminal_choices();
	list_t export_all_weapon_locker();
	list_t export_all_world_configuration_start_rooms();
	list_t export_all_zone();
	list_t export_all_zone_data();

};
#endif
