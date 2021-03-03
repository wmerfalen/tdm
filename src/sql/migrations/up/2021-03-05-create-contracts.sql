 DROP TABLE public.contract_steps;
 DROP TABLE public.player_contract_state;
 DROP TABLE public.contracts;
CREATE TABLE public.contracts (
	id SERIAL UNIQUE,
	c_vnum integer NOT NULL UNIQUE,
	c_description TEXT NOT NULL,
	c_title TEXT NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);
CREATE TABLE public.contract_steps (
	id SERIAL UNIQUE,
	s_contract_vnum integer not null,
	s_task_type integer NOT NULL,
	s_task_target integer NOT NULL,
	s_description TEXT NOT NULL,
	s_object_yaml TEXT,
	s_mob_vnum integer,
	s_room_vnum integer,
	s_quota integer,
	s_is_optional boolean NOT NULL DEFAULT false,
	s_order integer not null,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_contract_vnum
			FOREIGN KEY (s_contract_vnum)
			REFERENCES public.contracts(c_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
CREATE TABLE public.player_contract_state (
	id SERIAL UNIQUE,
	pc_player_id integer NOT NULL,
	pc_contract_vnum integer NOT NULL,
	pc_state_data TEXT,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_cvnum
			FOREIGN KEY (pc_contract_vnum)
			REFERENCES public.contracts(c_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_player_id
			FOREIGN KEY (pc_player_id)
			REFERENCES public.player(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
