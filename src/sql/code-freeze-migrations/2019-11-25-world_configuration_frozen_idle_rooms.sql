ALTER TABLE world_configuration_start_rooms ADD COLUMN frozen_room INT NOT NULL DEFAULT 0;
ALTER TABLE world_configuration_start_rooms ADD COLUMN idle_room INT NOT NULL DEFAULT 0;
