/** VERSION: 1 */
CREATE TABLE world_configuration_start_rooms (
	id SERIAL,
	mortal_start_room INTEGER NOT NULL,
	immortal_start_room INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT now(),
	is_active BOOLEAN 
);
