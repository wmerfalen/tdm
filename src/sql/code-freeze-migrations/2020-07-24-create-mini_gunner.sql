CREATE TABLE mini_gunner_sentinel (
	id SERIAL PRIMARY KEY,
	mgs_mob_vnum INTEGER NOT NULL DEFAULT -1,
	mgs_face_direction VARCHAR(1) NOT NULL DEFAULT 'N',
	mgs_room_vnum INTEGER NOT NULL REFERENCES room(room_number)
);
