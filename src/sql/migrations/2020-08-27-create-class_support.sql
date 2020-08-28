CREATE TABLE class_support (
	support_id SERIAL,
	support_player_id INTEGER NOT NULL,
	support_primary_type VARCHAR(16) NOT NULL DEFAULT 'MK46',
	support_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	support_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	support_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
