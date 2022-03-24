# Listing Uniques
` js list_op()`

# Instantiating a Unique
` js ira('nickname') `

# Example
- Instantiating the 'DXC DemonLance XIV' (base object PSG-1 sniper rifle)
`
65000H 6500M 65000V > js ira('dlance');
str: psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}
running: "instantiate_rifle_attachment psg1.yml{barrel:anti-matter-barrel.yml,magazine:depleted-uranium-magazine.yml,sight:four-x-sight.yml,stock:printer-stock.yml,under_barrel:taser-underbarrel.yml}"
[+] Done
[+] It is done.
instantiated one for you

65000H 6500M 65000V > 

65000H 6500M 65000V > i
You are carrying:
(2) PSG-1 Sniper Rifle
`

# Functionally unique weapons
As of 2022-03-28, a different form of unique weapons exists. Instead of it being a deep object that is parsed, 
it is instead an object that is backed by the code itself. The code determines what the functionality of the 
weapon is. 

Take, for example, the DST7A shotgun. The code for this weapon exists at `mods/weapons/shotgun-dst7a.cpp`.
The code for this shotgun determines what sounds occur when the weapon is fired.
Also, when the target of the DST7A gets hit with it's munitions, we start a custom
affects dissolver which starts melting the flesh off the target's bones.

Functionally unique weapons allow us to do:
- Add sound effects to different modes of combat
	- i.e.: the pumping of a shotgun's forestock
- Add non-standard affects to different parts of combat
	- i.e.: when targets get hit by munitions, their flesh starts to melt
- Add passive effects that would otherwise be impossible to do with simple yaml files
	- i.e.: for every shot over 300 damage, add a grenade charge. 
	The user of the weapon may then fire said grenade charge.`

## Functionally unique weapons as equipment/inventory items
The following checklist are TODO items since currently the only way the DST7A shotgun can be
spawned is through it's constructor and via the admin command `admin:dst7a`. 

- [x] Award the player with the unique weapon
- [x] Save unique weapon to the DB
- [x] Allow unique weapon to be loaded from the DB
	- [x] The corresponding unique ranged weapon object is instantiated
- [x] Allow unique weapon to be:
	- [x] given
	- [x] taken
	- [x] dropped
	- [x] junked

## Requirements for building new Functionally Unique Weapons
1) Inside this function, you need to add code that detects if it's a unique weapon:
	`mods::globals::register_object()`
2) inside this function you must do whatever is necessary to deallocate the object:
	`mods::weapons::dispose_unique_weapon(const uuid_t& obj_uuid)`
3) for combat, you must add your custom logic to recognize a unique weapon inside this function:
	`mods::weapons::dispatch_unique_ranged_weapon_event(const uuid_t& attacker_uuid, damage_event_t event, player_ptr_t victim)`
4) You must copy the same general flow as an existing unique weapon. Use mods/weapons/shotgun-dst7a.hpp for an example
