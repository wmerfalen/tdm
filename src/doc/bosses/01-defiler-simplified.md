# The Defiler
- Allied Food Market murderer
- Wields:
	- SCAR-H plus incendiary rounds
	- Shock Naginata
- Behaviour:
	- Is a stationary target
	- Snipes from his room
	- If gets sniped, phases out of room and into a random one
	- Ensnares victims using demonic incantations




# Quests revolving around blueprints

Brainstorm some in-game assets related to the Defiler theme:

# Incendiary magazines
- Assault Rifle mags

# Parts of a customized G36C that deals anti-matter damage
## Part List
	- rifle/impact-modded-g36c.yml
		- modified to do anti-matter damage
	- attachment/impact-modded-grip.yml
		- added accuracy points
		- added recoil reduction
	- attachment/westcorp-acog-scope.yml
		- 2.0 zoom magnification
  - attachment/anti-matter-barrel.yml


# Blueprints that depend on code changes:
--- [   S T A R T   ] ---

# Rail-Modded SASG-12 blueprint
	- rifle/rail-modded-sasg12.yml
		- each shot builds a rail shot
		- when your rail shot is ready, a message stating so appears:
		--- without the rail message: ---
		sniper::241H 436M 705V [3/12]> 

		--- with the rail message ---
		sniper::241H 436M 705V [3/12] [RAIL]> 

		- to use the RAIL, you use the `snipe` command:

		sniper::241H 436M 705V [3/12] [RAIL]> snipe shotgunner east

		--- battle damage messages go here ---

		sniper::241H 436M 705V [3/12]> 

			- rail shots do these types of damages:
				- anti-matter
				- shock
				- emp

--- [   E N D   ] ---
