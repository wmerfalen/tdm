# NEW COMMANDS
`admin:reset:zone`
`admin:uuids`
`admin:list:zone-table`
	- lists zones. if you pass in an argument, it will filter `arg1` using that value
`admin:queue:refresh`
	- queue refresh of mobs/objects/zones
`mbuild giveme:next_vnum`
`mbuild giveme:free_vnum`
`rbuild giveme:next_vnum`
`admin:yaml:re-index`
	- reads rifle, armor, explosive yaml directories and categorized them
		- created objects are destructed once used










# LOOT MECHANICS

## Preliminary loot logic [UNFINISHED]

### New features for drops
	- see `mods/drops.cpp`
	- Common rifles, armor, explosives are rolled on each npc death
	- some hard-coded drops for specific NPC's that relate to contracts that they're a part of
		- i.e.: drones carry claymores and frags

Plans to change the rarity/OP-ness of loot are underway. Currently, there are a few functions which are just
calling the same ordinary loot function. We need to fill these functions in and adjust the tier of loot that
is rewarded.
	[ ] Determine how to generate the following tiers:
		[ ] tier 1
		[ ] tier 2
		[ ] tier 3













# GAMIFICATION

## Preliminary "achievements" system [UNFINISHED]
We are starting an achivements system. The code refers to these as 'notch'es. The term 'notch' i used because
it's like when someone tallies something on a piece of paper by notching vertical lines. 
	- see: `mods/orm/notch.*p`
	- see: `mods/notch.*p`
	- see: `mods/loot.cpp` (calls the notch functions)











# ORM IMPROVEMENTS

## Easier ORM code generation
A series of Boost PreProcessor functions have been created to make orm code generation
a lot more simpler and less verbose.
	- Example of this is in `mods/orm/notch.*p`

## ORM helper functions
Linking to a player by passing in the column name as a string:
```
		MENTOC_ORM_LINK_TO_PLAYER_USING("n_player_id");
```

Deleting and loading by player id:
```
		MENTOC_ORM_DELETE_BY_PLAYER(notch);
		MENTOC_ORM_LOAD_BY_PLAYER(notch);
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
```

## Loading by player id and other columns
```
		auto load_by_player_and_notch(const uint64_t& player_id,std::string_view notch_name) {
			auto s = mods::orm::util::load_by_player_and<notch,sql_compositor>(this, {{"n_name",notch_name.data()}});
			return s;
		}
```










# BUILDER TOOLS

## `NODROP` NPC flag
A `NODROP` flag has been added for NPC's. Currently, the Orthos Spawn Sentinel has this flag set. Any NPC
that has `NODROP` as one of their `action` flags will not drop loot into their corpse when they die.
This is to deincentivize players from killing high level guard NPC's... especially where in areas where
combat usually is frowned upon (like near the recall).

An example of this:
```
	action: "ISNPC NODROP",
```

## Easier way to get a non-used vnum
`mbuild giveme:next_vnum`
	- will give you the next available vnum by running sql to get the value of `max(virtual_number) + 1`
`mbuild giveme:free_vnum`
	- will give you an unused vnum between the value of zero and the value of `mbuild giveme:next_vnum`
		- if there isn't an unused value between zero and `mbuild giveme:next_vnum` then this function is
		equivalent to running `mbuild giveme:next_vnum`
`rbuild giveme:next_vnum`
	- grab next available room vnum











# UI IMPROVEMENTS

## Better stats page
Typing `stats <WEAPON>` now gives you better output.
This same functionality is used for the `do_score` command (in some parts).









# CONTENT

## RAID content (alpha)
Dynamically created mobs that are scaled according to a specific level
	- doc/building/2022-quarter-1-feature-01-raids-version1.md

## Orthos Spawn Sentinels
A beefy NPC that chills near the spawn.
	- plans are for preventing any hostile activity from making it into the spawn

## Fixing problematic or immersion-breaking content
The main cobalt FOB content had lots of immersion-breaking content. Descriptions and
titles of rooms have been fixed. 

## Cobalt FOB inventory room/NPCs
A new inventory room has been created. As of 2022-05-29, this content is unfinished.
	[ ] Plans are to make this a shop where MP can be spent to upgrade gear

## Hydro Processing plant
A new area has been started underneath the FOB. Go south, then west, and take your first
down direction. It should lead way underground.
	[ ] Need to populate this place with dynamic mobs
	- would like to Drop copper ore from NPC's randomly
		- brainstorm: trading in/using copper ore to enhance weapons/armor?
	
## Fountain drones refinished
The level 2 drones near the Henley Fountain have been fixed and they now spawn there.
	[ ] Need to do the same thing for the euclid avenue areas south of Henley Fountain



