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
CREATE TABLE class_engineer (
	engineer_id SERIAL,
	engineer_player_id INTEGER NOT NULL,
	engineer_primary_type VARCHAR(16) NOT NULL DEFAULT 'FMG9',
	engineer_primary_weapon_id INTEGER NOT NULL DEFAULT 0,
	engineer_secondary_type VARCHAR(16) NOT NULL DEFAULT 'CZP10',
	engineer_secondary_weapon_id INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
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
