CREATE TABLE public.hq_locations (
	id SERIAL UNIQUE,
	hq_affiliation varchar(32),
	hq_room_vnum integer UNIQUE NOT NULL,
	hq_level integer NOT NULL DEFAULT 1,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (hq_room_vnum)
			REFERENCES public.room(room_number)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
