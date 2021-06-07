CREATE TABLE public.weapon_locker (
	id SERIAL UNIQUE,
	w_room_vnum INTEGER[] NOT NULL,
	w_yaml TEXT[] NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE public.armor_locker (
	id SERIAL UNIQUE,
	a_room_vnum INTEGER[] NOT NULL,
	a_yaml TEXT[] NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE public.room_signs (
	id SERIAL UNIQUE,
	r_room_vnum INTEGER NOT NULL,
	r_name TEXT NOT NULL,
	r_text TEXT NOT NULL,
	PRIMARY KEY(id),
		CONSTRAINT fk_room_vnum
			FOREIGN KEY (r_room_vnum)
			REFERENCES public.room(room_number)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
