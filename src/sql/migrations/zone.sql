CREATE TABLE zone (
	id SERIAL,
	zone_start INTEGER NOT NULL,
	zone_end INTEGER NOT NULL,
	zone_name VARCHAR(64) NOT NULL,
	lifespan INTEGER NOT NULL,
	reset_mode INTEGER NOT NULL
);
