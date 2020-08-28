CREATE TABLE class_marine (
	marine_id SERIAL,
	marine_player_id INTEGER NOT NULL,
	marine_primary_type VARCHAR(16) NOT NULL DEFAULT 'M16A4',
	marine_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	marine_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	marine_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
