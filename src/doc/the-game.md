# Siege MUD
Siege MUD is a text-based looter shooter set in a world similar to modern Earth. 
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
- plastic explosives
- grenades
	- flash-bang
	- smoke
	- incendiary
	- frag
	- EMP
	- Chaff
- breach charges
- chemical weapons
- drones
	- aerial drones
	- land-based drones
- claymore mines

# 

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
			[ ] chance to injure
			[ ] cooldown between shots
			[x] critical chance
			[ ] critical range
			[ ] disorient amount
			[ ] headshot bonus
			[x] max range
			[ ] reload time
			[ ] effective firing range
			[x] damage dice count
			[x] damage dice sides
	- CONSUMBALE
		- AMMUNITION 
			[ ] done
		- FOOD 
			[ ] done
		- PILLS 
			[ ] done
		- HERBS 
			[ ] done
		- HALLUCINOGENS 
			[ ] done
		- PEDS 
			[ ] done
		- GASOLINE
			[ ] done
		- attribute support
			[ ] weight_in_lbs
			[ ] consumed_by
			[ ] adds_hp
			[ ] adds_movement
			[ ] adds_room_range
			[ ] adds_strength
			[ ] adds_intelligence
			[ ] adds_charisma
			[ ] adds_dexterity
			[ ] adds_constitution
			[ ] adds_wisdom
			[ ] adds_weight
			[ ] adds_experience
			[ ] adds_armor_class
			[ ] adds_hitroll
			[ ] adds_damage_roll
			[ ] ticks_until_zero
			[ ] ammo_type
			[ ] capacity
			[ ] adds_fire_damage
			[ ] adds_critical_chance
			[ ] adds_ammo_max
			[ ] adds_chance_to_injure
			[ ] adds_clip_size
			[ ] adds_cooldown_between_shots
			[ ] adds_critical_range
			[ ] adds_disorient_amount
			[ ] adds_headshot_bonus
			[ ] adds_max_range
			[ ] adds_reload_time
			[ ] adds_muzzle_velocity
			[ ] adds_damage_dice_count
			[ ] adds_damage_dice_sides
	- EXPLOSIVE
		- FRAG GRENADE 
			[x] done
		- INCENDIARY GRENADE 
			[x] done
		- REMOTE EXPLOSIVE 
			[ ] done
		- REMOTE CHEMICAL 
			[ ] done
		- EMP GRENADE 
			[ ] done
		- CLAYMORE MINE 
			[ ] done
		- SMOKE GRENADE 
			[ ] done
		- C4 
			[ ] done
		- BREACH CHARGE 
			[ ] done
		- THERMITE CHARGE 
			[ ] done
		- FLASHBANG GRENADE 
			[x] done
		- SENSOR GRENADE 
			[x] done
		- attributes
			[ ] alternate_explosion_type
			[x] chance_to_injure
				[x] revive once injured
				[x] calculate from rifle attack
			[ ] critical_chance
			[ ] critical_range
			[ ] blast_radius
			[ ] damage_per_second
			[ ] disorient_amount
			[ ] range_modifier
			[ ] loudness_type
			
	- GADGET
		- GRAPPLING HOOK 
			[ ] done
		- BARBED WIRE 
			[ ] done
		- CAMERA 
			[x] done
		- THERMAL CAMERA 
			[x] done
		- NIGHT VISION CAMERA 
			[x] done
		- MOTION SENSOR 
			[ ] done
		- DEPLOYABLE SHIELD 
			[ ] done
		- TRIPWIRE 
			[ ] done
		- attributes
			[ ] csv_capabilities
			[ ] csv_attach_to
			[ ] electronic
			[ ] health_points
			[ ] shield_points
			[ ] durability_profile
			[ ] move_points
			[ ] damage_points
	- ATTACHMENT
		- SIGHT
			[ ] done
		- MUZZLE
			[ ] done
		- MAGAZINE 
			[ ] done
		- attributes
			[ ] zoom_multiplier
			[ ] ammunition_amount
			[ ] silence_percent
			[ ] durability_points
			[ ] incendiary_damage
			[ ] radiation_damage
			[ ] poison_damage
			[ ] electric_damage
			[ ] armor_penetration_amount
			[ ] disorient_amount
			[ ] emp_amount
			[ ] emp_radius
			[ ] attach_to


