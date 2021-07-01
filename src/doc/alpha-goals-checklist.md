# Alpha goals checklist

The mud is in a very close state to having an alpha testing environment. Documented here are
a checklist of items neeeded before a successfull alpha could be deployed.

# component: regen hp when sleeping/resting
	- status:
		- not done
			- [ ] when user is sleeping/resting regenerate hp at a slow pace

# component: melee combat
	- status: 
		- not ready, needs testing and to honor object properties
			- most of this is coded aready, it just needs to be verified
			- [x] verify i can attack with melee weapons
			- [ ] verify attacks honor elemental damage percents
			- [x] melee weapons can be picked up/dropped/junk'd
			- [ ] melee weapons can only be equipped in primary position
			- [ ] melee weapons take precedence over secondary weapons
			- [x] melee weapon elemental damage can be blocked/resisted by armor
				- [x] melee elemental damage respects resistances of target's armor
			- [x] melee weapon attacks don't need to be spammed like snipe command does

# component: sniper class
	- status:
		- unfinished, may need testing as well
		- [ ]

# component: practice and leveling up
	- status:
		- needs work. stats affect the user in predictable ways
			- strength
				- [x] increases damage done with melee weapons
				- [ ] increases likelihood of double attacks of same-room weapon and automatic melee attacks
				- [ ] increases chance of penetration damage with melee weapons
				- [ ] decreases mobility penalty that come from carry/wield weights
			- constitution (toughness)
				- [ ] affects movement points
				- [ ] affects resistance to elemental damage
				- [x] adds N.X (float) percent to resistances
				- [x] adds resistance to getting injured
			- dexterity (felxibility/fastness)
				- [ ] affects possibility of doubling up melee weapon attacks
				- [ ] affects successful chance of fleeing melee combat
				- [ ] affects possibility of dodging the following:
					- [ ] melee attacks
					- [ ] claymore damage
					- [ ] flashbang
					- [ ] chance of being tackled
				- [ ] affects possibility of not being detected by cameras
			- intelligence
				- [ ] affects hacking ability
				- [ ] gives N.X (float) additional practice sessions
				- [ ] decreases installation ticks
				- [ ] increases drone scan distance
				- [ ] increases likelihood of drone going unnoticed
			- electronics
				- [ ] decreases install time of:
					- [ ] cameras
				- [ ] increases room distance scanning of cameras by N.X(float)
			- chemistry
				- [ ] increases likelihood of finding rare loot
				- [ ] increases effectiveness of:
					- [ ] steroid usage
					- [ ] hgh usage
					- [ ] medkit usage
				- [ ] increases likelihood of healing to:
					- [ ] remove corrossive damage altogether
				- [ ] adds N.X (float) bonus corrosive damage to weapons
				- [ ] adds N.X (float) bonus radioactive damage to weapons
			- [ ] marksmanship
				- [ ] increases critical chance
				- [ ] increases critical damage
				- [ ] increases target limb success chance
			- [ ] sniping
				- [ ] all nerfs/buffs below apply ONLY to sniper rifles
					- [ ] gives N.X (float) additional free sniper ammo per level
					- [ ] increases penetration damage
						- [ ] likelihood to hit more than one npc in a room
					- [ ] increases likelihood of ignoring armor
					- [ ] gives N.X (float) 
			- [ ] weapon handling
				- [ ] decreases reload time ticks
				- [ ] increases likelihood of having first strike
				- [ ] decreases weapon swap ticks
				- [ ] decreases ticks between shots
				- [ ] bonus N.X (float) percent elemental damage
				- [ ] decreases likelihood of durability loss
			- [ ] demolitions
				- [ ] increases damage done with:
					- [ ] fragmentation grenades
					- [ ] incendiary grenades
					- [ ] claymore mines
					- [ ] underbarrels
						- [ ] fragmentation
						- [ ] incendiary
				- [ ] increases N.X (float)  distance of thrown grenades
				- [ ] increases N.X (float)  distance of thrown incendiary grenades
			- [ ] armor
				- [ ] increases likelihood of stopping penetrating damage
				- [ ] decrease N.X (float) damage from same room damage
				- [ ] increases likelihood of absorbing critical damage
					- [ ] N.X (float) percentage of critical damage absorbed by piece of armor
						- [ ] scales per level
				- [ ] increases likelihood of deflecting shrapnel to attacker
				- [ ] decreases continuous elemental damage duration
				- [ ] reduces carry weight of armor
				- [ ] decreases negative impact of armor on mobility
				- [ ] increases chance of armor absorbing more damage than player does when:
					- [ ] claymore explosion damage dealt
					- [ ] fragmentation explosion
					- [ ] fragmentation shrapnel
				- [ ] [group dynamics]
					- [ ] chance of blocking shrapnel from hurting team members
						- [ ] requires [take lead]
			- [ ] medical
				- [ ] grants N.X (float) healing shot
				- [ ] decreases revive ticks
				- [ ] adds N.X (float) bonus hp points to healed opponents
				- [ ] adds N.X (float) bonus hp points to self-heaing 
				- [ ] increases chance to injure opponents
	
