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
