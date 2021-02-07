CREATE TABLE public.mob_equipment (
	id SERIAL UNIQUE,
	meq_profile_name varchar(32),
	meq_vnum integer UNIQUE NOT NULL,
	meq_light varchar(32),    
	meq_finger_r varchar(32), 
	meq_finger_l varchar(32), 
	meq_neck_1 varchar(32),    
	meq_neck_2 varchar(32),    
	meq_body varchar(32),     
	meq_head varchar(32),     
	meq_legs varchar(32),     
	meq_feet varchar(32),     
	meq_hands varchar(32),    
	meq_arms varchar(32),     
	meq_shield varchar(32),   
	meq_about varchar(32),    
	meq_waist varchar(32),    
	meq_wrist_r varchar(32),  
	meq_wrist_l varchar(32),  
	meq_wield varchar(32),    
	meq_hold varchar(32),     
	meq_secondary varchar(32),
	meq_shoulders_l varchar(32),
	meq_shoulders_r varchar(32),
	meq_backpack varchar(32),
	meq_goggles varchar(32),
	meq_vest_pack varchar(32),
	meq_elbow_l varchar(32),
	meq_elbow_r varchar(32),
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id)
);
CREATE TABLE public.mob_equipment_map (
  id SERIAL UNIQUE,
	mmap_mob_vnum integer NOT NULL,
	mmap_mob_equipment_vnum integer NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id),
		CONSTRAINT fk_mob_vnum
			FOREIGN KEY (mmap_mob_vnum)
			REFERENCES public.mobile(mob_virtual_number)
			ON DELETE CASCADE
			ON UPDATE CASCADE,
		CONSTRAINT fk_meq_vnum
			FOREIGN KEY (mmap_mob_equipment_vnum)
			REFERENCES public.mob_equipment(meq_vnum)
			ON DELETE CASCADE
			ON UPDATE CASCADE
);
