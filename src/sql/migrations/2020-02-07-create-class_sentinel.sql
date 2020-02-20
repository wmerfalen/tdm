CREATE TABLE class_sentinel (
	sentinel_id SERIAL,
	sentinel_player_id INTEGER NOT NULL,
	sentinel_primary_type VARCHAR(16) NOT NULL DEFAULT 'MP5',
	sentinel_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	sentinel_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	sentinel_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	sentinel_heal_level INTEGER NOT NULL DEFAULT 0,
	sentinel_intimidate_level INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
