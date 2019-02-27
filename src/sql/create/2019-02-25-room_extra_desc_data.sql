CREATE TABLE room_extra_desc_data (
	id SERIAL,
	room_number INTEGER NOT NULL,
	ex_keyword VARCHAR(64),
	ex_description VARCHAR(1024),
	PRIMARY KEY(id)
);