+++++++++++
| CLASSES |
+++++++++++
	- Sentinel
	- Daemon
	- Pistolita
		- Excellent pistol handling
		- Psychic Ability: 
	- Tank [ TODO: rename this class ]
	- [theme:The Good Guy, natural leader] NAME TBA [ TODO ]
	- Ethereal Zero
	- Catalyst
	- Zenith
	- MK-X
	- Syndrome
	- Malady
	- Contagion
		- 
	- 
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


++++++++++++++++++++
| TYPES OF POISONS |
++++++++++++++++++++
	- Friendly attacks
		- Poisoned NPC's will attack their own side
	- Re-animate
		- Necromancer skill to bring corpse back from the dead
	- Acidic blood
		- Poisoned NPC will bleed acid
	- Explode
		- Poisoned NPC will combust

	- Triad Allocation
			1) Each triad has 3 traits
			2) It is not possible to fill up all 3 allocations with 3 points
			3) There are only 6 points available to spread over all allocations
			4) There can only be one allocation that has 3 points. 
			5) It is not possible to have 2 allocations with 3 points.
		- Military Triad
			- [Melee]   [x] [ ] [ ]
			- [Weapons] [x] [x] [x]
			- [Intel]   [x] [x] [ ]
		- Speed/Armor tradeoff MAX 4 points
			- [Speed]   [ ] [ ] [ ]
			- [Armor]   [ ] [ ] [ ]
			- Examples: 
				- 3 Speed, 1 Armor [HYPER - Agile, Stealthy, Quiet, Fast, Lots of Movement Points, Recovers MP fast]
				- 3 Armor, 1 Speed [TANK - Slow, Sturdy, Natural Defender, Anchor]
				- 2 Speed, 2 Armor [BALANCED - Well-rounded, Somewhat Agile, Average strength, Average Recovery times]

	-  Melee Martial Arts
			- Submission Grappling
				- Skills
					- Standing Attacks
						- Hip throw [ slams enemy to the ground ]
							* -1 HP
							* Cause enemy to be grounded
						- Pull guard [ pulls enemy to ground. riskier, but if successful can transition to grounded attacks immediately ]
							* -0 HP
							* On success: Cause enemy to be grounded, open to grounded attacks
							* On error: enemy is standing and you are grounded and vulnerable
								* Must standup to use Primary
								* Can only use secondary (i.e.: pistol)
								* You are now on your back
					- Grounded Attacks [ enemy must be thrown to ground ]
						- Arm bar [ breaks enemy arm ]
							* prevents enemy from using two handed weapon (i.e.: LMG)
							* two arm bars leaves enemy completely immobilized
								- death stroke
						- Knee bar [ breaks enemy knee ]
							* -1 dexterity
						- R.N.C. [ choke enemy unconscious ]
							* Cannot be interrupted (i.e.: by other attacking NPCs)
							* must roll 3+ 3 times for success 
							* on success: Enemy is unconscious and can be finished off
							* on error: enemy has chance of escaping your guard and standing up
			- Muay Thai 
				- Skills
					- Stiff Jab [ stun ]
						* -1 constitution
					- Teep kick [ chance to knock opponent down ]
						* downed enemies are 10% more likely to be fatally shot
					- Leg kick [ deteriorates target mobility ]
						* -1 dexterity [accumulative]
					- Knee stomp [ harder attack on leg dexterity of target ]
						* -1 dexterity
					- Elbow [ chance to break nose of target not using headgear ]
						* +1 bleed
			- Eskrima (Stick fighting martial art, not limited to wooden batons, can use lethal bladed weapons)
				- Skills
					- 

	- Class Template
		- Theme: keywords to describe class
		- Loadout:
			- Primary [*] -> CLASS_WEAPON
			- Secondary [*] -> CLASS_WEAPON
		- Psychic Ability:
			- Ability that transcends space/time/physics
		- Global Weapon Ability:
			- Ability that enhances primary or secondary weapon
		- Military Triad [6 points]
			- [Melee]   [ ] [ ] [ ]
			- [Weapons] [ ] [ ] [ ]
			- [Intel]   [ ] [ ] [ ]
		- Speed/Amor tradeoff [4 points]
			- [Speed]   [ ] [ ] [ ]
			- [Armor]   [ ] [ ] [ ] 
		[*] -> CLASS_WEAPON: Cannot be junked/dropped/traded. PC can still use other weapons if so chooses.

  ####   ######  #    #   #####     #    #    #  ######  #
 #       #       ##   #     #       #    ##   #  #       #
  ####   #####   # #  #     #       #    # #  #  #####   #
      #  #       #  # #     #       #    #  # #  #       #
 #    #  #       #   ##     #       #    #   ##  #       #
  ####   ######  #    #     #       #    #    #  ######  ######


  ####   #         ##     ####    ####
 #    #  #        #  #   #       #
 #       #       #    #   ####    ####
 #       #       ######       #       #
 #    #  #       #    #  #    #  #    #
  ####   ######  #    #   ####    ####
	- Sentinel Class
		- Theme: Anchor class, can take punishment, defensive tank
		- Loadout:
			- Primary:
				- MP5 Sub Machine Gun
				- Alternate SPAS-12 Shotgun
			- Secondary:
				- CZ P-10 C
				- file: cz-p10-c.html 
			- Psychic Ability:
				- Intimidate (very effective form of preventing crits from enemies)
				- Bolster Defense
					- 
			- Passive Abilities:
				- Damage resistence for every second guarding a room
			- Global Weapon Ability:
			- Triad Allocation:
				- [Weapons] [x] [x] [x]
				- [Melee]   [x] [x] [ ]
				- [Intel]   [x] [ ] [ ]
			- Play style: imagine the heavy and tall looking knights guarding the gates
			- NOTE: this could be an npc or player class


	 #####   ######  #    #   ####   #    #
	 #    #  #       ##  ##  #    #  ##   #
	 #    #  #####   # ## #  #    #  # #  #
	 #    #  #       #    #  #    #  #  # #
	 #    #  #       #    #  #    #  #   ##
	 #####   ######  #    #   ####   #    #


		####   #         ##     ####    ####
	 #    #  #        #  #   #       #
	 #       #       #    #   ####    ####
	 #       #       ######       #       #
	 #    #  #       #    #  #    #  #    #
		####   ######  #    #   ####    ####

	- DAEMON CLASS
		- Theme: Demonic force, Military Brutality, Hellfire, Incendiary
		- Loadout:
			- Primary:
				- SCAR-H + auxiliary grenade launcher
			- Secondary:
				- Machine Pistol: Uzi or Glock (see: Rainbow Six Vegas 2 glock handgun that is rapid fire)
			- Psychic Ability:
				- Torture (inflict cruel damage on target)
			- Passive Abilities:
				- Kills with Melee weapons gain increased chance of doubling up attacks
			- Global Weapon Ability:
				- Turn primary/secondary ammunition into incendiary
			- Triad Allocation:
				- [Weapons] [x] [x] [x]
				- [Melee]   [x] [x] [ ]
				- [Intel]   [x] [ ] [ ]
			- Play style: does lots of damage, can take decent amount of punishment, quickly get aquainted with new weapons

	- SYNDROME CLASS
		- Theme: High tech user
		- Loadout:
			- Primary:
				- Vector ACP
			- Secondary:
				- CZ P-10 C
				- file: cz-p10-c.html 
			- Psychic Ability:
				- Invisibility
				- Disable gagdets
			- Passive Abilities:
				- Damage resistence for every second guarding a room
			- Global Weapon Ability:
				- Tracking shot (NPCs hit with tracking shot appear on your radar)
			- Triad Allocation:
				- [Weapons] [x] [x] [x]
				- [Melee]   [x] [x] [ ]
				- [Intel]   [x] [ ] [ ]
			- Play style: 

  #################
	### CONTAGION ###
  #################
	- CONTAGION CLASS
		- Theme: Anchor class, can take punishment, defensive tank
		- Loadout:
			- Primary:
				- MP5 Sub Machine Gun
				- Alternate SPAS-12 Shotgun
			- Secondary:
				- CZ P-10 C
				- file: cz-p10-c.html 
			- Psychic Ability:
				- Intimidate (very effective form of preventing crits from enemies)
				- Bolster Defense
					- 
			- Passive Abilities:
				- Damage resistence for every second guarding a room
			- Global Weapon Ability:
			- Triad Allocation:
				- [Weapons] [x] [x] [x]
				- [Melee]   [x] [x] [ ]
				- [Intel]   [x] [ ] [ ]
			- Play style: imagine the heavy and tall looking knights guarding the gates
			- NOTE: this could be an npc or player class
	### NEW CLASS ##
	### NEW CLASS ##
	### NEW CLASS ##


	- TANK CLASS [ RENAME THIS ]
		- Theme: bullet sponge, heavy armour but slow, anchor
		- Loadout:
			- Primary:
				- 
			- Secondary:
				- 
		- Global Weapon Ability:
			- 
		- Triad Allocation:
			- [Weapons] [x] [x] [ ]
			- [Melee]   [x] [x] [x]
			- [Intel]   [x] [ ] [ ]

	- ENGINEER CLASS
		- Theme: gadgets expert, hacking, intelligence is key, drone is ur friend
		- Loadout:
			- Primary:
				- 552 Commando
			- Secondary:
				- 5.7 USG

	- FORGE CLASS
		- Theme:  forged in fire, arms dealer, weapon/armour repair+modification+deconstruction
		- Loadout:
			- Primary:
				- P90
			- Secondary:
				- Glock
			- Psychic Ability:
				- Weapon Link
					- when wielding a weapon that FORGE created get Enhanced Weapon Profiency Bonus
			- Global Weapon Ability
				- Forge random attributes on an existing weapon potentially making it OP
					- Naturally, this is a chaotic ability but it can yield surprising results

	- Sub Focus: {CLASS_NAME} CLASS
		- Theme: 
		- Loadout:
			- Frag/Incendiary Grenade launcher (CLASS OBJECT)
			- Machine Pistol: Uzi or Glock (see: Rainbow Six Vegas 2 glock handgun that is rapid fire)
			- Demon has PSYCHIC ABILITY: Torture (inflict cruel damage on target)
			- Global Weapon Ability: Turn primary/secondary ammunition into incendiary
			- Alignment: Evil
	
