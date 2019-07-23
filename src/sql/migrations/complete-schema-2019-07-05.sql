CREATE TABLE affected_type (
	id SERIAL,
	aff_fk_id INTEGER NOT NULL,
	aff_location INTEGER NOT NULL,
	aff_modifier INTEGER NOT NULL
);
CREATE TABLE extra_description (
	id SERIAL,
	obj_fk_id INTEGER NOT NULL,
	extra_keyword VARCHAR(64),
	extra_description VARCHAR(1024)
);
CREATE TABLE mobile (
	mob_id SERIAL,
	mob_virtual_number INTEGER NOT NULL,
	mob_name VARCHAR(256) NOT NULL,
	mob_short_description VARCHAR(64) NOT NULL,
	mob_long_description VARCHAR(512) NOT NULL,
	mob_description TEXT,
	mob_action_bitvector VARCHAR(2) NOT NULL,
	mob_affection_bitvector VARCHAR(2) NOT NULL,
	mob_ability_strength INTEGER NOT NULL,
	mob_ability_strength_add INTEGER NOT NULL,
	mob_ability_intelligence INTEGER NOT NULL,
	mob_ability_wisdom INTEGER NOT NULL,
	mob_ability_dexterity INTEGER NOT NULL,
	mob_ability_constitution INTEGER NOT NULL,
	mob_ability_charisma INTEGER NOT NULL,
	mob_alignment INTEGER NOT NULL,
	mob_attack_type INTEGER NOT NULL,
	mob_level INTEGER NOT NULL,
	mob_hitroll INTEGER NOT NULL,
	/* mob_thac0 INTEGER NOT NULL, */
	/* mob_armor_class VARCHAR(2) NOT NULL, */
	mob_armor INTEGER NOT NULL,
	mob_max_hitpoints INTEGER NOT NULL,
	mob_max_mana INTEGER NOT NULL,
	mob_max_move INTEGER NOT NULL,
	/* mob_barehand_damage INTEGER NOT NULL, */
	mob_gold INTEGER NOT NULL,
	mob_exp INTEGER NOT NULL,
	mob_load_position INTEGER NOT NULL, 
	mob_default_position INTEGER NOT NULL, 
	mob_sex INTEGER NOT NULL,
	mob_hitpoints INTEGER,
	mob_mana INTEGER NOT NULL,
	mob_move INTEGER NOT NULL,
	mob_damnodice INTEGER NOT NULL,
	mob_damsizedice INTEGER NOT NULL,
	mob_damroll INTEGER NOT NULL,
	mob_weight INTEGER NOT NULL,
	mob_height INTEGER NOT NULL,
	mob_class INTEGER NOT NULL
);
CREATE TABLE mob_zone (
	id SERIAL,
	zone_id INTEGER NOT NULL,
	mob_id INTEGER NOT NULL,
	room_id INTEGER NOT NULL,
	max_existing INTEGER 
);

