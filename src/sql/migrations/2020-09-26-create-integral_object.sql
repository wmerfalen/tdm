CREATE TABLE integral_object (
	object_id SERIAL,
	object_room_vnum INTEGER NOT NULL,
	object_type VARCHAR(16) NOT NULL,
	object_vnum INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE camera_feed (
	feed_id SERIAL,
	feed_type VARCHAR(16) NOT NULL,
	feed_vnum INTEGER NOT NULL,
	feed_room_vnum INTEGER NOT NULL,
	feed_order INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

DROP TABLE mini_game;

CREATE TABLE mini_game (
	game_id SERIAL,
	game_type VARCHAR(16) NOT NULL,
	game_vnum INTEGER NOT NULL,
	game_name VARCHAR(32),
	game_action TEXT NOT NULL DEFAULT 'unlock',
	game_room_vnum INTEGER NOT NULL,
	game_order INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