Proficiencies
=============
	- Description:
		All proficiencies are available to all classes. A proficiency is a skill. It can be
		trained, but only to a point. In order to develop past the point where training stops
		taking effect, the player needs to make practical use of that skill. Using proficiencies 
		are the only way to master it.

	- Profieciency name: "The Natural"
		- Description:
			We've all seen them before: the individual that can pick up a new skill, talent, sport
			and instantly excel past even the most devoted who have trained their entire lives.
			The natural is frustratingly talented, able to pick up any weapon or device and
			instantly maximize it's usage.
		- The following stats are applied:
			- +30% Damage
			- +10% Reload Speed
			- +10% Critical Chance
			- +20% Pistol Whip/Rifle Slap damage

	- Proficiency name: "Core Intimidation"
		- Description:
			Deep inside every human lives a core of panic when the body is under unusual amounts
			of stress. The adrenal gland is triggered during "Core Intimidation" and the target
			freezes in place-- a second lasting a lifetime. 
			- 30% chance to cause target to panic/freeze in terror
			- +20% Chance to get reliable intel from interrogation
		
	- Proficiency name: "Netwide Disassembly"
		- 

Focus:GRENADE LAUNCHER
	- Demon

Focus:SNIPER CLASS
	-  Create base Sniper Rifle in object system
	-  Make sure if it's a class sniper rifle, that it must be BOUND to it's owner
	-  Sniper Rifle is NON-TRANSFERABLE if it's a CLASS SNIPER RIFLE
	-  Sniper Rifles bought and sold on the "BLACK MARKET" aren't bound to anyone, but aren't as powerful as CLASS SNIPER RIFLES
	-  BONUS: A Sniper Rifle may have different types of modifications, slots, tweaks made to it
		Slots:
			- Scope
		
