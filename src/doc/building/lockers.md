# Weapons lockers

## For the help page
`admin:weapon-locker:help`

## Installing a weapons locker
`admin:weapon-locker:install <quota> <yaml>...[yaml-N]`
- use this command several times to set different quotas on different items
```
admin:weapon-locker:install 2 rifle/psg1.yml
admin:weapon-locker:install 3 rifle/mp5.yml
```

If you specify a quota with several items after it, the quota will apply to all yaml files you specify after it

## Uninstalling
`admin:weapon-locker:uninstall`
- will not only remove the database data, but will also remove the item from the room

## Placing the locker in the room
`admin:weapon-locker:install:container`
- If the weapon locker isn't in the room, this will create one and put it in the room you're in

## Listing db contents
`admin:weapon-locker:list`

## Removing items
`admin:weapon-locker:remove:item <id>...[item-N]`;

## Setting quota ( TODO )
*THIS FUNCTION ISNT IMPLEMENTED*
`admin:weapon-locker:quota <count> [id]...[id-N]`
- If you specify a specific set of ID's, then the quota will be applied to those ID's. 
- If you specify just a count, then the quota will be set for all items in the weapons locker

# Ammo lockers
- same as `admin:weapon-locker:*` just replace `weapon` with `ammo`

# Armor lockers
- same as `admin:weapon-locker:*` just replace `weapon` with `armor`
