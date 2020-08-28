CREATE TABLE class_contagion (
	contagion_id SERIAL,
	contagion_player_id INTEGER NOT NULL,
	contagion_primary_type VARCHAR(16) NOT NULL DEFAULT 'M3',
	contagion_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	contagion_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	contagion_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
