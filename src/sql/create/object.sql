DROP TABLE object;
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

CREATE TABLE affected_type (
	id SERIAL,
	aff_fk_id INTEGER NOT NULL,
	aff_location INTEGER NOT NULL,
	aff_modifier INTEGER NOT NULL
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
