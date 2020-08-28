CREATE TABLE class_sniper (
	sniper_id SERIAL,
	sniper_player_id INTEGER NOT NULL,
	sniper_primary_type VARCHAR(16) NOT NULL DEFAULT 'PSG1',
	sniper_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	sniper_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	sniper_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
