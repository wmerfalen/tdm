CREATE TYPE rarity_t AS ENUM ('COMMON', 'UNCOMMON', 'RARE', 'LEGENDARY', 'GOD_TIER');
CREATE TYPE rifle_type_t AS ENUM ('SHOTGUN', 'ASSAULT_RIFLE', 'SUB_MACHINE_GUN', 'SNIPER', 'HANDGUN', 'MACHINE_PISTOL', 'LIGHT_MACHINE_GUN');

CREATE TABLE object_rifle (
rifle_id SERIAL,
rifle_accuracy_map_0 FLOAT DEFAULT 10.0,
rifle_accuracy_map_1 FLOAT DEFAULT 10.0,
rifle_accuracy_map_2 FLOAT DEFAULT 10.0,
rifle_accuracy_map_3 FLOAT DEFAULT 10.0,
rifle_damage_map_0 FLOAT DEFAULT 10.0,
rifle_damage_map_1 FLOAT DEFAULT 10.0,
rifle_damage_map_2 FLOAT DEFAULT 10.0,
rifle_damage_map_3 FLOAT DEFAULT 10.0,
rifle_ammo_max INTEGER,
rifle_ammo_type VARCHAR(32) NOT NULL,
rifle_chance_to_injure FLOAT,
rifle_clip_size INTEGER,
rifle_cooldown_between_shots FLOAT,
rifle_critical_chance FLOAT,
rifle_critical_range INTEGER,
rifle_damage_per_second FLOAT,
rifle_disorient_amount FLOAT,
rifle_headshot_bonus FLOAT,
rifle_max_range INTEGER,
rifle_range_multiplier FLOAT,
rifle_reload_time FLOAT,
rifle_rounds_per_minute INTEGER,
rifle_muzzle_velocity INTEGER,
rifle_effective_firing_range INTEGER,
rifle_str_type rifle_type_t NOT NULL,
rifle_type INTEGER NOT NULL,
rifle_manufacturer VARCHAR(256) NOT NULL,
rifle_name VARCHAR(256) NOT NULL,
rifle_vnum INTEGER NOT NULL,
rifle_rarity rarity_t NOT NULL DEFAULT 'COMMON',
rifle_file VARCHAR(2048),
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

ALTER TABLE object ALTER COLUMN obj_file TYPE varchar(2048);
ALTER TABLE object ADD COLUMN obj_belongs_to_player INTEGER NOT NULL DEFAULT 0;
