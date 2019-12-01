Focus:CLASSES
	- Sub Focus: Triad Allocation
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
	- Sub Focus: Melee Martial Arts
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

	- Sub Focus: DAEMON CLASS
		- Theme: Demonic force, Military Brutality, Hellfire, Incendiary
		- Loadout:
			- Primary:
				- Frag/Incendiary Grenade launcher
			- Secondary:
				- Machine Pistol: Uzi or Glock (see: Rainbow Six Vegas 2 glock handgun that is rapid fire)
			- Psychic Ability:
				- Torture (inflict cruel damage on target)
			- Global Weapon Ability:
				- Turn primary/secondary ammunition into incendiary
			- Triad Allocation:
				- [Weapons] [x] [x] [x]
				- [Melee]   [x] [x] [ ]
				- [Intel]   [x] [ ] [ ]
			- 

	- Sub Focus: {CLASS_NAME} CLASS
		- Theme: 
		- Loadout:
			- Frag/Incendiary Grenade launcher (CLASS OBJECT)
			- Machine Pistol: Uzi or Glock (see: Rainbow Six Vegas 2 glock handgun that is rapid fire)
			- Demon has PSYCHIC ABILITY: Torture (inflict cruel damage on target)
			- Global Weapon Ability: Turn primary/secondary ammunition into incendiary
			- Alignment: Evil
	

	- Sub Focus: 

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

