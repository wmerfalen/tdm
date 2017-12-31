CREATE TABLE extra_description (
	id SERIAL,
	obj_fk_id INTEGER NOT NULL,
	extra_keyword VARCHAR(64),
	extra_description VARCHAR(1024)
);
