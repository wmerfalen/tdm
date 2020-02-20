CREATE TYPE explosive_type_t AS ENUM ('FRAG_GRENADE', 'INCENDIARY_GRENADE', 'REMOTE_EXPLOSIVE', 'REMOTE_CHEMICAL', 'EMP_GRENADE', 'CLAYMORE_MINE', 'SMOKE_GRENADE', 'C4', 'BREACH_CHARGE', 'FLASHBANG_GRENADE', 'SENSOR_GRENADE');
CREATE TYPE explosive_alternate_explosion_type_t AS ENUM ('NONE', 'SCAN');
CREATE TYPE explosive_loudness_type_t AS ENUM('SILENT', 'SUSPICIOUS', 'LOUD', 'DEAFENING', 'WORLD_ENDING'); 

CREATE TABLE object_explosive (
	explosive_id SERIAL,
	explosive_alternate_explosion_type explosive_alternate_explosion_type_t NOT NULL DEFAULT 'NONE',
	explosive_chance_to_injure FLOAT NOT NULL DEFAULT 0.0,
	explosive_critical_chance FLOAT NOT NULL DEFAULT 0.0,
	explosive_critical_range INTEGER NOT NULL DEFAULT 1,
	explosive_blast_radius INTEGER NOT NULL DEFAULT 1,
	explosive_damage_per_second FLOAT NOT NULL DEFAULT 1.0,
	explosive_disorient_amount FLOAT NOT NULL DEFAULT 0.0,
	explosive_loudness_type explosive_loudness_type_t NOT NULL DEFAULT 'LOUD',
	explosive_str_type explosive_type_t NOT NULL,
	explosive_type INTEGER NOT NULL,
	explosive_manufacturer VARCHAR(256) NOT NULL,
	explosive_name VARCHAR(256) NOT NULL,
	explosive_vnum INTEGER NOT NULL,
	explosive_rarity rarity_t NOT NULL DEFAULT 'COMMON',
	explosive_file VARCHAR(2048),
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
