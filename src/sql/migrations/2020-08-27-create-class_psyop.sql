CREATE TABLE class_psyop (
	psyop_id SERIAL,
	psyop_player_id INTEGER NOT NULL,
	psyop_primary_type VARCHAR(16) NOT NULL DEFAULT 'SCARH',
	psyop_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	psyop_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	psyop_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
