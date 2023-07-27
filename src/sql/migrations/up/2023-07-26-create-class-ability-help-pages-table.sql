CREATE TABLE public.class_ability_help_pages(
	id SERIAL UNIQUE,
	ch_class TEXT,
	ch_title TEXT,
	ch_ability TEXT,
	ch_usage TEXT,
	ch_description TEXT,
	ch_examples TEXT,
	ch_keywords TEXT NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);

