CREATE TABLE class_engineer (
	engineer_id SERIAL,
	engineer_player_id INTEGER NOT NULL,
	engineer_primary_type VARCHAR(16) NOT NULL DEFAULT 'FMG-9',
	engineer_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	engineer_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	engineer_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
