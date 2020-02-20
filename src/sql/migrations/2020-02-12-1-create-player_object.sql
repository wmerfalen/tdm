CREATE TYPE player_object_type_t AS ENUM ('rifle','explosive','attachment','gadget','drone', 'armor', 'consumable');

CREATE TABLE player_object (
po_id SERIAL,
po_player_id INTEGER NOT NULL,
po_type player_object_type_t NOT NULL,
po_type_id INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
