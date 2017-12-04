CREATE TABLE mobile (
	id SERIAL,
	room_number INTEGER NOT NULL,
	zone INTEGER NOT NULL,
	name VARCHAR(256) NOT NULL,
	alias_list VARCHAR(256) NOT NULL,
	short_description VARCHAR(64) NOT NULL,
	long_description VARCHAR(512) NOT NULL,
	detailed_description TEXT,
	action_bitvector VARCHAR(2) NOT NULL,
	affection_bitvector VARCHAR(2) NOT NULL,
	alignment INTEGER NOT NULL,
	type_flag VARCHAR(2),
	mob_level INTEGER,
	thac0 INTEGER,
	armor_class VARCHAR(2),
	max_hitpoints INTEGER,
	barehand_damage INTEGER,
	gold INTEGER,
	experience_points INTEGER,
	load_position VARCHAR(2),
	default_position VARCHAR(2)
	sex INTEGER
);
