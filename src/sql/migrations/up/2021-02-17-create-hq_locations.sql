CREATE TABLE public.hq_locations (
	id SERIAL UNIQUE,
	hq_affiliation varchar(32),
	hq_room_vnum integer UNIQUE NOT NULL,
	hq_level integer NOT NULL DEFAULT 1,
	hq_basic_mob_count integer NOT NULL DEFAULT 10,
	hq_advanced_mob_count integer NOT NULL DEFAULT 10,
	hq_elite_mob_count integer NOT NULL DEFAULT 10,
	hq_suv_count integer NOT NULL DEFAULT 10,
	hq_sedan_count integer NOT NULL DEFAULT 20,
	hq_armored_van_count integer NOT NULL DEFAULT 2,
	hq_replenish_ticks integer NOT NULL DEFAULT 150,
	hq_replenish_basic_count integer NOT NULL DEFAULT 15,
	hq_replenish_advanced_count integer NOT NULL DEFAULT 10,
	hq_replenish_elite_count integer NOT NULL DEFAULT 2,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (hq_room_vnum)
			REFERENCES public.room(room_number)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
