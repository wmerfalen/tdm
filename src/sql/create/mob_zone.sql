CREATE TABLE mob_zone (
	id SERIAL,
	zone_id INTEGER NOT NULL,
	mob_id INTEGER NOT NULL,
	room_id INTEGER NOT NULL,
	max_existing INTEGER 
);

