# Tactical Denial
Tactical Denial is a text-based looter shooter set in a world similar to modern Earth. 
The core gameplay emphasizes ranged and melee combat. Think: Diablo III 
looter mechanics mixed with an exciting fast-paced military shooter.

# An expanding arsenal
- sniper rifles
- assault rifles
- pistols
- sub machine guns
- light machine guns
- machine pistols
- shotguns
- grenades
	- flash-bang
	- smoke
	- incendiary
	- frag
- claymore mines
### NIX - plastic explosives
	#### NIX - EMP
	#### NIX - Chaff
### NIX - breach charges
### NIX - chemical weapons
### NIX - drones
### NIX 	- aerial drones
### NIX 	- land-based drones
### NIX - remote view cameras

### NIX # TODO
### NIX - must implement drone assisted sniping
### NIX - must implement attach+detonate for C4
### NIX - implement install/uninstall minigame functions
### NIX - implement actions on both wires/lineup mini games
### NIX - crud operations for mini games
### NIX - fix medic healing. currently, anyone can heal
### NIX - implement buffs/nerfs for all courses [ huge ]
### NIX - implement `install_computer_choice` and `add_computer_choice` commands
### NIX - tactical gadgetry course help file
### NIX - implement melee functions for the striking clinic course
### NIX - implement save/recall of striking combos
### NIX - crud operations for save/recall (maybe use the proficiency skills db?)
### NIX - implement MCMAP Judo course
### NIX - implement MCMAP Submission Grappling course

# The Roguelike that turned into a full-blown mud
The first goal of Tactical Denial is to a roguelike mud setup as soon as possible.
This will allow the audience to play the game while development can focus on 
building out gameplay features in an iterative manner. Once the dynamics are solid
enough, we can compose those into skillsets that are divided by assets.

