# Alpha goals checklist

The mud is in a very close state to having an alpha testing environment. Documented here are
a checklist of items neeeded before a successfull alpha could be deployed.

# component: scripted sequences
	- status:
		- good to go

# component: builder system
	# sub-component: room builder
		- status: good
	# sub-component: zone builder
		- status: good
	# sub-component: contract builder
		- status: good
	# sub-component: scripted sequences builder
		- status: good
	# sub-component: yaml file import
		- status: solid
	# sub-component: mob equipment builder
		- status: solid
	# sub-component: mob builder
		- status: solid

# component: rifle attachment system
	- status: solid




# unfinished components

# component: deep object parser (rifle attachments)
	- status: solid

# component: melee combat
	- status: 
		- not ready, needs testing and to honor object properties
			- most of this is coded aready, it just needs to be verified
	- todo:
		- verify melee weapons work in combat

# component: procedurally generated raid houses
	- status:
		- components in place but still way too chaotic, needs work

# component: practice system
	- status:
		- needs verification
	- todo:
		- stats affect the user in predictable ways
			- strength
				- affects grappling ability
				- affects flee possibility
			- constitution
				- affects movement points
				- affects resistance to elemental damage
				- adds N.X (float) percent to resistances
			- dexterity
				- affects possibility of doubling up melee weapon attacks
				- affects successful chance of fleeing melee combat
				- affects possibility of dodging the following:
					- melee attacks
					- claymore damage
					- flashbang
					- chance of being tackled
				- affects possibility of not being detected by cameras
			- intelligence
				- affects hacking ability
				- gives N.X (float) additional practice sessions
				- 
			- electronics
				- decreases install time of:
					- cameras
				- increases room distance scanning of cameras by N.X(float)
			- chemistry
				- increases likelihood of finding rare loot
				- increases effectiveness of:
					- steroid usage
					- hgh usage
					- medkit usage
				- increases likelihood of healing to:
					- remove corrossive damage altogether
				- adds N.X (float) bonus corrosive damage to weapons
				- adds N.X (float) bonus radioactive damage to weapons
			- marksmanship
				- increases critical chance
				- increases critical damage
				- increases target limb success chance
			- sniping
				- all nerfs/buffs below apply ONLY to sniper rifles
					- gives N.X (float) additional free sniper ammo per level
					- increases penetration damage
						- likelihood to hit more than one npc in a room
					- increases likelihood of ignoring armor
					- gives N.X (float) 
			- weapon handling
				- decreases reload time ticks
				- increases likelihood of having first strike
				- decreases weapon swap ticks
				- decreases ticks between shots
				- bonus N.X (float) percent elemental damage
				- decreases likelihood of durability loss
			- demolitions
				- increases damage done with:
					- fragmentation grenades
					- incendiary grenades
					- claymore mines
					- underbarrels
						- fragmentation
						- incendiary
				- increases N.X (float)  distance of thrown grenades
				- increases N.X (float)  distance of thrown incendiary grenades
			- armor
				- increases likelihood of stopping penetrating damage
				- decrease N.X (float) damage from same room damage
				- increases likelihood of absorbing critical damage
					- N.X (float) percentage of critical damage absorbed by piece of armor
						- scales per level
				- increases likelihood of deflecting shrapnel to attacker
				- decreases continuous elemental damage duration
				- chance of hit bullets becoming ammunition
			- medical
				- grants N.X (float) healing shot
				- decreases revive ticks
				- adds N.X (float) bonus hp points to healed opponents
				- adds N.X (float) bonus hp points to self-heaing 
				- increases chance to injure opponents

			- charisma
				- undecided
			- strategy
				- undecided
			- wisdom
				- undecided
	
# component: contracts (quests)
	- status:
		- not all contract events are implemented
	- todo:
		- see: player.cpp for contract events that haven't been implemented yet

# component: randomized loot drops
	- status:
		- randomized loot can be generated but is not dropped when an NPC dies

