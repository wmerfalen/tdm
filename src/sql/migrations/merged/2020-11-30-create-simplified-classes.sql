ALTER TABLE class_sniper DROP COLUMN sniper_primary_type;
ALTER TABLE class_sniper DROP COLUMN sniper_primary_weapon_id;
ALTER TABLE class_sniper DROP COLUMN sniper_secondary_type;
ALTER TABLE class_sniper DROP COLUMN sniper_secondary_weapon_id;
DROP TABLE class_marine;
CREATE TABLE class_marine (
	marine_id SERIAL,
	marine_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
DROP TABLE class_breacher;
CREATE TABLE class_breacher (
	breacher_id SERIAL,
	breacher_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

DROP TABLE class_engineer;
CREATE TABLE class_engineer (
	engineer_id SERIAL,
	engineer_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
DROP TABLE class_medic;
CREATE TABLE class_medic (
	medic_id SERIAL,
	medic_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
DROP TABLE class_support;
CREATE TABLE class_support (
	support_id SERIAL,
	support_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
DROP TABLE class_ghost;
CREATE TABLE class_ghost (
	ghost_id SERIAL,
	ghost_player_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
