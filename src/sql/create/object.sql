CREATE TABLE object (
	id SERIAL,
	obj_item_number INTEGER NOT NULL,
	obj_flags INTEGER NOT NULL,
	obj_name varchar(128) NOT NULL,
	obj_description varchar(1024) NOT NULL,
	obj_short_description varchar(64) NOT NULL,
	obj_action_description varchar(32) NOT NULL,
	obj_type INTEGER NOT NULL,
	obj_worn_on INTEGER NOT NULL,
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
