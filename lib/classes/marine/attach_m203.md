# Attach grenade launcher

# Ability
Ability allows user to attach a grenade launcher to the underbarrel attachment of the currently wielded weapon.

# Usage
`marine:attach_m203`

# Example
## 1) Attaching to an Assault Rifle
```
marine::273H 199M 409V [32/32]> marine:attach_m203
You attach a frag underbarrel to a M16A Assault Rifle
```

## 2) Using it
You'll want to use `marine:fire <direction> <distance>` to fire the frag.
```
marine::273H 199M 409V [32/32]> marine:fire west 2
A fragmentation grenade explodes!

marine::273H 199M 409V [32/32]>
```
## 3) Removing it 
## `marine:detach_m203`
If you run out of grenades, the underbarrel attachment will automatically detach itself. So really, you only have to worry about attaching it and firing it at people. But if you really want to manually remove it, use the following:
```

marine::273H 199M 401V [32/32]> marine:attach_m203
You attach a frag underbarrel to a USMC M16A4 Assault Rifle

marine::273H 199M 401V [32/32]> marine:detach_m203
You detach a frag underbarrel.

marine::273H 199M 401V [32/32]>

```

# See also:
```
marine:detach_203
marine:fire
```
