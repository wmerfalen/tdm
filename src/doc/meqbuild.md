# Builder documentation: Building mob equipment 

# possible wear flags
	std::map<std::string,uint8_t> wear_flags_map = {
		{"light",WEAR_LIGHT},
		{"finger_r",WEAR_FINGER_R},
		{"finger_l",WEAR_FINGER_L},
		{"neck_1",WEAR_NECK_1},
		{"neck_2",WEAR_NECK_2},
		{"body",WEAR_BODY},
		{"head",WEAR_HEAD},
		{"legs",WEAR_LEGS},
		{"feet",WEAR_FEET},
		{"hands",WEAR_HANDS},
		{"arms",WEAR_ARMS},
		{"shield",WEAR_SHIELD},
		{"about",WEAR_ABOUT},
		{"waist",WEAR_WAIST},
		{"wrist_r",WEAR_WRIST_R},
		{"wrist_l",WEAR_WRIST_L},
		{"wield",WEAR_WIELD},
		{"primary",WEAR_PRIMARY},
		{"hold",WEAR_HOLD},
		{"secondary",WEAR_SECONDARY},
		{"shoulders_l",WEAR_SHOULDERS_L},
		{"shoulders_r",WEAR_SHOULDERS_R},
		{"backpack",WEAR_BACKPACK},
		{"goggles",WEAR_GOGGLES},
		{"vest_pack",WEAR_VEST_PACK},
		{"elbow_l",WEAR_ELBOW_L},
		{"elbow_r",WEAR_ELBOW_R}
	};

# possible commands
	- `map-list`
	- `map-assign`

# preferred way to build it (using javascript)
```
	var mobs = {
		surgeon: {
			mbuild: {
				attr: {
					virt: 602,
					mana: 815,
					max_mana: 815,
					hit: 8580,
					max_hit: 8580,
					move: 240,
					max_move: 240,
					damroll: 215,
					gold: 85000,
					level: 80,
					weight: 10,
					height: 4,
					strength: 185,
					strength_add: 183,
					intelligence: 182,
					dexterity: 184,
					constitution: 188,
					charisma: 85,
					damnodice: 184,
					damsizedice: 186,
					name: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
					sex: "FEMALE",
					action: "ISNPC",
					short_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
					long_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
					description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
				} /** end attr */
			}, /** end mbuild */
			meqbuild: {
				name: 'triton-field-surgeon',
				virtual_number: 5,
				positions: {
					wield: 'melee/operating-scalpel.yml',
					secondary: 'melee/operating-scalpel.yml',
					goggles: 'armor/operating-goggles.yml',
					hands: 'armor/rubber-gloves.yml',
					body: 'armor/plain-white-lab-coat.yml',
					legs: 'armor/pale-brown-pants.yml',
					feet: 'armor/static-free-shoes.yml'
				}
			},
			zbuild: {
				command: 'mob',
				zone_id: 0,
				mob_vnum: 602,
				room_vnum: 145,
				max: 1,
				if_flag: 0,
			}
		},// end scientist
	};//end mobs

	
	/** 
		* call auto_feed to take care of everything for us
		*/
	auto_feed_mob_list(mobs);

```


