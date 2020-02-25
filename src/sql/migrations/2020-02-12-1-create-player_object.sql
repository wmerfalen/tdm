CREATE TYPE player_object_type_t AS ENUM ('rifle','explosive','attachment','gadget','drone', 'armor', 'consumable','object');

DROP TABLE player_object;
CREATE TABLE player_object (
po_id SERIAL,
po_player_id INTEGER NOT NULL,
po_type player_object_type_t NOT NULL DEFAULT 'object',
po_type_vnum INTEGER,
po_type_id INTEGER,
po_type_load VARCHAR(4) NOT NULL DEFAULT 'id',
po_wear_position INTEGER NOT NULL,
po_in_inventory INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

