# boss drop rates
	- Example from Diablo 2 
		- The countess
			25%: nothing
			40%: gold
			20%: item
			10%: 'junk' (eg. arrows)
			5%: ring, amulet, jewel, gem or rune

# loot types
	- COMMON
		- 99% drop rate
	- RARE
		- 94% drop rate
	- EPIC
		- 18.5% drop rate
	- LEGENDARY
		- 7.5% drop rate


# forge engine
	- orm classes `rifle_index` and `armor_index`
	- use ACMD: `add_armor_index` 
		- example: `add_armor_index body ballistic-vest.yml iron-vest.yml python-vest.yml`
	- use ACMD: `add_rifle_index` 
		- example: `add_rifle_index ar g36c.yml scar-heavy.yml`
	- generated types
		- completely wrapped the logic to create a randomly generated rifle of various rifle types
			- `mods::forge_engine::generated_rifle`
			- `mods::forge_engine::generated_armor`
				- needs content
				- `armor_index` table currently empty
	- `mods::loot` namespace
		- source file: mods/loot.cpp
		- exposes central loot reward function
			- `mods::loot::reward_player(player_ptr_t& player);`
		- reward ACMD now available to debug loot system
		- `value_scaler` function now available to debug value scaling
	- value scaler class
		- TODO: Balance `mods::forge_engine::value_scaler()`
			- This is where you'll want to tweak the values for game balance
			- source file: mods/forge-engine/value-scaler.(h|c)pp
	- stats and resistances
		- wearing armor with elemental resistance percents will add to your resistances
		- wearing armor that adds stat bonuses will add to your affect abilities (`aff_abils()`)
	
