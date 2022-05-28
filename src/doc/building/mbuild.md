# Builder documentation: Building mobs

# roam-pattern
	- javascript helper:
		- simply add 'roam-pattern': 'value' to your auto feed structure...
		- example: in far.js see the 'retail_associate' mob
	- mbuild command:
		- `mbuild roam-pattern <mob_index> <value>...[value-N]`
		- the value will gather strings using `gather_strings_starting_at(N)`
	- full javascript example:
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
				},
				'extended-type': 'MELEE_COMBATANT',
				'roam-pattern': 'Allied Foods',
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
	auto_feed_mob_list(mobs);
```