Sprint 2019-??
Focus:CRAFTING WEAPONS
	-  Create base types for weapons
		- shotgun
		- assault rifle
		- sub-machine gun
		- sniper rifle
		- handgun
		- machine pistol
		- light machine gun
	
	
	-  Create base types for explosives
			- frag grenade
			- incendiary grenade
			- remote detonation
			- mine
			- smoke grenade
			- flash bang
		
	
	-  Create base types for drones
			- grounded drone
			- aerial drone
			- aquatic drone
		

Sprint 2019-??
Focus:CRAFTING GADGETS
	-  Create base types for weapons

Sprint 2019-??
Focus:BASE ATTRIBUTES ALLOCATION
	- Allocate STR/INT/WIS/DEX/CON/CHA for each class
	- 

Sprint 2019-??
Focus:RIFLE CUSTOMIZATIONS

Examples (source: Ghost Recon Future Soldier)
 Trigger 

-- Full Auto Trigger
-- 3-Round Burst Trigger
-- Semi-Auto Trigger
-- Match Trigger
-- 2-Stage Trigger
 Magazines 

-- Armor Piercing Ammo
-- Buckshot
-- Dragon's Breath
-- Drum Magazine
-- Dual Magazines
-- Exacto Ammo
-- Extended Magazine
-- Incendiary Ammo
-- Raufoss Ammo
-- Standard Magazine
-- Stun Rounds

