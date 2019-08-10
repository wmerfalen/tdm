CREATE TYPE weapon_base AS ENUM (
	'rifle','explosive','drone','attachment'
);

CREATE TYPE weapon_type AS ENUM(
	'SHOTGUN',
	'ASSAULT_RIFLE',
	'SUB_MACHINE_GUN',
	'SNIPER',
	'HANDGUN',
	'MACHINE_PISTOL',
	'LIGHT_MACHINE_GUN',
	'FRAG_GRENADE',
	'INCENDIARY_GRENADE',
	'REMOTE_EXPLOSIVE',
	'REMOTE_CHEMICAL',
	'EMP_GRENADE',
	'CLAYMORE_MINE',
	'SMOKE_GRENADE',
	'FLASH_BANG',
	'GROUND_DRONE',
	'AERIAL_DRONE',
	'AQUATIC_DRONE',
	'SCOPE',
	'GRIP',
	'BARREL',
	'MAGAZINE',
	'SIDERAIL',
);
CREATE TABLE object_weapon (
	id SERIAL,
	obj_fk_id INTEGER NOT NULL,
	obj_ammo_max INTEGER NOT NULL,
	obj_ammo_type INTEGER NOT NULL,
	obj_cooldown INTEGER NOT NULL,
	obj_can_snipe BOOLEAN NOT NULL,
	obj_ammo INTEGER NOT NULL DEFAULT 0,
	obj_clip_size INTEGER NOT NULL,
	obj_holds_ammo BOOLEAN NOT NULL DEFAULT false,
	obj_type weapon_type,
	obj_base weapon_base,
	obj_is_ammo BOOLEAN NOT NULL DEFAULT false
);
