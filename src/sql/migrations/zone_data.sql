CREATE TABLE zone_data (
	id SERIAL,
	zone_id INTEGER NOT NULL,
	zone_command VARCHAR(1) NOT NULL,
	zone_if_flag VARCHAR(1) NOT NULL,
	zone_arg1 INTEGER NOT NULL,
	zone_arg2 INTEGER NOT NULL,
	zone_arg3 INTEGER NOT NULL
);

