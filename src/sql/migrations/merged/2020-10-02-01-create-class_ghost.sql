DROP TABLE class_ghost;
CREATE TABLE class_ghost (
	ghost_id SERIAL,
	ghost_player_id INTEGER NOT NULL,
	ghost_drone_scan_level INTEGER NOT NULL DEFAULT 0,
	ghost_stealth_level INTEGER NOT NULL DEFAULT 0,
	ghost_xray_shot_level INTEGER NOT NULL DEFAULT 0,
	ghost_feign_death_level INTEGER NOT NULL DEFAULT 0,
	ghost_summon_extraction_level INTEGER NOT NULL DEFAULT 0,
	ghost_plant_claymore_level INTEGER NOT NULL DEFAULT 0,
	ghost_penetrating_shot_level INTEGER NOT NULL DEFAULT 0,
	ghost_intimidation_level INTEGER NOT NULL DEFAULT 0,
	ghost_cryogenic_grenade_level INTEGER NOT NULL DEFAULT 0,
	ghost_flash_underbarrel_level INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
