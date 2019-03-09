ALTER TABLE player ALTER COLUMN player_affection_bitvector DROP NOT NULL;
ALTER TABLE player ALTER COLUMN player_affection_plr_bitvector DROP NOT NULL;
ALTER TABLE player ALTER COLUMN player_preferences DROP NOT NULL;

ALTER TABLE player ALTER COLUMN player_affection_bitvector DROP DEFAULT;
ALTER TABLE player ALTER COLUMN player_affection_plr_bitvector DROP DEFAULT;
ALTER TABLE player ALTER COLUMN player_preferences DROP DEFAULT;

ALTER TABLE player ALTER COLUMN player_affection_bitvector TYPE varchar(2048); 
ALTER TABLE player ALTER COLUMN player_affection_plr_bitvector TYPE varchar(2048); 
ALTER TABLE player ALTER COLUMN player_preferences TYPE varchar(2048);
