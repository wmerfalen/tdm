CREATE TABLE class_ghost (
	ghost_id SERIAL,
	ghost_player_id INTEGER NOT NULL,
	ghost_primary_type VARCHAR(16) NOT NULL DEFAULT 'PSG1',
	ghost_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	ghost_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	ghost_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