# component: contracts (quests)
	- status:
		- not all contract events are implemented
			- [ ]	void player::contract_destroy_item(obj_ptr_t& object);
			- [ ]	void player::contract_destroyed_door(const room_rnum& room_id,const int8_t& direction);
			- [ ]	void player::contract_quota_kill_mob_increase(player_ptr_t& mob);
			- [ ]	void player::contract_quota_destroyed_door(const room_rnum& room_id,const int8_t& direction);
			- [ ]	void player::contract_kill_mob(player_ptr_t& mob);
			- [ ]	void player::contract_talk_to(player_ptr_t& mob);

# component: randomized loot drops
	- status:
		- randomized loot can be generated but is not dropped when an NPC dies
		- [ ] when npc dies, run forge engine and drop a randomized loot inside the corpse

# component: 
```
SKILL_GHOST_DRONE_SCAN,
SKILL_GHOST_STEALTH,
SKILL_GHOST_SUMMON_EXTRACTION,
SKILL_GHOST_XRAY_SHOT,
SKILL_GHOST_FEIGN_DEATH,
SKILL_GHOST_PLANT_CLAYMORE,
SKILL_GHOST_PENETRATING_SHOT,
SKILL_GHOST_INTIMIDATION,
SKILL_GHOST_CRYOGENIC_GRENADE,
SKILL_GHOST_FLASH_UNDERBARREL,
SKILL_SNIPER_TRACKING_SHOT,
SKILL_SNIPER_SUMMON_UNDERBARREL_SHOTGUN,
SKILL_SNIPER_SUMMON_UNDERBARREL_GRENADE_LAUNCHER,
SKILL_SNIPER_GUIDED_MISSILE,
SKILL_SNIPER_TARGET_LIMB,
SKILL_SNIPER_PLANT_CLAYMORE,
SKILL_SNIPER_XRAY_SHOT,
SKILL_SNIPER_SUMMON_AREA_SCAN,
SKILL_GRENADE_USAGE,
SKILL_CLAYMORE_USAGE,
SKILL_BREACH_USAGE,
SKILL_CAMERA_PLACEMENT,
SKILL_DETECT_CAMERAS,
SKILL_CAMERA_DESTRUCTION,
SKILL_ELECTRIFIED_DAMAGE,
SKILL_PROTECT_FROM_EMP,
SKILL_CREATE_EMP_WEAPONS,
SKILL_COUNTER_SHOCK,
SKILL_TOGGLE_THERMAL,
SKILL_TOGGLE_NIGHT_VISION,
SKILL_HEADGEAR_INTROSPECTION,
SKILL_PROXIMITY_ALARM,
SKILL_DEPLOYABLE_TURRET,
SKILL_DEPLOYABLE_FIRE_TURRET,
SKILL_DOOR_CAMERA,
SKILL_BASIC_ARMOR,
SKILL_ADVANCED_ARMOR,
SKILL_ELITE_ARMOR,
SKILL_THIEF,
SKILL_MOLD,
SKILL_NO_FACTOR_BASIC,
SKILL_NO_FACTOR_ADVANCED,
SKILL_NO_FACTOR_ELITE,
SKILL_EXPLOSIVE_RESISTANCE,
SKILL_INJURE_RESISTANCE,
SKILL_MUNITIONS_REFLECTOR,
SKILL_SENTINEL_DISCIPLINE,
SKILL_DEPLOYABLE_SHIELD,
SKILL_SPRAY_CHANCE,
SKILL_HEADSHOT_CHANCE,
SKILL_LIMB_CHANCE,
SKILL_BETTER_SNIPING_ACCURACY,
SKILL_TARGET_LIMB,
SKILL_ACCURACY,
SKILL_VIP,
SKILL_TRACKER,
SKILL_QUIETER_SNIPER_SHOTS,
SKILL_FASTER_BREACHES,
SKILL_FURTHER_C4S,
SKILL_STRONGER_FRAG_ARM,
SKILL_BIGGER_CLAYMORES,
SKILL_FASTER_THERMITES,
SKILL_BIGGER_FIRE_NADES,
SKILL_CRAFTY_C4,
SKILL_BETTER_EXPLOSIONS,
SKILL_RETROFIT_GRENADE_LAUNCHER,
SKILL_RETROFIT_FIRE_GRENADE_LAUNCHER,
SKILL_RETROFIT_SMOKE_GRENADE_LAUNCHER,
SKILL_RETROFIT_SENSOR_GRENADE_LAUNCHER,
SKILL_RETROFIT_STUN_GRENADE_LAUNCHER,
SKILL_STEROID_DEALER,
SKILL_HGH_DEALER,
SKILL_NOXIOUS_NADE_CRAFTER,
SKILL_DUTCH_OVEN,
SKILL_FIRE_NADE_DEALER,
SKILL_PRO_WEAPON_HANDLER,
SKILL_BETTER_WEAPON_ACCURACY,
SKILL_FASTER_TRIGGER_FINGER,
SKILL_FASTER_RELOADING,
SKILL_BASIC_PISTOL,
SKILL_BASIC_ASSAULT_RIFLE,
SKILL_BASIC_SHOTGUN,
SKILL_BASIC_SNIPER_RIFLE,
SKILL_BASIC_SUB_MACHINE_GUN,
SKILL_INTERMEDIATE_PISTOL,
SKILL_INTERMEDIATE_ASSAULT_RIFLE,
SKILL_INTERMEDIATE_SHOTGUN,
SKILL_INTERMEDIATE_SNIPER_RIFLE,
SKILL_INTERMEDIATE_SUB_MACHINE_GUN,
SKILL_ADVANCED_PISTOL,
SKILL_ADVANCED_ASSAULT_RIFLE,
SKILL_ADVANCED_SHOTGUN,
SKILL_ADVANCED_SNIPER_RIFLE,
SKILL_ADVANCED_SUB_MACHINE_GUN,
SKILL_EXPERT_PISTOL,
SKILL_EXPERT_ASSAULT_RIFLE,
SKILL_EXPERT_SHOTGUN,
SKILL_EXPERT_SNIPER_RIFLE,
SKILL_EXPERT_SUB_MACHINE_GUN,
SKILL_ARMOR_PENETRATION_SHOT,
SKILL_NONSTOP_PENETRATION_SHOT,
SKILL_INCREASED_INJURE_CHANCE,
SKILL_ASSAULT_RIFLE_SHRAPNEL,
SKILL_ENTRY_DENIAL,
SKILL_MISDIRECTION,
SKILL_SNIPER_SUPPORT,
SKILL_REDUCED_DETECTION_CHANCE,
SKILL_INCREASED_AWARENESS,
SKILL_TRICK_MAGAZINE,
SKILL_CHAINABLE_BREACHING,
SKILL_HGH_MORE_POWERFUL,
SKILL_BASIC_HP_RECOVERY,
SKILL_PARASITIC_HP_RECOVERY,
SKILL_SUTURE,
SKILL_ADRENALINE_BOOST,
SKILL_PYREXIA_THROW_FIRE_NADE,
SKILL_PYREXIA_DRENCH_IN_GASOLINE,
SKILL_PYREXIA_DRENCH_ROOM_IN_GASOLINE,
SKILL_PYREXIA_CONVERT_TO_FIRE_NADE,
SKILL_PYREXIA_ATTACH_INC_LAUNCHER,
SKILL_PYREXIA_ATTACH_FLAMETHROWER,
SKILL_PYREXIA_CREATE_WALL_OF_FIRE,
SKILL_PYREXIA_SEAL_OFF_ROOM,
```


