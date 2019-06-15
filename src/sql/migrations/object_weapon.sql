CREATE TABLE object_weapon (
	id SERIAL,
	obj_fk_id INTEGER NOT NULL,
	obj_ammo_max INTEGER NOT NULL,
	obj_ammo_type INTEGER NOT NULL,
	obj_cooldown INTEGER NOT NULL,
	obj_can_snipe BOOLEAN NOT NULL
);
