#ifndef __MENTOC_MODS_SCHEMA_HEADER__
#define __MENTOC_MODS_SCHEMA_HEADER__
#include <string>
#include <vector>
#include <map>
#include <boost/hana.hpp>

namespace mods::schema { 
	using schema_list_t = std::map<std::string,std::vector<std::string>>;
/*
 * TODO: Implement boost::hana into this class.
// 1. Give introspection capabilities to 'Person'
struct Person {
  BOOST_HANA_DEFINE_STRUCT(Person,
    (std::string, name),
    (int, age)
  );
};
// 2. Write a generic serializer (bear with std::ostream for the example)
auto serialize = [](std::ostream& os, auto const& object) {
  hana::for_each(hana::members(object), [&](auto member) {
    os << member << std::endl;
  });
};
// 3. Use it
Person john{"John", 30};
serialize(std::cout, john);
// output:
// John
// 30
*/
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
									"player_type"
								}
			},
			{"affected_type",{
												 "id", //integer NOT NULL,
												 "aff_fk_id", //integer NOT NULL,
												 "aff_location", //integer NOT NULL,
												 "aff_modifier" //integer NOT NULL
											 }
			},

			{"extra_description",{
														 "id",		// integer NOT NULL,
														 "obj_fk_id",		// integer NOT NULL,
														 "extra_keyword",		// character varying(64),
														 "extra_description"		// character varying(1024)
													 }
			},

			{"mob_zone",{
										"id",		// integer NOT NULL,
										"zone_id",		// integer NOT NULL,
										"mob_id",		// integer NOT NULL,
										"room_id",		// integer NOT NULL,
										"max_existing"		// integer
									}
			},


			{"mobile",{
									"mob_id",		// integer NOT NULL,
									"mob_virtual_number",		// integer NOT NULL,
									"mob_name",		// character varying(256) NOT NULL,
									"mob_short_description",		// character varying(64) NOT NULL,
									"mob_long_description",		// character varying(512) NOT NULL,
									"mob_description",		// text,
									"mob_action_bitvector",		// character varying(2) NOT NULL,
									"mob_affection_bitvector",		// character varying(2) NOT NULL,
									"mob_ability_strength",		// integer NOT NULL,
									"mob_ability_strength_add",		// integer NOT NULL,
									"mob_ability_intelligence",		// integer NOT NULL,
									"mob_ability_wisdom",		// integer NOT NULL,
									"mob_ability_dexterity",		// integer NOT NULL,
									"mob_ability_constitution",		// integer NOT NULL,
									"mob_ability_charisma",		// integer NOT NULL,
									"mob_alignment",		// integer NOT NULL,
									"mob_attack_type",		// integer NOT NULL,
									"mob_level",		// integer NOT NULL,
									"mob_hitroll",		// integer NOT NULL,
									"mob_armor",		// integer NOT NULL,
									"mob_max_hitpoints",		// integer NOT NULL,
									"mob_max_mana",		// integer NOT NULL,
									"mob_max_move",		// integer NOT NULL,
									"mob_gold",		// integer NOT NULL,
									"mob_exp",		// integer NOT NULL,
									"mob_load_position",		// integer NOT NULL,
									"mob_default_position",		// integer NOT NULL,
									"mob_sex",		// integer NOT NULL,
									"mob_hitpoints",		// integer,
									"mob_mana",		// integer NOT NULL,
									"mob_move",		// integer NOT NULL,
									"mob_damnodice",		// integer NOT NULL,
									"mob_damsizedice",		// integer NOT NULL,
									"mob_damroll",		// integer NOT NULL,
									"mob_weight",		// integer NOT NULL,
									"mob_height",		// integer NOT NULL,
									"mob_class"		// integer NOT NULL
								}
			},
			{"object",{
									"id",		// integer NOT NULL,
									"obj_item_number",		// integer NOT NULL,
									"obj_flags",		// integer NOT NULL,
									"obj_name",		// character varying(128) NOT NULL,
									"obj_description",		// character varying(1024),
									"obj_short_description",		// character varying(64) NOT NULL,
									"obj_action_description",		// character varying(32),
									"obj_type",		// integer,
									"obj_worn_on",		// integer,
									"obj_type_data",		// character varying(16),
									"obj_extra_keyword",		// character varying(64),
									"obj_extra_description"		// character varying(1024)
								}
			},

			{"object_flags",{
												"id",		// integer NOT NULL,
												"obj_fk_id",		// integer NOT NULL,
												"value_0",		// integer NOT NULL,
												"value_1",		// integer NOT NULL,
												"value_2",		// integer NOT NULL,
												"value_3",		// integer NOT NULL,
												"type_flag",		// integer NOT NULL,
												"wear_flags",		// integer NOT NULL,
												"extra_flags",		// integer NOT NULL,
												"weight",		// integer NOT NULL,
												"cost",		// integer NOT NULL,
												"cost_per_day",		// integer NOT NULL,
												"timer",		// integer NOT NULL,
												"bitvector"		// integer NOT NULL
											}
			},

			{"object_weapon",{
												 "id",		// integer NOT NULL,
												 "obj_fk_id",		// integer NOT NULL,
												 "obj_ammo_max",		// integer NOT NULL,
												 "obj_ammo_type",		// integer NOT NULL,
												 "obj_cooldown",		// integer NOT NULL,
												 "obj_can_snipe"		// boolean NOT NULL
											 }
			},

			{"player",{
									"player_id",		// integer NOT NULL,
									"player_virtual_number",		// integer NOT NULL,
									"player_name",		// character varying(256) NOT NULL,
									"player_short_description",		// character varying(64) NOT NULL,
									"player_long_description",		// character varying(512) NOT NULL,
									"player_description",		// text,
									"player_action_bitvector",		// character varying(2) NOT NULL,
									"player_affection_bitvector",		// character varying(2) NOT NULL,
									"player_ability_strength",		// integer NOT NULL,
									"player_ability_strength_add",		// integer NOT NULL,
									"player_ability_intelligence",		// integer NOT NULL,
									"player_ability_wisdom",		// integer NOT NULL,
									"player_ability_dexterity",		// integer NOT NULL,
									"player_ability_constitution",		// integer NOT NULL,
									"player_ability_charisma",		// integer NOT NULL,
									"player_alignment",		// integer NOT NULL,
									"player_attack_type",		// integer NOT NULL,
									"player_level",		// integer NOT NULL,
									"player_hitroll",		// integer NOT NULL,
									"player_armor",		// integer NOT NULL,
									"player_max_hitpoints",		// integer NOT NULL,
									"player_max_mana",		// integer NOT NULL,
									"player_max_move",		// integer NOT NULL,
									"player_gold",		// integer NOT NULL,
									"player_exp",		// integer NOT NULL,
									"player_sex",		// integer NOT NULL,
									"player_hitpoints",		// integer,
									"player_mana",		// integer NOT NULL,
									"player_move",		// integer NOT NULL,
									"player_damnodice",		// integer NOT NULL,
									"player_damsizedice",		// integer NOT NULL,
									"player_damroll",		// integer NOT NULL,
									"player_weight",		// integer NOT NULL,
									"player_height",		// integer NOT NULL,
									"player_class",		// integer NOT NULL,
									"player_title",		// character varying(256) NOT NULL,
									"player_hometown",		// character varying(16) NOT NULL,
									"player_password",		// character varying(512),
									"player_type"		// character varying(3) NOT NULL
								}
			},

			{"room",{
								"id",		// integer NOT NULL,
								"room_number",		// integer NOT NULL,
								"zone",		// integer NOT NULL,
								"sector_type",		// integer NOT NULL,
								"name",		// character varying(256) NOT NULL,
								"description",		// text NOT NULL,
								"ex_keyword",		// character varying(256),
								"ex_description",		// text,
								"light",		// integer,
								"room_flag"		// integer NOT NULL
							}
			},

			{"room_direction_data",{
															 "id",		// integer NOT NULL,
															 "room_number",		// integer NOT NULL,
															 "exit_direction",		// integer NOT NULL,
															 "general_description",		// character varying(256) NOT NULL,
															 "keyword",		// character varying(16),
															 "exit_info",		// integer,
															 "exit_key",		// integer,
															 "to_room"		// integer NOT NULL
														 }
			},

			{"tasks",{
								 "id",		// integer,
								 "t_name"		// character varying(256)
							 }
			},

			{"tasks_notes",{
											 "id",		// integer,
											 "task_id",		// integer,
											 "t_notes"		// character varying(256)
										 }
			},

			{"zone",{
								"id",		// integer NOT NULL,
								"zone_start",		// integer NOT NULL,
								"zone_end",		// integer NOT NULL,
								"zone_name",		// character varying(64) NOT NULL,
								"lifespan",		// integer NOT NULL,
								"reset_mode"		// integer NOT NULL
							}
			},

			{"zone_data",{
										 "id",		// integer NOT NULL,
										 "zone_id",		// integer NOT NULL,
										 "zone_command",		// character varying(1) NOT NULL,
										 "zone_if_flag",		// character varying(1) NOT NULL,
										 "zone_arg1",		// integer NOT NULL,
										 "zone_arg2",		// integer NOT NULL,
										 "zone_arg3"		// integer NOT NULL
									 }
			}
	};

};
#endif
