CREATE TABLE attachment (
		id SERIAL PRIMARY KEY,
		att_obj_id INTEGER NOT NULL REFERENCES object(id),
		att_attached_to_obj_id INTEGER REFERENCES object(id),
		att_created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
