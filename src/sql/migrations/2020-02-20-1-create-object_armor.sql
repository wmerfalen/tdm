CREATE TYPE speed_profile_type_t AS ENUM ('UNHINDERED', 'FAST', 'HINDERED', 'SLOW', 'SLUGGISH');
CREATE TYPE durability_profile_type_t AS ENUM ('FLIMSY','DECENT','DURABLE','HARDENED','INDUSTRIAL_STRENGTH','GODLIKE','INDESTRUCTIBLE');
CREATE TABLE object_armor (
	armor_id SERIAL,
	armor_csv_capabilities VARCHAR(2048),
	armor_csv_attach_to VARCHAR(2048),
	armor_thac0 INTEGER NOT NULL,
	armor_weight_in_lbs FLOAT NOT NULL,
	armor_fire_resistance_percent FLOAT NOT NULL DEFAULT 0.0,
	armor_balistic_resistance_percent FLOAT NOT NULL DEFAULT 0.0,
	armor_speed_profile speed_profile_type_t NOT NULL DEFAULT 'HINDERED',
	armor_offensive_damage_amount INTEGER NOT NULL DEFAULT 0,
	armor_durability_profile durability_profile_type_t NOT NULL DEFAULT 'DURABLE',
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
