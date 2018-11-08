#ifndef __MENTOC_MODS_SCHEMA_HEADER__
#define __MENTOC_MODS_SCHEMA_HEADER__
#include <string>
#include <vector>
#include <map>
#include <boost/hana.hpp>

namespace mods::schema { 
	using schema_list_t = std::map<std::string,std::vector<std::string>>;
	constexpr static const char* PLACE_HOLDER_VALUE = "\x01";
/*!TODO: Implement boost::hana into this class.*/
	static schema_list_t db = {
			{"player",{
									"player_id",
									"player_virtual_number",
									"player_name",
									"player_short_description",
									"player_long_description",
									"player_description",
									"player_action_bitvector",
									"player_affection_bitvector",
									"player_ability_strength",
									"player_ability_strength_add",
									"player_ability_intelligence",
									"player_ability_wisdom",
									"player_ability_dexterity",
									"player_ability_constitution",
									"player_ability_charisma",
									"player_alignment",
									"player_attack_type",
									"player_level",
									"player_hitroll",
									"player_armor",
									"player_max_hitpoints",
									"player_max_mana",
									"player_max_move",
									"player_gold",
									"player_exp",
									"player_sex",
									"player_hitpoints",
									"player_mana",
									"player_move",
									"player_damnodice",
									"player_damsizedice",
									"player_damroll",
									"player_weight",
									"player_height",
									"player_class",
									"player_title",
									"player_hometown",
									"player_password",
									"player_type",
									"meta|player_name"
								}
			},
			{"affected_type",{
												 "id",
												 "object_number",
												 "aff_location",
												 "aff_modifier",
												 "meta|object_number"
											 }
			},

			{"extra_description",{
														 "id",
														 "object_number",
														 "extra_keyword",		
														 "extra_description",
														 "meta|object_number"
													 }
			},

			{"mob_zone",{
										"id",		
										"zone_id",
										"mob_id",
										"room_id",
										"max_existing"
									}
			},


			{"mobile",{
									"mob_id",		
									"mob_virtual_number",		
									"mob_name",		
									"mob_short_description",		
									"mob_long_description",		
									"mob_description",		
									"mob_action_bitvector",		
									"mob_affection_bitvector",		
									"mob_ability_strength",		
									"mob_ability_strength_add",		
									"mob_ability_intelligence",		
									"mob_ability_wisdom",		
									"mob_ability_dexterity",		
									"mob_ability_constitution",		
									"mob_ability_charisma",		
									"mob_alignment",		
									"mob_attack_type",		
									"mob_level",		
									"mob_hitroll",		
									"mob_armor",		
									"mob_max_hitpoints",		
									"mob_max_mana",		
									"mob_max_move",		
									"mob_gold",		
									"mob_exp",		
									"mob_load_position",		
									"mob_default_position",		
									"mob_sex",		
									"mob_hitpoints",		
									"mob_mana",		
									"mob_move",		
									"mob_damnodice",		
									"mob_damsizedice",		
									"mob_damroll",		
									"mob_weight",		
									"mob_height",		
									"mob_class",
									"meta|mob_name",
									"meta|mob_virtual_number",
									"meta|mob_class"
								}
			},
			{"object",{
									"id",		
									"object_number",		
									"object_flags",		
									"object_name",		
									"object_description",		
									"object_short_description",		
									"object_action_description",		
									"object_type",		
									"object_worn_on",		
									"object_type_data",		
									"object_extra_keyword",		
									"object_extra_description",
									"meta|object_number",
									"meta|object_name",
									"meta|object_type"
								}
			},

			{"object_flags",{
												"id",		
												"object_number",		
												"value_0",		
												"value_1",		
												"value_2",		
												"value_3",		
												"type_flag",		
												"wear_flags",		
												"extra_flags",		
												"weight",		
												"cost",		
												"cost_per_day",		
												"timer",		
												"bitvector",
												"meta|object_number"
											}
			},

			{"object_weapon",{
												 "id",		
												 "object_number",		
												 "object_ammo_max",		
												 "object_ammo_type",		
												 "object_cooldown",		
												 "object_can_snipe",
												 "meta|object_number"
											 }
			},

			{"room",{
								"id",		
								"room_number",		
								"zone",		
								"sector_type",		
								"name",		
								"description",		
								"ex_keyword",		
								"ex_description",		
								"light",		
								"room_flag",
								"meta|room_number",
								"meta|zone",
								"meta|name"
							}
			},

			{"room_direction_data",{
															 "id",		
															 "room_number",		
															 "exit_direction",		
															 "general_description",		
															 "keyword",		
															 "exit_info",		
															 "exit_key",		
															 "to_room",
															 "meta|room_number"
														 }
			},

			{"tasks",{
								 "id",		
								 "t_name"		
							 }
			},

			{"tasks_notes",{
											 "id",		
											 "task_id",		
											 "t_notes"		
										 }
			},

			{"zone",{
								"id",
								"zone_start",
								"zone_end",
								"zone_name",
								"lifespan",
								"reset_mode",
								"meta|zone_name"
							}
			},

			{"zone_data",{
										 "id",
										 "zone_id",
										 "zone_command",
										 "zone_if_flag",
										 "zone_arg1",
										 "zone_arg2",
										 "zone_arg3",
										 "meta|zone_id"
									 }
			}
	};

};
#endif
