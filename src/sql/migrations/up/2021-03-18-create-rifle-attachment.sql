CREATE TABLE public.rifle_attachment (
	id SERIAL UNIQUE,
	rifle_player_id integer NOT NULL,
	rifle_data TEXT NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_player_id
			FOREIGN KEY (rifle_player_id)
			REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