CREATE TABLE object_flags (
	id SERIAL,
	obj_fk_id INTEGER NOT NULL,
	value_0 INTEGER NOT NULL,
	value_1 INTEGER NOT NULL,
	value_2 INTEGER NOT NULL,
	value_3 INTEGER NOT NULL,
	type_flag INTEGER NOT NULL,
	wear_flags INTEGER NOT NULL,
	extra_flags INTEGER NOT NULL,
	weight INTEGER NOT NULL,
	cost INTEGER NOT NULL,
	cost_per_day INTEGER NOT NULL,
	timer INTEGER NOT NULL,
	bitvector INTEGER NOT NULL
);
CREATE TABLE object (
	id SERIAL,
	obj_item_number INTEGER NOT NULL,
	obj_flags INTEGER NOT NULL,
	obj_name varchar(128) NOT NULL,
	obj_description varchar(1024),
	obj_short_description varchar(64) NOT NULL,
	obj_action_description varchar(32),
	obj_type INTEGER ,
	obj_worn_on INTEGER,
	obj_type_data varchar(16),
	obj_extra_keyword VARCHAR(64),
	obj_extra_description VARCHAR(1024)
);
CREATE TABLE object_weapon (
	id SERIAL,
	obj_fk_id INTEGER NOT NULL,
	obj_ammo_max INTEGER NOT NULL,
	obj_ammo_type INTEGER NOT NULL,
	obj_cooldown INTEGER NOT NULL,
	obj_can_snipe BOOLEAN NOT NULL
);
CREATE TABLE player (
	player_id SERIAL,
	player_virtual_number INTEGER NOT NULL,
	player_name VARCHAR(256) NOT NULL,
	player_short_description VARCHAR(64) NOT NULL,
	player_long_description VARCHAR(512) NOT NULL,
	player_description TEXT,
	player_action_bitvector VARCHAR(2) NOT NULL,
	player_affection_bitvector VARCHAR(2) NOT NULL,
	player_ability_strength INTEGER NOT NULL,
	player_ability_strength_add INTEGER NOT NULL,
	player_ability_intelligence INTEGER NOT NULL,
	player_ability_wisdom INTEGER NOT NULL,
	player_ability_dexterity INTEGER NOT NULL,
	player_ability_constitution INTEGER NOT NULL,
	player_ability_charisma INTEGER NOT NULL,
	player_alignment INTEGER NOT NULL,
	player_attack_type INTEGER NOT NULL,
	player_level INTEGER NOT NULL,
	player_hitroll INTEGER NOT NULL,
	player_armor INTEGER NOT NULL,
	player_max_hitpoints INTEGER NOT NULL,
	player_max_mana INTEGER NOT NULL,
	player_max_move INTEGER NOT NULL,
	player_gold INTEGER NOT NULL,
	player_exp INTEGER NOT NULL,
	player_sex INTEGER NOT NULL,
	player_hitpoints INTEGER,
	player_mana INTEGER NOT NULL,
	player_move INTEGER NOT NULL,
	player_damnodice INTEGER NOT NULL,
	player_damsizedice INTEGER NOT NULL,
	player_damroll INTEGER NOT NULL,
	player_weight INTEGER NOT NULL,
	player_height INTEGER NOT NULL,
	player_class INTEGER NOT NULL,
	player_title VARCHAR(256) NOT NULL,
	player_hometown VARCHAR(16) NOT NULL,
	player_password VARCHAR(512),
	player_type VARCHAR(3) NOT NULL 
);
CREATE TABLE room_direction_data (
	id SERIAL,
	room_number INTEGER NOT NULL,
	exit_direction INTEGER NOT NULL,
	general_description VARCHAR(256) NOT NULL,
	keyword VARCHAR(16),
	exit_info INTEGER,
	exit_key INTEGER,
	to_room INTEGER NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE room (
	id SERIAL,
	room_number INTEGER NOT NULL UNIQUE,
	zone INTEGER NOT NULL,
	sector_type INTEGER NOT NULL,
	name VARCHAR(256) NOT NULL,
	description TEXT NOT NULL,
	ex_keyword VARCHAR(256),
	ex_description TEXT,
	light INTEGER,
	room_flag INTEGER NOT NULL
);
CREATE TABLE shops_producing (
	shop_producing_id SERIAL,
	shop_fk_id INTEGER NOT NULL,
   	shop_producing_obj_vnum INTEGER NOT NULL
);
CREATE TABLE shops (
	shop_id SERIAL,
	shop_vnum INTEGER NOT NULL,
	shop_profit_buy INTEGER NOT NULL,
	shop_profit_sell INTEGER NOT NULL,       /* Factor to multiply cost with     */
	shop_type INTEGER NOT NULL,  /* Which items to trade         */
	shop_no_such_item1 INTEGER NOT NULL,     /* Message if keeper hasn't got an item */
	shop_no_such_item2 INTEGER NOT NULL,     /* Message if player hasn't got an item */
	shop_missing_cash1 INTEGER NOT NULL,     /* Message if keeper hasn't got cash    */
	shop_missing_cash2 INTEGER NOT NULL,     /* Message if player hasn't got cash    */
	shop_do_not_buy INTEGER NOT NULL,        /* If keeper dosn't buy such things */
	shop_message_buy INTEGER NOT NULL,       /* Message when player buys item    */
	shop_message_sell INTEGER NOT NULL,      /* Message when player sells item   */
	shop_temper1 INTEGER NOT NULL,       /* How does keeper react if no money    */
	shop_bitvector INTEGER NOT NULL, /* Can attack? Use bank? Cast here? */
	shop_keeper INTEGER NOT NULL,    /* The mobile who owns the shop (rnum)  */
	shop_with_who INTEGER NOT NULL,      /* Who does the shop trade with?    */
	shop_in_room INTEGER NOT NULL,      /* Where is the shop?           */
	shop_open1 INTEGER NOT NULL,
	shop_open2 INTEGER NOT NULL,      /* When does the shop open?     */
	shop_close1 INTEGER NOT NULL,
	shop_close2 INTEGER NOT NULL,    /* When does the shop close?        */
	shop_bankAccount INTEGER NOT NULL,       /* Store all gold over 15000 (disabled) */
	shop_lastsort INTEGER NOT NULL      /* How many items are sorted in inven?  */
);

CREATE TABLE shops_producing (
	shop_producing_id SERIAL,
	shop_fk_id INTEGER NOT NULL,
   	shop_producing_obj_vnum INTEGER NOT NULL
);

CREATE TABLE shops_trade (
	shop_trade_id SERIAL,
	shop_fk_id INTEGER NOT NULL,
	shop_trade_type INTEGER NOT NULL,
	shop_trade_keywords VARCHAR(256) NOT NULL
);
CREATE TABLE shops_trade (
	shop_trade_id SERIAL,
	shop_fk_id INTEGER NOT NULL,
	shop_trade_type INTEGER NOT NULL,
	shop_trade_keywords VARCHAR(256) NOT NULL
);
CREATE TABLE zone_data (
	id SERIAL,
	zone_id INTEGER NOT NULL,
	zone_command VARCHAR(1) NOT NULL,
	zone_if_flag VARCHAR(1) NOT NULL,
	zone_arg1 INTEGER NOT NULL,
	zone_arg2 INTEGER NOT NULL,
	zone_arg3 INTEGER NOT NULL
);

CREATE TABLE zone (
	id SERIAL,
	zone_start INTEGER NOT NULL,
	zone_end INTEGER NOT NULL,
	zone_name VARCHAR(64) NOT NULL,
	lifespan INTEGER NOT NULL,
	reset_mode INTEGER NOT NULL
);
CREATE TABLE player (
		id SERIAL PRIMARY KEY,
		player_password VARCHAR(1024) NOT NULL,
		player_affection_plr_bitvector INTEGER NOT NULL DEFAULT 0,
		player_affection_bitvector INTEGER NOT NULL DEFAULT 0,
		player_name VARCHAR(24) NOT NULL DEFAULT 'recruit',
		player_short_description VARCHAR(512) NOT NULL default 'A lonely recruit',
		player_long_description TEXT,
		player_action_bitvector INTEGER NOT NULL DEFAULT 0,
		player_ability_strength INTEGER NOT NULL DEFAULT 1,
		player_ability_strength_add INTEGER NOT NULL DEFAULT 1,
		player_ability_intelligence INTEGER NOT NULL DEFAULT 1,
		player_ability_wisdom INTEGER NOT NULL DEFAULT 1,
		player_ability_dexterity INTEGER NOT NULL DEFAULT 1,
		player_ability_constitution INTEGER NOT NULL DEFAULT 1,
		player_ability_charisma INTEGER NOT NULL DEFAULT 1,
		player_ability_alignment INTEGER NOT NULL DEFAULT 0,
		player_attack_type INTEGER NOT NULL DEFAULT 0,
		player_max_hitpoints INTEGER NOT NULL DEFAULT 100,
		player_max_mana INTEGER NOT NULL DEFAULT 100,
		player_max_move INTEGER NOT NULL DEFAULT 100,
		player_gold INTEGER NOT NULL DEFAULT 0,
		player_exp INTEGER NOT NULL DEFAULT 0,
		player_sex VARCHAR(1) NOT NULL DEFAULT 'M',
		player_hitpoints INTEGER NOT NULL DEFAULT 100,
		player_mana INTEGER NOT NULL DEFAULT 100,
		player_move INTEGER NOT NULL DEFAULT 100,
		player_damroll INTEGER NOT NULL DEFAULT 6,
		player_weight INTEGER NOT NULL DEFAULT 10,
		player_height INTEGER NOT NULL DEFAULT 60,
		player_class VARCHAR(16) NOT NULL DEFAULT 'x',
		player_title VARCHAR(24) DEFAULT 'recruit',
		player_hometown VARCHAR(24) DEFAULT 'SLC',
		player_damnodice INTEGER NOT NULL DEFAULT 3,
		player_damsizedice INTEGER NOT NULL DEFAULT 6,
		player_type VARCHAR(6) NOT NULL DEFAULT 'x',
		player_alignment INTEGER NOT NULL DEFAULT 0,
		player_level INTEGER NOT NULL DEFAULT 1,
		player_hitroll INTEGER NOT NULL DEFAULT 3,
		player_armor INTEGER NOT NULL DEFAULT 3,
		player_birth TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		player_time_played INTEGER NOT NULL DEFAULT 0,
		player_logon TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
	);
CREATE TABLE room_extra_desc_data (
	id SERIAL,
	room_number INTEGER NOT NULL,
	ex_keyword VARCHAR(64),
	ex_description VARCHAR(1024),
	PRIMARY KEY(id)
);

DROP TABLE player;
CREATE TABLE player (
		id SERIAL PRIMARY KEY,
		player_password VARCHAR(1024) NOT NULL,
		player_affection_plr_bitvector INTEGER NOT NULL DEFAULT 0,
		player_affection_bitvector INTEGER NOT NULL DEFAULT 0,
		player_name VARCHAR(24) NOT NULL DEFAULT 'recruit',
		player_short_description VARCHAR(512) NOT NULL default 'A lonely recruit',
		player_long_description TEXT,
		player_action_bitvector INTEGER NOT NULL DEFAULT 0,
		player_ability_strength INTEGER NOT NULL DEFAULT 1,
		player_ability_strength_add INTEGER NOT NULL DEFAULT 1,
		player_ability_intelligence INTEGER NOT NULL DEFAULT 1,
		player_ability_wisdom INTEGER NOT NULL DEFAULT 1,
		player_ability_dexterity INTEGER NOT NULL DEFAULT 1,
		player_ability_constitution INTEGER NOT NULL DEFAULT 1,
		player_ability_charisma INTEGER NOT NULL DEFAULT 1,
		player_ability_alignment INTEGER NOT NULL DEFAULT 0,
		player_attack_type INTEGER NOT NULL DEFAULT 0,
		player_max_hitpoints INTEGER NOT NULL DEFAULT 100,
		player_max_mana INTEGER NOT NULL DEFAULT 100,
		player_max_move INTEGER NOT NULL DEFAULT 100,
		player_gold INTEGER NOT NULL DEFAULT 0,
		player_exp INTEGER NOT NULL DEFAULT 0,
		player_sex VARCHAR(1) NOT NULL DEFAULT 'M',
		player_hitpoints INTEGER NOT NULL DEFAULT 100,
		player_mana INTEGER NOT NULL DEFAULT 100,
		player_move INTEGER NOT NULL DEFAULT 100,
		player_damroll INTEGER NOT NULL DEFAULT 6,
		player_weight INTEGER NOT NULL DEFAULT 10,
		player_height INTEGER NOT NULL DEFAULT 60,
		player_class VARCHAR(16) NOT NULL DEFAULT 'x',
		player_title VARCHAR(24) DEFAULT 'recruit',
		player_hometown VARCHAR(24) DEFAULT 'SLC',
		player_damnodice INTEGER NOT NULL DEFAULT 3,
		player_damsizedice INTEGER NOT NULL DEFAULT 6,
		player_type VARCHAR(6) NOT NULL DEFAULT 'x',
		player_alignment INTEGER NOT NULL DEFAULT 0,
		player_level INTEGER NOT NULL DEFAULT 1,
		player_hitroll INTEGER NOT NULL DEFAULT 3,
		player_armor INTEGER NOT NULL DEFAULT 3,
		player_birth TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		player_time_played INTEGER NOT NULL DEFAULT 0,
		player_logon TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		player_preferences INTEGER NOT NULL DEFAULT 0
	);
ALTER TABLE player ALTER COLUMN player_affection_bitvector DROP NOT NULL;
ALTER TABLE player ALTER COLUMN player_affection_plr_bitvector DROP NOT NULL;
ALTER TABLE player ALTER COLUMN player_preferences DROP NOT NULL;

ALTER TABLE player ALTER COLUMN player_affection_bitvector DROP DEFAULT;
ALTER TABLE player ALTER COLUMN player_affection_plr_bitvector DROP DEFAULT;
ALTER TABLE player ALTER COLUMN player_preferences DROP DEFAULT;

ALTER TABLE player ALTER COLUMN player_affection_bitvector TYPE varchar(2048); 
ALTER TABLE player ALTER COLUMN player_affection_plr_bitvector TYPE varchar(2048); 
ALTER TABLE player ALTER COLUMN player_preferences TYPE varchar(2048);
ALTER TABLE object_flags ADD COLUMN weapon_flags INTEGER NOT NULL DEFAULT 0;
ALTER TABLE object_flags ADD COLUMN ammo_max INTEGER NOT NULL DEFAULT 250;
ALTER TABLE object_flags ADD COLUMN clip_size INTEGER NOT NULL DEFAULT 21;

/** VERSION: 1 */
CREATE TABLE world_configuration_start_rooms (
	id SERIAL,
	mortal_start_room INTEGER NOT NULL,
	immortal_start_room INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT now(),
	is_active BOOLEAN 
);
CREATE TABLE player_classes (
	id SERIAL PRIMARY KEY,
	pclass_name VARCHAR(24) NOT NULL,
	pclass_description TEXT
);

CREATE TABLE player_races (
	id SERIAL PRIMARY KEY,
	prace_name VARCHAR(24) NOT NULL,
	prace_description TEXT
);


CREATE TYPE base_attribute AS ENUM (
	'STR','INT','DEX','WIS','CHA','CON','LCK'
);

CREATE TABLE player_race_perks (
	id SERIAL PRIMARY KEY,
	prperk_id INTEGER NOT NULL REFERENCES player_races(id),
	prperk_name VARCHAR(64) NOT NULL,
	prperk_description TEXT,
	prperk_base_attribute_mutate base_attribute NOT NULL,
	prperk_base_attribute_amount_add INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE skill_trees (
	id SERIAL PRIMARY KEY,
	sktree_class INTEGER NOT NULL REFERENCES player_classes(id),
	sktree_skill_name VARCHAR(32) NOT NULL,
	sktree_skill_description TEXT,
	sktree_skill_level_unlock INTEGER NOT NULL,
	sktree_skill_parent_id INTEGER REFERENCES skill_trees(id),
	sktree_skill_order INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE specialty_skills (
	id SERIAL PRIMARY KEY,
	spskill_name VARCHAR(64) NOT NULL,
	spskill_description TEXT
);

CREATE TABLE player_generated_data (
	id SERIAL PRIMARY KEY,
	pgdata_player_id INTEGER NOT NULL REFERENCES player(id),
	pgdata_class INTEGER NOT NULL REFERENCES player_classes(id),
	pgdata_race INTEGER NOT NULL REFERENCES player_races(id),
	pgdata_race_perks INTEGER NOT NULL REFERENCES player_race_perks(id),
	pgdata_specialty_skill INTEGER REFERENCES specialty_skills(id)
);


CREATE TABLE player_skill_trees_unlocked (
	id SERIAL PRIMARY KEY,
	pstun_player_id INTEGER NOT NULL REFERENCES player(id),
	pstun_skill_tree_id INTEGER NOT NULL REFERENCES skill_trees(id)
);