Optics

-- Backscatter Optic
-- Custom Sniper Optic
-- High Power Optic
-- Iron Sights
-- Magnified HWS
-- Red Dot
-- Tac Scope
-- Thermal Optic

Muzzle

-- Compensator
-- Flash Hider
-- Standard Muzzle
-- Suppressor

Underbarrel

-- 40MM HEDP Launcher
-- 40MM Smoke Launcher
-- 40MM EMP Launcher
-- Angled Foregrip
-- Bipod
-- Bipod Grip
-- Posted Slide
-- Rail Cover
-- UB Shotgun
-- UB Stun Gun (MP Only) 
-- Vertical Foregrip

Stock

-- Collapsed
-- Extended
-- Fixed
-- Folded

Side Rail

-- Aiming Laser (Campaign Only)
-- OTR Scanner
-- Rail Cover

Gas System

-- Standard Gas System
-- Over-Gassed
-- Under-Gassed

Barrel

-- Long Barrel
-- Short Barrel
-- Standard Barrel

- MP5
	- recommended stats (scraped)
		- Damage
		- 27 (0-18m)
	- 18 (28+m)
		-	Suppressed
		- 22 (0-18m)
	- 15 (28+m)
		- Rate of Fire
		- 800 RPM
		- Mobility
		- 50
		- Magazine Size
		- 30+1

- FNP-P9
	- recommended stats (scraped)
		- Damage
		- 45 DPS at (0-12m)
	- 27 DPS at (22+m)
		- Suppressed
		- 38 DPS at (0-12m)
	- 23 DPS at (22+m)
		- Rate of Fire
		- ~550 RPM
		- Mobility
		- 50
		- Magazine Size
		- 16+1
