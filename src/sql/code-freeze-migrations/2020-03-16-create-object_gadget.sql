CREATE TYPE gadget_type_t AS ENUM ('GRAPPLING_HOOK', 'BARBED_WIRE', 'CAMERA', 'THERMAL_CAMERA', 'NIGHT_VISION_CAMERA', 'MOTION_SENSOR', 'DEPLOYABLE_SHIELD', 'TRIPWIRE' );

CREATE TABLE object_gadget (
	gadget_id SERIAL,
	gadget_csv_capabilities VARCHAR(2048),
	gadget_csv_attach_to VARCHAR(2048),
	gadget_electronic INTEGER NOT NULL DEFAULT 0,
	gadget_health_points INTEGER NOT NULL DEFAULT 0,
	gadget_shield_points INTEGER NOT NULL DEFAULT 0,
	gadget_durability_profile durability_profile_type_t NOT NULL DEFAULT 'DURABLE',
	gadget_move_points INTEGER NOT NULL DEFAULT 0,
	gadget_damage_points INTEGER NOT NULL DEFAULT 0,
	gadget_str_type gadget_type_t NOT NULL,
	gadget_type INTEGER NOT NULL,
	gadget_manufacturer VARCHAR(256) NOT NULL,
	gadget_name VARCHAR(256) NOT NULL,
	gadget_vnum INTEGER NOT NULL,
	gadget_rarity rarity_t NOT NULL DEFAULT 'COMMON',
	gadget_file VARCHAR(2048),
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
