
CREATE TABLE public.scripted_sequences (
	id SERIAL UNIQUE,
	s_sequence_vnum INTEGER NOT NULL UNIQUE,
	PRIMARY KEY(id)
);
CREATE TABLE public.contract_step_callback (
	id SERIAL UNIQUE,
		s_contract_vnum INTEGER NOT NULL,
		s_task_type TEXT,
		s_task_target TEXT,
		s_task_vnum INTEGER,
		s_sequence_vnum INTEGER NOT NULL,
	PRIMARY KEY(id),
		CONSTRAINT fk_contract_vnum
			FOREIGN KEY (s_contract_vnum)
			REFERENCES public.contracts(c_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_sequence_vnum
			FOREIGN KEY (s_sequence_vnum)
			REFERENCES public.scripted_sequences(s_sequence_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
CREATE TABLE public.scripted_steps (
	id SERIAL UNIQUE,
	s_sequence_vnum INTEGER NOT NULL,
	s_wait_ticks INTEGER NOT NULL DEFAULT 0,
	s_mob INTEGER,
	s_obj INTEGER,
	s_room INTEGER,
	s_quantity INTEGER,
	s_order INTEGER NOT NULL DEFAULT 0,
	s_interpret TEXT,
	s_yaml TEXT,
	s_type TEXT NOT NULL,
	s_dialogue TEXT,
	PRIMARY KEY(id),
		CONSTRAINT fk_sequence_vnum
			FOREIGN KEY (s_sequence_vnum)
			REFERENCES public.scripted_sequences(s_sequence_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
