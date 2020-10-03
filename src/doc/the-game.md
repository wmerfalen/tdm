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
- remote view cameras

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
	- GHOST [illusionist]
		- can apply stealth to self and WGE
		- H&K G36C Assault Rifle
	- SNIPER [assassin]
		- can snipe from long distances
		- PSG-1 Sniper Rifle

	- [#########################################] -
	- [ ELEMENTALIST ARCHETYPES                 ] -
	- [#########################################] -
	- PYREXIA [fire elementalist]
		- can apply fire to WGE
		- SASG-12 Shotgun
	- MALODY [poison elementalist]
		- can apply poison to WGE
		- MP5 Sub-Machine Gun

	- [#########################################] -
	- [ WARRIOR ARCHETYPES                      ] -
	- [#########################################] -
	- TANK [warrior]
		- can deploy shield
		- P90 Sub-Machine Gun

	- [#########################################] -
	- [ GUNSLINGER ARCHETYPES                   ] -
	- [#########################################] -
	- PISTOL SPECIALIST [specialist]
		- can dual wield pistols. critical/headshot chance increased
		- Dual Desert Eagle

	- [#########################################] -
	- [ ENGINEER ARCHETYPES                     ] -
	- [#########################################] -
	- FORGE [blacksmith]
		- can create/modify/combine WGE
		- VARIED

	- [#########################################] -
	- [ PSYCHIC ARCHETYPES                      ] -
	- [#########################################] -
	- INTERCEPT [psychic]
		- can remotely control NPCs for a limited amount of time
		- Vector

	- [#########################################] -
	- [ CLERIC ARCHETYPES                       ] -
	- [#########################################] -
	- MEDIC [healer]
		- can heal self and others
		- FMG-9

	- [########################] -
	- [ BEASTMASTER ARCHETYPES ] -
	- [########################] -
	- SYNDROME [ beastmaster ]
		- has flying/ground UAV drone. Can control any other drone
		- [ weapon TBA ]

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
			[ ] cooldown between shots
			[x] critical chance
			[x] critical range
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
			[x] critical_chance
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

+++++++++++++++++++
| Fields of study |
+++++++++++++++++++
		- [Electronics] [ ]
			- Use of cameras
			- Able to disable electronics
			- Adept at electronics damage
			- Can adapt electronics to be resistant to EMP
			- Can adapt electronics to become EMP
			- Can detect electronics
			- Can disrupt cameras
			- Can appear invisible to cameras (feed loop)
			- Can hack servers
			- Can disable electronics protection (electronic locks)
		- [Armor] [ ]
			- Can enhance armor
			- Can penetrate armor with normal ammo
			- Can take armor from NPCs
			- Can mold collected armor
			- Held armor has no weight bearance
			- Heavy armor doesn't affect movement
			- Explosions less likely to injure
			- Chance to injure decreased
		- [Marksmanship] [ ]
			- Chance to hit during spray is increased
			- Chance to headshot is increased
			- Chance to target limb is increased
			- Sniping accuracy increased
		- [Sniping] [ ]
			- Accuracy increased with sniper rifles
			- Exclusive access to advanced sniper rifles
			- Tracking bullets
		- [Demolitions] [ ]
			- Breach charge placement time decreased
			- C4 remote detonation distance increased
			- Grenade throw distance increased
			- Claymore damage increased
			- Thermite breach charge placement time decreased
			- Incendiary grenade damage increased
			- Can craft C4
			- Chance to do 33% more damage with any explosive
		- [Chemistry] [ ]
			- Can create steroids
			- Can create human growth hormone
			- Can create noxious gas grenades
			- Seal off room to make air-tight
			- Can create incendiary ammo
		- [Weapon Handling] [ ]
			- Enhanced familiarity with weapons
			- Increased accuracy with any weapon
			- Decreased cooldown between shots
			- Faster reloading
+++++++++++++++++++++
| WEAPON ATTRIBUTES |
+++++++++++++++++++++
 	- Description: 
		- Attributes for weapons apply to a specific character.
	- Familiarity
		- Increases damage by 4% per level
		- Decreases reload time by 4% per level
		- Increases injure chance by 2% per level
		- Increases critical chance by 2.5% per level
		- Increases limb damage chance by 3% per level
		- Increases headshot chance by 0.5% per level



++++++++++++++++++++
| TYPES OF POISONS |
++++++++++++++++++++
	- Friendly attacks
		- Poisoned NPC's will attack their own side
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
