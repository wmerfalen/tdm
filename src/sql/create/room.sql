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

