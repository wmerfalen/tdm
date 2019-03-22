CREATE TABLE player_flags (
	id SERIAL,
	player_id INTEGER NOT NULL,
	chunk_index INTEGER NOT NULL DEFAULT 0,
	flag_value INTEGER NOT NULL,
	UNIQUE (player_id,chunk_index)
);