[##########################################]
[            SIMPLE MOB CLASSES            ]
[ key: WGE = weapons/gadgets/explosives    ]
[##########################################]
	- [#########################################] -
	- [ ROGUE ARCHETYPES                        ] -
	- [#########################################] -
    "  [1] Sniper             [          ROGUE ARCHETYPE ]\r\n"
		"   [ Sniper sub-classes                             ]\r\n"
		"     -> [1G] GHOST       [         stealth operator ]\r\n" 
						- Military Triad
							- [Melee]   [x] [ ] [ ]
							- [Weapons] [x] [x] [x]
							- [Intel]   [x] [x] [ ]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [ ] [ ]
							- [Armor]   [x] [x] [x]
		"     -> [1M] Marksman    [           weapons expert ]\r\n" 
						- Military Triad
							- [Melee]   [x] [ ] [ ]
							- [Weapons] [x] [x] [x]
							- [Intel]   [x] [x] [ ]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [ ]
							- [Armor]   [x] [x] [ ]
		"     -> [1T] Bandit      [                    thief ]\r\n" 
						- Military Triad
							- [Melee]   [x] [x] [ ]
							- [Weapons] [x] [x] [ ]
							- [Intel]   [x] [x] [x]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [x]
							- [Armor]   [x] [ ] [ ]

	- [#########################################] -
	- [ ALCHEMIST ARCHETYPES                    ] -
	- [#########################################] -
		"  [3] Chemist            [      ALCHEMIST ARCHETYPE ]\r\n"
		"   [ Chemist sub-classes                            ]\r\n"
		"     -> [3M] Malady      [ poison weapon specialist ]\r\n"
						- Military Triad
							- [Melee]   [x] [x] [ ]
							- [Weapons] [x] [x] [ ]
							- [Intel]   [x] [x] [x]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [ ]
							- [Armor]   [x] [x] [ ]
		"     -> [3P] Pyrexia     [    incendiary specialist ]\r\n"
						- Military Triad
							- [Melee]   [x] [ ] [ ]
							- [Weapons] [x] [x] [x]
							- [Intel]   [x] [x] [x]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [x]
							- [Armor]   [x] [ ] [ ]
		"     -> [3C] Dealer      [   enhancement specialist ]\r\n"
						- Military Triad
							- [Melee]   [x] [ ] [ ]
							- [Weapons] [x] [x] [x]
							- [Intel]   [x] [x] [x]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [ ] [ ]
							- [Armor]   [x] [x] [x]

	- [#########################################] -
	- [ WARRIOR ARCHETYPES                      ] -
	- [#########################################] -
		"  [2] Marine             [        WARRIOR ARCHETYPE ]\r\n"
		"   [ Marine sub-classes                             ]\r\n"
		"     -> [2B] Butcher     [ bladed weapon specialist ]\r\n"
						- Military Triad
							- [Melee]   [x] [x] [x]
							- [Weapons] [x] [x] [ ]
							- [Intel]   [x] [x] [ ]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [ ]
							- [Armor]   [x] [x] [ ]
		"     -> [2S] Striker     [      martial arts expert ]\r\n"
						- Military Triad
							- [Melee]   [x] [x] [x]
							- [Weapons] [x] [x] [x]
							- [Intel]   [x] [ ] [ ]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [x]
							- [Armor]   [x] [ ] [ ]
		"     -> [2M] Obstructor  [       submissions expert ]\r\n"
						- Military Triad
							- [Melee]   [x] [x] [x]
							- [Weapons] [x] [x] [ ]
							- [Intel]   [x] [x] [ ]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [ ] [ ]
							- [Armor]   [x] [x] [x]


	- [#########################################] -
	- [ ENGINEER ARCHETYPES                     ] -
	- [#########################################] -
		"  [4] Engineer           [     BLACKSMITH ARCHETYPE ]\r\n"
		"   [ Engineer sub-classes                           ]\r\n"
		"     -> [4F] Forge       [          weapons crafter ]\r\n"
						- Military Triad
							- [Melee]   [x] [x] [ ]
							- [Weapons] [x] [x] [ ]
							- [Intel]   [x] [x] [x]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [ ]
							- [Armor]   [x] [x] [ ]
		"     -> [4D] Syndrome    [  autonomous drone expert ]\r\n"
						- Military Triad
							- [Melee]   [x] [ ] [ ]
							- [Weapons] [x] [x] [x]
							- [Intel]   [x] [x] [x]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [x] [x]
							- [Armor]   [x] [ ] [ ]
		"     -> [4M] Machinist   [        technology expert ]\r\n"
						- Military Triad
							- [Melee]   [x] [x] [ ]
							- [Weapons] [x] [x] [ ]
							- [Intel]   [x] [x] [x]
						- Speed/Armor tradeoff MAX 4 points
							- [Speed]   [x] [ ] [ ]
							- [Armor]   [x] [x] [x]


+++++++++++++++++++++++
| ITEM TYPE CHECKLIST |
+++++++++++++++++++++++
	- vim sorcery
		# s/^([^,]\+,/\t[ ] /
		# s/,.*//g
	- RIFLE
		- SHOTGUN
			[x] Done
		- ASSAULT RIFLE
			[x] Done
		- SUB MACHINE GUN 
			[x] Done
		- SNIPER 
			[x] Done
		- HANDGUN 
			[x] Done
		- PISTOL 
			[x] Done
		- MACHINE PISTOL 
			[x] Done
		- LIGHT MACHINE GUN 
			[x] Done
		- attribute support
			[x] chance to injure
				[x] revive injured
			[x] cooldown between shots
			[x] critical chance
			[x] critical range
			[ ] disorient amount
			[x] headshot bonus
			[x] max range
			[ ] reload time
			[ ] effective firing range
			[x] damage dice count
			[x] damage dice sides
### NIX 	- CONSUMBALE
### NIX 		- AMMUNITION 
### NIX 			[ ] done
### NIX 		- FOOD 
### NIX 			[ ] done
### NIX 		- PILLS 
### NIX 			[ ] done
### NIX 		- HERBS 
### NIX 			[ ] done
### NIX 		- HALLUCINOGENS 
### NIX 			[ ] done
### NIX 		- PEDS 
### NIX 			[ ] done
### NIX 		- GASOLINE
### NIX 			[ ] done
### NIX 		- attribute support
### NIX 			[ ] weight_in_lbs
### NIX 			[ ] consumed_by
### NIX 			[ ] adds_hp
### NIX 			[ ] adds_movement
### NIX 			[ ] adds_room_range
### NIX 			[ ] adds_strength
### NIX 			[ ] adds_intelligence
### NIX 			[ ] adds_charisma
### NIX 			[ ] adds_dexterity
### NIX 			[ ] adds_constitution
### NIX 			[ ] adds_wisdom
### NIX 			[ ] adds_weight
### NIX 			[ ] adds_experience
### NIX 			[ ] adds_armor_class
### NIX 			[ ] adds_hitroll
### NIX 			[ ] adds_damage_roll
### NIX 			[ ] ticks_until_zero
### NIX 			[ ] ammo_type
### NIX 			[ ] capacity
### NIX 			[ ] adds_fire_damage
### NIX 			[ ] adds_critical_chance
### NIX 			[ ] adds_ammo_max
### NIX 			[ ] adds_chance_to_injure
### NIX 			[ ] adds_clip_size
### NIX 			[ ] adds_cooldown_between_shots
### NIX 			[ ] adds_critical_range
### NIX 			[ ] adds_disorient_amount
### NIX 			[ ] adds_headshot_bonus
### NIX 			[ ] adds_max_range
### NIX 			[ ] adds_reload_time
### NIX 			[ ] adds_muzzle_velocity
### NIX 			[ ] adds_damage_dice_count
### NIX 			[ ] adds_damage_dice_sides
	- EXPLOSIVE
		- FRAG GRENADE 
			[x] done
		- INCENDIARY GRENADE 
			[x] done
		- FLASHBANG GRENADE 
			[x] done
		- SENSOR GRENADE 
			[x] done
		- CLAYMORE MINE 
			[ ] done
		- SMOKE GRENADE 
			[ ] done
		- attributes
			[ ] alternate_explosion_type
			[x] chance_to_injure
			[x] critical_chance
			[ ] critical_range
			[ ] blast_radius
			[ ] damage_per_second
			[ ] disorient_amount
			[ ] range_modifier
			[ ] loudness_type
			
### NIX		- REMOTE EXPLOSIVE 
### NIX			[ ] done
### NIX		- REMOTE CHEMICAL 
### NIX			[ ] done
### NIX		- EMP GRENADE 
### NIX			[ ] done
### NIX		- C4 
### NIX			[ ] done
### NIX		- BREACH CHARGE 
### NIX			[ ] done
### NIX		- THERMITE CHARGE 
### NIX			[ ] done
### NIX - GADGET
### NIX		- GRAPPLING HOOK 
### NIX			[ ] done
### NIX		- BARBED WIRE 
### NIX			[ ] done
### NIX		- CAMERA 
### NIX			[x] done
### NIX		- THERMAL CAMERA 
### NIX			[x] done
### NIX		- NIGHT VISION CAMERA 
### NIX			[x] done
### NIX		- MOTION SENSOR 
### NIX			[ ] done
### NIX		- DEPLOYABLE SHIELD 
### NIX			[ ] done
### NIX 		- TRIPWIRE 
### NIX 			[ ] done
### NIX		- attributes
### NIX			[ ] csv_capabilities
### NIX			[ ] csv_attach_to
### NIX			[ ] electronic
### NIX			[ ] health_points
### NIX			[ ] shield_points
### NIX			[ ] durability_profile
### NIX			[ ] move_points
### NIX			[ ] damage_points
### KEEP WHATEVER IS DONE SO FAR	- ATTACHMENT
### KEEP WHATEVER IS DONE SO FAR		- SIGHT
### KEEP WHATEVER IS DONE SO FAR			[ ] done
### KEEP WHATEVER IS DONE SO FAR		- MUZZLE
### KEEP WHATEVER IS DONE SO FAR			[ ] done
### KEEP WHATEVER IS DONE SO FAR		- MAGAZINE 
### KEEP WHATEVER IS DONE SO FAR			[ ] done
### KEEP WHATEVER IS DONE SO FAR		- attributes
### KEEP WHATEVER IS DONE SO FAR			[ ] zoom_multiplier
### KEEP WHATEVER IS DONE SO FAR			[ ] ammunition_amount
### KEEP WHATEVER IS DONE SO FAR			[ ] silence_percent
### KEEP WHATEVER IS DONE SO FAR			[ ] durability_points
### KEEP WHATEVER IS DONE SO FAR			[ ] incendiary_damage
### KEEP WHATEVER IS DONE SO FAR			[ ] radiation_damage
### KEEP WHATEVER IS DONE SO FAR			[ ] poison_damage
### KEEP WHATEVER IS DONE SO FAR			[ ] electric_damage
### KEEP WHATEVER IS DONE SO FAR			[ ] armor_penetration_amount
### KEEP WHATEVER IS DONE SO FAR			[ ] disorient_amount
### KEEP WHATEVER IS DONE SO FAR			[ ] emp_amount
### KEEP WHATEVER IS DONE SO FAR			[ ] emp_radius
### KEEP WHATEVER IS DONE SO FAR			[ ] attach_to
### KEEP WHATEVER IS DONE SO FAR

++++++++++++++++++++++++
| Dyanmic: destruction |
++++++++++++++++++++++++
	- of walls, doors
	- the dealing of (ATK)
		- with weapons, technology
		- with perks
		- enhanced by intel
			- scanning with
				- cameras, sensor grandes
		- enhanced by precision
			- aiming
	- the prevention of (DEF)
		- with armor
		- with resistances
+++++++++++++++++++++++++
| Dyanmic: intelligence |
+++++++++++++++++++++++++
	- enemy position
		- from cameras
		- from sensor grenades
		- from satelites
	- enhances damage by knowing position
++++++++++++++++++++++
| Dynamic: breaching |
++++++++++++++++++++++
	- of soft walls/doors
	- of reinforced walls/doors
		- but only by thermite charges
+++++++++++++++++++++
| Dynamic: movement |
+++++++++++++++++++++
	- of players, npcs
	- blocking off a direction
	- moving an npc to a separate room

### NIX +++++++++++++++++++
### NIX | Fields of study |
### NIX +++++++++++++++++++
### NIX 		- [Electronics] [ ]
### NIX 			- Use of cameras
### NIX 			- Able to disable electronics
### NIX 			- Adept at electronics damage
### NIX 			- Can adapt electronics to be resistant to EMP
### NIX 			- Can adapt electronics to become EMP
### NIX 			- Can detect electronics
### NIX 			- Can disrupt cameras
### NIX 			- Can appear invisible to cameras (feed loop)
### NIX 			- Can hack servers
### NIX 			- Can disable electronics protection (electronic locks)
### NIX 		- [Armor] [ ]
### NIX 			- Can enhance armor
### NIX 			- Can penetrate armor with normal ammo
### NIX 			- Can take armor from NPCs
### NIX 			- Can mold collected armor
### NIX 			- Held armor has no weight bearance
### NIX 			- Heavy armor doesn't affect movement
### NIX 			- Explosions less likely to injure
### NIX 			- Chance to injure decreased
### NIX 		- [Marksmanship] [ ]
### NIX 			- Chance to hit during spray is increased
### NIX 			- Chance to headshot is increased
### NIX 			- Chance to target limb is increased
### NIX 			- Sniping accuracy increased
### NIX 		- [Sniping] [ ]
### NIX 			- Accuracy increased with sniper rifles
### NIX 			- Exclusive access to advanced sniper rifles
### NIX 			- Tracking bullets
### NIX 		- [Demolitions] [ ]
### NIX 			- Breach charge placement time decreased
### NIX 			- C4 remote detonation distance increased
### NIX 			- Grenade throw distance increased
### NIX 			- Claymore damage increased
### NIX 			- Thermite breach charge placement time decreased
### NIX 			- Incendiary grenade damage increased
### NIX 			- Can craft C4
### NIX 			- Chance to do 33% more damage with any explosive
### NIX 		- [Chemistry] [ ]
### NIX 			- Can create steroids
### NIX 			- Can create human growth hormone
### NIX 			- Can create noxious gas grenades
### NIX 			- Seal off room to make air-tight
### NIX 			- Can create incendiary ammo
### NIX 		- [Weapon Handling] [ ]
### NIX 			- Enhanced familiarity with weapons
### NIX 			- Increased accuracy with any weapon
### NIX 			- Decreased cooldown between shots
### NIX 			- Faster reloading
### NIX +++++++++++++++++++++
### NIX | WEAPON ATTRIBUTES |
### NIX +++++++++++++++++++++
### NIX  	- Description: 
### NIX 		- Attributes for weapons apply to a specific character.
### NIX 	- Familiarity
### NIX 		- Increases damage by 4% per level
### NIX 		- Decreases reload time by 4% per level
### NIX 		- Increases injure chance by 2% per level
### NIX 		- Increases critical chance by 2.5% per level
### NIX 		- Increases limb damage chance by 3% per level
### NIX 		- Increases headshot chance by 0.5% per level
### NIX 
### NIX 
### NIX 
### NIX ++++++++++++++++++++
### NIX | TYPES OF POISONS |
### NIX ++++++++++++++++++++
### NIX 	- Friendly attacks
### NIX 		- Poisoned NPC's will attack their own side
### NIX 	- Acidic blood
### NIX 		- Poisoned NPC will bleed acid
### NIX 	- Explode
### NIX 		- Poisoned NPC will combust
### NIX 
### NIX 	- Triad Allocation
### NIX 			1) Each triad has 3 traits
### NIX 			2) It is not possible to fill up all 3 allocations with 3 points
### NIX 			3) There are only 6 points available to spread over all allocations
### NIX 			4) There can only be one allocation that has 3 points. 
### NIX 			5) It is not possible to have 2 allocations with 3 points.
### NIX 		- Military Triad
### NIX 			- [Melee]   [x] [ ] [ ]
### NIX 			- [Weapons] [x] [x] [x]
### NIX 			- [Intel]   [x] [x] [ ]
### NIX 		- Speed/Armor tradeoff MAX 4 points
### NIX 			- [Speed]   [ ] [ ] [ ]
### NIX 			- [Armor]   [ ] [ ] [ ]
### NIX 			- Examples: 
### NIX 				- 3 Speed, 1 Armor [HYPER - Agile, Stealthy, Quiet, Fast, Lots of Movement Points, Recovers MP fast]
### NIX 				- 3 Armor, 1 Speed [TANK - Slow, Sturdy, Natural Defender, Anchor]
### NIX 				- 2 Speed, 2 Armor [BALANCED - Well-rounded, Somewhat Agile, Average strength, Average Recovery times]
### NIX 	-  Melee Martial Arts
### NIX 			- Submission Grappling
### NIX 				- Skills
### NIX 					- Standing Attacks
### NIX 						- Hip throw [ slams enemy to the ground ]
### NIX 							* -1 HP
### NIX 							* Cause enemy to be grounded
### NIX 						- Pull guard [ pulls enemy to ground. riskier, but if successful can transition to grounded attacks immediately ]
### NIX 							* -0 HP
### NIX 							* On success: Cause enemy to be grounded, open to grounded attacks
### NIX 							* On error: enemy is standing and you are grounded and vulnerable
### NIX 								* Must standup to use Primary
### NIX 								* Can only use secondary (i.e.: pistol)
### NIX 								* You are now on your back
### NIX 					- Grounded Attacks [ enemy must be thrown to ground ]
### NIX 						- Arm bar [ breaks enemy arm ]
### NIX 							* prevents enemy from using two handed weapon (i.e.: LMG)
### NIX 							* two arm bars leaves enemy completely immobilized
### NIX 								- death stroke
### NIX 						- Knee bar [ breaks enemy knee ]
### NIX 							* -1 dexterity
### NIX 						- R.N.C. [ choke enemy unconscious ]
### NIX 							* Cannot be interrupted (i.e.: by other attacking NPCs)
### NIX 							* must roll 3+ 3 times for success 
### NIX 							* on success: Enemy is unconscious and can be finished off
### NIX 							* on error: enemy has chance of escaping your guard and standing up
### NIX 			- Muay Thai 
### NIX 				- Skills
### NIX 					- Stiff Jab [ stun ]
### NIX 						* -1 constitution
### NIX 					- Teep kick [ chance to knock opponent down ]
### NIX 						* downed enemies are 10% more likely to be fatally shot
### NIX 					- Leg kick [ deteriorates target mobility ]
### NIX 						* -1 dexterity [accumulative]
### NIX 					- Knee stomp [ harder attack on leg dexterity of target ]
### NIX 						* -1 dexterity
### NIX 					- Elbow [ chance to break nose of target not using headgear ]
### NIX 						* +1 bleed
### NIX 			- Eskrima (Stick fighting martial art, not limited to wooden batons, can use lethal bladed weapons)
### NIX 				- Skills
### NIX 					- 
### NIX 
### NIX Proficiencies
### NIX =============
### NIX 	- Description:
### NIX 		All proficiencies are available to all classes. A proficiency is a skill. It can be
### NIX 		trained, but only to a point. In order to develop past the point where training stops
### NIX 		taking effect, the player needs to make practical use of that skill. Using proficiencies 
### NIX 		are the only way to master it.
### NIX 
### NIX 	- Profieciency name: "The Natural"
### NIX 		- Description:
### NIX 			We've all seen them before: the individual that can pick up a new skill, talent, sport
### NIX 			and instantly excel past even the most devoted who have trained their entire lives.
### NIX 			The natural is frustratingly talented, able to pick up any weapon or device and
### NIX 			instantly maximize it's usage.
### NIX 		- The following stats are applied:
### NIX 			- +30% Damage
### NIX 			- +10% Reload Speed
### NIX 			- +10% Critical Chance
### NIX 			- +20% Pistol Whip/Rifle Slap damage
### NIX 
### NIX 	- Proficiency name: "Core Intimidation"
### NIX 		- Description:
### NIX 			Deep inside every human lives a core of panic when the body is under unusual amounts
### NIX 			of stress. The adrenal gland is triggered during "Core Intimidation" and the target
### NIX 			freezes in place-- a second lasting a lifetime. 
### NIX 			- 30% chance to cause target to panic/freeze in terror
### NIX 			- +20% Chance to get reliable intel from interrogation
### NIX 		
### NIX 	- Proficiency name: "Netwide Disassembly"
### NIX 		- 
### NIX 
### NIX Focus:GRENADE LAUNCHER
### NIX 	- Demon
### NIX 
### NIX Examples (source: Ghost Recon Future Soldier)
### NIX  Trigger 
### NIX 
### NIX -- Full Auto Trigger
### NIX -- 3-Round Burst Trigger
### NIX -- Semi-Auto Trigger
### NIX -- Match Trigger
### NIX -- 2-Stage Trigger
### NIX  Magazines 
### NIX 
### NIX -- Armor Piercing Ammo
### NIX -- Buckshot
### NIX -- Dragon's Breath
### NIX -- Drum Magazine
### NIX -- Dual Magazines
### NIX -- Exacto Ammo
### NIX -- Extended Magazine
### NIX -- Incendiary Ammo
### NIX -- Raufoss Ammo
### NIX -- Standard Magazine
### NIX -- Stun Rounds
### NIX 
### NIX Optics
### NIX 
### NIX -- Backscatter Optic
### NIX -- Custom Sniper Optic
### NIX -- High Power Optic
### NIX -- Iron Sights
### NIX -- Magnified HWS
### NIX -- Red Dot
### NIX -- Tac Scope
### NIX -- Thermal Optic
### NIX 
### NIX Muzzle
### NIX 
### NIX -- Compensator
### NIX -- Flash Hider
### NIX -- Standard Muzzle
### NIX -- Suppressor
### NIX 
### NIX Underbarrel
### NIX 
### NIX -- 40MM HEDP Launcher
### NIX -- 40MM Smoke Launcher
### NIX -- 40MM EMP Launcher
### NIX -- Angled Foregrip
### NIX -- Bipod
### NIX -- Bipod Grip
### NIX -- Posted Slide
### NIX -- Rail Cover
### NIX -- UB Shotgun
### NIX -- UB Stun Gun (MP Only) 
### NIX -- Vertical Foregrip
### NIX 
### NIX Stock
### NIX 
### NIX -- Collapsed
### NIX -- Extended
### NIX -- Fixed
### NIX -- Folded
### NIX 
### NIX Side Rail
### NIX 
### NIX -- Aiming Laser (Campaign Only)
### NIX -- OTR Scanner
### NIX -- Rail Cover
### NIX 
### NIX Gas System
### NIX 
### NIX -- Standard Gas System
### NIX -- Over-Gassed
### NIX -- Under-Gassed
### NIX 
### NIX Barrel
### NIX 
### NIX -- Long Barrel
### NIX -- Short Barrel
### NIX -- Standard Barrel
### NIX 
### NIX - MP5
### NIX 	- recommended stats (scraped)
### NIX 		- Damage
### NIX 		- 27 (0-18m)
### NIX 	- 18 (28+m)
### NIX 		-	Suppressed
### NIX 		- 22 (0-18m)
### NIX 	- 15 (28+m)
### NIX 		- Rate of Fire
### NIX 		- 800 RPM
### NIX 		- Mobility
### NIX 		- 50
### NIX 		- Magazine Size
### NIX 		- 30+1
### NIX 
### NIX - FNP-P9
### NIX 	- recommended stats (scraped)
### NIX 		- Damage
### NIX 		- 45 DPS at (0-12m)
### NIX 	- 27 DPS at (22+m)
### NIX 		- Suppressed
### NIX 		- 38 DPS at (0-12m)
### NIX 	- 23 DPS at (22+m)
### NIX 		- Rate of Fire
### NIX 		- ~550 RPM
### NIX 		- Mobility
### NIX 		- 50
### NIX 		- Magazine Size
### NIX 		- 16+1
### NIX 
### NIX #### HALF-BAKED IDEAS ####
### NIX #### HALF-BAKED IDEAS ####
### NIX #### HALF-BAKED IDEAS ####
### NIX 
### NIX # Sniping from SUPER far away
### NIX - the idea is that a user can snipe a target as long as they
### NIX   have an autonomous drone in that same room. The target room must 
### NIX 	have specific textures in order for this to be possible.
### NIX 	For example, if the target room is a building, it must either
### NIX 	be on the roof or in a room with a window in it. If the target
### NIX 	is in a wide open space like a parking lot, then you simply need
### NIX 	your drone to be at or near the target room.
### NIX 
### NIX 	- The Process
### NIX 		- deploy drone
### NIX 		- n, n, n, n, e, n, e, e, n
### NIX 		- look
### NIX 			- displays room
### NIX 			- displays mobs in room
### NIX 				- "A pissed of Los Employee is standing here."
### NIX 		- target los via drone
### NIX 		* the game takes a number of ticks in order for the drone to lock on *
### NIX 		* drone finishes locking on target. can now engage *
### NIX 		- snipe target
### NIX 			- this will *always* be "snipe target" in order for this mechanic to work
### NIX 		- NPC reacts to getting shot
### NIX 			- NPC can snipe back. must roll to see if sees attacker
### NIX 
### NIX 	- [#########################################] -
### NIX 	- [ GUNSLINGER ARCHETYPES                   ] -
### NIX 	- [#########################################] -
### NIX 	- PISTOL SPECIALIST [specialist]
### NIX 		- can dual wield pistols. critical/headshot chance increased
### NIX 		- Dual Desert Eagle
### NIX 
### NIX 	- [#########################################] -
### NIX 	- [ PSYCHIC ARCHETYPES                      ] -
### NIX 	- [#########################################] -
### NIX 	- INTERCEPT [psychic]
### NIX 		- can remotely control NPCs for a limited amount of time
### NIX 		- Vector
### NIX 
### NIX ##########################
## HALF BAKED IDEAS END ##
## HALF BAKED IDEAS END ##
## HALF BAKED IDEAS END ##
##########################
