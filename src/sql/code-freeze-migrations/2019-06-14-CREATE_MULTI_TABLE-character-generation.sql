CREATE TABLE player_classes (
	id SERIAL PRIMARY KEY,
	pclass_name VARCHAR(24) NOT NULL,
	pclass_description TEXT
);

CREATE TABLE player_races (
	id SERIAL PRIMARY KEY,
	prace_name VARCHAR(24) NOT NULL,
	prace_description TEXT
);


CREATE TYPE base_attribute AS ENUM (
	'STR','INT','DEX','WIS','CHA','CON','LCK'
);

CREATE TABLE player_race_perks (
	id SERIAL PRIMARY KEY,
	prperk_id INTEGER NOT NULL REFERENCES player_races(id),
	prperk_name VARCHAR(64) NOT NULL,
	prperk_description TEXT,
	prperk_base_attribute_mutate base_attribute NOT NULL,
	prperk_base_attribute_amount_add INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE skill_trees (
	id SERIAL PRIMARY KEY,
	sktree_class INTEGER NOT NULL REFERENCES player_classes(id),
	sktree_skill_name VARCHAR(32) NOT NULL,
	sktree_skill_description TEXT,
	sktree_skill_level_unlock INTEGER NOT NULL,
	sktree_skill_parent_id INTEGER REFERENCES skill_trees(id),
	sktree_skill_order INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE specialty_skills (
	id SERIAL PRIMARY KEY,
	spskill_name VARCHAR(64) NOT NULL,
	spskill_description TEXT
);

CREATE TABLE player_generated_data (
	id SERIAL PRIMARY KEY,
	pgdata_player_id INTEGER NOT NULL REFERENCES player(id),
	pgdata_class INTEGER NOT NULL REFERENCES player_classes(id),
	pgdata_race INTEGER NOT NULL REFERENCES player_races(id),
	pgdata_race_perks INTEGER NOT NULL REFERENCES player_race_perks(id),
	pgdata_specialty_skill INTEGER REFERENCES specialty_skills(id)
);


CREATE TABLE player_skill_trees_unlocked (
	id SERIAL PRIMARY KEY,
	pstun_player_id INTEGER NOT NULL REFERENCES player(id),
	pstun_skill_tree_id INTEGER NOT NULL REFERENCES skill_trees(id)
);
