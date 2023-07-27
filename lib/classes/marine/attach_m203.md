# Attach grenade launcher

# Ability
Ability allows user to attach a grenade launcher to the underbarrel attachment of the currently wielded weapon.

# Usage
```
marine:attach_m203
```

# Example
## 1) Attaching to an Assault Rifle
```
$> marine:attach_m203
You attach a frag underbarrel to a M16A Assault Rifle
```

## 2) Using it
You'll want to use `marine:fire <direction> <distance>` to fire the frag.
```
$> marine:fire west 2
A fragmentation grenade explodes!
```
## 3) Removing it 
If you run out of grenades, the underbarrel attachment will automatically detach itself. So really, you only have to worry about attaching it and firing it at people. But if you really want to manually remove it, use the following:
```
$> marine:attach_m203
You attach a frag underbarrel to a USMC M16A4 Assault Rifle

$> marine:detach_m203
You detach a frag underbarrel.

$>
```

# See also:
```
marine:detach_203
marine:fire
```
