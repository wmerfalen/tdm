DROP TABLE class_pyrexia;
CREATE TABLE class_pyrexia (
	pyrexia_id SERIAL,
	pyrexia_player_id INTEGER NOT NULL,
	pyrexia_fire_nade_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_drench_object_in_gasoline_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_drench_room_in_gasoline_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_convert_to_fire_nade_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_attach_incendiary_grenade_launcher_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_attach_flamethrower_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_create_wall_of_fire_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_seal_off_room_with_wall_of_fire_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_summon_napalm_strike_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_solar_assistance_level INTEGER NOT NULL DEFAULT 0,
	pyrexia_force_weapon_overheat_level INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