# component: passive skillsets
```
SKILL_DETECT_CAMERAS,50
SKILL_ELECTRIFIED_DAMAGE,100
SKILL_PROTECT_FROM_EMP,200
SKILL_COUNTER_SHOCK,4
SKILL_HEADGEAR_INTROSPECTION,3050
SKILL_PROXIMITY_ALARM,3050
SKILL_NO_FACTOR_BASIC,400
SKILL_NO_FACTOR_ADVANCED,850
SKILL_NO_FACTOR_ELITE,900
SKILL_EXPLOSIVE_RESISTANCE,2050
SKILL_INJURE_RESISTANCE,3050
SKILL_MUNITIONS_REFLECTOR,4
SKILL_SENTINEL_DISCIPLINE,3
SKILL_SPRAY_CHANCE,150
SKILL_HEADSHOT_CHANCE,250
SKILL_LIMB_CHANCE,350
SKILL_BETTER_SNIPING_ACCURACY,450
SKILL_ACCURACY,150
SKILL_VIP,250
SKILL_TRACKER,350
SKILL_FASTER_BREACHES,50
SKILL_FURTHER_C4S,4
SKILL_STRONGER_FRAG_ARM,50
SKILL_BIGGER_CLAYMORES,50
SKILL_FASTER_THERMITES,50
SKILL_BIGGER_FIRE_NADES,50
SKILL_BETTER_EXPLOSIONS,33
SKILL_STEROID_DEALER,50
SKILL_HGH_DEALER,50
SKILL_DUTCH_OVEN,50
SKILL_FIRE_NADE_DEALER,50
SKILL_PRO_WEAPON_HANDLER,50
SKILL_BETTER_WEAPON_ACCURACY,50
SKILL_FASTER_TRIGGER_FINGER,50
SKILL_FASTER_RELOADING,50
SKILL_ARMOR_PENETRATION_SHOT,150
SKILL_INCREASED_INJURE_CHANCE,150
SKILL_ASSAULT_RIFLE_SHRAPNEL,150
SKILL_ENTRY_DENIAL,2050
SKILL_MISDIRECTION,2050
SKILL_REDUCED_DETECTION_CHANCE,4
SKILL_INCREASED_AWARENESS,2050
SKILL_TRICK_MAGAZINE,2050
SKILL_HGH_MORE_POWERFUL,2050
SKILL_BASIC_HP_RECOVERY,2050
SKILL_PARASITIC_HP_RECOVERY,2050
SKILL_SUTURE,2050
SKILL_ADRENALINE_BOOST,2050
```
