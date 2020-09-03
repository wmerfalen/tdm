ALTER TABLE object_gadget DROP COLUMN exit_key;
ALTER TABLE object_gadget DROP COLUMN min_level;
ALTER TABLE object_gadget DROP COLUMN worth;
ALTER TABLE object_gadget ADD COLUMN gadget_exit_key INTEGER NOT NULL DEFAULT 0;
ALTER TABLE object_gadget ADD COLUMN gadget_min_level INTEGER NOT NULL DEFAULT 0;
ALTER TABLE object_gadget ADD COLUMN gadget_worth INTEGER NOT NULL DEFAULT 0;

ALTER TABLE object_armor DROP COLUMN hp;
ALTER TABLE object_armor DROP COLUMN classification;
ALTER TABLE object_armor DROP COLUMN worth;
ALTER TABLE object_armor ADD COLUMN armor_hp INTEGER NOT NULL DEFAULT 0;
ALTER TABLE object_armor ADD COLUMN armor_classification VARCHAR(8) NOT NULL DEFAULT 'BASIC';
ALTER TABLE object_armor ADD COLUMN armor_worth INTEGER NOT NULL DEFAULT 0;
