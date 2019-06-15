CREATE TABLE object (
	id SERIAL,
	obj_item_number INTEGER NOT NULL,
	obj_flags INTEGER NOT NULL,
	obj_name varchar(128) NOT NULL,
	obj_description varchar(1024),
	obj_short_description varchar(64) NOT NULL,
	obj_action_description varchar(32),
	obj_type INTEGER ,
	obj_worn_on INTEGER,
	obj_type_data varchar(16),
	obj_extra_keyword VARCHAR(64),
	obj_extra_description VARCHAR(1024)
);
