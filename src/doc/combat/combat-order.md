# Giving structure to unarmed melee combat

# The problem
When in melee combat, the legacy CircleMUD combat system
consists of the player and the NPC taking turns dealing
damage to each other. This is fine, but many PC's want
to utilize specific skills during combat.

The default way of using skills in combat is to spam the
command until the MUD recognizes and executes that command.
Think of the "kick" or "knee" abilities that you have
to explicitly spam during melee.

# The solution
Combat Order was made so that you can specify which unarmed
melee combat commands that you will execute during combat.

# Setting your first three commands
Example:
```
80H 145M 243V [7/7]> combat_order set jab cross uppercut
Setting Technique: 'jab'...
Setting Technique: 'cross'...
Setting Technique: 'uppercut'...
Your combat order is now...
[1]: jab
[2]: cross
[3]: uppercut

80H 145M 243V [7/7]>

```

