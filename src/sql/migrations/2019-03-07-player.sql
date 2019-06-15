DROP TABLE player;
CREATE TABLE player (
		id SERIAL PRIMARY KEY,
		player_password VARCHAR(1024) NOT NULL,
		player_affection_plr_bitvector INTEGER NOT NULL DEFAULT 0,
		player_affection_bitvector INTEGER NOT NULL DEFAULT 0,
		player_name VARCHAR(24) NOT NULL DEFAULT 'recruit',
		player_short_description VARCHAR(512) NOT NULL default 'A lonely recruit',
		player_long_description TEXT,
		player_action_bitvector INTEGER NOT NULL DEFAULT 0,
		player_ability_strength INTEGER NOT NULL DEFAULT 1,
		player_ability_strength_add INTEGER NOT NULL DEFAULT 1,
		player_ability_intelligence INTEGER NOT NULL DEFAULT 1,
		player_ability_wisdom INTEGER NOT NULL DEFAULT 1,
		player_ability_dexterity INTEGER NOT NULL DEFAULT 1,
		player_ability_constitution INTEGER NOT NULL DEFAULT 1,
		player_ability_charisma INTEGER NOT NULL DEFAULT 1,
		player_ability_alignment INTEGER NOT NULL DEFAULT 0,
		player_attack_type INTEGER NOT NULL DEFAULT 0,
		player_max_hitpoints INTEGER NOT NULL DEFAULT 100,
		player_max_mana INTEGER NOT NULL DEFAULT 100,
		player_max_move INTEGER NOT NULL DEFAULT 100,
		player_gold INTEGER NOT NULL DEFAULT 0,
		player_exp INTEGER NOT NULL DEFAULT 0,
		player_sex VARCHAR(1) NOT NULL DEFAULT 'M',
		player_hitpoints INTEGER NOT NULL DEFAULT 100,
		player_mana INTEGER NOT NULL DEFAULT 100,
		player_move INTEGER NOT NULL DEFAULT 100,
		player_damroll INTEGER NOT NULL DEFAULT 6,
		player_weight INTEGER NOT NULL DEFAULT 10,
		player_height INTEGER NOT NULL DEFAULT 60,
		player_class VARCHAR(16) NOT NULL DEFAULT 'x',
		player_title VARCHAR(24) DEFAULT 'recruit',
		player_hometown VARCHAR(24) DEFAULT 'SLC',
		player_damnodice INTEGER NOT NULL DEFAULT 3,
		player_damsizedice INTEGER NOT NULL DEFAULT 6,
		player_type VARCHAR(6) NOT NULL DEFAULT 'x',
		player_alignment INTEGER NOT NULL DEFAULT 0,
		player_level INTEGER NOT NULL DEFAULT 1,
		player_hitroll INTEGER NOT NULL DEFAULT 3,
		player_armor INTEGER NOT NULL DEFAULT 3,
		player_birth TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		player_time_played INTEGER NOT NULL DEFAULT 0,
		player_logon TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		player_preferences INTEGER NOT NULL DEFAULT 0
	);
