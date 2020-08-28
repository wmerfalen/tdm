CREATE TABLE class_medic (
	medic_id SERIAL,
	medic_player_id INTEGER NOT NULL,
	medic_primary_type VARCHAR(16) NOT NULL DEFAULT 'AUGPARA',
	medic_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	medic_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	medic_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
