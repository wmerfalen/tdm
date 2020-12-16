DROP TABLE class_forge;
CREATE TABLE class_forge (
	forge_id SERIAL,
	forge_player_id INTEGER NOT NULL,
	forge_craft_shotgun_underbarrel_level INTEGER NOT NULL DEFAULT 0,
	forge_craft_incendiary_underbarrel_level INTEGER NOT NULL DEFAULT 0,
	forge_craft_flash_underbarrel_level INTEGER NOT NULL DEFAULT 0,
	forge_craft_silencer_level INTEGER NOT NULL DEFAULT 0,
	forge_craft_ammunition_level INTEGER NOT NULL DEFAULT 0,
	forge_craft_armor_plating_level INTEGER NOT NULL DEFAULT 0,
	forge_craft_high_velocity_ammo_level INTEGER NOT NULL DEFAULT 0,
	forge_dismantle_weapon_level INTEGER NOT NULL DEFAULT 0,
	forge_trade_attribute_level INTEGER NOT NULL DEFAULT 0,
	forge_repair_armor_level INTEGER NOT NULL DEFAULT 0,
	forge_repair_weapon_level INTEGER NOT NULL DEFAULT 0,
	forge_reinforce_weapon_level INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
